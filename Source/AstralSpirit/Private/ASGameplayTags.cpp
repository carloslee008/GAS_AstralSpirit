// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameplayTags.h"
#include "GameplayTagsManager.h"

FASGameplayTags FASGameplayTags::GameplayTags;

void FASGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	 
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases Armor, Health Regeneration, and Block Chance"));
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases Mana, Mana Regeneration, and Elemental Resistances"));
	
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("Increases Critical Hit Chance, Critical Hit Damage, and Dodge Chance"));

	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Max Health"));
	
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Max Mana"));
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of health regenerated per 1 second"));
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of mana regenerated per 1 second"));

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces physical damage taken"));
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance of dealing critical damage"));
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Amount of damage dealt when dealing critical hits"));
	
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to block incoming damage"));
	
	GameplayTags.Attributes_Secondary_DodgeChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.DodgeChance"),
		FString("Chance of avoiding damage"));
	
	GameplayTags.Attributes_Secondary_FireResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.FireResistance"),
		FString("Reduces fire damage taken"));
	
	GameplayTags.Attributes_Secondary_ColdResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ColdResistance"),
		FString("Reduces cold damage taken"));
	
	GameplayTags.Attributes_Secondary_LightningResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.LightningResistance"),
		FString("Reduces lightning damage taken"));
	
	GameplayTags.Attributes_Secondary_PoisonResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.PoisonResistance"),
		FString("Reduces poison damage taken"));

	/*
	 * Vital Attributes
	 */

	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Health"),
		FString("Health"));

	GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Vital.Mana"),
		FString("Mana"));
	
	/*
	 * Meta Attributes
	 */

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute"));
	
	/*
	 * Input Tags
	 */

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button"));

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 Key"));

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 Key"));

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 Key"));

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 Key"));

	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.5"),
		FString("Input Tag for 5 Key"));
	
	GameplayTags.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.6"),
		FString("Input Tag for 6 Key"));

	/*
	 * Damage Types
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type"));

	GameplayTags.Damage_Cold = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Cold"),
		FString("Cold Damage Type"));
	
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type"));
	
	GameplayTags.Damage_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Poison"),
		FString("Poison Damage Type"));
	
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type"));

	/*
	 * Debuff Types
	 */

	GameplayTags.Debuff_Ignite = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Ignite"),
		FString("Debuff for Ignite"));

	GameplayTags.Debuff_Slow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Slow"),
		FString("Debuff for Slow"));

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff for Stun"));
	
	GameplayTags.Debuff_Frozen = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frozen"),
		FString("Debuff for Frozen"));

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance"));

	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage"));

	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration"));
	
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency"));
	

	/*
	 * Map of Damage Types to Resistances
	 */

	GameplayTags.DamageToResistances.Add(GameplayTags.Damage_Cold, GameplayTags.Attributes_Secondary_ColdResistance);
	GameplayTags.DamageToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Secondary_FireResistance);
	GameplayTags.DamageToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Secondary_LightningResistance);
	GameplayTags.DamageToResistances.Add(GameplayTags.Damage_Poison, GameplayTags.Attributes_Secondary_PoisonResistance);

	/*
	 * Map of Damage Types to Debuffs
	 */

	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Cold, GameplayTags.Debuff_Frozen);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Ignite);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Poison, GameplayTags.Debuff_Slow);
	
	/*
	 * Effects
	 */
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting"));

	/*
	 * Abilities
	 */

	GameplayTags.Abilities = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities"),
		FString("Ability Tag"));

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability Tag - like the nullptr for Ability Tags"));
	
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag"));

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag"));

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("Fire Bolt Ability Tag"));

	GameplayTags.Abilities_Lightning_LightningBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.LightningBolt"),
		FString("Lightning Bolt Ability Tag"));

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag"));

	/*
	 * Passive Abilities
	 */

	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.LifeSiphon"),
		FString("Life Siphon"));

	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"),
		FString("Mana Siphon"));

	GameplayTags.Abilities_Passive_AstralProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Passive.AstralProtection"),
		FString("Astral Protection"));

	/*
	 * Ability Statuses and Types
	 */

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability Tag"));

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
	FString("Locked Status"));

	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status"));

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status"));

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status"));

	GameplayTags.Abilities_Type_Pressable = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Pressable"),
		FString("Type Pressable"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Type Passive"));

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None"));
	
	/*
	 * Cooldown
	 */

	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("Fire Bolt Cooldown Tag"));

	GameplayTags.Cooldown_Lightning_LightningBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.LightningBolt"),
		FString("Lightning Bolt Cooldown Tag"));

	GameplayTags.Cooldown_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Lightning.Electrocute"),
		FString("Electrocute Cooldown Tag"));

	/*
	 * Combat Sockets
	 */
	
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon"));

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand"));

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand"));

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail"));

	/*
	 * Montage Tags
	 */
	
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack 1"));

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack 2"));

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack 3"));

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack 4"));

	/*
	 * Player Tags
	 */

	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Attack.CursorTrace"),
		FString("Block tracing under the cursor")
		);

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Attack.InputHeld"),
		FString("Block Input Held callback for input")
		);

	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Attack.InputPressed"),
		FString("Block Input Pressed callback for input")
		);

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Attack.InputReleased"),
		FString("Block Input Released callback for input")
		);
}
