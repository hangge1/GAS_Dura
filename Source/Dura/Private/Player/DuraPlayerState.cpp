// Copyright by person HDD  


#include "Player/DuraPlayerState.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "Net/UnrealNetwork.h"


ADuraPlayerState::ADuraPlayerState()
{
	AbilitiesSystemComponent = CreateDefaultSubobject<UDuraAbilitySystemComponent>("AbilitiesSystemComponent");
	AbilitiesSystemComponent->SetIsReplicated(true);
	AbilitiesSystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UDuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.0f;
}

UAbilitySystemComponent* ADuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitiesSystemComponent;
}

void ADuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ADuraPlayerState, Level);
}

void ADuraPlayerState::OnRep_Level(int32 OldLevel)
{

}
