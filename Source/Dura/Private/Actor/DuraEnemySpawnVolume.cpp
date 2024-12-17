// Copyright by person HDD  


#include "Actor/DuraEnemySpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Actor/DuraEnemySpawnPoint.h"

ADuraEnemySpawnVolume::ADuraEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

    Box = CreateDefaultSubobject<UBoxComponent>("Box");
    SetRootComponent(Box);

    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionObjectType(ECC_WorldStatic);
    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void ADuraEnemySpawnVolume::LoadActor_Implementation()
{
    if(bReached)
    {
        Destroy();
    }
}

bool ADuraEnemySpawnVolume::ShouldLoadTransform_Implementation()
{
    return true;
}

void ADuraEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
    Box->OnComponentBeginOverlap.AddDynamic(this, &ADuraEnemySpawnVolume::OnBoxOverlap);
}

void ADuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!OtherActor->Implements<UPlayerInterface>())
    {
        return;
    }

    bReached = true;
    for (ADuraEnemySpawnPoint* Point : SpawnPoints)
    {
        if(IsValid(Point))
        {
            Point->SpawnEnemy();
        }
    }

    Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

