// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DuraCharacterBase.generated.h"

class USkeletalMeshComponent;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class DURA_API ADuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADuraCharacterBase();

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(VisibleAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;


	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitiesSystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
