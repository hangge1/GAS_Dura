// Copyright by person HDD  


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "DuraGameplayTags.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/DuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UDuraAttributeSet* AS = CastChecked<UDuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(PlayerState);
    AttributePointsChangedDelegate.Broadcast(DuraPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UDuraAttributeSet* AS = CastChecked<UDuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(PlayerState);
    DuraPlayerState->OnAttributePointsChangedDelegate.AddLambda([this](int32 NewAttributePoints)
    {
        AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
    });
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FDuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
