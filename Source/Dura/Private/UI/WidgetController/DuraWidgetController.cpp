// Copyright by person HDD  


#include "UI/WidgetController/DuraWidgetController.h"

#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "Player/DuraPlayerState.h"
#include "Player/DuraPlayerController.h"

void UDuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PC;
	PlayerState = WCParams.PS;
	AbilitySystemComponent = WCParams.ASC;
	AttributeSet = WCParams.AS;
}

void UDuraWidgetController::BroadcastInitialValue()
{

}

void UDuraWidgetController::BindCallbacksToDependencies()
{

}

void UDuraWidgetController::BroadcastAbilityInfo()
{
    if(!GetDuraASC()->bStartupAbilitiesGiven)return;

    FForEachAbility BroadcastDelegate;
    BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec) 
    {
        FDuraAbilityInfo AbilityInfo = AbilityInfoDataTable->FindAbilityInfoForTag(UDuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
        AbilityInfo.InputTag = UDuraAbilitySystemComponent::GetSlotFromSpec(AbilitySpec);
        AbilityInfo.StatusTag = UDuraAbilitySystemComponent::GetStatusFromSpec(AbilitySpec);
        AbilityInfoDelegate.Broadcast(AbilityInfo);
    });

    GetDuraASC()->ForEachAbility(BroadcastDelegate);
}

ADuraPlayerController* UDuraWidgetController::GetDuraPC()
{
    if(!DuraPlayerController)
    {
        DuraPlayerController = Cast<ADuraPlayerController>(PlayerController);
    }
    return DuraPlayerController;
}

ADuraPlayerState* UDuraWidgetController::GetDuraPS()
{
    if(!DuraPlayerState)
    {
        DuraPlayerState = Cast<ADuraPlayerState>(PlayerState);
    }
    return DuraPlayerState;
}

UDuraAbilitySystemComponent* UDuraWidgetController::GetDuraASC()
{
    if(!DuraAbilitySystemComponent)
    {
        DuraAbilitySystemComponent = Cast<UDuraAbilitySystemComponent>(AbilitySystemComponent);
    }
    return DuraAbilitySystemComponent;
}

UDuraAttributeSet* UDuraWidgetController::GetDuraAS()
{
    if(!DuraAttributeSet)
    {
        DuraAttributeSet = Cast<UDuraAttributeSet>(AttributeSet);
    }
    return DuraAttributeSet;
}
