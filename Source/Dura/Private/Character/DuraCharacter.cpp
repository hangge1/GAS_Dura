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
#include "DuraGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

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

void ADuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial /*= nullptr*/)
{
    if (ADuraPlayerController* PlayerController = Cast<ADuraPlayerController>(GetController()))
	{
        PlayerController->ShowMagicCircle(DecalMaterial);
        PlayerController->bShowMouseCursor = false;
    }
}

void ADuraCharacter::HideMagicCircle_Implementation()
{
    if (ADuraPlayerController* PlayerController = Cast<ADuraPlayerController>(GetController()))
	{
        PlayerController->HideMagicCircle();
        PlayerController->bShowMouseCursor = true;
    }
}

void ADuraCharacter::OnRep_Stunned()
{
    UDuraAbilitySystemComponent* DuraASC = Cast<UDuraAbilitySystemComponent>(AbilitiesSystemComponent);
    if(DuraASC)
    {
        FGameplayTagContainer BlockTags;
        BlockTags.AddTag(FDuraGameplayTags::Get().Player_Block_CursorTrace);
        BlockTags.AddTag(FDuraGameplayTags::Get().Player_Block_InputHeld);
        BlockTags.AddTag(FDuraGameplayTags::Get().Player_Block_InputPressed);
        BlockTags.AddTag(FDuraGameplayTags::Get().Player_Block_InputReleased);
        
        if(bIsStunned)
        {
            DuraASC->AddLooseGameplayTags(BlockTags);
            StunnedDebuffComponent->Activate();
        }
        else
        {
            DuraASC->RemoveLooseGameplayTags(BlockTags);
            StunnedDebuffComponent->Deactivate();
        }
    }
}

void ADuraCharacter::OnRep_Burned()
{
    if(bIsBurned)
    {
        BurnDebuffComponent->Activate();
    }
    else
    {
        BurnDebuffComponent->Deactivate();
    }
}

int32 ADuraCharacter::GetSpellPoints_Implementation() const
{
     ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
     return DuraPlayerState->GetSpellPoints();   
}

int32 ADuraCharacter::GetAttributePoints_Implementation() const
{
     ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(GetPlayerState());
     return DuraPlayerState->GetAttributePoints();
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

    UDuraAbilitySystemComponent* DuraASC = CastChecked<UDuraAbilitySystemComponent>(GetAbilitySystemComponent());
    DuraASC->UpdateAbilityStatues(DuraPlayerState->GetPlayerLevel());
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

    OnASCRegistered.Broadcast(AbilitiesSystemComponent);
    AbilitiesSystemComponent->RegisterGameplayTagEvent(FDuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved)
    .AddUObject(this, &ADuraCharacter::StunTagChanged);


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
