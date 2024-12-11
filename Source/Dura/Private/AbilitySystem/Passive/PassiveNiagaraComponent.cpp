// Copyright by person HDD  


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <AbilitySystem\DuraAbilitySystemComponent.h>
#include <Interaction\CombatInterface.h>


UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
    bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
    Super::BeginPlay();

    UDuraAbilitySystemComponent* OwnerASC = Cast<UDuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()));
    if(OwnerASC)
    {
        OwnerASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
    }
    else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
    {
        CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* NewASC)
        {
            if(UDuraAbilitySystemComponent* ASC = Cast<UDuraAbilitySystemComponent>(NewASC))
            {
                ASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
            }
        });
    }
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
    if(AbilityTag.MatchesTagExact(PassiveSpellTag))
    {
        if(bActivate)
        {
            Activate();
        }
        else
        {
            Deactivate();
        }
    }
}
