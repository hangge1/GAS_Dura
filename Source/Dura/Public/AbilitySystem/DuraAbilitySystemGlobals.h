// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "DuraAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
