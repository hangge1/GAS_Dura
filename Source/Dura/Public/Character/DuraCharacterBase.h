// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuraCharacterBase.generated.h"

UCLASS(Abstract)
class DURA_API ADuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADuraCharacterBase();

protected:
	virtual void BeginPlay() override;
};
