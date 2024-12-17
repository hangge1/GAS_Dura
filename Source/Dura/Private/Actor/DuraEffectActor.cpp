// Copyright by person HDD  


#include "Actor/DuraEffectActor.h"
#include "Components/SceneComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "Kismet/KismetMathLibrary.h"



ADuraEffectActor::ADuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}


void ADuraEffectActor::Tick(float DelTaTime)
{
    Super::Tick(DelTaTime);

    RunningTime += DelTaTime;

    const float SinPeriod = 2 * PI / SinePeriodConstant;
    if(RunningTime > SinPeriod)
    {
        RunningTime = 0;
    }

    ItemMovement(DelTaTime);
}

void ADuraEffectActor::ItemMovement(float DeltaTime)
{
    if(bRotates)
    {
        const FRotator DeltaRotation(0.f, DeltaTime * RotationRate, 0.f);
        CalculatedRotation = UKismetMathLibrary::ComposeRotators(CalculatedRotation, DeltaRotation);    
    }

    if(bSinusoidalMovement)
    {
        const float Sine = SineAmplitude * FMath::Sin(RunningTime * SinePeriodConstant);
        CalculatedLocation = InitialLocation + FVector(0.f, 0.f, Sine);
    }
}


void ADuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
    InitialLocation = GetActorLocation();
    CalculatedLocation = InitialLocation;
    CalculatedRotation = GetActorRotation();
}

void ADuraEffectActor::StartSinusoidalMovement()
{
    bSinusoidalMovement = true;
    InitialLocation = GetActorLocation();
    CalculatedLocation = InitialLocation;
}

void ADuraEffectActor::StartRotation()
{
    bRotates = true;
    CalculatedRotation = GetActorRotation();
}

void ADuraEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (!bApplyEffectsToEnemies && Target->ActorHasTag(FName("Enemy")))
	{
		return;
	}

	UAbilitySystemComponent*  ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (ASC == nullptr)
	{
		return;
	}

	check(GameplayEffectClass);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandler = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);

	const bool bIsInfinite = EffectSpecHandle.Data->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if (bIsInfinite && InfiniteEffectRemovePolicy == EEffectRemovePolicy::RemoveOnEndOverlap)
	{
		ActiveHandlers.Add(ActiveEffectHandler, ASC);
	}
	
	if (bDestroyOnEffectApplication && !bIsInfinite)
	{
		Destroy();
	}
}

void ADuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (!bApplyEffectsToEnemies && TargetActor->ActorHasTag(FName("Enemy")))
	{
		return;
	}

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
	if (!bApplyEffectsToEnemies && TargetActor->ActorHasTag(FName("Enemy")))
	{
		return;
	}

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



