// Copyright by person HDD  


#include "CheckPoint/MapEntrance.h"
#include "CheckPoint/CheckPoint.h"
#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Game/DuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"


void AMapEntrance::LoadActor_Implementation()
{
    //Do nothing when loading a Map entrance
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor->Implements<UPlayerInterface>())
    {
        bReached = true;

        //Save World State
        if(ADuraGameModeBase* DuraGM = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
        {
            DuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
        }

        //Save Player State
        IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

        UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
    }
}

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    Sphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::HighlightActor_Implementation()
{
    
}
