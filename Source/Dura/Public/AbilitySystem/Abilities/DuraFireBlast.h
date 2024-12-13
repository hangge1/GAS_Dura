// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraDamageGameplayAbility.h"
#include "DuraFireBlast.generated.h"

class ADuraFireBall;
/**
 * 
 */
UCLASS()
class DURA_API UDuraFireBlast : public UDuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
    virtual FString GetDescription(int32 Level) override;
    virtual FString GetNextLevelDescription(int32 Level) override;

    UFUNCTION(BlueprintCallable)
    TArray<ADuraFireBall*> SpawnFireBalls();
protected:

    UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
    int32 NumFireBalls = 12;

private:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<ADuraFireBall> FireBallClass;
};
