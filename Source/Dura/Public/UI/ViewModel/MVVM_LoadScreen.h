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
    UFUNCTION(BlueprintPure)
    UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index);

    void InitializeLoadSlots();

    UFUNCTION(BlueprintCallable)
    void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

    UFUNCTION(BlueprintCallable)
    void NewGameButtonPressed(int32 Slot);

    UFUNCTION(BlueprintCallable)
    void SelectSlotButtonPressed(int32 Slot);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

private:
    UPROPERTY()
    TMap<int32, UMVVM_LoadSlot*> LoadSlots;

    UPROPERTY()
    TObjectPtr<UMVVM_LoadSlot> LoadSlots_0;
    UPROPERTY()
    TObjectPtr<UMVVM_LoadSlot> LoadSlots_1;
    UPROPERTY()
    TObjectPtr<UMVVM_LoadSlot> LoadSlots_2;
};
