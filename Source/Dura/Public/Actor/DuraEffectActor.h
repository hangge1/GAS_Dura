// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DuraEffectActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class DURA_API ADuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
