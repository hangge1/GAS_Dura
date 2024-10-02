// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DuraPlayerController.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;

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

private:
	void Move(const FInputActionValue& InputValue);

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;


};
