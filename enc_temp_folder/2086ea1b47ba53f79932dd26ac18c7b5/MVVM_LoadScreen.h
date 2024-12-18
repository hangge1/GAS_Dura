// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class DURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
    
    UPROPERTY(BlueprintAssignable)
    FSlotSelected SlotSelected;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

    UFUNCTION(BlueprintPure)
    UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index);

    void InitializeLoadSlots();

    void LoadData();

    UFUNCTION(BlueprintCallable)
    void EnablePlayAndDeleteButton(bool bEnabled);

    UFUNCTION(BlueprintCallable)
    void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

    UFUNCTION(BlueprintCallable)
    void DeleteButtonPressed();

    UFUNCTION(BlueprintCallable)
    void PlayButtonPressed();

    void SetPlayButtonName(FString InPlayButtonName);
    FString GetPlayButtonName() const { return PlayButtonName; }

    void SetDeleteButtonName(FString InDeleteButtonName);
    FString GetDeleteButtonName() const { return DeleteButtonName; }

    void SetQuitButtonName(FString InQuitButtonName);
    FString GetQuitButtonName() const { return QuitButtonName; }

    void SetPlayButtonEnable(bool InPlayButtonEnable);
    bool GetPlayButtonEnable() const { return PlayButtonEnable; }

    void SetDeleteButtonEnable(bool InDeleteButtonEnable);
    bool GetDeleteButtonEnable() const { return DeleteButtonEnable; }

    void SetQuitButtonEnable(bool InQuitButtonEnable);
    bool GetQuitButtonEnable() const { return QuitButtonEnable; }
private:
    UPROPERTY()
    TMap<int32, UMVVM_LoadSlot*> LoadSlots;

    UPROPERTY()
    TObjectPtr<UMVVM_LoadSlot> LoadSlots_0;
    UPROPERTY()
    TObjectPtr<UMVVM_LoadSlot> LoadSlots_1;
    UPROPERTY()
    TObjectPtr<UMVVM_LoadSlot> LoadSlots_2;

    UPROPERTY()
    UMVVM_LoadSlot* SelectedSlot;

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString PlayButtonName = TEXT("PLAY");

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString DeleteButtonName = TEXT("DELETE");

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString QuitButtonName = TEXT("QUIT");

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool PlayButtonEnable = false;

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool DeleteButtonEnable = false;

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool QuitButtonEnable = true;
};
