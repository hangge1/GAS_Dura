// Copyright by person HDD  


#include "AbilitySystem/Abilities/DuraFireBlast.h"

FString UDuraFireBlast::GetDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    return FString::Printf(TEXT(
            //Title
            "<Title>FIRE BLAST</>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

             //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Number of Fire Balls
            "\n<Default>Launched %d </>"
            "<Default>fire balls in all directions, each coming back and exploding upon return, causing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> radial fire damage with</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            NumFireBalls,
            DamageValue);
}

FString UDuraFireBlast::GetNextLevelDescription(int32 Level)
{
    const int32 DamageValue = Damage.GetValueAtLevel(Level);
    const float ManaCost = FMath::Abs(GetManaCost(Level));
    const float Cooldown = GetCooldown(Level);

    return FString::Printf(TEXT(
            //Title
            "<Title>NEXT LEVEL: </>\n\n"

            // Level
            "<Small>Level: </><Level>%d</>\n"

            //ManaCost
            "<Small>ManaCost: </><ManaCost>%.1f</>\n"

             //Cooldown
            "<Small>Cooldown: </><Cooldown>%.1f</>\n"

            //Number of Fire Balls
            "\n<Default>Launched %d </>"
            "<Default>fire balls in all directions, each coming back and exploding upon return, causing </>"

            //Damage
            "<Damage>%d</>"
            "<Default> radial fire damage with</>"), 

            //Values
            Level, 
            ManaCost, 
            Cooldown,
            NumFireBalls,
            DamageValue);
}

TArray<ADuraFireBall*> UDuraFireBlast::SpawnFireBalls()
{
    return TArray<ADuraFireBall*>();
}
