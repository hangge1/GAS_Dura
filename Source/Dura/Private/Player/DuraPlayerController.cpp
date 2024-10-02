// Copyright by person HDD  


#include "Player/DuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"



ADuraPlayerController::ADuraPlayerController()
{
	bReplicates = true;
}

void ADuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(DuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);

	SubSystem->AddMappingContext(DuraContext, 0);

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

	UEnhancedInputComponent* inputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	check(MoveAction);

	inputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADuraPlayerController::Move);
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
