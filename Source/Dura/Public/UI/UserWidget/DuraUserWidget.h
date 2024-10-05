// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
