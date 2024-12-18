// Copyright by person HDD  


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/DuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/DuraGameInstance.h"


UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
    return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::CreateAndInitLoadSlots()
{ 
    for (int32 i = 0; i < DefaultSlotNumber; i++)
    {
        UMVVM_LoadSlot* NewSlot = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);

        NewSlot->SetLoadSlotName(FString::Printf(TEXT("LoadSlot_%d"), i)); //LoadSlot_0、LoadSlot_1、LoadSlot_2
        NewSlot->SetSlotIndex(i);
        NewSlot->SelectSlotButtonClick.AddUObject(this, &UMVVM_LoadScreen::SelectSlotButtonPressed);

        LoadSlots.Add(i, NewSlot);
    }
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(UMVVM_LoadSlot* LoadSlot)
{
    check(LoadSlot);

    for (TTuple<int32, UMVVM_LoadSlot*> Tuple : LoadSlots)
    {
        UMVVM_LoadSlot* Slot = Tuple.Value;
        Slot->SetSelectSlotButtonEnable(Slot != LoadSlot);
    }

    SetPlayButtonEnable(true);
    SetDeleteButtonEnable(true);

    CurrentSelectedSlot = LoadSlot;
}

void UMVVM_LoadScreen::LoadSavedSlotDatas()
{
    ADuraGameModeBase* DuraGameModeBase = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    if(!IsValid(DuraGameModeBase))
    {
        return;
    }

    for (const TTuple<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
    {
        ULoadScreenSaveGame* SaveGame = 
        DuraGameModeBase->GetOrCreateSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
        check(SaveGame);

        LoadSlot.Value->PlayerStartTag = SaveGame->PlayerStartTag;
        LoadSlot.Value->SetSlotStatus(SaveGame->SlotStatus);
        LoadSlot.Value->SetMapName(SaveGame->MapName);
        LoadSlot.Value->SetPlayerName(SaveGame->PlayerName);
        LoadSlot.Value->SetPlayerLevel(SaveGame->PlayerLevel);
    }
}

void UMVVM_LoadScreen::EnablePlayAndDeleteButton(bool bEnabled)
{
    SetPlayButtonEnable(bEnabled);
    SetDeleteButtonEnable(bEnabled);
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
    if(!IsValid(CurrentSelectedSlot))
    {
        return;
    }
    
    ADuraGameModeBase::DeleteSlot(CurrentSelectedSlot->GetLoadSlotName(), CurrentSelectedSlot->GetSlotIndex());

    CurrentSelectedSlot->SetSlotStatus(Vacant);
    CurrentSelectedSlot->SetSelectSlotButtonEnable(true);

    EnablePlayAndDeleteButton(true);
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
    ADuraGameModeBase* DuraGameMode = CastChecked<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    UDuraGameInstance* DuraGameInstance = DuraGameMode->GetGameInstance<UDuraGameInstance>();
    DuraGameInstance->PlayerStartTag = CurrentSelectedSlot->PlayerStartTag;
    DuraGameInstance->LoadSlotName = CurrentSelectedSlot->GetLoadSlotName();
    DuraGameInstance->LoadSlotIndex = CurrentSelectedSlot->GetSlotIndex();

    if(IsValid(CurrentSelectedSlot))
    {
        DuraGameMode->TravelToMap(CurrentSelectedSlot);
    } 
}

void UMVVM_LoadScreen::SetPlayButtonName(FString InPlayButtonName)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayButtonName, InPlayButtonName);
}

void UMVVM_LoadScreen::SetDeleteButtonName(FString InDeleteButtonName)
{
     UE_MVVM_SET_PROPERTY_VALUE(DeleteButtonName, InDeleteButtonName);
}

void UMVVM_LoadScreen::SetQuitButtonName(FString InQuitButtonName)
{
     UE_MVVM_SET_PROPERTY_VALUE(QuitButtonName, InQuitButtonName);
}

void UMVVM_LoadScreen::SetPlayButtonEnable(bool InPlayButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayButtonEnable, InPlayButtonEnable);
}

void UMVVM_LoadScreen::SetDeleteButtonEnable(bool InDeleteButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(DeleteButtonEnable, InDeleteButtonEnable);
}

void UMVVM_LoadScreen::SetQuitButtonEnable(bool InQuitButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(QuitButtonEnable, InQuitButtonEnable);
}



