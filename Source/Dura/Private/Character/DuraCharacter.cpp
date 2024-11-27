// Copyright by person HDD  


#include "Character/DuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/DuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Player/DuraPlayerController.h"
#include "UI/HUD/DuraHUD.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ADuraCharacter::ADuraCharacter()
{
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->SetUsingAbsoluteRotation(true);
    CameraBoom->bDoCollisionTest = false;

    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false;

    LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
    LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
    LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

    CharacterClass = ECharacterClass::Elementalist;
}

void ADuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    DuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void ADuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    DuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void ADuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    DuraPlayerState->AddToLevel(InPlayerLevel);
}

void ADuraCharacter::AddToXP_Implementation(int32 InXP)
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    DuraPlayerState->AddToXP(InXP);
}

void ADuraCharacter::LevelUp_Implementation()
{
    MulticastLevelUpParticles();
}

int32 ADuraCharacter::GetXP_Implementation() const
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    return DuraPlayerState->GetXP();
}

int32 ADuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    return DuraPlayerState->LevelUpInfoDataAsset->FindLevelForXP(InXP);
}

int32 ADuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    return DuraPlayerState->LevelUpInfoDataAsset->LevelUpInformation[Level].AttributePointAward;
}

int32 ADuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
    return DuraPlayerState->LevelUpInfoDataAsset->LevelUpInformation[Level].SpellPointAward;
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

void ADuraCharacter::MulticastLevelUpParticles_Implementation() const
{
    if(IsValid(LevelUpNiagaraComponent))
    {
        const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
        const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
        const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();

        LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
        LevelUpNiagaraComponent->Activate(true);
    }
}
