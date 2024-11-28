// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/DuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


struct FGameplayTag;
struct FGameplayAttribute;
struct FDuraAttributeInfo;
class UAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FDuraAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class DURA_API UAttributeMenuWidgetController : public UDuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;

    UFUNCTION(BlueprintCallable)
    void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

    UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
    FOnPlayerStatChangedSignature AttributePointsChangedDelegate;
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
