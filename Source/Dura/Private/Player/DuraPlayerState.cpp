// Copyright by person HDD  


#include "Player/DuraPlayerState.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"


ADuraPlayerState::ADuraPlayerState()
{
	AbilitiesSystemComponent = CreateDefaultSubobject<UDuraAbilitySystemComponent>("AbilitiesSystemComponent");
	AbilitiesSystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UDuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ADuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitiesSystemComponent;
}
