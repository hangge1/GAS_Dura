// Copyright by person HDD  


#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/DuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/DuraGameInstance.h"


UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index)
{
    return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::InitializeLoadSlots()
{
    LoadSlots_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
    LoadSlots_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
    LoadSlots_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);

    LoadSlots_0->SetLoadSlotName(TEXT("LoadSlot_0"));
    LoadSlots_1->SetLoadSlotName(TEXT("LoadSlot_1"));
    LoadSlots_2->SetLoadSlotName(TEXT("LoadSlot_2"));

    LoadSlots_0->SetSlotIndex(0);
    LoadSlots_1->SetSlotIndex(1);
    LoadSlots_2->SetSlotIndex(2);

    LoadSlots.Add(0, LoadSlots_0);
    LoadSlots.Add(1, LoadSlots_1);
    LoadSlots.Add(2, LoadSlots_2);

    auto SelectSlotButtonClickLambda = [this](UMVVM_LoadSlot* LoadSlot){
        //1 DisAble LoadSlot's SELECT SLOT BUTTON
        //2 Enable Other's LoadSlot's SELECT SLOT BUTTON
        for (TTuple<int32, UMVVM_LoadSlot*> Tuple : LoadSlots)
        {
            UMVVM_LoadSlot* Slot = Tuple.Value;
            Slot->SetSelectSlotButtonEnable(Slot != LoadSlot);
        }

        //3 Enable Play And DELETE BUTTON
        SetPlayButtonEnable(true);
        SetDeleteButtonEnable(true);

        //4 Set SelectedSlot
        SelectedSlot = LoadSlot;
    };

    LoadSlots_0->SelectSlotButtonClick.AddLambda(SelectSlotButtonClickLambda);
    LoadSlots_1->SelectSlotButtonClick.AddLambda(SelectSlotButtonClickLambda);
    LoadSlots_2->SelectSlotButtonClick.AddLambda(SelectSlotButtonClickLambda);
}

void UMVVM_LoadScreen::LoadData()
{
    ADuraGameModeBase* DuraGameModeBase = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    if(!IsValid(DuraGameModeBase))
    {
        return;
    }


    check(DuraGameModeBase);

    for (const TTuple<int32, UMVVM_LoadSlot*>& LoadSlot : LoadSlots)
    {
        ULoadScreenSaveGame* LoadScreenSaveGameObject 
        = DuraGameModeBase->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

        const FString PlayerName = LoadScreenSaveGameObject->PlayerName;
        TEnumAsByte<ESaveSlotStatus> SlotStatus = LoadScreenSaveGameObject->SlotStatus;

        LoadSlot.Value->SetSlotStatus(SlotStatus);
        LoadSlot.Value->PlayerStartTag = LoadScreenSaveGameObject->PlayerStartTag;
        LoadSlot.Value->SetMapName(LoadScreenSaveGameObject->MapName);
        LoadSlot.Value->SetPlayerName(PlayerName);
        LoadSlot.Value->InitializeSlot();
        LoadSlot.Value->SetPlayerLevel(LoadScreenSaveGameObject->PlayerLevel);
    }
}

void UMVVM_LoadScreen::EnablePlayAndDeleteButton(bool bEnabled)
{
    SetPlayButtonEnable(bEnabled);
    SetDeleteButtonEnable(bEnabled);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
    ADuraGameModeBase* DuraGameMode = Cast<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    if(!IsValid(DuraGameMode))
    {
        GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, TEXT("Please switch to Single Player"));
        return;
    }

    LoadSlots[Slot]->SetSlotStatus(Taken);
    LoadSlots[Slot]->PlayerStartTag = DuraGameMode->DefaultPlayerStartTag;
    LoadSlots[Slot]->SetPlayerName(EnteredName);
    LoadSlots[Slot]->SetMapName(DuraGameMode->DefaultMapName);
    LoadSlots[Slot]->SetPlayerLevel(1);
    LoadSlots[Slot]->MapAssetName = DuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();
    

    DuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
    LoadSlots[Slot]->InitializeSlot();

    UDuraGameInstance* DuraGameInstance = DuraGameMode->GetGameInstance<UDuraGameInstance>();
    DuraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
    DuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->GetSlotIndex();
    DuraGameInstance->PlayerStartTag = DuraGameMode->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
    if(!IsValid(SelectedSlot))
    {
        return;
    }

    ADuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->GetSlotIndex());
    SelectedSlot->SetSlotStatus(Vacant);
    SelectedSlot->InitializeSlot();
    SelectedSlot->SetSelectSlotButtonEnable(true);

    EnablePlayAndDeleteButton(true);
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
    ADuraGameModeBase* DuraGameMode = CastChecked<ADuraGameModeBase>(UGameplayStatics::GetGameMode(this));
    UDuraGameInstance* DuraGameInstance = DuraGameMode->GetGameInstance<UDuraGameInstance>();
    DuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
    DuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
    DuraGameInstance->LoadSlotIndex = SelectedSlot->GetSlotIndex();

    if(IsValid(SelectedSlot))
    {
        DuraGameMode->TravelToMap(SelectedSlot);
    } 
}

void UMVVM_LoadScreen::SetPlayButtonName(FString InPlayButtonName)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayButtonName, InPlayButtonName);
}

void UMVVM_LoadScreen::SetDeleteButtonName(FString InDeleteButtonName)
{
     UE_MVVM_SET_PROPERTY_VALUE(DeleteButtonName, InDeleteButtonName);
}

void UMVVM_LoadScreen::SetQuitButtonName(FString InQuitButtonName)
{
     UE_MVVM_SET_PROPERTY_VALUE(QuitButtonName, InQuitButtonName);
}

void UMVVM_LoadScreen::SetPlayButtonEnable(bool InPlayButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(PlayButtonEnable, InPlayButtonEnable);
}

void UMVVM_LoadScreen::SetDeleteButtonEnable(bool InDeleteButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(DeleteButtonEnable, InDeleteButtonEnable);
}

void UMVVM_LoadScreen::SetQuitButtonEnable(bool InQuitButtonEnable)
{
    UE_MVVM_SET_PROPERTY_VALUE(QuitButtonEnable, InQuitButtonEnable);
}

