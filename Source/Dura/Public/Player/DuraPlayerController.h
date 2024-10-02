// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DuraPlayerController.generated.h"

class UInputMappingContext;

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

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> DuraContext;
};
