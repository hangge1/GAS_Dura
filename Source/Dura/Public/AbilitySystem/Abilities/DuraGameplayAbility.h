// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

    virtual FString GetDescription(int32 Level);
    virtual FString GetNextLevelDescription(int32 Level);
    static FString GetLockedDescription(int32 Level);

protected:
    
    float GetManaCost(float InLevel = 1.f) const;
    float GetCooldown(float InLevel = 1.f) const;

    
};
