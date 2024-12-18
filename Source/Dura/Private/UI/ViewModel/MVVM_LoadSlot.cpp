// Copyright by person HDD  


#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "FieldNotificationDeclaration.h"
#include "Game/DuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/DuraGameInstance.h"

void UMVVM_LoadSlot::InitializeSlot()
{
    const int32 WidgetSwitcherIndex = SlotStatus;
    SetSlotStatus(static_cast<ESaveSlotStatus>(WidgetSwitcherIndex));
}

void UMVVM_LoadSlot::NewGameButtonPressed()
{
    SetSlotStatus(EnterName);
}

void UMVVM_LoadSlot::SelectSlotButtonPressed()
{
    SelectSlotButtonClick.Broadcast(this);
}

void UMVVM_LoadSlot::NewSlotButtonPressed(const FString& EnteredPlayerName)
{
    ADuraGameModeBase* DuraGameMode = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    if(!IsValid(DuraGameMode))
    {
        GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, TEXT("Please switch to Single Player"));
        return;
    }

    SetSlotStatus(Taken);
    PlayerStartTag = DuraGameMode->DefaultPlayerStartTag;
    SetPlayerName(EnteredPlayerName);
    SetMapName(DuraGameMode->DefaultMapName);
    SetPlayerLevel(1);
    MapAssetName = DuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
    

    DuraGameMode->SaveSlotData(this, SlotIndex);
    InitializeSlot();

    UDuraGameInstance* DuraGameInstance = DuraGameMode->GetGameInstance<UDuraGameInstance>();
    DuraGameInstance->LoadSlotName = GetLoadSlotName();
    DuraGameInstance->LoadSlotIndex = GetSlotIndex();
    DuraGameInstance->PlayerStartTag = DuraGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
    UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
    UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);     
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

void UMVVM_LoadSlot::SetPlayerLevel(int32 InPlayerLevel)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InPlayerLevel);
}

void UMVVM_LoadSlot::SetSlotStatus(int32 InSlotStatus)
{
    UE_MVVM_SET_PROPERTY_VALUE(SlotStatus, InSlotStatus);
}

void UMVVM_LoadSlot::SetEnterNameString(FString InEnterNameString)
{
    UE_MVVM_SET_PROPERTY_VALUE(EnterNameString, InEnterNameString);
}

void UMVVM_LoadSlot::SetNewSlotString(FString InNewSlotString)
{
    UE_MVVM_SET_PROPERTY_VALUE(NewSlotString, InNewSlotString);
}

void UMVVM_LoadSlot::SetSelectSlotString(FString InSelectSlotString)
{
    UE_MVVM_SET_PROPERTY_VALUE(SelectSlotString, InSelectSlotString);
}

void UMVVM_LoadSlot::SetNewGameString(FString InNewGameString)
{
    UE_MVVM_SET_PROPERTY_VALUE(NewGameString, InNewGameString);
}

void UMVVM_LoadSlot::SetSelectSlotButtonEnable(bool InSelectSlotButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(SelectSlotButtonEnable, InSelectSlotButtonEnable);
}
