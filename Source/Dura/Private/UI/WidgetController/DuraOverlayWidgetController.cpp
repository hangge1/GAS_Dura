// Copyright by person HDD  


#include "UI/WidgetController/DuraOverlayWidgetController.h"
#include <AbilitySystem/DuraAttributeSet.h>



void UDuraOverlayWidgetController::BroadcastInitialValue()
{
	UDuraAttributeSet* AS = CastChecked<UDuraAttributeSet>(AttributeSet);


	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());

	OnManaChanged.Broadcast(AS->GetMana());
	OnMaxManaChanged.Broadcast(AS->GetMaxMana());
}

void UDuraOverlayWidgetController::BindCallbacksToDependencies()
{
	UDuraAttributeSet* AS = CastChecked<UDuraAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).
		AddUObject(this, &UDuraOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).
		AddUObject(this, &UDuraOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute()).
		AddUObject(this, &UDuraOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute()).
		AddUObject(this, &UDuraOverlayWidgetController::MaxManaChanged);
}

void UDuraOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data)
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UDuraOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UDuraOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data)
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UDuraOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data)
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
