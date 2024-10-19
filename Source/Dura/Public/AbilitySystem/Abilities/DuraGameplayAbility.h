// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "DuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;
};
