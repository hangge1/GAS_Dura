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
	// Inherited via IEnemyInterface
	void HighlightActor() override;

	void UnHighlightActor() override;


protected:
	UPROPERTY(BlueprintReadOnly)
	bool bIsHighlight;
};
