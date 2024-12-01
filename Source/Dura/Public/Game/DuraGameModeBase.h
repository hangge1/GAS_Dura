// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;

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
};
