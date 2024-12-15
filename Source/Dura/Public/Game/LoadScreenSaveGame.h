// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
    Vacant,
    EnterName,
    Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
    TSubclassOf<UGameplayAbility> GameplayAbilityClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTag AbilityTag = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTag AbilityStatus = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTag AbilitySlot = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTag AbilityType = FGameplayTag();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 AbilityLevel;
};

UCLASS()
class DURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

    UPROPERTY()
    FString SlotName = FString();

    UPROPERTY()
    int32 SlotIndex = 0;

    UPROPERTY()
    FString PlayerName = FString("Default Name");

    UPROPERTY()
    TEnumAsByte<ESaveSlotStatus> SlotStatus = Vacant;

    UPROPERTY()
    FString MapName = FString("Default Map Name");

    UPROPERTY()
    FName PlayerStartTag;

    UPROPERTY()
    bool bFirstTimeLoadIn = true;

    /* Player */
    UPROPERTY()
    int32 PlayerLevel = 1;

    UPROPERTY()
    int32 XP = 0;

    UPROPERTY()
    int32 SpellPoints = 0;

    UPROPERTY()
    int32 AttributePoints = 0;


    /* Attributes */
    UPROPERTY()
    float Strength = 0;

    UPROPERTY()
    float Intelligence = 0;

    UPROPERTY()
    float Resilience = 0;

    UPROPERTY()
    float Vigor = 0;

    /* Abilities */
    UPROPERTY()
    TArray<FSavedAbility> SavedAbilities;
};
