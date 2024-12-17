// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CustomCalculations/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "ASAbilityTypes.h"
#include "ASGameplayTags.h"
#include "GameplayTagsManager.h"
#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ASAttributeSet.h"

struct ASDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	ASDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UASAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UASAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UASAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UASAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const ASDamageStatics& DamageStatics()
{
	static ASDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage from each Damage Type
	float Damage = 0.f;
	FGameplayTagContainer AllDamageTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(FASGameplayTags::Get().Damage);
	for (const auto& Tag : AllDamageTags)
	{
		// Get Damage Set by Caller Magnitude
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Tag);
		Damage += DamageTypeValue;
	}
	// Capture Block Chance on Target, and determine if there was a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);
	
	// If Blocked, Halve the Damage.
	const bool bBlocked = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) <= TargetBlockChance;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UASAbilitySystemBlueprintLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	Damage = bBlocked ? Damage / 2.f : Damage;

	// Capture Target Armor on Target, and subtract from Damage
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0, TargetArmor);
	Damage -= TargetArmor;

	// Capture Critical Hit Chance and Critical Hit Damage on Source, and determine if there was a successful Crit Hit
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0, SourceCriticalHitChance);
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0, SourceCriticalHitDamage);

	// If Critically Hit, multiply by Critical Hit Damage
	const bool bCriticalHit = FMath::FRandRange(UE_SMALL_NUMBER, 100.f) <= SourceCriticalHitChance;
	UASAbilitySystemBlueprintLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	Damage = bCriticalHit ? Damage * SourceCriticalHitDamage : Damage;	

	const FGameplayModifierEvaluatedData EvaluatedData(UASAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
