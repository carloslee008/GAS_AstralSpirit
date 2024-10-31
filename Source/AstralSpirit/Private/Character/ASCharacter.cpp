// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ASPlayerState.h"

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
}

void AASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AASCharacter::InitAbilityActorInfo()
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ASPlayerState, this);
	AbilitySystemComponent = ASPlayerState->GetAbilitySystemComponent();
	AttributeSet = ASPlayerState->GetAttributeSet();
}