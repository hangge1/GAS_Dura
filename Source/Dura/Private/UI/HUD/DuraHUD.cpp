// Copyright by person HDD  


#include "UI/HUD/DuraHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/UserWidget/DuraUserWidget.h"
#include "UI/WidgetController/DuraOverlayWidgetController.h"
#include "AbilitySystemComponent.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"

UDuraOverlayWidgetController* ADuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UDuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ADuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void ADuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_DuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_DuraHUD"));

	OverlayWidget = CreateWidget<UDuraUserWidget>(GetWorld(), OverlayWidgetClass);

	const FWidgetControllerParams Params(PC, PS, ASC, AS);
	UDuraOverlayWidgetController* Controller = GetOverlayWidgetController(Params);

	OverlayWidget->SetWidgetController(Controller);

	Controller->BroadcastInitialValue();

	OverlayWidget->AddToViewport();
}

void ADuraHUD::BeginPlay()
{
	Super::BeginPlay();
}
