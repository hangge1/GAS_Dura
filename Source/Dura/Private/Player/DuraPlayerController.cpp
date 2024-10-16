// Copyright by person HDD  


#include "Player/DuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/DuraEnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"


ADuraPlayerController::ADuraPlayerController()
{
	bReplicates = true;
}

void ADuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(DuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(DuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI Inputmode;
	Inputmode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Inputmode.SetHideCursorDuringCapture(false);
	SetInputMode(Inputmode);
}

void ADuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputConfig);

	UDuraEnhancedInputComponent* DurainputComponent = CastChecked<UDuraEnhancedInputComponent>(InputComponent);
	check(MoveAction);

	DurainputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADuraPlayerController::Move);
	DurainputComponent->BindAbilityActions(InputConfig, this, &ADuraPlayerController::AbilityInputTagPressed,
		&ADuraPlayerController::AbilityInputTagReleased, &ADuraPlayerController::AbilityInputTagHeld);
}

void ADuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MouseTrace();
}

void ADuraPlayerController::Move(const FInputActionValue& InputValue)
{
	FVector2d AxisValue = InputValue.Get<FVector2d>();

	FRotator CtlRotation = GetControlRotation();
	CtlRotation.Pitch = 0.0f;
	CtlRotation.Roll = 0.0f;

	FVector ForwardVector = FRotationMatrix(CtlRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(CtlRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlPawn = GetPawn())
	{
		ControlPawn->AddMovementInput(ForwardVector, AxisValue.Y);
		ControlPawn->AddMovementInput(RightVector, AxisValue.X);
	}
}

void ADuraPlayerController::MouseTrace()
{
	FHitResult hitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);

	if (!hitResult.bBlockingHit) return;

	lastActor = thisActor;
	thisActor = Cast<IEnemyInterface>(hitResult.GetActor());

	/*
	* We Can Do sth by condition of whether lastActor is null and thisActor is null
	* 
	* if lastActor == null && thisActor == null
	*	- Do nothing
	* 
	* if lastActor == null && thisActor != null
	*	- thisActor->Highlight
	* 
	* if lastActor != null && thisActor == null
	*   - lastActor->UnHighlight
	* 
	* if lastActor != null && thisActor != null
	* 
	*	- if lastActor == thisActor
	*		- do nothing
	* 
	*   - if lastActor != thisActor
	*		- lastActor->UnHighlight && thisActor->Highlight 
	*/

	if (lastActor == nullptr)
	{
		if (thisActor == nullptr)
		{
			//do nothing
		}
		else
		{
			thisActor->HighlightActor();
		}
	}
	else
	{
		if (thisActor == nullptr)
		{
			lastActor->UnHighlightActor();
		}
		else
		{
			if (lastActor == thisActor)
			{
				//do nothing
			}
			else
			{
				lastActor->UnHighlightActor();
				thisActor->HighlightActor();
			}
			
		}
	}
}

void ADuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void ADuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void ADuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}
