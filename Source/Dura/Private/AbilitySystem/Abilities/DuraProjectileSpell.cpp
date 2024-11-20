// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraProjectileSpell.h"
#include "Actor/DuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "DuraGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

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

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
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
