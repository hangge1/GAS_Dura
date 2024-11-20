// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
#include "DuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraSummonAbility : public UDuraGameplayAbility
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    TArray<FVector> GetSpawnLocations();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Summoning")
    TSubclassOf<APawn> GetRandomMinionClass();

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    int32 NumMinions = 5;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    TArray<TSubclassOf<APawn>> MinionClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float MinSpawnDistnace = 150.f;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float MaxSpawnDistnace = 400.f;

    UPROPERTY(EditDefaultsOnly, Category = "Summoning")
    float SpawnSpread = 90;
};
