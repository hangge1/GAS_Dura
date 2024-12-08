// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraFireBolt.h"
#include "Actor/DuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "DuraGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"

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

        TArray<FRotator> Rotators = UDuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

        for (const FRotator& Rot : Rotators)
        {
            FTransform SpawnTransform;
		    SpawnTransform.SetLocation(SocketLocation);
		    SpawnTransform.SetRotation(Rot.Quaternion());

		    AActor* Owner = GetOwningActorFromActorInfo();
		    ADuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADuraProjectile>(
			    ProjectileClass,
			    SpawnTransform,
			    Owner,
			    Cast<APawn>(Owner),
			    ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		    );

            Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		    Projectile->FinishSpawning(SpawnTransform);
        }
    }

}
