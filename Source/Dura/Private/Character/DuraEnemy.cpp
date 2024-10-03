// Copyright by person HDD  


#include "Character/DuraEnemy.h"
#include "../Dura.h"

#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystem/DuraAttributeSet.h"

ADuraEnemy::ADuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitiesSystemComponent = CreateDefaultSubobject<UDuraAbilitySystemComponent>("AbilitiesSystemComponent");
	AbilitiesSystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UDuraAttributeSet>("AttributeSet");
}

void ADuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(HIGHLIGHT_COLOR_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(HIGHLIGHT_COLOR_RED);
}

void ADuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
