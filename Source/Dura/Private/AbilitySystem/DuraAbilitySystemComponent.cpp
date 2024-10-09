// Copyright by person HDD  


#include "AbilitySystem/DuraAbilitySystemComponent.h"



void UDuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDuraAbilitySystemComponent::EffectApplied);
}

void UDuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for (const auto& tag: TagContainer)
	{
		const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *tag.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
	}
}
