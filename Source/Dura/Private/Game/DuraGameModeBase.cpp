// Copyright by person HDD  


#include "Game/DuraGameModeBase.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Game/DuraGameInstance.h"
#include "EngineUtils.h"
#include "Interaction/SaveInterface.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Dura/DuraLogChannels.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void ADuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
    DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);

    USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
    ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGame);
    LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
    LoadScreenSaveGame->SlotStatus = static_cast<ESaveSlotStatus>(LoadSlot->GetSlotStatus());
    LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
    LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
    LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;

    UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

ULoadScreenSaveGame* ADuraGameModeBase::GetOrCreateSaveSlotData(const FString& SlotName, int32 SlotIndex) const
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

    return GetOrCreateSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);

}

void ADuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveGame)
{
    UDuraGameInstance* DuraGameInstance = GetGameInstance<UDuraGameInstance>();
    const FString InGameLoadSlotName = DuraGameInstance->LoadSlotName;
    const int32 InGameLoadSlotIndex = DuraGameInstance->LoadSlotIndex;
    DuraGameInstance->PlayerStartTag = SaveGame->PlayerStartTag;

    UGameplayStatics::SaveGameToSlot(SaveGame, InGameLoadSlotName, InGameLoadSlotIndex);
}

void ADuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName)
{
    FString WorldName = World->GetMapName();
    WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

    UDuraGameInstance* DuraGameInstace = CastChecked<UDuraGameInstance>(GetGameInstance());
    if(ULoadScreenSaveGame* SaveData = GetOrCreateSaveSlotData(DuraGameInstace->LoadSlotName, DuraGameInstace->LoadSlotIndex))
    {
        if(DestinationMapAssetName != FString(""))
        {
            SaveData->MapAssetName = DestinationMapAssetName;
            SaveData->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
        }

        if(!SaveData->HasMap(WorldName))
        {
            FSaveMap NewSaveMap;
            NewSaveMap.MapAssetName = WorldName;
            SaveData->SavedMaps.Add(NewSaveMap);
        }

        FSaveMap SavedMap = SaveData->GetSavedMapWithMapName(WorldName);
        SavedMap.SavedActors.Empty(); // clear it out, we'll fill it in with actors

        for (FActorIterator It(World); It; ++It)
        {
            AActor* Actor = *It;
            if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

            FSaveActor SavedActor;
            SavedActor.ActorName = Actor->GetFName();
            SavedActor.Transform = Actor->GetTransform();

            FMemoryWriter MemoryWriter(SavedActor.Bytes);

            FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
            Archive.ArIsSaveGame = true;
            Actor->Serialize(Archive);

            SavedMap.SavedActors.AddUnique(SavedActor);
        }

        for (FSaveMap& MapToReplace : SaveData->SavedMaps)
        {
            if(MapToReplace.MapAssetName == WorldName)
            {
                MapToReplace = SavedMap;
            }
        }

        UGameplayStatics::SaveGameToSlot(SaveData, DuraGameInstace->LoadSlotName, DuraGameInstace->LoadSlotIndex);
    }
}

void ADuraGameModeBase::LoadWorldState(UWorld* World)
{
    FString WorldName = World->GetMapName();
    WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

    UDuraGameInstance* DuraGameInstace = CastChecked<UDuraGameInstance>(GetGameInstance());

    if(UGameplayStatics::DoesSaveGameExist(DuraGameInstace->LoadSlotName, DuraGameInstace->LoadSlotIndex))
    {
        ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(DuraGameInstace->LoadSlotName, DuraGameInstace->LoadSlotIndex));
        if(!SaveGame)
        {
            UE_LOG(LogDura, Error, TEXT("Failed to load slot"));
            return;
        }

        for (FActorIterator It(World); It; ++It)
        {
            AActor* Actor = *It;
            if(!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

            for (FSaveActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
            {
                if(SavedActor.ActorName == Actor->GetFName())
                {
                    if(ISaveInterface::Execute_ShouldLoadTransform(Actor))
                    {
                        Actor->SetActorTransform(SavedActor.Transform);
                    }

                    FMemoryReader MemoryReader(SavedActor.Bytes);
                    FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
                    Archive.ArIsSaveGame = true;
                    Actor->Serialize(Archive); // converts binary bytes back into variables

                    ISaveInterface::Execute_LoadActor(Actor);
                }
            }
        }
    }
}

void ADuraGameModeBase::TravelToMap(UMVVM_LoadSlot* LoadSlot)
{
    TSoftObjectPtr<UWorld> WorldSoftPtr = Maps.FindChecked(LoadSlot->GetMapName());
    UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot, WorldSoftPtr);
}

FString ADuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
    for (const TTuple<FString, TSoftObjectPtr<UWorld>>& Map : Maps)
    {
        if(Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
        {
            return Map.Key;
        }
    }
    return FString();
}

void ADuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
    ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
    if(!IsValid(SaveGame)) return;

    UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
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
