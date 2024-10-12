// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DuraHUD.generated.h"

class UDuraUserWidget;
class UDuraOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeMenuWidgetController;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class DURA_API ADuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UDuraOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UDuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UDuraOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDuraOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
