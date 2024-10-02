// Copyright by person HDD  


#include "Character/DuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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
