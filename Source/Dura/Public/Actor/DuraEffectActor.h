// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class DURA_API ADuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
};
