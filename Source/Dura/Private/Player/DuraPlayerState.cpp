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

void ADuraPlayerState::AddToXP(int32 InXP)
{
    XP += InXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void ADuraPlayerState::AddToLevel(int32 InLevel)
{
    Level += InLevel;
    OnLevelChangedDelegate.Broadcast(Level, true);
}

void ADuraPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
    AttributePoints += InAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADuraPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
    SpellPoints += InSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ADuraPlayerState::SetXP(int32 InXP)
{
    XP = InXP;
    OnXPChangedDelegate.Broadcast(XP);
}

void ADuraPlayerState::SetLevel(int32 InLevel)
{
    Level = InLevel;
    OnLevelChangedDelegate.Broadcast(Level, false);
}

void ADuraPlayerState::SetAttributePoints(int32 InAttributePoints)
{
    AttributePoints = InAttributePoints;
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADuraPlayerState::SetSpellPoints(int32 InSpellPoints)
{
    SpellPoints = InSpellPoints;
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ADuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADuraPlayerState, Level);
	DOREPLIFETIME(ADuraPlayerState, XP);

    DOREPLIFETIME(ADuraPlayerState, SpellPoints);
	DOREPLIFETIME(ADuraPlayerState, AttributePoints);
}

void ADuraPlayerState::OnRep_Level(int32 OldLevel)
{
    OnLevelChangedDelegate.Broadcast(Level, true);
}

void ADuraPlayerState::OnRep_XP(int32 OldXP)
{
    OnXPChangedDelegate.Broadcast(XP);
}

void ADuraPlayerState::OnRep_AttributePoints(int32 Old_AttributePoints)
{
    OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ADuraPlayerState::OnRep_SpellPoints(int32 Old_SpellPoints)
{
    OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
