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
    void StoreOwnerPlayerController();
protected:
    
    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    FVector MouseHitLocation;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<AActor> MouseHitActor;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<APlayerController> OwnerPlayerController;
};
