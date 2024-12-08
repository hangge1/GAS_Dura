// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraFireBolt.h"
#include "Actor/DuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "DuraGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

FString UDuraFireBolt::GetDescription(int32 Level)
{   
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    if(Level == 1)
    {
        return FString::Printf(TEXT(
            //Title
            "<Title>FIRE BOLT</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

             //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            "\n<Default>Launched a bolt of fire, exploding on impact and dealing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> fire damage with a chance to burn</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            DamageValue);
    }
    else
    {
        return FString::Printf(TEXT(
            //Title
            "<Title>FIRE BOLT</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

            //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Number of FireBolts
            "\n<Default>Launched %d a bolt of fire, exploding on impact and dealing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> fire damage with a chance to burn</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            FMath::Min(Level, NumProjectiles), 
            DamageValue);
    }
}

FString UDuraFireBolt::GetNextLevelDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    return FString::Printf(TEXT(
            //Title
            "<Title>Next Level: </>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

            //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Number of FireBolts
            "\n<Default>Launched %d a bolt of fire, exploding on impact and dealing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> fire damage with a chance to burn</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            FMath::Min(Level, NumProjectiles), 
            DamageValue);
}

void UDuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, 
    bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
    const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
            GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		if(bOverridePitch) Rotation.Pitch = PitchOverride;


        const FVector Forward = Rotation.Vector();
        const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
        const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

        //NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());     
        if(NumProjectiles > 1)
        {
            const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
            const FVector Start = SocketLocation + FVector(0, 0, 5);
            for (int32 i = 0; i < NumProjectiles; i++)
            {
                const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
                UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, 
                Start + Direction * 75.0f, 1, FLinearColor::Red, 120, 1);
            }
        }
        else
        {
            //Single projectile
            const FVector Start = SocketLocation + FVector(0, 0, 10);
            UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, 
                Start + Forward * 75.0f, 1, FLinearColor::Green, 120, 1);
        }

        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, 
        SocketLocation + Rotation.Vector() * 100.0f, 1, FLinearColor::White, 120, 2);
        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, 
        SocketLocation + LeftOfSpread * 100.0f, 1, FLinearColor::Gray, 120, 2);
        UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, 
        SocketLocation + RightOfSpread * 100.0f, 1, FLinearColor::Gray, 120, 2);
    }
}
