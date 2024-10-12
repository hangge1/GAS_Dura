// Copyright by person HDD  


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "DuraGameplayTags.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UDuraAttributeSet* AS = CastChecked<UDuraAttributeSet>(AttributeSet);

	check(AttributeInfo);

	FDuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FDuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
