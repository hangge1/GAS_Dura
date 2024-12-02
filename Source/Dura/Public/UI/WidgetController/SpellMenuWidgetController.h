// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DuraWidgetController.h"
#include "Player/DuraPlayerState.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquippedButtonEnabled);


UCLASS(BlueprintType, Blueprintable)
class DURA_API USpellMenuWidgetController : public UDuraWidgetController
{
	GENERATED_BODY()
public:

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FOnPlayerStatChangedSignature OnSpellPointsChangedDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FSpellGlobeSelectedSignature SpellGlobeButtonEnabledChanged;

	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;

    UFUNCTION(BlueprintCallable)
    void SpellGlobeSelected(const FGameplayTag& AbilityTag);

private:
    void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, 
        bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);
};
