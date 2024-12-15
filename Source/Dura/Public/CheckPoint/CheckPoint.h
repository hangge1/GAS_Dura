// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class DURA_API ACheckPoint : public APlayerStart
{
	GENERATED_BODY()
public:
    ACheckPoint(const FObjectInitializer& ObjectInitializer);

protected:
    UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent)
    void CheckpointReached(UMaterialInstanceDynamic* MaterialInstanceDynamic);

    void HandleGlowEffects();
private:
    
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> CheckPointMesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Sphere;
};
