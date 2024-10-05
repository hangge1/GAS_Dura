// Copyright by person HDD  


#include "AbilitySystem/DuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

UDuraAttributeSet::UDuraAttributeSet()
{
	InitHealth(100.0f);
	InitMaxHealth(100.0f);

	InitMana(50.0f);
	InitMaxMana(50.0f);
}

void UDuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UDuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Health, OldHealth);
}

void UDuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UDuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Mana, OldMana);
}

void UDuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, MaxMana, OldMaxMana);
}
