// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectButton, bool, bEnable);

/**
 * 
 */
UCLASS()
class DURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

    UPROPERTY(BlueprintAssignable)
    FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

    UPROPERTY(BlueprintAssignable)
    FEnableSelectButton EnableSelectButton;

    void InitializeSlot();

    void SetLoadSlotName(FString InLoadSlotName);

    FString GetLoadSlotName() const { return LoadSlotName; }

    void SetPlayerName(FString InPlayerName);

    FString GetPlayerName() const { return PlayerName; }

    void SetSlotIndex(int32 InSlotIndex);

    int32 GetSlotIndex() const { return SlotIndex; }

    UPROPERTY()
    TEnumAsByte<ESaveSlotStatus> SlotStatus;
private:
    
    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString LoadSlotName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    int32 SlotIndex;
};