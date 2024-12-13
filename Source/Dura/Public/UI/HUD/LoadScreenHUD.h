// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class ULoadScreenWidget;
class UMVVM_LoadScreen;
/**
 * 
 */
UCLASS()
class DURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> LoadScreenWidgetClass;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;
protected:
    virtual void BeginPlay() override;
};
