// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct DURA_API FDuraGameplayTags
{
public:
	static const FDuraGameplayTags& Get() 
	{
		return DuraGameplayTags;
	}

	static void InitializeNativeGameplayTags();

	/*
	* Primary Attributes
	*/
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	/*
	* Secondary Attributes
	*/
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_Armor_Penetration;
	FGameplayTag Attributes_Secondary_Block_Chance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

    FGameplayTag Attributes_Meta_IncomingXP;

	/*
	* InputTag
	*/
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	/* DamageTag */
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

    /* Attributes_Resistance */
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

    /* Debuff Tag */
    FGameplayTag Debuff_Burn;
    FGameplayTag Debuff_Stun;
    FGameplayTag Debuff_Arcane;
    FGameplayTag Debuff_Physical;

    FGameplayTag Debuff_Chance;
    FGameplayTag Debuff_Damage;
    FGameplayTag Debuff_Duration;
    FGameplayTag Debuff_Frequency;

    /* Abilities */
    FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

    FGameplayTag Abilities_HitReact;

    FGameplayTag Abilities_Status_Locked;
    FGameplayTag Abilities_Status_Eligible;
    FGameplayTag Abilities_Status_UnLocked;
    FGameplayTag Abilities_Status_Equipped;

    FGameplayTag Abilities_Type_Offensive;
    FGameplayTag Abilities_Type_Passive;
    FGameplayTag Abilities_Type_None;


	FGameplayTag Abilities_Fire_Bolt;
	FGameplayTag Abilities_Lightning_Electrocute;

    FGameplayTag Abilities_Passive_HaloOfProtection;
    FGameplayTag Abilities_Passive_LifeSiphon;
    FGameplayTag Abilities_Passive_ManaSiphon;


	FGameplayTag CoolDown_Fire_Bolt;

	/*
	* CombatSocket
	*/
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

    /*
	* Montage Attack
	*/
    FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;


	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	FGameplayTag Effect_HitReact;

    FGameplayTag Player_Block_InputPressed;
    FGameplayTag Player_Block_InputHeld;
    FGameplayTag Player_Block_InputReleased;
    FGameplayTag Player_Block_CursorTrace;


private:
	static FDuraGameplayTags DuraGameplayTags;
};
