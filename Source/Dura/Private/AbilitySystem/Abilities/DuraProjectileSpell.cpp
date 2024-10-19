// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraProjectileSpell.h"
#include "Kismet/KismetSystemLibrary.h"

void UDuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(this, TEXT("ActivateAbility (C++)"), true, true, FLinearColor::Yellow, 5.f);
}
