// Copyright by person HDD  


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
    const int32 WidgetSwitcherIndex = SlotStatus.GetIntValue();
    SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
    UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetSlotIndex(int32 InSlotIndex)
{
    UE_MVVM_SET_PROPERTY_VALUE(SlotIndex, InSlotIndex);
}
