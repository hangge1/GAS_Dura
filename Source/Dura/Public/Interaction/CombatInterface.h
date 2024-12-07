// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    UAnimMontage* Montage = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag MontageTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SocketTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    USoundBase* ImpactSound = nullptr;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    
    UFUNCTION(BlueprintNativeEvent)
	int32 GetPlayerLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die(const FVector& DeathImpulse) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetAvatar();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TArray<FTaggedMontage> GetAttackMontages();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UNiagaraSystem* GetBloodEffect();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FTaggedMontage GetTaggedMontagedByTag(const FGameplayTag& MontageTag);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    int32 GetMinionCount();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void IncrementMinionCount(int32 Amount);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    ECharacterClass GetCharacterClass();

    virtual FOnASCRegistered GetOnASCRegisteredDelegate() = 0;

    virtual FOnDeath GetOnDeathDelegate() = 0;
};
