// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;
/**
 * 
 */
UCLASS()
class DURA_API ADuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category= "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

    UPROPERTY(EditDefaultsOnly, Category= "Ability Info")
    TObjectPtr<UAbilityInfo> AbilityInfo;

    void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

    ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

    static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

    ULoadScreenSaveGame* RetrieveInGameSaveData();

    void SaveInGameProgressData(ULoadScreenSaveGame* SaveGame);

    void SaveWorldState(UWorld* World);
    void LoadWorldState(UWorld* World);

    void TravelToMap(UMVVM_LoadSlot* LoadSlot);

    virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USaveGame> LoadScreenSaveGameClass;

    UPROPERTY(EditDefaultsOnly)
    FString DefaultMapName;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> DefaultMap;

    UPROPERTY(EditDefaultsOnly)
    FName DefaultPlayerStartTag;

    UPROPERTY(EditDefaultsOnly)
    TMap<FString, TSoftObjectPtr<UWorld>> Maps;

protected:
    virtual void BeginPlay() override;
};
