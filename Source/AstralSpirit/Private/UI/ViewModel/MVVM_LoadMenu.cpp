// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "Game/ASGameInstance.h"
#include "Game/ASGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	// Create and initialize LoadSlot_0
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	
	// Create and initialize LoadSlot_1
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	
	// Create and initialize LoadSlot_2
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);

	SetNumLoadSlots(LoadSlots.Num());
}

// Returns a pointer to the load slot view model at the given index.
// Will crash if the index is not found due to FindChecked.
UMVVM_LoadSlot* UMVVM_LoadMenu::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

// Called when the player enters a name for a new slot and presses the create button.
void UMVVM_LoadMenu::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	// Get reference to the current game mode
	AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ASGameMode))
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Emerald, FString("Please switch to Single Player."));
		return;
	}

	// Set the default map name on the selected slot
	LoadSlots[Slot]->SetMapName(ASGameMode->DefaultMapName);

	// Set the entered player name on the selected slot
	LoadSlots[Slot]->SetPlayerName(EnteredName);

	// Set player level
	LoadSlots[Slot]->SetPlayerLevel(1);
	
	// Set slot status to taken
	LoadSlots[Slot]->SlotStatus = Taken;

	// Set player location
	LoadSlots[Slot]->PlayerStartTag = ASGameMode->DefaultPlayerStartTag;

	// Set map asset name
	LoadSlots[Slot]->MapAssetName = ASGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();

	// Save the slot data using the game mode's method
	ASGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	
	LoadSlots[Slot]->InitializeSlot();

	// UASGameInstance* ASGameInstance = Cast<UASGameInstance>(ASGameMode->GetGameInstance());
	// ASGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	// ASGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	// ASGameInstance->PlayerStartTag = ASGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadMenu::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadMenu::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AASGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadMenu::PlayButtonPressed()
{
	AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(this));
	UASGameInstance* ASGameInstance = Cast<UASGameInstance>(ASGameMode->GetGameInstance());
	ASGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	ASGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	ASGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;

	if (IsValid(SelectedSlot))
	{
		ASGameMode->TravelToMap(SelectedSlot);
	}
}

void UMVVM_LoadMenu::LoadData()
{
	AASGameModeBase* ASGameMode = Cast<AASGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(ASGameMode)) return;
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadMenuSaveGame* SaveObject = ASGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
		LoadSlot.Value->InitializeSlot();
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
	}
}

void UMVVM_LoadMenu::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
