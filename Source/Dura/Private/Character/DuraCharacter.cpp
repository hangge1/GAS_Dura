// Copyright by person HDD  


#include "Character/DuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/DuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Player/DuraPlayerController.h"
#include "UI/HUD/DuraHUD.h"

ADuraCharacter::ADuraCharacter()
{
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
}

void ADuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void ADuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
}

void ADuraCharacter::InitAbilityActorInfo()
{
	ADuraPlayerState* playerState = GetPlayerState<ADuraPlayerState>();
	check(playerState);

	AbilitiesSystemComponent = playerState->GetAbilitySystemComponent();
	AttributeSet = playerState->GetAttributeSet();

	check(AbilitiesSystemComponent);
	AbilitiesSystemComponent->InitAbilityActorInfo(playerState, this);

	if (ADuraPlayerController* PayerController = Cast<ADuraPlayerController>(GetController()))
	{
		if (ADuraHUD* HUD = Cast<ADuraHUD>(PayerController->GetHUD()))
		{
			HUD->InitOverlay(PayerController, playerState, AbilitiesSystemComponent, AttributeSet);
		}
	}

	
}
