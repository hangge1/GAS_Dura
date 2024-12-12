// Copyright by person HDD  


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    if(Level == 1)
    {
        return FString::Printf(TEXT(
            //Title
            "<Title>ARCANE SHARDS</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

             //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            "\n<Default>Summon shards of arcane energy, causing radial arcane damage of </>"

            //Damage
            "<Damage>%d</>"
            "<Default> at the shard origin.</>"), 

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
            "<Title>ARCANE SHARDS</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

            //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Addition Number of Shock Targets
            "\n<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"

            //Damage
            "<Damage>%d</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            FMath::Min(Level, MaxNumShards), 
            DamageValue);
    }
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

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
        "\n<Default>Summon %d shards of arcane energy, causing radial arcane damage of </>"

        //Damage
        "<Damage>%d</>"), 

        //Values
        Level, 
        ManaCost, 
        Cooldown,
        FMath::Min(Level, MaxNumShards), 
        DamageValue);
}
