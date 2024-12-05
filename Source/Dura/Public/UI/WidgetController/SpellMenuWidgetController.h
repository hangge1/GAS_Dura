// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DuraWidgetController.h"
#include "Player/DuraPlayerState.h"
#include "DuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquippedButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

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

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FWaitForEquipSelectionSignature WaitForEquipDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FWaitForEquipSelectionSignature StopWaitForEquipDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;

	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;

    UFUNCTION(BlueprintCallable)
    void SpellGlobeSelected(const FGameplayTag& AbilityTag);

    UFUNCTION(BlueprintCallable)
    void SpendPointButtonPressed();

    UFUNCTION(BlueprintCallable)
    void GlobeDeSelect();

    UFUNCTION(BlueprintCallable)
    void EquipButtonPressed();

    UFUNCTION(BlueprintCallable)
    void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

    void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& Slot, const FGameplayTag& PrevSlot);
private:
    void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, 
        bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);

    FSelectedAbility SelectedAbility = {FDuraGameplayTags::Get().Abilities_None, FDuraGameplayTags::Get().Abilities_Status_Locked};
    int CurrentSpellPoints = 0;
    bool bWaitingForEquipSelection = false;

    FGameplayTag SelectedSlot;
};
