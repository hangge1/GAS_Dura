// Copyright by person HDD  


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "DuraGameplayTags.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"


struct DuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor); //ArmorDef
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor_Penetration); //Block_ChanceDef
	DECLARE_ATTRIBUTE_CAPTUREDEF(Block_Chance); //Block_ChanceDef

	DuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, Armor_Penetration, Source, false)

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, Armor, Target, false)	
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, Block_Chance, Target, false)
	}
};

static const DuraDamageStatics& DamageStatics()
{
	static DuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().Armor_PenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().Block_ChanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FDuraGameplayTags::Get().Damage);

	// Capture BlockChance on Target, and determine if there was a successful Block
	
	float TargetBlockChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Block_ChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.0f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	// If Block, Halve the damage.
	Damage = bBlocked ? Damage * 0.5f : Damage;

	float TargetArmor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.0f, TargetArmor);

	float SourceArmorPenetration = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Armor_PenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.0f, SourceArmorPenetration);
	
	UCharacterClassInfo* CharacterClassInfo = UDuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	
	// ArmorPenetration ignores a percentage of the Target's Armor
	//每4点有效穿透忽略敌人1%护甲
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoeff = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoeff) / 100.f;

	// Armor ignore percentage of the Target's Damage
	//每3点护甲抵抗1%伤害
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoeff = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoeff) / 100.f;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UDuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
