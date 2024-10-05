// Copyright by person HDD  


#include "UI/UserWidget/DuraUserWidget.h"

void UDuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
