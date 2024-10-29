// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class DURA_API ADuraAIController : public AAIController
{
	GENERATED_BODY()
public:
	ADuraAIController();

protected:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
};
