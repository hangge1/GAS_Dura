// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraGameplayAbility.h"
#include "AbilitySystem/DuraAttributeSet.h"

FString UDuraGameplayAbility::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), TEXT("Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum "), Level);
}

FString UDuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage</>"), Level);
}

FString UDuraGameplayAbility::GetLockedDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>Spell Locked</>\n<Default>Until Level: %d</>"), Level);
}

float UDuraGameplayAbility::GetManaCost(float InLevel /*= 1.f*/) const
{
    float ManaCost = 0.0f;
    if(const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for(FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if(Mod.Attribute == UDuraAttributeSet::GetManaAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
                break;
            }
        }
    }
    return ManaCost;
}

float UDuraGameplayAbility::GetCooldown(float InLevel /*= 1.f*/) const
{
    float Cooldown = 0.0f;
    if(const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
    {
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
        
    }
    return Cooldown;
}