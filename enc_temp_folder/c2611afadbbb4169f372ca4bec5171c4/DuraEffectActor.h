// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "DuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovePolicy
{
	RemoveOnEndOverlap,
	DoNotRmove
};

UCLASS()
class DURA_API ADuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADuraEffectActor();

    virtual void Tick(float DelTaTime) override;
protected:
	virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly)
    FVector CalculatedLocation;

    UPROPERTY(BlueprintReadOnly)
    FRotator CalculatedRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    bool bRotates = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    float RotationRate = 45.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    bool bSinusoidalMovement = false;

    UFUNCTION(BlueprintCallable)
    void StartSinusoidalMovement();

    UFUNCTION(BlueprintCallable)
    void StartRotation();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    float SineAmplitude = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    float SinePeriodConstant = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
    FVector InitialLocation;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	//Config Instant GameplayEffect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//Config Duration GameplayEffect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	//Config Infinite GameplayEffect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovePolicy InfiniteEffectRemovePolicy = EEffectRemovePolicy::RemoveOnEndOverlap;


	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveHandlers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.0f;

private:
    float RunningTime = 0.f;

    void ItemMovement(float DeltaTime);
};
