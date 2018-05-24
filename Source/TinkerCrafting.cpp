#include "StdAfx.h"
#include "config.h"
#include "WeenieObject.h"
#include "TinkerCrafting.h"
#include <fstream>

#define TINKERCRAFTING_OUTPUT 0 //Output tinker stats to server output - Mostly for debugging purposes

//Percent numbers in decimal format
#define TINKERCRAFTING_BONUS_CHANCE .0 //Static bonus in decimal added to the players chance. This is added on before the imbue cap
#define TINKERCRAFTING_IMBUE_CAP .33 //Cap chance to 33% not reduce chance by a third - Incrase to make imbues easier

//Rolls are 0.0 - 1.0 ie: .25, .48, .63, etc... - Rolls to attempt to beat the player 
#define TINKERCRAFT_ROLL_MIN 0.0 //Minimum roll - Increasing this makes the hard rolls harder 
#define TINKERCRAFT_ROLL_MAX 1.0 //Maximum roll - Decreasing this makes the easy rolls easier

bool TinkerCrafting::CalculateChance(CWeenieObject *Salvage, CWeenieObject *Equipment, int PlayerSkill)
{
	//Todo : Playerskill is not reflecting buffs. 
	//It does use the Attributes + Base Skill to
	//come up with the skill level but buffing
	//did not change this number.

	//Salvage
	double SalvageWS = Salvage->InqIntQuality(ITEM_WORKMANSHIP_INT, 0); //Salvage Material - Workmanship
	if (Salvage->InqIntQuality(ITEM_TYPE_INT, 0) == ITEM_TYPE::TYPE_TINKERING_MATERIAL)
		SalvageWS /= (double)Salvage->InqIntQuality(NUM_ITEMS_IN_MATERIAL_INT, 1);
	int iMatType = Salvage->InqIntQuality(MATERIAL_TYPE_INT, 0);
	char hMatType[16];
	_itoa(iMatType, hMatType, 16);
	std::string SalvageMaterialName; //Salvage Material - English name
	int SalvageModValue; //Salvage Material - Mod value
	int SalvageImbueBool; //Salvage Material - Imbue Bool
	//MaterialTypeEnumMapper::MaterialTypeToString((unsigned int)iMatType, SalvageMaterialName);
	TinkerCrafting::MaterialInfo((unsigned int)iMatType, SalvageMaterialName, SalvageModValue, SalvageImbueBool);

	//Equipment
	int EquipmentTinkCount = Equipment->InqIntQuality(NUM_TIMES_TINKERED_INT, 0);
	int EquipmentTinkAttempt = EquipmentTinkCount + 1; //Equipment - Tinker attempt number (always 1 higher then current amount)
	float EquipmentTinkMod = TinkerCrafting::TinkerAttemptMod(EquipmentTinkAttempt);
	
	int EquipmentWS = Equipment->InqIntQuality(ITEM_WORKMANSHIP_INT, 0); //Equipment - Workmanship of the item that is being tinkered

	//Player
	PlayerSkill; //Player - Tinker skill from the player that is being used

	int ReqBetterMats = (SalvageWS <= EquipmentWS) ? 1 : 2;

	//Calculation
	float Difficulty = floor (((5 * SalvageModValue) + (2 * EquipmentWS * SalvageModValue) - (SalvageWS * ReqBetterMats * SalvageModValue / 5)) * EquipmentTinkMod);
	float chance = (1 - (1 / (1 + exp(0.03 * (PlayerSkill - Difficulty))))) + TINKERCRAFTING_BONUS_CHANCE;
	if (SalvageImbueBool && chance > (float)TINKERCRAFTING_IMBUE_CAP)
		chance = (float) TINKERCRAFTING_IMBUE_CAP;
	float rando = Random::RollDice(TINKERCRAFT_ROLL_MIN, TINKERCRAFT_ROLL_MAX); //Roll the dice!

	if (TINKERCRAFTING_OUTPUT) {
		LOG(Data, Normal, csprintf("Salvage:\n  Salvage WS: %f\n  Material: %i\n", SalvageWS, iMatType));
		LOG(Data, Normal, csprintf("Equipment:\n  Workmanship: %i\n  Attempt: %i\n", EquipmentWS, EquipmentTinkAttempt));
		LOG(Data, Normal, csprintf("Character:\n  Skill Level: %i\n", PlayerSkill));
		LOG(Data, Normal, csprintf("Results:\n  Imbue: %i\n  Difficulty: %f\n  Chance: %f\n  Roll: %f\n", SalvageImbueBool, Difficulty, chance, rando));
		//LOG(Data, Normal, csprintf("Tinker chance: %f. Roll: %f.", successChance, rando));
	}

	return (rando <= chance) ? true : false;
}

float TinkerCrafting::TinkerAttemptMod(int tink) {
	switch (tink)
	{
	case 1:
		return 1.0f;
	case 2:
		return 1.1f;
	case 3:
		return 1.3f;
	case 4:
		return 1.6f;
	case 5:
		return 2.0f;
	case 6:
		return 2.5f;
	case 7:
		return 3.0f;
	case 8:
		return 3.5f;
	case 9:
		return 4.0f;
	case 10:
		return 4.5f;
	default:
		return 1.0f;
	}
}
bool TinkerCrafting::MaterialInfo(unsigned int ct, std::string &name, int &mod, int &imbue) {
	mod = 0;
	imbue = 0;
	switch (ct)
	{
	case 1u:
		name = "Ceramic";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 2u:
		name = "Porcelain";
		mod = 12;
		imbue = 0;
		return TRUE;
	case 3u:
		name = "Cloth";
		return TRUE;
	case 4u:
		name = "Linen";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 5u:
		name = "Satin";
		return TRUE;
	case 6u:
		name = "Silk";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 7u:
		name = "Velvet";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 8u:
		name = "Wool";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 9u:
		name = "Gem";
		return TRUE;
	case 0xAu:
		name = "Agate";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0xBu:
		name = "Amber";
		return TRUE;
	case 0xCu:
		name = "Amethyst";
		return TRUE;
	case 0xDu:
		name = "Aquamarine";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0xEu:
		name = "Azurite";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0xFu:
		name = "Black Garnet";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x10u:
		name = "Black Opal";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x11u:
		name = "Bloodstone";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x12u:
		name = "Carnelian";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x13u:
		name = "Citrine";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x14u:
		name = "Diamond";
		return TRUE;
	case 0x15u:
		name = "Emerald";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x16u:
		name = "Fire Opal";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x17u:
		name = "Green Garnet";
		return TRUE;
	case 0x18u:
		name = "Green Jade";
		return TRUE;
	case 0x19u:
		name = "Hematite";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x1Au:
		name = "Imperial Topaz";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x1Bu:
		name = "Jet";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x1Cu:
		name = "Lapis Lazuli";
		return TRUE;
	case 0x1Du:
		name = "Lavender Jade";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x1Eu:
		name = "Malachite";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x1Fu:
		name = "Moonstone";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x20u:
		name = "Onyx";
		return TRUE;
	case 0x21u:
		name = "Opal";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x22u:
		name = "Peridot";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x23u:
		name = "Red Garnet";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x24u:
		name = "Red Jade";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x25u:
		name = "Rose Quartz";
		mod = 25;
		imbue = 0;
		return TRUE;
	case 0x26u:
		name = "Ruby";
		return TRUE;
	case 0x27u:
		name = "Sapphire";
		return TRUE;
	case 0x28u:
		name = "Smoky Quartz";
		return TRUE;
	case 0x29u:
		name = "Sunstone";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x2Au:
		name = "Tiger Eye";
		return TRUE;
	case 0x2Bu:
		name = "Tourmaline";
		return TRUE;
	case 0x2Cu:
		name = "Turquoise";
		return TRUE;
	case 0x2Du:
		name = "White Jade";
		return TRUE;
	case 0x2Eu:
		name = "White Quartz";
		return TRUE;
	case 0x2Fu:
		name = "White Sapphire";
		mod = 20;
		imbue = 1;
		return TRUE;
	case 0x30u:
		name = "Yellow Garnet";
		return TRUE;
	case 0x31u:
		name = "Yellow Topaz";
		mod = 20;
		imbue = 0;
		return TRUE;
	case 0x32u:
		name = "Zircon";
		mod = 20;
		imbue = 0;
		return TRUE;
	case 0x33u:
		name = "Ivory";
		mod = 0;
		imbue = 0;
		return TRUE;
	case 0x34u:
		name = "Leather";
		mod = 0;
		imbue = 0;
		return TRUE;
	case 0x35u:
		name = "Armoredillo Hide";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x36u:
		name = "Gromnie Hide";
		return TRUE;
	case 0x37u:
		name = "Reed Shark Hide";
		return TRUE;
	case 0x38u:
		name = "Metal";
		return TRUE;
	case 0x39u:
		name = "Brass";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x3Au:
		name = "Bronze";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x3Bu:
		name = "Copper";
		mod = 15;
		imbue = 0;
		return TRUE;
	case 0x3Cu:
		name = "Gold";
		mod = 10;
		imbue = 0;
		return TRUE;
	case 0x3Du:
		name = "Iron";
		mod = 12;
		imbue = 0;
		return TRUE;
	case 0x3Eu:
		name = "Pyreal";
		return TRUE;
	case 0x3Fu:
		name = "Silver";
		mod = 15;
		imbue = 0;
		return TRUE;
	case 0x40u:
		name = "Steel";
		mod = 12;
		imbue = 0;
		return TRUE;
	case 0x41u:
		name = "Stone";
		return TRUE;
	case 0x42u:
		name = "Alabaster";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x43u:
		name = "Granite";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x44u:
		name = "Marble";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x45u:
		name = "Obsidian";
		return TRUE;
	case 0x46u:
		name = "Sandstone";
		return TRUE;
	case 0x47u:
		name = "Serpentine";
		return TRUE;
	case 0x48u:
		name = "Wood";
		return TRUE;
	case 0x49u:
		name = "Ebony";
		mod = 12;
		imbue = 0;
		return TRUE;
	case 0x4Au:
		name = "Mahogany";
		mod = 12;
		imbue = 0;
		return TRUE;
	case 0x4Bu:
		name = "Oak";
		mod = 10;
		imbue = 0;
		return TRUE;
	case 0x4Cu:
		name = "Pine";
		mod = 11;
		imbue = 0;
		return TRUE;
	case 0x4Du:
		name = "Teak";
		mod = 12;
		imbue = 0;
		return TRUE;
	case 0x80u:
		name = "NumMaterialTypes";
		mod = 11;
		imbue = false;
		return TRUE;
	default:
		name = "Unknown";
		mod = 11;
		imbue = false;
		return FALSE;
	}
	return false;
}