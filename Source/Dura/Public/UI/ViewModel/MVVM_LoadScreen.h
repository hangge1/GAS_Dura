// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class DURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

    UFUNCTION(BlueprintPure)
    UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index);

    UFUNCTION(BlueprintCallable)
    void EnablePlayAndDeleteButton(bool bEnabled);

    UFUNCTION(BlueprintCallable)
    void PlayButtonPressed();
    
    UFUNCTION(BlueprintCallable)
    void DeleteButtonPressed();

    void CreateAndInitLoadSlots();

    void LoadSavedSlotDatas();


    // Setter and Getter
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
    // End Setter and Getter

protected:
    void SelectSlotButtonPressed(UMVVM_LoadSlot* LoadSlot);

private:

    UPROPERTY()
    int32 DefaultSlotNumber = 3;

    UPROPERTY()
    TMap<int32, UMVVM_LoadSlot*> LoadSlots;

    UPROPERTY()
    UMVVM_LoadSlot* CurrentSelectedSlot;

    /* Fields Notifies */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString PlayButtonName = TEXT("PLAY");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString DeleteButtonName = TEXT("DELETE");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    FString QuitButtonName = TEXT("QUIT");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool PlayButtonEnable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool DeleteButtonEnable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
    bool QuitButtonEnable = true;
};
