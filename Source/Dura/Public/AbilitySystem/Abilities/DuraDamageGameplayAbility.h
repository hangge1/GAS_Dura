// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
#include "DuraAbilitiesTypes.h"
#include "DuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraDamageGameplayAbility : public UDuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

    UFUNCTION(BlueprintPure)
    FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
        AActor* TargetActor = nullptr, 
        FVector InRadialDamageOrigin = FVector::ZeroVector,
        bool bOverrideKnockbackDirection = false,
        FVector InKnockbackDirectionOverride = FVector::ZeroVector,
        bool bOverrideDeathImpulse = false,
        FVector InDeathImpulseDirectionOverride = FVector::ZeroVector,
        bool bOverridePitch = false,
        float PitchOverride = 0.f
    ) const;

    UFUNCTION(BlueprintPure)
    float GetDamageAtLevel() const;
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FGameplayTag DamageType;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FScalableFloat Damage;

    //Some Debuff Params
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffChance = 20.f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffDamage = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffFrequency = 1.f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DebuffDuration = 5.f;
    
    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float DeathImpulseMagnitude = 1000.f;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float KnockbackMagnitude = 1000.f; 

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float KnockbackChance = 0.f;

    
    //RadialDamage etc..
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
    bool bIsRadialDamage = false;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
    float RadialDamageInnerRadius = 0.f;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Damage")
    float RadialDamageOuterRadius = 0.f;

    UFUNCTION(BlueprintCallable)
    FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

};
