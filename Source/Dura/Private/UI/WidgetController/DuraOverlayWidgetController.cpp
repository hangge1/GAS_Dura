// Copyright by person HDD  


#include "UI/WidgetController/DuraOverlayWidgetController.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Player/DuraPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"



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
    ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(PlayerState);
    DuraPlayerState->OnXPChangedDelegate.AddUObject(this, &UDuraOverlayWidgetController::OnXPChanged);

	UDuraAttributeSet* AS = CastChecked<UDuraAttributeSet>(AttributeSet);
    
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue);});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });
    

    if(UDuraAbilitySystemComponent* DuraASC = Cast<UDuraAbilitySystemComponent>(AbilitySystemComponent))
    {
        if(DuraASC->bStartupAbilitiesGiven)
        {
            OnInitializeStartupAbilities(DuraASC);
        }
        else
        {
            DuraASC->AbilitiesGivenDelegate.AddUObject(this, &UDuraOverlayWidgetController::OnInitializeStartupAbilities);
        } 

        DuraASC->EffectAssetTags.AddLambda(
		    [this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				/*const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);

				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);*/

				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	    );
    }

	
    
}

void UDuraOverlayWidgetController::OnInitializeStartupAbilities(UDuraAbilitySystemComponent* ASC)
{
    //TODO: Get information about all given abilities, look up their Ability Info and broadcast it to widgets
    if(!ASC->bStartupAbilitiesGiven)return;

    FForEachAbility BroadcastDelegate;
    BroadcastDelegate.BindLambda([this, ASC](const FGameplayAbilitySpec& AbilitySpec) 
    {
        FDuraAbilityInfo AbilityInfo = AbilityInfoDataTable->FindAbilityInfoForTag(UDuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
        AbilityInfo.InputTag = UDuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
        AbilityInfoDelegate.Broadcast(AbilityInfo);
    });

    ASC->ForEachAbility(BroadcastDelegate);
}

void UDuraOverlayWidgetController::OnXPChanged(int32 NewXP) const 
{
    const ADuraPlayerState* DuraPlayerState = CastChecked<ADuraPlayerState>(PlayerState);
    const ULevelUpInfo* LevelUpInfo = DuraPlayerState->LevelUpInfoDataAsset;
    checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out in DuraPlayerState Blueprint"));

    const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
    const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

    if(Level <= MaxLevel && Level > 0)
    {
        const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
        const int32 PrevioutLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

        const int32 DeltaLevelRequirement = LevelUpRequirement - PrevioutLevelUpRequirement;       
        const int32 XPForThisLevel = NewXP - PrevioutLevelUpRequirement;

        const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
        OnXPPercentChanged.Broadcast(XPBarPercent);
    }
}
