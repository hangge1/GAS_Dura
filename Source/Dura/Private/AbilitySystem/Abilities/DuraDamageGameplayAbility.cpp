// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include <Interaction/CombatInterface.h>

void UDuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat>& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UDuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
    if(TaggedMontages.Num() > 0)
    {
        const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
        return TaggedMontages[Selection];
    }

    return FTaggedMontage();
}

float UDuraDamageGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType) const
{
    checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbility %s does not contain DamageType %s"), *GetNameSafe(this), *DamageType.ToString());
    return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}
