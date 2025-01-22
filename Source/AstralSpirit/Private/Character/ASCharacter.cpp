// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ASPlayerController.h"
#include "Player/ASPlayerState.h"
#include "UI/HUD/ASHUD.h"

AASCharacter::AASCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AASCharacter::AddToXP_Implementation(int32 InXP)
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->AddToXP(InXP);
}

int32 AASCharacter::GetPlayerLevel()
{
	const AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->GetPlayerLevel();
}

void AASCharacter::InitAbilityActorInfo()
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ASPlayerState, this);
	Cast<UASAbilitySystemComponent>(ASPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = ASPlayerState->GetAbilitySystemComponent();
	AttributeSet = ASPlayerState->GetAttributeSet();

	// For local player when in multiplayer
	if (AASPlayerController* ASPlayerController = Cast<AASPlayerController>(GetController()))
	{
		if (AASHUD* ASHUD = Cast<AASHUD>(ASPlayerController->GetHUD()))
		{
			ASHUD->InitOverlay(ASPlayerController, ASPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
