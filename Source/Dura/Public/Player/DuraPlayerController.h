// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "DuraPlayerController.generated.h"


class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class IHighlightInterface;
class UDuraInputConfig;
class UDuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class UNiagaraSystem;
class AMagicCircle;

enum class ETargetingStatus : uint8
{
    TargetingEnemy,
    TargetingNotEnemy,
    NotTargeting
};

/**
 * 
 */
UCLASS()
class DURA_API ADuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADuraPlayerController();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

    UFUNCTION(BlueprintCallable)
    void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

    UFUNCTION(BlueprintCallable)
    void HideMagicCircle();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;

private:
	void Move(const FInputActionValue& InputValue);

	void MouseTrace();
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }
	bool bShiftKeyDown = false;


	TObjectPtr<AActor> lastActor;
	TObjectPtr<AActor> thisActor;
	FHitResult hitResult;
    void HighlightActor(AActor* InActor);
    void UnHighlightActor(AActor* InActor);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UDuraAbilitySystemComponent> DuraAbilitySystemComponent;

	UDuraAbilitySystemComponent* GetASC();

	// Click to Move 
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.0f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
    ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

    UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AMagicCircle> MagicCircleClass;

    UPROPERTY()
    TObjectPtr<AMagicCircle> MagicCiecle;

    void UpdateMagicCircleLocation();
};
