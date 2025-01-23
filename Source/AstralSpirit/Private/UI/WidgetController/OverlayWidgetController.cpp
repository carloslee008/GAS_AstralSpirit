// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/ASPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UASAttributeSet* ASAttributeSet = CastChecked<UASAttributeSet>(AttributeSet);
	const AASPlayerState* ASPlayerState = CastChecked<AASPlayerState>(PlayerState);
	
	OnHealthChanged.Broadcast(ASAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ASAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(ASAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(ASAttributeSet->GetMaxMana());
	OnXPPercentChangedDelegate.Broadcast(ASPlayerState->GetPlayerXP());
	
}

// Configure Broadcast events to update the overlay widget components
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AASPlayerState* ASPlayerState = CastChecked<AASPlayerState>(PlayerState);
	ASPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	ASPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}	
	);
	
	const UASAttributeSet* ASAttributeSet = CastChecked<UASAttributeSet>(AttributeSet);
	

	/* 
	 *	Vital Attributes
	 */
	
	// Health
	BindAttributeChange(ASAttributeSet->GetHealthAttribute(), &OnHealthChanged);
	
	// Max Health
	BindAttributeChange(ASAttributeSet->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	
	// Mana
	BindAttributeChange(ASAttributeSet->GetManaAttribute(), &OnManaChanged);
	
	// Max Mana
	BindAttributeChange(ASAttributeSet->GetMaxManaAttribute(), &OnMaxManaChanged);
	
	if (UASAbilitySystemComponent* ASASC = Cast<UASAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASASC);
		}
		else
		{
			ASASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		ASASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, let's say Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True,
					// "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UASAbilitySystemComponent* ASAbilitySystemComponent)
{
	if (!ASAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, ASAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FASAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(ASAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = ASAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	ASAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AASPlayerState* ASPlayerState = CastChecked<AASPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = ASPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Cannot find LevelUpInfo. Please fill out ASPlayerState Blueprint."));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;

	if (Level < MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 CurrentLevelXPRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForCurrentLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForCurrentLevel) / static_cast<float>(CurrentLevelXPRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
	
}

void UOverlayWidgetController::BindAttributeChange(const FGameplayAttribute& Attribute,
                                                   FOnAttributeChangedSignature* AttributeChangeDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[AttributeChangeDelegate](const FOnAttributeChangeData& Data)
		{
			AttributeChangeDelegate->Broadcast(Data.NewValue);
		}
	);
}
