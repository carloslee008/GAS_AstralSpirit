// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ASGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/ASInputComponent.h"
#include "Interaction/EnemyInterface.h"

AASPlayerController::AASPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AASPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void AASPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		// Closest location on the spline to ControlledPawn
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		
		// Direction to spline
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AASPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor possible scenarios:
	 * A. Both LastActor && ThisActor are NULL
	 * - Do nothing
	 * B. LastActor is NULL && ThisActor is VALID
	 * - Highlight
	 * C. LastActor is VALID && ThisActor is NULL
	 * - Unhighlight
	 * D. Both Actors are VALID, but different actors
	 * - Unhighlight LastActor, Highlight ThisActor
	 * E. Both Actors are VALID, same actor
	 * - Do nothing
	 */
	
	if (LastActor != ThisActor)
	{
		if (ThisActor) ThisActor->HighlightActor(); // Case B and D
		if (LastActor) LastActor->UnHighlightActor(); // Case C and D
	}
}

void AASPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FASGameplayTags::Get().InputTag_LMB))
	{
		bIsTargeting = ThisActor ? true : false; // If hovering highlighted actor, true
		bAutoRunning = false;
	}
}

void AASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	/**
	 * For non-LMB inputs
	 **/
	if (!InputTag.MatchesTagExact(FASGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}
	/**
	 * For LMB
	 **/
	
	if (bIsTargeting) // If hovering highlightable actor
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				// Checks to see there's at least one path point in the array
				CachedDestination = NavPath->PathPoints.IsEmpty() ? ControlledPawn->GetActorLocation() : NavPath->PathPoints.Last(); 
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bIsTargeting = false;
	}
}

void AASPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	/**
	 * For non-LMB inputs
	 **/
	if (!InputTag.MatchesTagExact(FASGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	/**
	 * For LMB
	 **/
	if (bIsTargeting) // If hovered
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else // Click to move
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UASAbilitySystemComponent* AASPlayerController::GetASC()
{
	if (ASAbilitySystemComponent == nullptr)
	{
		ASAbilitySystemComponent = Cast<UASAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return ASAbilitySystemComponent;
}

void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(ASContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// If locally controlled when in multiplayer
	if (Subsystem)
	{
		Subsystem->AddMappingContext(ASContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UASInputComponent* ASInputComponent = CastChecked<UASInputComponent>(InputComponent);

	ASInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AASPlayerController::Move);
	ASInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AASPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
	
}
