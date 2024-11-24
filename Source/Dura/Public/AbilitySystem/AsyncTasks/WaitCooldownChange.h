// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeReamining);

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class DURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:   
    UPROPERTY(BlueprintAssignable)
    FCooldownChangeSignature CooldownStart;

    UPROPERTY(BlueprintAssignable)
    FCooldownChangeSignature CooldownEnd;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, 
        const FGameplayTag& InCooldownTag);


    UFUNCTION(BlueprintCallable)
    void EndTask();

protected:
    
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> ASC;

    FGameplayTag CooldownTag;

    void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

    void OnActiveEffectAdded(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& GameplayEffectSpec, 
        FActiveGameplayEffectHandle ActiveEffectHandle);
};
