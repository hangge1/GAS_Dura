// Copyright by person HDD  


#include "UI/WidgetController/DuraWidgetController.h"

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
