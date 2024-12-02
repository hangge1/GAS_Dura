// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ActiveGameplayEffectHandle.h"
#include "DuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);


/**
 * 
 */
UCLASS()
class DURA_API UDuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

    FAbilityStatusChanged AbilityStatusChanged;

	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities);

	FEffectAssetTags EffectAssetTags;
    FAbilitiesGiven AbilitiesGivenDelegate;
    bool bStartupAbilitiesGiven = false;

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
    void ForEachAbility(const FForEachAbility& Delegate);

    static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
    static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

    FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

    void UpgradeAttribute(const FGameplayTag& AttributeTag);

    UFUNCTION(Server, Reliable)
    void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

    void UpdateAbilityStatues(int32 Level);

    UFUNCTION(Server, Reliable)
    void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);
protected:

	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, 
        const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	virtual void OnRep_ActivateAbilities() override;

    UFUNCTION(Client, Reliable)
    void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
