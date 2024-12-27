// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASEnemy.h"

#include "AbilitySystemComponent.h"
#include "ASGameplayTags.h"
#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "AI/ASAIController.h"
#include "AstralSpirit/AstralSpirit.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/ASUserWidget.h"

AASEnemy::AASEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
	AttributeSet = CreateDefaultSubobject<UASAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AASEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Only run by server, replicated to clients
	// Initialize Blackboard and run Behavior Tree
	if (!HasAuthority()) return;
	ASAIController = Cast<AASAIController>(NewController);
	ASAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	ASAIController->RunBehaviorTree(BehaviorTree);
	ASAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	ASAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AASEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UASAbilitySystemBlueprintLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	if (UASUserWidget* ASUserWidget = Cast<UASUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		ASUserWidget->SetWidgetController(this);
	}

	if (const UASAttributeSet* ASAttributeSet = Cast<UASAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ASAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ASAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FASGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AASEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(ASAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(ASAttributeSet->GetMaxHealth());
	}
}

void AASEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = NewCount ? 0 : BaseWalkSpeed;
	ASAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AASEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UASAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeDefaultAttributes();	
	}
}

void AASEnemy::InitializeDefaultAttributes() const
{
	UASAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AASEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AASEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AASEnemy::GetPlayerLevel()
{
	return Level;
}

void AASEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AASEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AASEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}
