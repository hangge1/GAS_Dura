// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Actor/DuraProjectile.h"
#include "DuraFireBall.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API ADuraFireBall : public ADuraProjectile
{
	GENERATED_BODY()
	
public:

protected:
    virtual void BeginPlay() override;

    virtual void OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};