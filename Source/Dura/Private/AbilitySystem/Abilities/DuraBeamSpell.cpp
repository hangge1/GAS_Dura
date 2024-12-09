// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraBeamSpell.h"
#include "GameFramework/Character.h"
#include <Interaction\CombatInterface.h>
#include <Kismet\KismetSystemLibrary.h>
#include <AbilitySystem\DuraAbilitySystemLibrary.h>

void UDuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
    if(HitResult.bBlockingHit)
    {
        MouseHitLocation = HitResult.ImpactPoint;
        MouseHitActor = HitResult.GetActor();
    }
    else
    {
        CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
    }
}

void UDuraBeamSpell::StoreOwnerVariables()
{
    
    if(CurrentActorInfo)
    {
        OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
        OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
    }
}

void UDuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
    check(OwnerCharacter);
    if(OwnerCharacter->Implements<UCombatInterface>())
    {
        if(USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
        {
            TArray<AActor*> ActorsToIgnore;
            ActorsToIgnore.Add(OwnerCharacter);
            FHitResult HitResult;
            const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
            UKismetSystemLibrary::SphereTraceSingle(
                OwnerCharacter, 
                SocketLocation, 
                BeamTargetLocation, 
                10.f, 
                TraceTypeQuery1, 
                false, 
                ActorsToIgnore, 
                EDrawDebugTrace::None, 
                HitResult, 
                true
            );

            if(HitResult.bBlockingHit)
            {
                MouseHitLocation = HitResult.ImpactPoint;
                MouseHitActor = HitResult.GetActor();
            }
        }
    }
    
}

void UDuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAddditionalTargets)
{  
    TArray<AActor*> IgnoreActors;
    IgnoreActors.Add(GetAvatarActorFromActorInfo());
    IgnoreActors.Add(MouseHitActor);

    TArray<AActor*> OverlappingActors;
    UDuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), 
        OverlappingActors, 
        IgnoreActors, 
        850,
        MouseHitLocation
    );

    //int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
    int32 NumAdditionalTargets = 5;
    UDuraAbilitySystemLibrary::GetClosetTargets(NumAdditionalTargets, OverlappingActors, 
        MouseHitActor->GetActorLocation(), OutAddditionalTargets);
    

}
