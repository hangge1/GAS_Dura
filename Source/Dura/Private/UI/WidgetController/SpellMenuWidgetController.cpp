// Copyright by person HDD  


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"

void USpellMenuWidgetController::BroadcastInitialValue()
{
    BroadcastAbilityInfo();

    OnSpellPointsChangedDelegate.Broadcast(GetDuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
    GetDuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
    {
        if(AbilityInfoDataTable)
        {
            FDuraAbilityInfo Info = AbilityInfoDataTable->FindAbilityInfoForTag(AbilityTag);
            Info.StatusTag = StatusTag;
            AbilityInfoDelegate.Broadcast(Info);
        }
    });

    GetDuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 NewSpellPoints)
    {
        OnSpellPointsChangedDelegate.Broadcast(NewSpellPoints);
    });
}
