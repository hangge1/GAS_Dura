// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraBeamSpell.h"

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

void UDuraBeamSpell::StoreOwnerPlayerController()
{
    if(CurrentActorInfo)
    {
        OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
    }
}
