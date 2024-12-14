// Copyright by person HDD  


#include "UI/HUD/LoadScreenHUD.h"
#include "UI/Widget/LoadScreenWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"

void ALoadScreenHUD::BeginPlay()
{
    Super::BeginPlay();

    LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
    LoadScreenViewModel->InitializeLoadSlots();

    LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
    LoadScreenWidget->AddToViewport();
    LoadScreenWidget->BlueprintInitializeWidget();
}
