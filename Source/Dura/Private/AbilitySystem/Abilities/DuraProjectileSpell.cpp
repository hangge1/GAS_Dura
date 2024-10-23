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

void UDuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;


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
		EfeectContextHandle.AddHitResult(HiResult);



		const FGameplayEffectSpecHandle EffectSpecHandler = SourceASC->MakeOutgoingSpec(
			DamageEffectClass, GetAbilityLevel(), EfeectContextHandle
		);

		const FDuraGameplayTags GameplayTags = FDuraGameplayTags::Get();
		const float ScaledDamage = Damage.GetValueAtLevel(/*GetAbilityLevel()*/20);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandler, GameplayTags.Damage, ScaledDamage);
		Projectile->DamageEffectSpecHandle = EffectSpecHandler;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
