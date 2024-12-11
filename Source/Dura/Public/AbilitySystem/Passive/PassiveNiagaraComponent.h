// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
    UPassiveNiagaraComponent();

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag PassiveSpellTag;

protected:
    virtual void BeginPlay() override;
    void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);
};
