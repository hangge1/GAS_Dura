// Copyright by person HDD  


#include "Character/DuraCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Dura.h"


// Sets default values
ADuraCharacterBase::ADuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector ADuraCharacterBase::GetCombatSocketLocation() const
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void ADuraCharacterBase::InitAbilityActorInfo() {}

void ADuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyAttributeInitEffectToSelf(PrimaryInitEffectClass, 1.0f);
	ApplyAttributeInitEffectToSelf(SecondaryInitEffectClass, 1.0f);
	//must put after SecondaryInit
	//caust it depend on Secondary Attribute of MaxHealth and MaxMana
	ApplyAttributeInitEffectToSelf(VitalInitEffectClass, 1.0f); 
}

void ADuraCharacterBase::ApplyAttributeInitEffectToSelf(TSubclassOf<UGameplayEffect> AttributeInitEffectClass, float Level) const
{
	check(AbilitiesSystemComponent);
	check(AttributeInitEffectClass);

	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle GameplaySpecHandle = GetAbilitySystemComponent()
		->MakeOutgoingSpec(AttributeInitEffectClass, Level, EffectContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplaySpecHandle.Data.Get(), AbilitiesSystemComponent);
}

void ADuraCharacterBase::AddCharacterAbilities()
{
	UDuraAbilitySystemComponent* DuraASC = CastChecked<UDuraAbilitySystemComponent>(AbilitiesSystemComponent);
	if (!HasAuthority()) return;

	DuraASC->AddCharacterAbilities(StartupAbilities);
}

UAbilitySystemComponent* ADuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitiesSystemComponent;
}

