// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API UElectrocute : public UDuraBeamSpell
{
	GENERATED_BODY()
public:
      
    virtual FString GetDescription(int32 Level) override;
    virtual FString GetNextLevelDescription(int32 Level) override;	
};
