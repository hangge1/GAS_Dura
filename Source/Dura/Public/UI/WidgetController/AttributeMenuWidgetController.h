// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DURA_API UAttributeMenuWidgetController : public UDuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
};
