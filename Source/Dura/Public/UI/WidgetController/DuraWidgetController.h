// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "DuraWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FDuraAbilityInfo&, Info);

class UAbilitySystemComponent;
class UAttributeSet;
class ADuraPlayerController;
class ADuraPlayerState;
class UDuraAbilitySystemComponent;
class UDuraAttributeSet;


USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

public:
	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* pc, APlayerState* ps, UAbilitySystemComponent* asc, UAttributeSet* as)
		: PC(pc), PS(ps), ASC(asc), AS(as)
	{
	}


	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	UPROPERTY()
	TObjectPtr<APlayerState> PS;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AS;
};

/**
 * 
 */
UCLASS()
class DURA_API UDuraWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
    FAbilityInfoSignature AbilityInfoDelegate;

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue();

	virtual void BindCallbacksToDependencies();

    void BroadcastAbilityInfo();
protected:

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

    //----
    
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<ADuraPlayerController> DuraPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<ADuraPlayerState> DuraPlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UDuraAbilitySystemComponent> DuraAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UDuraAttributeSet> DuraAttributeSet;

    ADuraPlayerController* GetDuraPC();
    ADuraPlayerState* GetDuraPS();
    UDuraAbilitySystemComponent* GetDuraASC();
    UDuraAttributeSet* GetDuraAS();
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfoDataTable;
};
