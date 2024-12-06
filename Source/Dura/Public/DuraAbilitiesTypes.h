#pragma once

#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "DuraAbilitiesTypes.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
    GENERATED_BODY()

    FDamageEffectParams(){}

    UPROPERTY()
    TObjectPtr<UObject> WorldContextObject = nullptr;

    UPROPERTY()
    TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

    UPROPERTY()
    float BaseDamage = 0.f;
    
    UPROPERTY()
    float AbilityLevel = 1.f;

    UPROPERTY()
    FGameplayTag DamageType = FGameplayTag();

    UPROPERTY()
    float DebuffChance = 0.f;

    UPROPERTY()
    float DebuffDamage = 0.f;

    UPROPERTY()
    float DebuffDuration = 0.f;

    UPROPERTY()
    float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FDuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(bool bInIsBlockHit) { bIsBlockedHit = bInIsBlockHit; }
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

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
