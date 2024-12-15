// Copyright by person HDD  


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem\DuraAbilitySystemComponent.h"
#include "Interaction\CombatInterface.h"
#include "DuraGameplayTags.h"

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
        ActivateIfEquipped(OwnerASC);
    }
    else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
    {
        CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* NewASC)
        {
            if(UDuraAbilitySystemComponent* ASC = Cast<UDuraAbilitySystemComponent>(NewASC))
            {
                ASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
                ActivateIfEquipped(ASC);
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

void UPassiveNiagaraComponent::ActivateIfEquipped(UDuraAbilitySystemComponent* ASC)
{
    const bool bStartupAbilitiesActivated = ASC->bStartupAbilitiesGiven;
    if(bStartupAbilitiesActivated)
    {
        if(ASC->GetStatusFromAbilityTag(PassiveSpellTag) == FDuraGameplayTags::Get().Abilities_Status_Equipped)
        {
            Activate();
        }
    }
}
