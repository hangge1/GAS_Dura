// Copyright by person HDD  


#include "AbilitySystem/DuraAbilitySystemGlobals.h"
#include "DuraAbilitiesTypes.h"

FGameplayEffectContext* UDuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FDuraGameplayEffectContext();
}
