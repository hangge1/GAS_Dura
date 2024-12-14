// Copyright by person HDD  


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game\DuraGameModeBase.h"
#include "Kismet\GameplayStatics.h"


UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
    return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::InitializeLoadSlots()
{
    LoadSlots_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
    LoadSlots_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
    LoadSlots_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);

    LoadSlots_0->SetLoadSlotName(TEXT("LoadSlot_0"));
    LoadSlots_1->SetLoadSlotName(TEXT("LoadSlot_1"));
    LoadSlots_2->SetLoadSlotName(TEXT("LoadSlot_2"));

    LoadSlots.Add(0, LoadSlots_0);
    LoadSlots.Add(1, LoadSlots_1);
    LoadSlots.Add(2, LoadSlots_2);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
    ADuraGameModeBase* DuraGameMode = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    LoadSlots[Slot]->SetPlayerName(EnteredName);

    DuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
    LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
    LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
    
}
