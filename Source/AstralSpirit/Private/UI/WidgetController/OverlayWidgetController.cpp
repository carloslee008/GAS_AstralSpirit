// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UASAttributeSet* ASAttributeSet = CastChecked<UASAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(ASAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ASAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(ASAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(ASAttributeSet->GetMaxMana());
	
}

// Configure Broadcast events to update the overlay widget components
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UASAttributeSet* ASAttributeSet = CastChecked<UASAttributeSet>(AttributeSet);

	// Vital Attributes
	
	// Health
	BindAttributeChange(ASAttributeSet->GetHealthAttribute(), &OnHealthChanged);
	
	// Max Health
	BindAttributeChange(ASAttributeSet->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	
	// Mana
	BindAttributeChange(ASAttributeSet->GetManaAttribute(), &OnManaChanged);
	
	// Max Mana
	BindAttributeChange(ASAttributeSet->GetMaxManaAttribute(), &OnMaxManaChanged);
	
	Cast<UASAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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
