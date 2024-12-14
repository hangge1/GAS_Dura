// Copyright by person HDD  


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
    //TODO: Check slot status based on loaded data
    SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
    UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

FString UMVVM_LoadSlot::GetLoadSlotName() const
{
    return LoadSlotName;
}
