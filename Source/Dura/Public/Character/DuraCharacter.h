// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Character/DuraCharacterBase.h"
#include "DuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API ADuraCharacter : public ADuraCharacterBase
{
	GENERATED_BODY()
	
public:
	ADuraCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilityActorInfo();
};
