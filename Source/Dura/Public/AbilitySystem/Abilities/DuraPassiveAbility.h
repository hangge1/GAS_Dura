// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
#include "DuraPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraPassiveAbility : public UDuraGameplayAbility
{
	GENERATED_BODY()
public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

    void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
