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
        const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistnace, MaxSpawnDistnace);
        SpawnLocations.Add(ChosenSpawnLocation);

        DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f, 12, FColor::Cyan, false, 3.f);
        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),Location, Location + Direction * MaxSpawnDistnace, 4.0f, FLinearColor::Green, 5.0f);
        
        DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistnace, 15.f, 12, FColor::Green, false, 3.f);
        DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistnace, 15.f, 12, FColor::Green, false, 3.f);
    }

  
    return SpawnLocations;
}
