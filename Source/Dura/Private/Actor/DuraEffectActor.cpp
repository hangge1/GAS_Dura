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
	
	FGameplayEffectSpecHandle ESHandler = ASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, ECHandler);
	ASC->ApplyGameplayEffectSpecToSelf(*ESHandler.Data.Get());
}


