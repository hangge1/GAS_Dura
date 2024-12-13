// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraDamageGameplayAbility.h"
#include "DuraFireBlast.generated.h"

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

protected:

    UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
    int32 NumFireBalls = 12;
};
