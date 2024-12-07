// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
    UDebuffNiagaraComponent();

    FGameplayTag DebuffTag;

protected:
    virtual void BeginPlay() override;

    void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

    UFUNCTION()
    void OnOwnerDeath(AActor* DeadActor);
};
