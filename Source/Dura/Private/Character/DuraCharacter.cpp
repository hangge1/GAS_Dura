// Copyright by person HDD  


#include "Character/DuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/DuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Player/DuraPlayerController.h"
#include "UI/HUD/DuraHUD.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"

ADuraCharacter::ADuraCharacter()
{
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

    CharacterClass = ECharacterClass::Elementalist;
}

void ADuraCharacter::AddToXP_Implementation(int32 InXP)
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    DuraPlayerState->AddToXP(InXP);
}

void ADuraCharacter::LevelUp_Implementation()
{
    
}

void ADuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

    //服务器端, 初始化ASC
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ADuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

    if(!HasAuthority())
    {
        //客户端, 初始化ASC
	    InitAbilityActorInfo();
    }
    
}

int32 ADuraCharacter::GetPlayerLevel_Implementation() const
{
	ADuraPlayerState* playerState = GetPlayerState<ADuraPlayerState>();
	check(playerState);

	return playerState->GetPlayerLevel();
}

void ADuraCharacter::InitAbilityActorInfo()
{
	ADuraPlayerState* playerState = GetPlayerState<ADuraPlayerState>();
	check(playerState);

	AbilitiesSystemComponent = playerState->GetAbilitySystemComponent();
	AttributeSet = playerState->GetAttributeSet();

	check(AbilitiesSystemComponent);
	AbilitiesSystemComponent->InitAbilityActorInfo(playerState, this);

	Cast<UDuraAbilitySystemComponent>(playerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	if (ADuraPlayerController* PlayerController = Cast<ADuraPlayerController>(GetController()))
	{
		if (ADuraHUD* HUD = Cast<ADuraHUD>(PlayerController->GetHUD()))
		{
			HUD->InitOverlay(PlayerController, playerState, AbilitiesSystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}
