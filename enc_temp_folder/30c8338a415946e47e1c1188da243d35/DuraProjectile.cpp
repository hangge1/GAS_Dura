// Copyright by person HDD  


#include "Actor/DuraProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "../Dura.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystem/DuraAbilitySystemLibrary.h>



ADuraProjectile::ADuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionObjectType(ECC_Projectile);

	SetRootComponent(Sphere);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 500.0f;
	ProjectileMovement->MaxSpeed = 550.0f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

}

void ADuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ADuraProjectile::OnSphereOverlap);
	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());

}

void ADuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) OnHit();

	Super::Destroyed();
}

void ADuraProjectile::OnHit()
{
    UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
    if( LoopingSoundComponent ) LoopingSoundComponent->Stop();
    bHit = true;
}

void ADuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor)
		return;
	
    if(!UDuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor))
        return;

	if (!bHit) OnHit();
	

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
            const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
            DamageEffectParams.DeathImpulse = DeathImpulse;

            const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
            if(bKnockback)
            {
                FRotator Rotation = GetActorRotation();
                Rotation.Pitch = 45.f;

                const FVector KnockbackDirection = Rotation.Vector();
                const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackMagnitude;
                DamageEffectParams.KnockbackForce = KnockbackForce;
            }
            
            DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
            UDuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}

		Destroy();
	}
	else
	{
		bHit = true;
	}
}
