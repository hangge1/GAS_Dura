// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSelectSlotButtonClick, UMVVM_LoadSlot* /* Who Slot Clicked */);
/**
 * 
 */
UCLASS()
class DURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
    UPROPERTY()
    FName PlayerStartTag;

    UPROPERTY()
    FString MapAssetName;

    FSelectSlotButtonClick SelectSlotButtonClick;

    UFUNCTION(BlueprintCallable)
    void NewGameButtonPressed();

    UFUNCTION(BlueprintCallable)
    void SelectSlotButtonPressed();

    UFUNCTION(BlueprintCallable)
    void NewSlotButtonPressed(const FString& EnteredPlayerName);

    void InitSlotStatus();


    //Setter and Getter
    void SetLoadSlotName(FString InLoadSlotName);
    FString GetLoadSlotName() const { return LoadSlotName; }

    void SetMapName(FString InMapName);
    FString GetMapName() const { return MapName; }

    void SetPlayerName(FString InPlayerName);
    FString GetPlayerName() const { return PlayerName; }

    void SetPlayerLevel(int32 InPlayerLevel);
    int32 GetPlayerLevel() const { return PlayerLevel; }

    void SetSlotStatus(int32 InSlotStatus);
    int32 GetSlotStatus() const { return SlotStatus; }

    void SetEnterNameString(FString InEnterNameString);
    FString GetEnterNameString() const { return EnterNameString; }
    
    void SetSlotIndex(int32 InSlotIndex) { SlotIndex = InSlotIndex; };
    int32 GetSlotIndex() const { return SlotIndex; }

    void SetNewSlotString(FString InNewSlotString);
    FString GetNewSlotString() const { return NewSlotString; }

    void SetSelectSlotString(FString InSelectSlotString);
    FString GetSelectSlotString() const { return SelectSlotString; }

    void SetNewGameString(FString InNewGameString);
    FString GetNewGameString() const { return NewGameString; }

    void SetSelectSlotButtonEnable(bool InSelectSlotButtonEnable);
    bool GetSelectSlotButtonEnable() const { return SelectSlotButtonEnable; }
    //End Setter and Getter
private:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    int32 SlotStatus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString LoadSlotName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess="true"))
    int32 SlotIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString MapName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    int32 PlayerLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString EnterNameString = TEXT("Enter Name: ");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString NewSlotString = TEXT("NEW SLOT");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString SelectSlotString = TEXT("SELECT SLOT");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString NewGameString = TEXT("NEW GAME");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool SelectSlotButtonEnable = true;
};
