// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class DURA_API ACheckPoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()
public:
    ACheckPoint(const FObjectInitializer& ObjectInitializer);

    /* Save Interface */
    virtual bool ShouldLoadTransform_Implementation() override { return false; }

    virtual void LoadActor_Implementation() override;
    /* End Save Interface */

    UPROPERTY(BlueprintReadOnly, SaveGame)
    bool bReached = false;
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
