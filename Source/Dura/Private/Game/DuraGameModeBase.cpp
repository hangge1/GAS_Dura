// Copyright by person HDD  


#include "Game/DuraGameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"

void ADuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
    DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);

    USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGame);
    LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
    LoadScreenSaveGame->SlotStatus = LoadSlot->SlotStatus;
    LoadScreenSaveGame->MapName = LoadSlot->GetMapName();


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

void ADuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
    TSoftObjectPtr<UWorld> WorldSoftPtr = Maps.FindChecked(LoadSlot->GetMapName());
    UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, WorldSoftPtr);
}

AActor* ADuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    if(PlayerStarts.Num() > 0)
    {
         AActor* SelectedActor = PlayerStarts[0];
         for (AActor* Actor : PlayerStarts)
         {
            if(APlayerStart* ps = Cast<APlayerStart>(Actor))
            {
                if(ps->PlayerStartTag == FName("TheTag"))
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
