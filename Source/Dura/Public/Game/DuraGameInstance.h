// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DuraGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UDuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
    
    UPROPERTY()
    FName PlayerStartTag = FName();

    UPROPERTY()
    FString LoadSlotName = FString();

    UPROPERTY()
    int32 LoadSlotIndex = 0;
};
