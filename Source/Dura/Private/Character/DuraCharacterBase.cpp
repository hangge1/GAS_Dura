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

UAnimMontage* ADuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ADuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); //auto replicated
	MulticastHandleDeath();
}

void ADuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetEnableGravity(true);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	bDead = true;
}

// Called when the game starts or when spawned
void ADuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector ADuraCharacterBase::GetCombatSocketLocation_Implementation() const
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

bool ADuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ADuraCharacterBase::GetAvatar_Implementation()
{
	return this;
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

void ADuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

UAbilitySystemComponent* ADuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitiesSystemComponent;
}

