// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraFireBolt.h"


FString UDuraFireBolt::GetDescription(int32 Level)
{   
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    if(Level == 1)
    {
        return FString::Printf(TEXT(
            //Title
            "<Title>FIRE BOLT</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

             //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            "\n<Default>Launched a bolt of fire, exploding on impact and dealing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> fire damage with a chance to burn</>"), 

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
            "<Title>FIRE BOLT</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

            //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Number of FireBolts
            "\n<Default>Launched %d a bolt of fire, exploding on impact and dealing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> fire damage with a chance to burn</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            FMath::Min(Level, NumProjectiles), 
            DamageValue);
    }
}

FString UDuraFireBolt::GetNextLevelDescription(int32 Level)
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

            //Number of FireBolts
            "\n<Default>Launched %d a bolt of fire, exploding on impact and dealing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> fire damage with a chance to burn</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            FMath::Min(Level, NumProjectiles), 
            DamageValue);
}