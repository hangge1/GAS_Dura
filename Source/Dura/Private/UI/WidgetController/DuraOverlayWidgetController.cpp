// Copyright by person HDD  


#include "UI/WidgetController/DuraOverlayWidgetController.h"
#include "AbilitySystem/DuraAttributeSet.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Player/DuraPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "DuraGameplayTags.h"



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
    GetDuraPS()->OnXPChangedDelegate.AddUObject(this, &UDuraOverlayWidgetController::OnXPChanged);
    GetDuraPS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel, bool bLevelUp)
    {
        OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
    });
    

	UDuraAttributeSet* AS = GetDuraAS();
    
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data){ OnHealthChanged.Broadcast(Data.NewValue);});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });
    

    if(GetDuraASC())
    {
        GetDuraASC()->AbilityEquipped.AddUObject(this, &UDuraOverlayWidgetController::OnAbilityEquipped);

        if(GetDuraASC()->bStartupAbilitiesGiven)
        {
            BroadcastAbilityInfo();
        }
        else
        {
            GetDuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UDuraOverlayWidgetController::BroadcastAbilityInfo);
        } 

        GetDuraASC()->EffectAssetTags.AddLambda(
		    [this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
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

void UDuraOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, 
        const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{
    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();

    FDuraAbilityInfo LastSlotInfo;
    LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_UnLocked;
    LastSlotInfo.InputTag = PrevSlot;
    LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
    //Broadcast empty info if PrevSlot is a valid slot. Only if equipping an already-equipped all
    AbilityInfoDelegate.Broadcast(LastSlotInfo);

    FDuraAbilityInfo Info = AbilityInfoDataTable->FindAbilityInfoForTag(AbilityTag);
    Info.StatusTag = StatusTag;
    Info.InputTag = Slot;
    AbilityInfoDelegate.Broadcast(Info);
}

void UDuraOverlayWidgetController::OnXPChanged(int32 NewXP) 
{
    const ULevelUpInfo* LevelUpInfo = GetDuraPS()->LevelUpInfoDataAsset;
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
        OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
    }
}
