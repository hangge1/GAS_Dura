// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "DuraPlayerController.generated.h"


class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
class IEnemyInterface;
class UDuraInputConfig;
class UDuraAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class DURA_API ADuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADuraPlayerController();

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


	IEnemyInterface* lastActor;
	IEnemyInterface* thisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UDuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UDuraAbilitySystemComponent> DuraAbilitySystemComponent;

	UDuraAbilitySystemComponent* GetASC();
};
