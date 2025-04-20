// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// TODO : Check Slot status based on loaded data
	SetWidgetSwitcherIndex.Broadcast(2);
}
