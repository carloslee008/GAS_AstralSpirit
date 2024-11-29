// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/ASInputComponent.h"
#include "Interaction/EnemyInterface.h"

AASPlayerController::AASPlayerController()
{
	bReplicates = true;
}

void AASPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CursorTrace();
}

void AASPlayerController::CursorTrace()
{
	FHitResult CursorHit;
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

	if (LastActor == nullptr)
	{
		// Case B
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		// Case C
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (ThisActor != LastActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void AASPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AASPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
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
