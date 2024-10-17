// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerController.h"
#include "EnhancedInputSubsystems.h"

AASPlayerController::AASPlayerController()
{
	bReplicates = true;
}

void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(ASContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(ASContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
