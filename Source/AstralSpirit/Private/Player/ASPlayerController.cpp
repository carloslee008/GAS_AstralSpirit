// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ASGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/ASInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/DamageTextComponent.h"

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

void AASPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlocked, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlocked, bCriticalHit);
	}
}

void AASPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	// Check if blocking the input and if so then cancel AutoRunning
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_InputPressed))
	{
		bAutoRunning = false;
		return;
	}
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
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->UnHighlightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
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
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(FASGameplayTags::Get().InputTag_LMB))
	{
		bIsTargeting = ThisActor ? true : false; // If hovering highlighted actor, true
		bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AASPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	/**
	 * For non-LMB inputs, activate ability
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
	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	if (!bIsTargeting && !bShiftKeyDown) // Click to move if not hovering an actor or shift-clicking
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
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_InputReleased))
			{
				return;
			}
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}
		FollowTime = 0.f;
		bIsTargeting = false;
	}
}

void AASPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
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
	if (bIsTargeting || bShiftKeyDown) // If hovering highlightable object OR shift-clicking
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

	if (IsValid(GetPawn()))
	{
		ActiveSpringArm = Cast<USpringArmComponent>(GetPawn()->GetComponentByClass(USpringArmComponent::StaticClass()));
		ActiveCamera = Cast<UCameraComponent>(GetPawn()->GetComponentByClass(UCameraComponent::StaticClass()));
		ActiveCapsuleComponent = Cast<UCapsuleComponent>(GetPawn()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	}

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
	ASInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AASPlayerController::ShiftPressed);
	ASInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AASPlayerController::ShiftReleased);
	ASInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}


void AASPlayerController::SyncOccludedActors()
{
	if (!ShouldCheckCameraOcclusion()) return;

	// Camera is currently colliding, show all current occluded actors and do not perform further occlusion
	if (ActiveSpringArm->bDoCollisionTest)
	{
		ForceShowOccludedActors();
		return;
	}

	FVector Start = ActiveCamera->GetComponentLocation();
	FVector End = GetPawn()->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionObjectTypes;
	CollisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;

	auto ShouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration :EDrawDebugTrace::None;

	// Line Trace from Camera to Player
	bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(),
		Start,
		End,
		ActiveCapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
		ActiveCapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace,
		CollisionObjectTypes,
		true,
		ActorsToIgnore,
		ShouldDebug,
		OutHits,
		true
	);

	if (bGotHits)
	{
		// The list of actors hit by the line trace
		TSet<const AActor*> ActorsJustOccluded;

		// Hide actors from hit result
		for (FHitResult Hit : OutHits)
		{
			const AActor* HitActor = Cast<AActor>(Hit.GetActor());
			HideOccludedActor(HitActor);
			ActorsJustOccluded.Add(HitActor);
		}

		// Show actors that are currently hidden but not occluded by the camera anymore
		for (auto& Elem : OccludedActors)
		{
			if (!ActorsJustOccluded.Contains(Elem.Value.Actor) && Elem.Value.IsOccluded)
			{
				ShowOccludedActor(Elem.Value);

				if (DebugLineTraces)
				{
					UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, but it's not occluded anymore with the new hits."), *Elem.Value.Actor->GetName());
				}
			}
		}
	}
	else
	{
		ForceShowOccludedActors();
	}
}

bool AASPlayerController::HideOccludedActor(const AActor* Actor)
{
	FCameraOccludedActor* ExistingOccludedActor = OccludedActors.Find(Actor);

	if (ExistingOccludedActor && ExistingOccludedActor->IsOccluded)
	{
		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was already occluded. Ignoring."), *Actor->GetName());
		return false;
	}
	if (ExistingOccludedActor && IsValid(ExistingOccludedActor->Actor))
	{
		ExistingOccludedActor->IsOccluded = true;
		OnHideOccludedActor(*ExistingOccludedActor);

		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s exists, but was not occluded. Occluding it now."), *Actor->GetName());
	}
	else
	{
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		FCameraOccludedActor OccludedActor;
		OccludedActor.Actor = Actor;
		OccludedActor.StaticMeshComponent = StaticMeshComponent;
		if (StaticMeshComponent)
		{
			OccludedActor.Materials = StaticMeshComponent->GetMaterials();
			OccludedActor.IsOccluded = true;
			OccludedActors.Add(Actor, OccludedActor);
			OnHideOccludedActor(OccludedActor);
		}

		if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s does not exist, adding and occluding it now."), *Actor->GetName());
	}
	return true;
}

void AASPlayerController::ShowOccludedActor(FCameraOccludedActor& OccludedActor)
{
	if(!IsValid(OccludedActor.Actor))
	{
		OccludedActors.Remove(OccludedActor.Actor);
	}

	OccludedActor.IsOccluded = false;
	OnShowOccludedActor(OccludedActor);
}

void AASPlayerController::ForceShowOccludedActors()
{
	for (auto& Elem : OccludedActors)
	{
		if (Elem.Value.IsOccluded)
		{
			ShowOccludedActor(Elem.Value);

			if (DebugLineTraces) UE_LOG(LogTemp, Warning, TEXT("Actor %s was occluded, force to show again"), *Elem.Value.Actor->GetName());
		}
	}
}

bool AASPlayerController::OnHideOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int i = 0; i < OccludedActor.StaticMeshComponent->GetNumMaterials(); ++i)
	{
		OccludedActor.StaticMeshComponent->SetMaterial(i, FadeMaterial);
	}
	// Changes Collision profile so the mouse doesn't collide with the occluded objects.
	OccludedActor.StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	return true;
}

bool AASPlayerController::OnShowOccludedActor(const FCameraOccludedActor& OccludedActor) const
{
	for (int matIdx = 0; matIdx < OccludedActor.Materials.Num(); ++matIdx)
	{
		OccludedActor.StaticMeshComponent->SetMaterial(matIdx, OccludedActor.Materials[matIdx]);
	}
	// Changes Collision profile so the mouse collides with walls again.
	OccludedActor.StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	return true;
}

void AASPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FASGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
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
