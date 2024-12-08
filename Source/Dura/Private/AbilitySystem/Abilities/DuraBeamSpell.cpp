// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraBeamSpell.h"
#include "GameFramework/Character.h"

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
