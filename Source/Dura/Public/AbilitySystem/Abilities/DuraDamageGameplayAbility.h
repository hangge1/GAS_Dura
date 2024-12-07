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

    FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;
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
    float DeathUmpulseMagnitude = 60.f;


    UFUNCTION(BlueprintCallable)
    FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

};
