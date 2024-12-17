// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "DuraEnemySpawnPoint.generated.h"

class ADuraEnemy;
/**
 * 
 */
UCLASS()
class DURA_API ADuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
    
    UFUNCTION(BlueprintCallable)
    void SpawnEnemy();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
    TSubclassOf<ADuraEnemy> EnemyClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
    int32 EnemyLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
    ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
