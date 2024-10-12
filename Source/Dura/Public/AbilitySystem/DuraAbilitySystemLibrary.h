// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DuraAbilitySystemLibrary.generated.h"

class UDuraOverlayWidgetController;

/**
 * 
 */
UCLASS()
class DURA_API UDuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UDuraOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
