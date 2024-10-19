// Copyright by person HDD  


#include "Player/DuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/DuraEnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "DuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"


ADuraPlayerController::ADuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

	AutoRun();
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
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult);

	if (!hitResult.bBlockingHit) return;

	lastActor = thisActor;
	thisActor = Cast<IEnemyInterface>(hitResult.GetActor());

	//Opt
	if (lastActor != thisActor)
	{
		if (lastActor) lastActor->UnHighlightActor();
		if (thisActor) thisActor->HighlightActor();
	}
}

void ADuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FDuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = thisActor ? true : false;
		bAutoRunning = false;
	}	
}

void ADuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FDuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	
	if (!bTargeting)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				if (NavPath->PathPoints.Num() != 0)
				{
					for (const FVector& point : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(point, ESplineCoordinateSpace::World);
					}

					if (NavPath->PathPoints.Num() > 1) CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.0f;
		bTargeting = false;
	}
	else
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	}
}

void ADuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FDuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting)
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (hitResult.bBlockingHit)  CachedDestination = hitResult.ImpactPoint;

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UDuraAbilitySystemComponent* ADuraPlayerController::GetASC()
{
	if (DuraAbilitySystemComponent == nullptr)
	{
		DuraAbilitySystemComponent = 
			Cast<UDuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return DuraAbilitySystemComponent;
}

void ADuraPlayerController::AutoRun()
{
	if (bAutoRunning == false) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->
			FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->
			FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)  bAutoRunning = false;
	}
}
