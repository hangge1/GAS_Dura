// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Interaction/HighlightInterface.h"
#include "Dura/Dura.h"
#include "CheckPoint.generated.h"

class USphereComponent;
class UStaticMeshComponent;
/**
 * 
 */
UCLASS()
class DURA_API ACheckPoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
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

    /* Highlight Interface */

    virtual void HighlightActor_Implementation() override;
    virtual void UnHighlightActor_Implementation() override;
    virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;

    /* End Hightlight Interface */

    UFUNCTION(BlueprintImplementableEvent)
    void CheckpointReached(UMaterialInstanceDynamic* MaterialInstanceDynamic);

    void HandleGlowEffects();

    UPROPERTY(BlueprintReadWrite)
    int32 CustomDepthStencilOverride = HIGHLIGHT_COLOR_TAN;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<USceneComponent> MoveToComponent;
private:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> CheckPointMesh;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> Sphere;
};
