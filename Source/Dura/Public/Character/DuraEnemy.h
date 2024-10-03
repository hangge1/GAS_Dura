// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Character/DuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "DuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DURA_API ADuraEnemy : public ADuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	ADuraEnemy();

	//~ Imp From IEnemyInterface
	void HighlightActor() override;
	void UnHighlightActor() override;
	//~ end Imp From IEnemyInterface

protected:
	virtual void BeginPlay() override;
};
