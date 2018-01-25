/**
 * * The Ruby Server - a free and open-source Pokémon MMORPG server emulator
 * Copyright (C) 2018  Mark Samman (TFS) <mark.samman@gmail.com>
 *                     Leandro Matheus <kesuhige@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef FS_POKEMONS_H_776E8327BCE2450EB7C4A260785E6C0D
#define FS_POKEMONS_H_776E8327BCE2450EB7C4A260785E6C0D

#include "creature.h"


const uint32_t MAX_LOOTCHANCE = 100000;
const uint32_t MAX_STATICWALK = 100;

struct LootBlock {
	uint16_t id;
	uint32_t countmax;
	uint32_t chance;

	//optional
	int32_t subType;
	int32_t actionId;
	std::string text;

	std::vector<LootBlock> childLoot;
	LootBlock() {
		id = 0;
		countmax = 0;
		chance = 0;

		subType = -1;
		actionId = -1;
	}
};

struct summonBlock_t {
	std::string name;
	uint32_t chance;
	uint32_t speed;
	uint32_t max;
	bool force = false;
};

class BaseMove;
struct moveBlock_t {
	constexpr moveBlock_t() = default;
	~moveBlock_t();
	moveBlock_t(const moveBlock_t& other) = delete;
	moveBlock_t& operator=(const moveBlock_t& other) = delete;
	moveBlock_t(moveBlock_t&& other) :
		move(other.move),
		chance(other.chance),
		speed(other.speed),
		range(other.range),
		minCombatValue(other.minCombatValue),
		maxCombatValue(other.maxCombatValue),
		combatMove(other.combatMove),
		isMelee(other.isMelee) {
		other.move = nullptr;
	}

	BaseMove* move = nullptr;
	uint32_t chance = 100;
	uint32_t speed = 2000;
	uint32_t range = 0;
	int32_t minCombatValue = 0;
	int32_t maxCombatValue = 0;
	bool combatMove = false;
	bool isMelee = false;
};

struct voiceBlock_t {
	std::string text;
	bool yellText;
};

class PokemonType
{
	struct PokemonInfo {
		LuaScriptInterface* scriptInterface;

		std::map<CombatType_t, int32_t> elementMap;

		std::vector<voiceBlock_t> voiceVector;

		std::vector<LootBlock> lootItems;
		std::vector<std::string> scripts;
		std::vector<moveBlock_t> attackMoves;
		std::vector<moveBlock_t> defenseMoves;
		std::vector<summonBlock_t> summons;

		Skulls_t skull = SKULL_ORANGE;
		Outfit_t outfit = {};
		Gender_t gender = {};
		RaceType_t race = RACE_BLOOD;

		LightInfo light = {};
		float dittoChance = 0;

		uint16_t lookcorpse = 0;

		uint64_t experience = 0;

		uint32_t manaCost = 0;
		uint32_t yellChance = 0;
		uint32_t yellSpeedTicks = 0;
		uint32_t staticAttackChance = 95;
		uint32_t maxSummons = 0;
		uint32_t changeTargetSpeed = 0;
		uint32_t conditionImmunities = 0;
		uint32_t damageImmunities = 0;
		uint32_t baseSpeed = 200;

		int32_t creatureAppearEvent = -1;
		int32_t creatureDisappearEvent = -1;
		int32_t creatureMoveEvent = -1;
		int32_t creatureSayEvent = -1;
		int32_t thinkEvent = -1;
		int32_t targetDistance = 1;
		int32_t runAwayHealth = 0;
		int32_t health = 100;
		int32_t healthMax = 100;
		int32_t changeTargetChance =0;
		int32_t defense = 0;
		int32_t armor = 0;

		bool canPushItems = false;
		bool canPushCreatures = false;
		bool pushable = true;
		bool isSummonable = false;
		bool isIllusionable = false;
		bool isConvinceable = false;
		bool isAttackable = true;
		bool isHostile = true;
		bool hiddenHealth = false;
		bool canWalkOnEnergy = true;
		bool canWalkOnFire = true;
		bool canWalkOnPoison = true;
	};

	public:
		PokemonType() = default;

		// non-copyable
		PokemonType(const PokemonType&) = delete;
		PokemonType& operator=(const PokemonType&) = delete;

		std::string name;
		std::string nameDescription;

		PokemonInfo info;

		void createLoot(Container* corpse);
		bool createLootContainer(Container* parent, const LootBlock& lootblock);
		std::vector<Item*> createLootItem(const LootBlock& lootBlock);
};

class Pokemons
{
	public:
		Pokemons() = default;
		// non-copyable
		Pokemons(const Pokemons&) = delete;
		Pokemons& operator=(const Pokemons&) = delete;

		bool loadFromXml(bool reloading = false);
		bool isLoaded() const {
			return loaded;
		}
		bool reload();

		PokemonType* getPokemonType(const std::string& name);

		static uint32_t getLootRandom();

	private:
		ConditionDamage* getDamageCondition(ConditionType_t conditionType,
		                                    int32_t maxDamage, int32_t minDamage, int32_t startDamage, uint32_t tickInterval);
		bool deserializeMove(const pugi::xml_node& node, moveBlock_t& sb, const std::string& description = "");

		PokemonType* loadPokemon(const std::string& file, const std::string& pokemonName, bool reloading = false);

		void loadLootContainer(const pugi::xml_node& node, LootBlock&);
		bool loadLootItem(const pugi::xml_node& node, LootBlock&);

		std::map<std::string, PokemonType> pokemons;
		std::map<std::string, std::string> unloadedPokemons;
		std::unique_ptr<LuaScriptInterface> scriptInterface;

		bool loaded = false;
};

#endif
