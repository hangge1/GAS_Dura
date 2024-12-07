// Copyright by person HDD  


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "DuraGameplayTags.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "DuraAbilitiesTypes.h"


struct DuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor_Penetration); 
	DECLARE_ATTRIBUTE_CAPTUREDEF(Block_Chance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	DuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, Armor_Penetration, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, Block_Chance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, CriticalHitResistance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDuraAttributeSet, PhysicalResistance, Target, false);
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

	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectSpec& Spec, 
    const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
    FAggregatorEvaluateParameters EvaluationParameters, 
    const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();
    for( const TTuple<FGameplayTag, FGameplayTag>& Pair : GameplayTags.DamageTypesToDebuffs )
    {
        const FGameplayTag& DamageType = Pair.Key;
        const FGameplayTag& DebuffType = Pair.Value;
        const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
        if( TypeDamage > -.5f ) // .5 padding for floating point precision
        {
            // Determine if there was a successful debuff
            const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

            float TargetDebuffResistance = 0.f;
            const FGameplayTag ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
            ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
            TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);

            const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
            const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
            if( bDebuff )
            {
                //TODO: What do we do?
            }
        }
    }
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, 
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
    const FDuraGameplayTags& Tags = FDuraGameplayTags::Get();

	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor_Penetration, DamageStatics().Armor_PenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Block_Chance, DamageStatics().Block_ChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);


	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

    int32 SourcePlayerLevel = 1;
    if(SourceAvatar->Implements<UCombatInterface>())
    {
        SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
    }

    int32 TargetPlayerLevel = 1;
    if(TargetAvatar->Implements<UCombatInterface>())
    {
        TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
    }

	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

    

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

    // Debuff
    DetermineDebuff(Spec, ExecutionParams, EvaluationParameters, TagsToCaptureDefs);


	// Get Damage Set by Caller Magnitude
    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();
	float Damage = 0.0f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair: GameplayTags.DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(!TagsToCaptureDefs.Contains(DamageTypeTag),
			TEXT("TagToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ResistanceTag.ToString())
		const FGameplayEffectAttributeCaptureDefinition& CaptureDef = TagsToCaptureDefs[ResistanceTag];
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}

	// Capture BlockChance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().Block_ChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.0f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	UDuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

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
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoeff = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoeff) / 100.f;

	// Armor ignore percentage of the Target's Damage
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoeff = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoeff) / 100.f;
	

	float SourceCritHitChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCritHitChance);
	SourceCritHitChance = FMath::Max<float>(0.0f, SourceCritHitChance);

	float TargetCritHitResistence = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCritHitResistence);
	TargetCritHitResistence = FMath::Max<float>(0.0f, TargetCritHitResistence);

	float SourceCritHitDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCritHitDamage);
	SourceCritHitDamage = FMath::Max<float>(0.0f, SourceCritHitDamage);

	// Critical Hit Resistence reduce Critical hit chance
	const FRealCurve* CritHitResistenceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CritHitResistenceCoeff = CritHitResistenceCurve->Eval(TargetPlayerLevel);

	const float EffectiveCriticalHitChance = SourceCritHitChance - TargetCritHitResistence * CritHitResistenceCoeff;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UDuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	// double damage plus critical hit damage if critical 
	Damage = bCriticalHit ? Damage * 2 + SourceCritHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(UDuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
