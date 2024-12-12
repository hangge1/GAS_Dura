// Copyright by person HDD  

#include "Character/DuraCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "DuraGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "../Dura.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

// Sets default values
ADuraCharacterBase::ADuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

    BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
    BurnDebuffComponent->SetupAttachment(GetRootComponent());
    BurnDebuffComponent->DebuffTag = FDuraGameplayTags::Get().Debuff_Burn;

    StunnedDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunnedDebuffComponent");
    StunnedDebuffComponent->SetupAttachment(GetRootComponent());
    StunnedDebuffComponent->DebuffTag = FDuraGameplayTags::Get().Debuff_Stun;
    

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
    

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


    EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachComponent");
    EffectAttachComponent->SetupAttachment(GetRootComponent());

    HaloOfProtextionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtextionNiagaraComponent");
    HaloOfProtextionNiagaraComponent->SetupAttachment(EffectAttachComponent);
    HaloOfProtextionNiagaraComponent->PassiveSpellTag = FDuraGameplayTags::Get().Abilities_Passive_HaloOfProtection;

    LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
    LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
    LifeSiphonNiagaraComponent->PassiveSpellTag = FDuraGameplayTags::Get().Abilities_Passive_LifeSiphon;

    ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
    ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
    ManaSiphonNiagaraComponent->PassiveSpellTag = FDuraGameplayTags::Get().Abilities_Passive_ManaSiphon;


}

void ADuraCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
}

void ADuraCharacterBase::GetLifetimeReplicatedProps(TArray< class FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADuraCharacterBase, bIsStunned);
    DOREPLIFETIME(ADuraCharacterBase, bIsBurned);
    DOREPLIFETIME(ADuraCharacterBase, bIsBeingShocked);
}

float ADuraCharacterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float DamageTaken = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    OnDamageDelegate.Broadcast(DamageTaken);
    return DamageTaken;
}

UAnimMontage* ADuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ADuraCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true)); //auto replicated
	MulticastHandleDeath(DeathImpulse);
}

UNiagaraSystem* ADuraCharacterBase::GetBloodEffect_Implementation()
{
    return BloodEffect;
}

FTaggedMontage ADuraCharacterBase::GetTaggedMontagedByTag_Implementation(const FGameplayTag& MontageTag)
{
    for (const FTaggedMontage& TaggedMontage : AttackMontages)
    {
        if(TaggedMontage.MontageTag == MontageTag)
        {
            return TaggedMontage;
        }
    }

    return FTaggedMontage();
}

int32 ADuraCharacterBase::GetMinionCount_Implementation()
{
    return MinionCount;
}

void ADuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
    MinionCount += Amount;
}

ECharacterClass ADuraCharacterBase::GetCharacterClass_Implementation()
{
    return CharacterClass;
}

bool ADuraCharacterBase::IsBeingShocked_Implementation()
{
    return bIsBeingShocked;
}

void ADuraCharacterBase::SetIsBeingShocked_Implementation(bool InIsBeingShocked)
{
    bIsBeingShocked = InIsBeingShocked;
}

void ADuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
    UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());

	Weapon->SetSimulatePhysics(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    Weapon->SetEnableGravity(true);
    Weapon->AddImpulse(DeathImpulse * 0.1, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	bDead = true;

    BurnDebuffComponent->Deactivate();
    StunnedDebuffComponent->Deactivate();

    OnDeathDelegate.Broadcast(this);
}

void ADuraCharacterBase::OnRep_Stunned()
{
    
}

void ADuraCharacterBase::OnRep_Burned()
{
    
}

void ADuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    bIsStunned = NewCount > 0;
    GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.0f : BaseWalkSpeed;
}

FOnASCRegistered& ADuraCharacterBase::GetOnASCRegisteredDelegate()
{
    return OnASCRegistered;
}

FOnDeathSignature& ADuraCharacterBase::GetOnDeathDelegate()
{
    return OnDeathDelegate;
}

FOnDamageSignature& ADuraCharacterBase::GetOnDamageDelegate()
{
    return OnDamageDelegate;
}

USkeletalMeshComponent* ADuraCharacterBase::GetWeapon_Implementation()
{
    return Weapon;
}

// Called when the game starts or when spawned
void ADuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector ADuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();

    if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon))
    {
        return Weapon->GetSocketLocation(WeaponTipSocketName);
    }

    if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
    {
        return GetMesh()->GetSocketLocation(LeftHandTipSocketName);
    }

    if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
    {
        return GetMesh()->GetSocketLocation(RightHandTipSocketName);
    }

    if(MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
    {
        return GetMesh()->GetSocketLocation(TailSocketName);
    }

    return FVector();
}

bool ADuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ADuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

void ADuraCharacterBase::InitAbilityActorInfo() 
{
    
}

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
	DuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);  
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

TArray<FTaggedMontage> ADuraCharacterBase::GetAttackMontages_Implementation()
{
    return AttackMontages;
}

UAbilitySystemComponent* ADuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitiesSystemComponent;
}

