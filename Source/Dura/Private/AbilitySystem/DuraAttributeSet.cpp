// Copyright by person HDD  


#include "AbilitySystem/DuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "DuraGameplayTags.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DuraPlayerController.h"
#include "Dura/DuraLogChannels.h"
#include "AbilitySystem/DuraAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"


UDuraAttributeSet::UDuraAttributeSet()
{
	const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor_Penetration, GetArmor_PenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Block_Chance, GetBlock_ChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	/* Resistance Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
}

void UDuraAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

    if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter))
    {
        return;
    }

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
    
    if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
        HandleIncomingXP(Props);  
    }

}

void UDuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		if (ADuraPlayerController* PC = Cast<ADuraPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}
		if (ADuraPlayerController* PC = Cast<ADuraPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UDuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
    if(Props.TargetCharacter->Implements<UCombatInterface>())
    {
        const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
        const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
        const int32 Reward = UDuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

        const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();

        FGameplayEventData Payload;
        Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
        Payload.EventMagnitude = Reward;

        UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, 
            GameplayTags.Attributes_Meta_IncomingXP,Payload);
    }
}

void UDuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}
}

void UDuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    if(Attribute == GetMaxHealthAttribute() && bTopOffHealth)
    {
        SetHealth(GetMaxHealth());
        bTopOffHealth = false;
    }
    if(Attribute == GetMaxManaAttribute() && bTopOffMana)
    {
        SetMana(GetMaxMana());
        bTopOffMana = false;
    }
}

void UDuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* Vital Attributes*/

	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	/* Primary Attributes */

	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	/* Secondary Attributes */

	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Armor_Penetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, Block_Chance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	/* Resistance Attributes */

	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void UDuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Health, OldHealth);
}

void UDuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UDuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Mana, OldMana);
}

void UDuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, MaxMana, OldMaxMana);
}

void UDuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Strength, OldStrength);
}

void UDuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Intelligence, OldIntelligence);
}

void UDuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Resilience, OldResilience);
}

void UDuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Resilience, OldVigor);
}

void UDuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Armor, OldArmor);
}

void UDuraAttributeSet::OnRep_Armor_Penetration(const FGameplayAttributeData& OldArmor_Penetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Armor_Penetration, OldArmor_Penetration);

}

void UDuraAttributeSet::OnRep_Block_Chance(const FGameplayAttributeData& OldBlock_Chance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, Block_Chance, OldBlock_Chance);
}

void UDuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UDuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UDuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UDuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UDuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UDuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, FireResistance, OldFireResistance);
}

void UDuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UDuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UDuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UDuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
    const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.0f);
	if (LocalIncomingDamage > 0.0f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.0f;
		if (bFatal)
		{
			if (ICombatInterface* Combat = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
                FVector Impulse = UDuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
				Combat->Die(Impulse);
			}
            SendXPEvent(Props);
		}
		else
		{
            if(Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
            {
                FGameplayTagContainer TagContainer;
			    TagContainer.AddTag(FDuraGameplayTags::Get().Effect_HitReact);
			    Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
            }

            const FVector& KnockbackForce = UDuraAbilitySystemLibrary::GetKocnbackForce(Props.EffectContextHandle); 
            if(!KnockbackForce.IsNearlyZero(1.f))
            {
                Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
            }
		}

		const bool bBlock = UDuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UDuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);

        if(UDuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle) && 
          Props.TargetCharacter != Props.SourceCharacter)
        {
            //Handle Debuff
            Debuff(Props);
        }
	}
}

void UDuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
    const float LocalIncomingXP = GetIncomingXP();
    SetIncomingXP(0.f);
        
    //TODO: See if we should level up
    // Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to ImcomingXP
    if(Props.SourceCharacter->Implements<UCombatInterface>() && Props.SourceCharacter->Implements<UPlayerInterface>())
    {
        const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
        const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
            
        const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
        const int32 NumLevelUps = NewLevel - CurrentLevel;
        if(NumLevelUps > 0)
        {
            IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
            
            //计算升级获得的属性点和技能点奖励
            int32 AttributePointReward = 0;
            int32 SpellPointReward = 0;
            for (int32 i = 0; i < NumLevelUps; ++i)
            {
                AttributePointReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i); 
                SpellPointReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i); 
            }
            IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointReward);
            IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointReward);

            bTopOffHealth = true;
            bTopOffMana = true;

            IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
        }

        IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
    }
}

void UDuraAttributeSet::Debuff(const FEffectProperties& Props)
{
    const FDuraGameplayTags& GameplayTags = FDuraGameplayTags::Get();
    

    const FGameplayTag DamageType = UDuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
    const float DebuffDamage = UDuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
    const float DebuffDuration = UDuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
    const float DebuffFrequency = UDuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

    //Create Dynamic GameplayEffect
    FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
    UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
    
    Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
    Effect->Period = DebuffFrequency;
    Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

    //Add Target Tag Component
    UTargetTagsGameplayEffectComponent& TargetTagComponent = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
    FGameplayTagContainer TagContainer;
    FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffs[DamageType];
    TagContainer.AddTag(DebuffTag);  
    if(DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
    {
        TagContainer.AddTag(GameplayTags.Player_Block_CursorTrace);
        TagContainer.AddTag(GameplayTags.Player_Block_InputHeld);
        TagContainer.AddTag(GameplayTags.Player_Block_InputPressed);
        TagContainer.AddTag(GameplayTags.Player_Block_InputReleased);
    }
    FInheritedTagContainer InheritedTagContainer;
    InheritedTagContainer.Added = TagContainer;
    TargetTagComponent.SetAndApplyTargetTagChanges(InheritedTagContainer);
    //End Add Target Tag Component

    Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
    Effect->StackLimitCount = 1;

    int32 Index = Effect->Modifiers.Num();
    Effect->Modifiers.Add(FGameplayModifierInfo());
    FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
    ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
    ModifierInfo.ModifierOp = EGameplayModOp::Additive;
    ModifierInfo.Attribute = UDuraAttributeSet::GetIncomingDamageAttribute();

    FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
    EffectContext.AddSourceObject(Props.SourceASC);

    FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);
    if(MutableSpec)
    {
        FDuraGameplayEffectContext* DuraContext = static_cast<FDuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
        TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable<FGameplayTag>(new FGameplayTag(DamageType));     
        DuraContext->SetDamageType(DebuffDamageType);

        Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
    } 
}

void UDuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}


