// Copyright by person HDD  


#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "DuraGameplayTags.h"



void UDuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UDuraAbilitySystemComponent::EffectApplied);

	const auto& Tags = FDuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, 
		FString::Printf(TEXT("Tag: %s"), *Tags.Attributes_Secondary_Armor.ToString())
	);

}

void UDuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//GEngine->AddOnScreenDebugMessage(1, 8.f, FColor::Blue, FString("Effect Applied!"));

	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
