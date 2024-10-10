// Copyright by person HDD  


#include "Character/DuraCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"


// Sets default values
ADuraCharacterBase::ADuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ADuraCharacterBase::InitAbilityActorInfo()
{
}

void ADuraCharacterBase::ApplyPrimaryAttributeInitEffect()
{
	check(AbilitiesSystemComponent);
	check(PrimaryInitEffectClass);

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle GameplaySpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(PrimaryInitEffectClass, 1.0f, EffectContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplaySpecHandle.Data.Get(), AbilitiesSystemComponent);

}

UAbilitySystemComponent* ADuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitiesSystemComponent;
}

