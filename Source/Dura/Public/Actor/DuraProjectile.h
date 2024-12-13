// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DuraAbilitiesTypes.h"
#include "DuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class DURA_API ADuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ADuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

    UPROPERTY()
    TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	
	virtual void Destroyed() override;

    UFUNCTION(BlueprintCallable)
    virtual void OnHit();

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    bool IsValidOverlap(AActor* OtherActor);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

    UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

    bool bHit = false;
private:
	

	UPROPERTY(EditAnywhere)
	float LifeSpan = 15.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
};
