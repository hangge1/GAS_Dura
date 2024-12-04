// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DuraProjectileSpell.h"
#include "DuraFireBolt.generated.h"


UCLASS()
class DURA_API UDuraFireBolt : public UDuraProjectileSpell
{
	GENERATED_BODY()
public:
        
    virtual FString GetDescription(int32 Level) override;
    virtual FString GetNextLevelDescription(int32 Level) override;

};
