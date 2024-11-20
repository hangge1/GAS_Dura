// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraSummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UDuraSummonAbility::GetSpawnLocations()
{
    const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
    const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
    const float DeltaSpread = SpawnSpread / NumMinions;
    
    const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
    TArray<FVector> SpawnLocations;
    for(int32 i = 0; i < NumMinions; i++)
    {
        const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
        FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistnace, MaxSpawnDistnace);

        FHitResult hit;
        GetWorld()->LineTraceSingleByChannel(hit, 
            ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), 
            ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);

        if(hit.bBlockingHit)
        {
            ChosenSpawnLocation = hit.ImpactPoint;
        }

        SpawnLocations.Add(ChosenSpawnLocation);
    }
  
    return SpawnLocations;
}
