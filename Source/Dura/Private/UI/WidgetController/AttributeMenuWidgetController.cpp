// Copyright by person HDD  


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "DuraGameplayTags.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Player/DuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	check(AttributeInfo);

	for (auto& Pair : GetDuraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

    AttributePointsChangedDelegate.Broadcast(GetDuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair : GetDuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

    GetDuraPS()->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewAttributePoints)
    {
        AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
    });
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    GetDuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FDuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
