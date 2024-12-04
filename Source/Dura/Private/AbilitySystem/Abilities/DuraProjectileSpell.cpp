// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraProjectileSpell.h"
#include "Actor/DuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "DuraGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

FString UDuraProjectileSpell::GetDescription(int32 Level)
{   
    const float Damage = DamageTypes[FDuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
    if(Level == 1)
    {
        return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launched a bolt of fire, exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), (int32)Damage, Level);
    }
    else
    {
        return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launched %d bolts of fire, exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), (int32)Damage, Level);
    }
}

FString UDuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
    const float Damage = DamageTypes[FDuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
    return FString::Printf(TEXT("<Title>Next Level: </>\n\n<Default>Launched %d bolts of fire, exploding on impact and dealing </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), (int32)Damage, Level);
}

void UDuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UDuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}

	if (GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
            GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		if(bOverridePitch)
        {
            Rotation.Pitch = PitchOverride;
        }


		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		AActor* Owner = GetOwningActorFromActorInfo();
		ADuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			Owner,
			Cast<APawn>(Owner),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		FGameplayEffectContextHandle EfeectContextHandle = SourceASC->MakeEffectContext();
		EfeectContextHandle.SetAbility(this); 
		EfeectContextHandle.AddSourceObject(Projectile);
		TArray<TWeakObjectPtr<AActor>> Actors;
		Actors.Add(Projectile);
		EfeectContextHandle.AddActors(Actors);
		FHitResult HiResult;
		HiResult.Location = ProjectileTargetLocation;
		EfeectContextHandle.AddHitResult(HiResult);


		const FGameplayEffectSpecHandle EffectSpecHandler = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), EfeectContextHandle
		);

		const FDuraGameplayTags GameplayTags = FDuraGameplayTags::Get();

		for (auto& Pair : DamageTypes)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandler, Pair.Key, ScaledDamage);
		}
		Projectile->DamageEffectSpecHandle = EffectSpecHandler;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
