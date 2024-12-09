// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraDamageGameplayAbility.h"
#include "DuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraBeamSpell : public UDuraDamageGameplayAbility
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void StoreMouseDataInfo(const FHitResult& HitResult);
    
    UFUNCTION(BlueprintCallable)
    void StoreOwnerVariables();

    UFUNCTION(BlueprintCallable)
    void TraceFirstTarget(const FVector& BeamTargetLocation);

    UFUNCTION(BlueprintCallable)
    void StoreAdditionalTargets(TArray<AActor*>& OutAddditionalTargets);

    UFUNCTION(BlueprintImplementableEvent)
    void PrimaryTargetDied(AActor* DeadActor);

    UFUNCTION(BlueprintImplementableEvent)
    void AdditianalTargetDied(AActor* DeadActor);
protected:
    
    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    FVector MouseHitLocation;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<AActor> MouseHitActor;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<APlayerController> OwnerPlayerController;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<ACharacter> OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Beam")
    int32 MaxNumShockTargets = 5;
};
