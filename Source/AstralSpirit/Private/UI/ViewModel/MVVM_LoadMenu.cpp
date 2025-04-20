// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "Game/ASGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	// Create and initialize LoadSlot_0
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlots.Add(0, LoadSlot_0);
	
	// Create and initialize LoadSlot_1
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlots.Add(1, LoadSlot_1);
	
	// Create and initialize LoadSlot_2
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_2");
	LoadSlots.Add(2, LoadSlot_2);
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

	// Set the entered player name on the selected slot
	LoadSlots[Slot]->PlayerName = EnteredName;

	// Save the slot data using the game mode's method
	ASGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadMenu::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(int32 Slot)
{
}
