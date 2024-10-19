// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
#include "DuraProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraProjectileSpell : public UDuraGameplayAbility
{
	GENERATED_BODY()
public:

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
};
