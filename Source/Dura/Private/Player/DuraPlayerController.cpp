// Copyright by person HDD  


#include "Player/DuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/DuraEnhancedInputComponent.h"
#include "Interaction/HighlightInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "DuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "UI/UserWidget/DamageTextComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Actor/MagicCircle.h"
#include "Dura/Dura.h"
#include "Interaction/EnemyInterface.h"

ADuraPlayerController::ADuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ADuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, 
	bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void ADuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
    if(!IsValid(MagicCiecle))
    {
        MagicCiecle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
        if(DecalMaterial)
        {
            MagicCiecle->SetMaterial(0, DecalMaterial);
        }
    }
}

void ADuraPlayerController::HideMagicCircle()
{
    if(IsValid(MagicCiecle))
    {
        MagicCiecle->Destroy();
    }
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
	check(ShiftAction);


	DurainputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADuraPlayerController::Move);
	DurainputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ADuraPlayerController::ShiftPressed);
	DurainputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ADuraPlayerController::ShiftReleased);


	DurainputComponent->BindAbilityActions(InputConfig, this, 
        &ADuraPlayerController::AbilityInputTagPressed, 
        &ADuraPlayerController::AbilityInputTagReleased, 
        &ADuraPlayerController::AbilityInputTagHeld);
}

void ADuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	MouseTrace();
	AutoRun();
    UpdateMagicCircleLocation();
}

void ADuraPlayerController::Move(const FInputActionValue& InputValue)
{
    if(GetASC() && GetASC()->HasMatchingGameplayTag(FDuraGameplayTags::Get().Player_Block_InputPressed))
        return;

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
    if(GetASC() && GetASC()->HasMatchingGameplayTag(FDuraGameplayTags::Get().Player_Block_CursorTrace))
    {
        UnHighlightActor(lastActor);
        UnHighlightActor(thisActor);
        lastActor = nullptr;
        thisActor = nullptr;
        return;
    }

    const ECollisionChannel TraceChannel = IsValid(MagicCiecle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, hitResult);

	if (!hitResult.bBlockingHit) return;

	lastActor = thisActor;
    if(IsValid(hitResult.GetActor()) && hitResult.GetActor()->Implements<UHighlightInterface>())
    {
	    thisActor = hitResult.GetActor();
    }
    else
    {
        thisActor = nullptr;
    }

	//Opt
	if (lastActor != thisActor)
	{
        UnHighlightActor(lastActor);
        HighlightActor(thisActor);
	}
}

void ADuraPlayerController::HighlightActor(AActor* InActor)
{
    if(IsValid(InActor) && InActor->Implements<UHighlightInterface>())
    {
        IHighlightInterface::Execute_HighlightActor(InActor);
    }
}

void ADuraPlayerController::UnHighlightActor(AActor* InActor)
{
    if(IsValid(InActor) && InActor->Implements<UHighlightInterface>())
    {
        IHighlightInterface::Execute_UnHighlightActor(InActor);
    }
}

void ADuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if(GetASC() && GetASC()->HasMatchingGameplayTag(FDuraGameplayTags::Get().Player_Block_InputPressed))
        return;

	if (InputTag.MatchesTagExact(FDuraGameplayTags::Get().InputTag_LMB))
	{
        TargetingStatus = (IsValid(thisActor) && thisActor->Implements<UEnemyInterface>()) ? 
            ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNotEnemy;
		bAutoRunning = false;
	}

    if(GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void ADuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if(GetASC() && GetASC()->HasMatchingGameplayTag(FDuraGameplayTags::Get().Player_Block_InputReleased))
        return;

	if (!InputTag.MatchesTagExact(FDuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
            if(IsValid(thisActor) && thisActor->Implements<UHighlightInterface>())
            {
                IHighlightInterface::Execute_SetMoveToLocation(thisActor, CachedDestination);
            }
            else if(GetASC() && !GetASC()->HasMatchingGameplayTag(FDuraGameplayTags::Get().Player_Block_InputPressed))
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
            }

			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				if (NavPath->PathPoints.Num() != 0)
				{
					for (const FVector& point : NavPath->PathPoints)
					{
						Spline->AddSplinePoint(point, ESplineCoordinateSpace::World);
					}

					if (NavPath->PathPoints.Num() > 0)
					{
						CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
						bAutoRunning = true;
					}
				}
			}        
		}
		FollowTime = 0.0f;
        TargetingStatus = ETargetingStatus::NotTargeting;
	}

}

void ADuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if(GetASC() && GetASC()->HasMatchingGameplayTag(FDuraGameplayTags::Get().Player_Block_InputHeld))
        return;

	if (!InputTag.MatchesTagExact(FDuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bShiftKeyDown)
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
		if (DistanceToDestination <= AutoRunAcceptanceRadius)  
            bAutoRunning = false;
	}
}

void ADuraPlayerController::UpdateMagicCircleLocation()
{
    if(IsValid(MagicCiecle))
    {
        MagicCiecle->SetActorLocation(hitResult.ImpactPoint);
    }
}
