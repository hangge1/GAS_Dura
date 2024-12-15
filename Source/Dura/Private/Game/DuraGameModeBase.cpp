// Copyright by person HDD  


#include "Game/DuraGameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include <Game\DuraGameInstance.h>

void ADuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
    DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);

    USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGame);
    LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
    LoadScreenSaveGame->SlotStatus = LoadSlot->SlotStatus;
    LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
    LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;


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

void ADuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
    if(UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
    {
        UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
    }
}

ULoadScreenSaveGame* ADuraGameModeBase::RetrieveInGameSaveData()
{
    UDuraGameInstance* DuraGameInstance = GetGameInstance<UDuraGameInstance>();
    check(DuraGameInstance);

    const FString InGameLoadSlotName = DuraGameInstance->LoadSlotName;
    const int32 InGameLoadSlotIndex = DuraGameInstance->LoadSlotIndex;

    return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);

}

void ADuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveGame)
{
    UDuraGameInstance* DuraGameInstance = GetGameInstance<UDuraGameInstance>();
    const FString InGameLoadSlotName = DuraGameInstance->LoadSlotName;
    const int32 InGameLoadSlotIndex = DuraGameInstance->LoadSlotIndex;
    DuraGameInstance->PlayerStartTag = SaveGame->PlayerStartTag;

    UGameplayStatics::SaveGameToSlot(SaveGame, InGameLoadSlotName, InGameLoadSlotIndex);
}

void ADuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
    TSoftObjectPtr<UWorld> WorldSoftPtr = Maps.FindChecked(LoadSlot->GetMapName());
    UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, WorldSoftPtr);
}

AActor* ADuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    UDuraGameInstance* DuraGameInstance = GetGameInstance<UDuraGameInstance>();
    check(DuraGameInstance);

    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    if(PlayerStarts.Num() > 0)
    {
         AActor* SelectedActor = PlayerStarts[0];
         for (AActor* Actor : PlayerStarts)
         {
            if(APlayerStart* ps = Cast<APlayerStart>(Actor))
            {
                if(ps->PlayerStartTag == DuraGameInstance->PlayerStartTag)
                {
                    SelectedActor = ps;
                    break;
                }
            }
         }
         return SelectedActor;
    }
    return nullptr;
}

void ADuraGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    Maps.Add(DefaultMapName, DefaultMap);
}
