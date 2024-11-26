// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Character/DuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "DuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API ADuraCharacter : public ADuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	ADuraCharacter();

    // Player Interface
    virtual void AddToXP_Implementation(int32 InXP) override;
    
    // EndPlayer Interface

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual int32 GetPlayerLevel() const override;
private:
	virtual void InitAbilityActorInfo() override;
};
