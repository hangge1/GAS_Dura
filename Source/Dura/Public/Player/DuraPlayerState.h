// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "DuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class DURA_API ADuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	ADuraPlayerState();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitiesSystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
