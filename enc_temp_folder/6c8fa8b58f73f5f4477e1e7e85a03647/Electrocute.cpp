// Copyright by person HDD  


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    if(Level == 1)
    {
        return FString::Printf(TEXT(
            //Title
            "<Title>ELECTROCUTE</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

             //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            "\n<Default>Emits a beam of lightning, connecting with the target, repeatedly causing</>"

            //Damage
            "<Damage>%d</>"
            "<Default> lightning damage with a chance to stun</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            DamageValue);
    }
    else
    {
        return FString::Printf(TEXT(
            //Title
            "<Title>Next Level: </>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

            //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Addition Number of Shock Targets
            "\n<Default>Emits a beam of lightning, propagating to %d additional targets nearby, causing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> lightning damage with a chance to stun</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            FMath::Min(Level, MaxNumShockTargets), 
            DamageValue);
    }
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);
    
    return FString::Printf(TEXT(
        //Title
        "<Title>ELECTROCUTE</>\n\n"

        // Level
        "<Small>Level: </><Level>%d</>\n"

        //ManaCost
        "<Small>ManaCost: </><ManaCost>%.1f</>\n"

        //Cooldown
        "<Small>Cooldown: </><Cooldown>%.1f</>\n"

        //Addition Number of Shock Targets
        "\n<Default>Emits a beam of lightning, propagating to %d additional targets nearby, causing </>"

        //Damage
        "<Damage>%d</>"
        "<Default> lightning damage with a chance to stun</>"), 

        //Values
        Level, 
        ManaCost, 
        Cooldown,
        FMath::Min(Level, MaxNumShockTargets), 
        DamageValue);
}
