// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraProjectileSpell.h"
#include "DuraFireBolt.generated.h"


UCLASS()
class DURA_API UDuraFireBolt : public UDuraProjectileSpell
{
	GENERATED_BODY()
public:   
    virtual FString GetDescription(int32 Level) override;
    virtual FString GetNextLevelDescription(int32 Level) override;

    UFUNCTION(BlueprintCallable)
    void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, 
        bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    float ProjectileSpread = 90.f;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    int32 MaxNumProjectiles = 5;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    float HomingAccelerationMin = 1600;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    float HomingAccelerationMax = 3200;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    bool bLaunchHomingProjectiles = true;
};
