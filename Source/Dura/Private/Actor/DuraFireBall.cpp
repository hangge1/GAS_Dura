// Copyright by person HDD  


#include "Actor/DuraFireBall.h"

void ADuraFireBall::BeginPlay()
{
    Super::BeginPlay();
    StartOutgoingTimeline();
}

void ADuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
