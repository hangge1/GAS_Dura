// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "DuraEnemySpawnVolume.generated.h"

class UBoxComponent;
class ADuraEnemySpawnPoint;

UCLASS()
class DURA_API ADuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	ADuraEnemySpawnVolume();

    /* Save Interface */

    virtual bool ShouldLoadTransform_Implementation() override;
    virtual void LoadActor_Implementation() override;

    /* End Save Interface */

    UPROPERTY(BlueprintReadOnly, SaveGame)
    bool bReached = false;
protected:
	virtual void BeginPlay() override;

    UFUNCTION()
    virtual void OnBoxOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    UPROPERTY(EditAnywhere)
    TArray<ADuraEnemySpawnPoint*> SpawnPoints;
private:
    
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> Box;
};
