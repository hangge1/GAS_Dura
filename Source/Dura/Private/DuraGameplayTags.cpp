// Copyright by person HDD  


#include "DuraGameplayTags.h"
#include "GameplayTagsManager.h"

FDuraGameplayTags FDuraGameplayTags::DuraGameplayTags;

void FDuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	DuraGameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage"));

	DuraGameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage"));

	DuraGameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration"));

	DuraGameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health"));

	/*
	 * Secondary Attributes
	 */
	DuraGameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_Armor_Penetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor_Penetration"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_Block_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Block_Chance"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), 
		FString("Reduces damage taken, improves Block Chance"));

	DuraGameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), 
		FString("Reduces damage taken, improves Block Chance"));


	/*
	 * Input Tags
	 */

	DuraGameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	DuraGameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	DuraGameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
	);

	DuraGameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
	);

	DuraGameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
	);

	DuraGameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
	);

	

	DuraGameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);

	/*
	* Damage Types
	*/

	DuraGameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
	);

	DuraGameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
	);

	DuraGameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
	);

	DuraGameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
	);

	/*
	* Damage Resistances
	*/

	DuraGameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Fire"),
		FString("Fire Resistance Type")
	);

	DuraGameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Lightning"),
		FString("Lightning Resistance Type")
	);

	DuraGameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Arcane"),
		FString("Arcane Resistance Type")
	);

	DuraGameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Resistance.Physical"),
		FString("Physical Resistance Type")
	);
	
	/* Map of Damage Types to Resistance */
	DuraGameplayTags.DamageTypesToResistances.Add(DuraGameplayTags.Damage_Arcane, DuraGameplayTags.Attributes_Resistance_Arcane);
	DuraGameplayTags.DamageTypesToResistances.Add(DuraGameplayTags.Damage_Fire, DuraGameplayTags.Attributes_Resistance_Fire);
	DuraGameplayTags.DamageTypesToResistances.Add(DuraGameplayTags.Damage_Lightning, DuraGameplayTags.Attributes_Resistance_Lightning);
	DuraGameplayTags.DamageTypesToResistances.Add(DuraGameplayTags.Damage_Physical, DuraGameplayTags.Attributes_Resistance_Physical);

	DuraGameplayTags.Effect_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting")
	);
	
}
