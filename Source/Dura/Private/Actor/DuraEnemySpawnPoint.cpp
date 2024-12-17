// Copyright by person HDD  


#include "Actor/DuraEnemySpawnPoint.h"
#include "Character/DuraEnemy.h"

void ADuraEnemySpawnPoint::SpawnEnemy()
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    ADuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<ADuraEnemy>(EnemyClass, GetActorTransform());
    Enemy->SetLevel(EnemyLevel);
    Enemy->SetCharacterClass(CharacterClass);
    Enemy->FinishSpawning(GetActorTransform());
    Enemy->SpawnDefaultController();
}
