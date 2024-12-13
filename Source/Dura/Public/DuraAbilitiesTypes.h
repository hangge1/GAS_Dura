#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "DuraAbilitiesTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
    GENERATED_BODY()

    FDamageEffectParams(){}

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UObject> WorldContextObject = nullptr;

    UPROPERTY(BlueprintReadWrite)
    TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

    UPROPERTY(BlueprintReadWrite)
    TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

    UPROPERTY(BlueprintReadWrite)
    float BaseDamage = 0.f;
    
    UPROPERTY(BlueprintReadWrite)
    float AbilityLevel = 1.f;

    UPROPERTY(BlueprintReadWrite)
    FGameplayTag DamageType = FGameplayTag();

    UPROPERTY(BlueprintReadWrite)
    float DebuffChance = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float DebuffDamage = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float DebuffDuration = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float DebuffFrequency = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float DeathImpulseMagnitude = 0.f;

    UPROPERTY(BlueprintReadWrite)
    FVector DeathImpulse = FVector::ZeroVector;

    UPROPERTY(BlueprintReadWrite)
    float KnockbackMagnitude = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float KnockbackChance = 0.f;

    UPROPERTY(BlueprintReadWrite)
    FVector KnockbackForce = FVector::ZeroVector;

    //RadialDamage etc..
    UPROPERTY(BlueprintReadWrite)
    bool bIsRadialDamage = false;

    UPROPERTY(BlueprintReadWrite)
    float RadialDamageInnerRadius = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float RadialDamageOuterRadius = 0.f;

    UPROPERTY(BlueprintReadWrite)
    FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FDuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
    bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
    float GetDebuffDamage() const { return DebuffDamage; }
    float GetDebuffDuration() const { return DebuffDuration; }
    float GetDebuffFrequency() const { return DebuffFrequency; }
    TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
    FVector GetDeathImpulse() const { return DeathImpulse; }
    FVector GetKnockbackForce() const { return KnockbackForce; }
    bool IsRadialDamage() const { return bIsRadialDamage; }
    float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
    float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
    FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	void SetIsBlockedHit(bool bInIsBlockHit) { bIsBlockedHit = bInIsBlockHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
    void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
    void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
    void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
    void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
    void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
    void SetDeathImpulse(FVector InDeathImpulse) { DeathImpulse = InDeathImpulse; }
    void SetKnockbackForce(FVector InKnockbackForce) { KnockbackForce = InKnockbackForce; }

    void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
    void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
    void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
    void SetRadialDamageOrigin(FVector InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}

	virtual FDuraGameplayEffectContext* Duplicate() const override
	{
		FDuraGameplayEffectContext* NewContext = new FDuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

    UPROPERTY()
    bool bIsSuccessfulDebuff = false;

    UPROPERTY()
    float DebuffDamage = 0.f;

    UPROPERTY()
    float DebuffDuration = 0.f;

    UPROPERTY()
    float DebuffFrequency = 0.f;

    TSharedPtr<FGameplayTag> DamageType;

    UPROPERTY()
    FVector DeathImpulse = FVector::ZeroVector;

    UPROPERTY()
    FVector KnockbackForce = FVector::ZeroVector;

    //RadialDamage etc..
    UPROPERTY()
    bool bIsRadialDamage = false;

    UPROPERTY()
    float RadialDamageInnerRadius = 0.f;

    UPROPERTY()
    float RadialDamageOuterRadius = 0.f;

    UPROPERTY()
    FVector RadialDamageOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits< FDuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FDuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
