// Copyright by person HDD  


#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "DuraGameplayTags.h"
#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
#include "Dura/DuraLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"



void UDuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDuraAbilitySystemComponent::ClientEffectApplied);
}

void UDuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UDuraGameplayAbility* DuraAbility = Cast<UDuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(DuraAbility->StartupInputTag);
            AbilitySpec.DynamicAbilityTags.AddTag(FDuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

    bStartupAbilitiesGiven = true;
    AbilitiesGivenDelegate.Broadcast();
}

void UDuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
    for (const TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UDuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UDuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UDuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        if(!Delegate.ExecuteIfBound(AbilitySpec))
        {
            UE_LOG(LogDura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
        }
    }
}

FGameplayTag UDuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    if(AbilitySpec.Ability)
    {
        for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
            {
                return Tag;
            }
        }
    }
    return FGameplayTag();
}

FGameplayTag UDuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
    {
        if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
        {
            return Tag;
        }
    }
    return FGameplayTag();
}

FGameplayTag UDuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
    for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
    {
        if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
        {
            return Tag;
        }
    }
    return FGameplayTag();
}

FGameplayAbilitySpec* UDuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
    FScopedAbilityListLock ActiveScopeLock(*this);
    for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
        {
            if(AbilityTag.MatchesTagExact(Tag))
            {
                return &AbilitySpec;
            }
        }
    }

    return nullptr;
}

void UDuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    if(GetAvatarActor()->Implements<UPlayerInterface>())
    {
        if(IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
        {
            ServerUpgradeAttribute(AttributeTag);
        }
    }
}

void UDuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
    FGameplayEventData Payload;
    Payload.EventTag = AttributeTag;
    Payload.EventMagnitude = 1.f;

    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

    if(GetAvatarActor()->Implements<UPlayerInterface>())
    {
        IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
    }
}

void UDuraAbilitySystemComponent::UpdateAbilityStatues(int32 Level)
{
    UAbilityInfo* AbilityInfo = UDuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
    for (const FDuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
    {
        if(!Info.AbilityTag.IsValid()) continue;
        if(Level < Info.LevelRequirement) continue;

        if(GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
        {
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
            AbilitySpec.DynamicAbilityTags.AddTag(FDuraGameplayTags::Get().Abilities_Status_Eligible);
            GiveAbility(AbilitySpec);
            MarkAbilitySpecDirty(AbilitySpec);
            ClientUpdateAbilityStatus(Info.AbilityTag, FDuraGameplayTags::Get().Abilities_Status_Eligible);
        }       
    }

    
}

void UDuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

    if(!bStartupAbilitiesGiven)
    {
        bStartupAbilitiesGiven = true;
        AbilitiesGivenDelegate.Broadcast();        
    }    
}

void UDuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
    AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}



void UDuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
