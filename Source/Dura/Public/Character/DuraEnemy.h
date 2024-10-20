// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "Character/DuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/DuraOverlayWidgetController.h"
#include "DuraEnemy.generated.h"

class UWidgetComponent;
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

	//~ Imp From ICombat Interface
	virtual int32 GetPlayerLevel() const override;
	//~ End Imp From ICombat Interface
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Defaults");
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> HealthBar;
};
