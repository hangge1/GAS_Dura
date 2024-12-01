// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DuraWidgetController.h"
#include "Player/DuraPlayerState.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DURA_API USpellMenuWidgetController : public UDuraWidgetController
{
	GENERATED_BODY()
public:

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FOnPlayerStatChangedSignature OnSpellPointsChangedDelegate;

	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;
};
