// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ASAbilityTypes.h"
#include "ASGameplayTags.h"
#include "Game/ASGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ASPlayerState.h"
#include "UI/HUD/ASHUD.h"

bool UASAbilitySystemBlueprintLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, AASHUD*& OutASHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutASHUD = Cast<AASHUD>(PC->GetHUD());
		if (OutASHUD)
		{
			AASPlayerState* PS = PC->GetPlayerState<AASPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			
			return true;
		}	
	}
	return false;
}

UOverlayWidgetController* UASAbilitySystemBlueprintLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AASHUD* ASHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, ASHUD))
	{
		return ASHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UASAbilitySystemBlueprintLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AASHUD* ASHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, ASHUD))
	{
		return ASHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USkillMenuWidgetController* UASAbilitySystemBlueprintLibrary::GetSkillMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AASHUD* ASHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, ASHUD))
	{
		return ASHUD->GetSkillMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UASAbilitySystemBlueprintLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
		
	AActor* AvatarActor = ASC->GetAvatarActor();

	//Get the class information based on character class
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	// Apply Primary Attributes
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	// Apply Secondary Attributes
	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	// Apply Vital Attributes
	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UASAbilitySystemBlueprintLibrary::GiveStartupAbilities(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UASAbilitySystemBlueprintLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ASGameMode == nullptr) return nullptr;
	return ASGameMode->CharacterClassInfo;
}

UAbilityInfo* UASAbilitySystemBlueprintLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ASGameMode == nullptr) return nullptr;
	return ASGameMode->AbilityInfo;
}

int32 UASAbilitySystemBlueprintLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
                                                                    ECharacterClass CharacterClass, int32 CharacterLevel)
{
	AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (ASGameMode == nullptr) return 0;
	
	UCharacterClassInfo* CharacterClassInfo = ASGameMode->CharacterClassInfo;

	FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

bool UASAbilitySystemBlueprintLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->IsBlockedHit();
	}
	return false;
}

bool UASAbilitySystemBlueprintLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UASAbilitySystemBlueprintLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UASAbilitySystemBlueprintLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UASAbilitySystemBlueprintLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UASAbilitySystemBlueprintLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (ASEffectContext->GetDamageType().IsValid())
		{
			return *ASEffectContext->GetDamageType();	
		}
	}
	return FGameplayTag();
}

FVector UASAbilitySystemBlueprintLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UASAbilitySystemBlueprintLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UASAbilitySystemBlueprintLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->IsRadialDamage();
	}
	return false;
}

float UASAbilitySystemBlueprintLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UASAbilitySystemBlueprintLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UASAbilitySystemBlueprintLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

bool UASAbilitySystemBlueprintLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FASGameplayEffectContext* ASEffectContext = static_cast<const FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ASEffectContext->IsCriticalHit();
	}
	return false;
}

void UASAbilitySystemBlueprintLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UASAbilitySystemBlueprintLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UASAbilitySystemBlueprintLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UASAbilitySystemBlueprintLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle,
	float InDamage)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetDebuffDamage(InDamage);
	}
}

void UASAbilitySystemBlueprintLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle,
	float InDuration)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetDebuffDuration(InDuration);
	}
}

void UASAbilitySystemBlueprintLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle,
	float InFrequency)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UASAbilitySystemBlueprintLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		ASEffectContext->SetDamageType(DamageType);
	}
}

void UASAbilitySystemBlueprintLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UASAbilitySystemBlueprintLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetKnockbackForce(InForce);
	}
}

void UASAbilitySystemBlueprintLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UASAbilitySystemBlueprintLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UASAbilitySystemBlueprintLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UASAbilitySystemBlueprintLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FASGameplayEffectContext* ASEffectContext = static_cast<FASGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		ASEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UASAbilitySystemBlueprintLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                                  TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                                  const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UASAbilitySystemBlueprintLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}
	
	if (MaxTargets < 1) return;
	
	OutClosestTargets = Actors;
	Algo::Sort(OutClosestTargets, [&Origin](const AActor* ActorA, const AActor* ActorB)
	{
		const float DistanceA = FVector::DistSquared(ActorA->GetActorLocation(), Origin);
		const float DistanceB = FVector::DistSquared(ActorB->GetActorLocation(), Origin);
		return DistanceA < DistanceB;
	});
	if (OutClosestTargets.Num() > MaxTargets)
	{
		// Remove unwanted actors from the end of the array
		OutClosestTargets.RemoveAt(MaxTargets, OutClosestTargets.Num() - MaxTargets);
	}
}

bool UASAbilitySystemBlueprintLibrary::IsSameTeam(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bSameTeam = bBothArePlayers || bBothAreEnemies;

	return bSameTeam;
}

FGameplayEffectContextHandle UASAbilitySystemBlueprintLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FASGameplayTags& GameplayTags = FASGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UASAbilitySystemBlueprintLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UASAbilitySystemBlueprintLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}
