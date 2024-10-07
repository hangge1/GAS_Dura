// Copyright by person HDD  


#include "Actor/DuraEffectActor.h"
#include "Components/SceneComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"



ADuraEffectActor::ADuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void ADuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent*  ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (ASC == nullptr)
	{
		return;
	}

	check(GameplayEffectClass);

	FGameplayEffectContextHandle ECHandler = ASC->MakeEffectContext();
	ECHandler.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle ESHandler = ASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, ECHandler);
	const FActiveGameplayEffectHandle ActiveEffectHandler = ASC->ApplyGameplayEffectSpecToSelf(*ESHandler.Data.Get());

	const bool bIsInfinite = ESHandler.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		ActiveHandlers.Add(ActiveEffectHandler, ASC);
	}
	
}

void ADuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void ADuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(ASC)) return;

		TArray<FActiveGameplayEffectHandle> Handlers;
		for (const auto& pair : ActiveHandlers)
		{
			if (pair.Value == ASC)
			{
				ASC->RemoveActiveGameplayEffect(pair.Key, 1);
				Handlers.Add(pair.Key);
			}
		}

		for (const auto& handler : Handlers)
		{
			ActiveHandlers.FindAndRemoveChecked(handler);
		}
	}
}


