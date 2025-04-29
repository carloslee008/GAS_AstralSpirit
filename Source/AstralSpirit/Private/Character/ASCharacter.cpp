// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacter.h"

#include "AbilitySystemComponent.h"
#include "ASGameplayTags.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/ASPlayerController.h"
#include "Player/ASPlayerState.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/ASGameModeBase.h"
#include "Game/LoadMenuSaveGame.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ASHUD.h"

AASCharacter::AASCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bEnableCameraLag = true;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
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
	LoadProgress();
}

void AASCharacter::LoadProgress()
{
	AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (ASGameMode)
	{
		ULoadMenuSaveGame* SaveData = ASGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;
		
		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			// Load in abilities from disk
			if (UASAbilitySystemComponent* ASASC = Cast<UASAbilitySystemComponent>(AbilitySystemComponent))
			{
				ASASC->AddCharacterAbilitiesFromSaveData(SaveData);
			}
			/* Set Player Info upon loading */
			if (AASPlayerState* ASPlayerState = Cast<AASPlayerState>(GetPlayerState()))
			{
				ASPlayerState->SetLevel(SaveData->PlayerLevel);
				ASPlayerState->SetXP(SaveData->XP);
				ASPlayerState->SetAttributePoints(SaveData->AttributePoints);
				ASPlayerState->SetSkillPoints(SaveData->SkillPoints);
			}
			UASAbilitySystemBlueprintLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
}

void AASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 AASCharacter::GetXP_Implementation() const
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->GetPlayerXP();
}

int32 AASCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AASCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 AASCharacter::GetSkillPointsReward_Implementation(int32 Level) const
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->LevelUpInfo->LevelUpInformation[Level].SkillPointReward;
}

void AASCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->AddToLevel(InPlayerLevel);

	if (UASAbilitySystemComponent* ASASC = Cast<UASAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASASC->UpdateAbilityStatuses(ASPlayerState->GetPlayerLevel());
	}
}

void AASCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->AddToAttributePoints(InAttributePoints);
}

void AASCharacter::AddToSkillPoints_Implementation(int32 InSkillPoints)
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->AddToSkillPoints(InSkillPoints);
}

int32 AASCharacter::GetAttributePoints_Implementation() const
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->GetPlayerAttributePoints();
}

int32 AASCharacter::GetSkillPoints_Implementation() const
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->GetPlayerSkillPoints();
}

void AASCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial, const float Radius)
{
	if (AASPlayerController* ASPlayerController = Cast<AASPlayerController>(GetController()))
	{
		ASPlayerController->ShowMagicCircle(DecalMaterial, Radius);
		ASPlayerController->bShowMouseCursor = false;
	}
}

void AASCharacter::HideMagicCircle_Implementation()
{
	if (AASPlayerController* ASPlayerController = Cast<AASPlayerController>(GetController()))
	{
		ASPlayerController->HideMagicCircle();
		ASPlayerController->bShowMouseCursor = true;
	}	
}

void AASCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (ASGameMode)
	{
		ULoadMenuSaveGame* SaveData = ASGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		SaveData->PlayerStartTag = CheckpointTag;

		/* Save Player Info upon loading */
		
		if (AASPlayerState* ASPlayerState = Cast<AASPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = ASPlayerState->GetPlayerLevel();
			SaveData->XP = ASPlayerState->GetPlayerXP();
			SaveData->AttributePoints = ASPlayerState->GetPlayerAttributePoints();
			SaveData->SkillPoints = ASPlayerState->GetPlayerSkillPoints();
		}
		
		SaveData->Strength = UASAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Dexterity = UASAttributeSet::GetDexterityAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UASAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false;

		if (!HasAuthority()) return;

		UASAbilitySystemComponent* ASASC = Cast<UASAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();

		/* Save Each Ability */
		SaveAbilityDelegate.BindLambda([this, ASASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const FGameplayTag AbilityTag = ASASC->GetAbilityTagFromSpec(AbilitySpec);
			UAbilityInfo* AbilityInfo = UASAbilitySystemBlueprintLibrary::GetAbilityInfo(this);
			FASAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			
			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbility = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = ASASC->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = ASASC->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;

			SaveData->SavedAbilities.AddUnique(SavedAbility);
			
			
		});
		ASASC->ForEachAbility(SaveAbilityDelegate);
		
		ASGameMode->SaveInGameProgressData(SaveData);
	}
}

void AASCharacter::AddToXP_Implementation(int32 InXP)
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->AddToXP(InXP);
}

void AASCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AASCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AASCharacter::GetPlayerLevel_Implementation()
{
	const AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	return ASPlayerState->GetPlayerLevel();
}

void AASCharacter::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
	}
	
	Super::MulticastHandleDeath_Implementation(DeathImpulse);
}

void AASCharacter::OnRep_Stunned()
{
	if (UASAbilitySystemComponent* ASASC = Cast<UASAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FASGameplayTags& GameplayTags = FASGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			ASASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			ASASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AASCharacter::OnRep_Ignited()
{
	if (bIsIgnited)
	{
		IgniteDebuffComponent->Activate();
	}
	else
	{
		IgniteDebuffComponent->Deactivate();
	}
}

void AASCharacter::InitAbilityActorInfo()
{
	AASPlayerState* ASPlayerState = GetPlayerState<AASPlayerState>();
	check(ASPlayerState);
	ASPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(ASPlayerState, this);
	Cast<UASAbilitySystemComponent>(ASPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = ASPlayerState->GetAbilitySystemComponent();
	AttributeSet = ASPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FASGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AASCharacter::StunTagChanged);

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
