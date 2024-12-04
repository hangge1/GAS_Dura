// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
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

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

    UFUNCTION(BlueprintCallable)
    FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

    float GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType) const;
};
