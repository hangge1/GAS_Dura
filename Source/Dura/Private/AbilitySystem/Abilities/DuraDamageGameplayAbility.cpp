// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include <Interaction/CombatInterface.h>

void UDuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

    const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UDuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, 
        FVector InRadialDamageOrigin,
        bool bOverrideKnockbackDirection,
        FVector InKnockbackDirectionOverride,
        bool bOverrideDeathImpulse,
        FVector InDeathImpulseDirectionOverride,
        bool bOverridePitch,
        float PitchOverride) const
{
    FDamageEffectParams Params;
    Params.WorldContextObject = GetAvatarActorFromActorInfo();
    Params.DamageGameplayEffectClass = DamageEffectClass;
    Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
    Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
    Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
    Params.AbilityLevel = GetAbilityLevel();
    Params.DamageType = DamageType;
    Params.DebuffChance = DebuffChance;
    Params.DebuffDamage = DebuffDamage;
    Params.DebuffDuration = DebuffDuration;
    Params.DebuffFrequency = DebuffFrequency;
    Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
    Params.KnockbackMagnitude = KnockbackMagnitude;
    Params.KnockbackChance = KnockbackChance;

    if(IsValid(TargetActor))
    {
        FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
        if(bOverridePitch)
        {
            Rotation.Pitch = PitchOverride;
        }
        else
        {
            Rotation.Pitch = 45.f;
        }
        const FVector ToTarget = Rotation.Vector();
        if(!bOverrideKnockbackDirection)
        {
            Params.KnockbackForce = ToTarget * KnockbackMagnitude;
        }
        if(!bOverrideDeathImpulse)
        {
            Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
        }
    }
    
    if(bOverrideKnockbackDirection)
    {
        InKnockbackDirectionOverride.Normalize();
        Params.KnockbackForce = InKnockbackDirectionOverride * KnockbackMagnitude;

        if(bOverridePitch)
        {
            FRotator KnockbackRotation = InKnockbackDirectionOverride.Rotation();
            KnockbackRotation.Pitch = PitchOverride;
            Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackMagnitude;
        }
    }

    if(bOverrideDeathImpulse)
    {
        InDeathImpulseDirectionOverride.Normalize();
        Params.DeathImpulse = InDeathImpulseDirectionOverride * DeathImpulseMagnitude;

        if(bOverridePitch)
        {
            FRotator DeathImpulseRotation = InDeathImpulseDirectionOverride.Rotation();
            DeathImpulseRotation.Pitch = PitchOverride;
            Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
        }
    }

    if(bIsRadialDamage)
    {
        Params.bIsRadialDamage = bIsRadialDamage;
        Params.RadialDamageOrigin = InRadialDamageOrigin;
        Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
        Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
    }

    return Params;
}

float UDuraDamageGameplayAbility::GetDamageAtLevel() const
{
    return Damage.GetValueAtLevel(GetAbilityLevel());
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