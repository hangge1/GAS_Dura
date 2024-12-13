// Copyright by person HDD  


#include "Actor/DuraFireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "GameplayCueManager.h"
#include "DuraGameplayTags.h"

void ADuraFireBall::BeginPlay()
{
    Super::BeginPlay();
    StartOutgoingTimeline();
}

void ADuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlamppedComponent, AActor* OtherActor, 
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(!IsValidOverlap(OtherActor))
        return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
            const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
            DamageEffectParams.DeathImpulse = DeathImpulse;    
            DamageEffectParams.TargetAbilitySystemComponent = TargetASC;

            UDuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

void ADuraFireBall::OnHit()
{
    if(GetOwner())
    {
        FGameplayCueParameters CueParams;
        CueParams.Location = GetActorLocation();

        //节省带宽
        UGameplayCueManager::ExecuteGameplayCue_NonReplicated(
            GetOwner(),
            FDuraGameplayTags::Get().GameplayCue_FireBlast,
            CueParams
        );
    }

    if( LoopingSoundComponent ) 
    { 
        LoopingSoundComponent->Stop();
        LoopingSoundComponent->DestroyComponent();
    } 

    bHit = true;
}
