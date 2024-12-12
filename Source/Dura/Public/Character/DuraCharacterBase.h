// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "DuraCharacterBase.generated.h"



class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UMaterialInstance;
class UNiagaraSystem;
class USoundBase;
class UDebuffNiagaraComponent;
class UPassiveNiagaraComponent;

UCLASS(Abstract)
class DURA_API ADuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	ADuraCharacterBase();

    virtual void Tick(float DeltaTime) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }


	//** ICombatInterface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;

	virtual bool IsDead_Implementation() const override;

	virtual AActor* GetAvatar_Implementation() override;

	virtual void Die(const FVector& DeathImpulse) override; 

    virtual UNiagaraSystem* GetBloodEffect_Implementation() override;

    virtual FTaggedMontage GetTaggedMontagedByTag_Implementation(const FGameplayTag& MontageTag) override;

    virtual int32 GetMinionCount_Implementation() override;

    virtual void IncrementMinionCount_Implementation(int32 Amount) override;

    virtual ECharacterClass GetCharacterClass_Implementation() override;

    virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;

    virtual FOnDeathSignature& GetOnDeathDelegate() override;

    virtual FOnDamageSignature& GetOnDamageDelegate() override;

    USkeletalMeshComponent* GetWeapon_Implementation() override;

    FOnASCRegistered OnASCRegistered;
    FOnDeathSignature OnDeathDelegate;
    FOnDamageSignature OnDamageDelegate;

    virtual bool IsBeingShocked_Implementation() override;
    virtual void SetIsBeingShocked_Implementation(bool InIsBeingShocked) override;
	//** End ICombatInterface

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

    UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
    bool bIsStunned = false;

    UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
    bool bIsBurned = false;

    UPROPERTY(Replicated, BlueprintReadOnly)
    bool bIsBeingShocked = false;

    UFUNCTION()
    virtual void OnRep_Stunned();

    UFUNCTION()
    virtual void OnRep_Burned();

    

protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;

    UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandTipSocketName;

    UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandTipSocketName;

    UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;

	bool bDead = false;

    virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitiesSystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> PrimaryInitEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> SecondaryInitEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
	TSubclassOf<UGameplayEffect> VitalInitEffectClass;

	virtual void InitAbilityActorInfo();

	virtual void InitializeDefaultAttributes() const;

	void ApplyAttributeInitEffectToSelf(TSubclassOf<UGameplayEffect> AttributeInitEffectClass, float Level) const;

	void AddCharacterAbilities();

	/* Dissolve Effects */
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* WeaponDynamicMaterialInstance);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TArray<FTaggedMontage> AttackMontages;

    virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    UNiagaraSystem* BloodEffect;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
    USoundBase* DeathSound;

    /* Minions */
    
    int32 MinionCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults");
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UDebuffNiagaraComponent> StunnedDebuffComponent;
private:

	UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

    UPROPERTY(EditDefaultsOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPassiveNiagaraComponent> HaloOfProtextionNiagaraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> EffectAttachComponent;
};
