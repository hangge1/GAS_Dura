// Copyright by person HDD  


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/DuraAbilitySystemComponent.h"
#include "Player/DuraPlayerState.h"


void USpellMenuWidgetController::BroadcastInitialValue()
{
    BroadcastAbilityInfo();

    OnSpellPointsChangedDelegate.Broadcast(GetDuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
    GetDuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
    {
        if(AbilityInfoDataTable)
        {
            FDuraAbilityInfo Info = AbilityInfoDataTable->FindAbilityInfoForTag(AbilityTag);
            Info.StatusTag = StatusTag;
            AbilityInfoDelegate.Broadcast(Info);
        }

        //处理UI打开时的更新
        if(SelectedAbility.AbilityTag.MatchesTagExact(AbilityTag))
        {
            SelectedAbility.Status = StatusTag;
            bool bShouldEnableSpellPointsButton = false;
            bool bShouldEnableEquipButton = false;
            ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
            FString Description;
            FString NextLevelDescription;
            GetDuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);  
            SpellGlobeButtonEnabledChanged.Broadcast(bShouldEnableSpellPointsButton, bShouldEnableEquipButton, Description, NextLevelDescription);
        }
    });

    GetDuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 NewSpellPoints)
    {
        OnSpellPointsChangedDelegate.Broadcast(NewSpellPoints);

        //处理UI打开时的更新
        CurrentSpellPoints = NewSpellPoints;
        bool bShouldEnableSpellPointsButton = false;
        bool bShouldEnableEquipButton = false;
        ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);
        
        FString Description;
        FString NextLevelDescription;
        GetDuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, Description, NextLevelDescription);  
        SpellGlobeButtonEnabledChanged.Broadcast(bShouldEnableSpellPointsButton, bShouldEnableEquipButton, Description, NextLevelDescription);  
    });
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
   const int32 SpellPoints = GetDuraPS()->GetSpellPoints();
   const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();
   
   const bool bTagValid = AbilityTag.IsValid();
   const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
   FGameplayAbilitySpec* AbilitySpec = GetDuraASC()->GetSpecFromAbilityTag(AbilityTag);
   const bool bSpecValid = AbilitySpec != nullptr;

   FGameplayTag AbilityStatus;
   if(!bTagValid || bTagNone || !bSpecValid)
   {
        AbilityStatus = GameplayTags.Abilities_Status_Locked;
   }
   else
   {
        AbilityStatus = GetDuraASC()->GetStatusFromSpec(*AbilitySpec);
   }

   //本地记录选择的技能
   SelectedAbility.AbilityTag = AbilityTag;
   SelectedAbility.Status = AbilityStatus;

   bool bShouldEnableSpellPointsButton = false;
   bool bShouldEnableEquipButton = false;
   ShouldEnableButtons(AbilityStatus, SpellPoints, bShouldEnableSpellPointsButton, bShouldEnableEquipButton);

   FString Description;
   FString NextLevelDescription;
   GetDuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.AbilityTag, Description, NextLevelDescription);  
   SpellGlobeButtonEnabledChanged.Broadcast(bShouldEnableSpellPointsButton, bShouldEnableEquipButton, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
    if(GetDuraASC() && SelectedAbility.AbilityTag.IsValid())
    {
        GetDuraASC()->ServerSpendSpellPoint(SelectedAbility.AbilityTag);
    }   
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, 
    bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();

    bShouldEnableSpellPointsButton = false;
    bShouldEnableEquipButton = false;
    if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
    {
        if(SpellPoints > 0)
        {
            bShouldEnableSpellPointsButton = true;
        }
    }
    else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
    {
        bShouldEnableEquipButton = true;
        if(SpellPoints > 0)
        {
            bShouldEnableSpellPointsButton = true;
        }
    }
    else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_UnLocked))
    {
        bShouldEnableEquipButton = true;
    }
}
