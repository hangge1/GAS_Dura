// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DuraWidgetController.h"
#include "Player/DuraPlayerState.h"
#include "DuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquippedButtonEnabled);

struct FSelectedAbility
{
    FGameplayTag AbilityTag = FGameplayTag();
    FGameplayTag Status = FGameplayTag();
};

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

    FSelectedAbility SelectedAbility = {FDuraGameplayTags::Get().Abilities_None, FDuraGameplayTags::Get().Abilities_Status_Locked};
    int CurrentSpellPoints = 0;
};
