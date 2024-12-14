// Copyright by person HDD  


#include "Game/DuraGameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Kismet\GameplayStatics.h"
#include <Game\LoadScreenSaveGame.h>

void ADuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
    if(UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
    {
        UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
    }

    USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGame);
    LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
    LoadScreenSaveGame->SlotStatus = LoadSlot->SlotStatus;

    UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* ADuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
    USaveGame* SaveGameObject = nullptr;
    if(UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
    {
        SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
    }
    else
    {
        SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    }

    ULoadScreenSaveGame* res = Cast<ULoadScreenSaveGame>(SaveGameObject);
    return res;
}
