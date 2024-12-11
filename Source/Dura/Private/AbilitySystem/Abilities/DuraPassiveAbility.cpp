// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraPassiveAbility.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

void UDuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
    const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if(UDuraAbilitySystemComponent* ASC = Cast<UDuraAbilitySystemComponent>(
        UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
    {
        ASC->DeactivatePassiveAbility.AddUObject(this, &UDuraPassiveAbility::ReceiveDeactivate);
    }
    
}
void UDuraPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
    if(AbilityTags.HasTagExact(AbilityTag))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
    }
}
