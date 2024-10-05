// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DuraHUD.generated.h"

class UDuraUserWidget;
class UDuraOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class DURA_API ADuraHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UDuraUserWidget> Overlay;

	UFUNCTION(BlueprintCallable)
	UDuraOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UDuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UDuraOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDuraOverlayWidgetController> OverlayWidgetControllerClass;
};
