#ifndef __IVANDEF_H__
#define __IVANDEF_H__

/*
 * Global defines for the project IVAN.
 * This file is created to decrease the need of including headers in other headers just for the sake of some silly macros,
 * because it decreases compilation efficiency and may even cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple source files
 * 2. all inline functions used in multiple source files and independent enough (do not require other headers)
 * 3. class construction macros used in multiple header files
 *
 * DO NOT INCLUDE ANY FILES IN THIS HEADER.
 */

#ifndef VERSION
#define VERSION "0.40d"
#endif

#define DATA_BASE_VALUE(type, data) type Get##data() const { return DataBase->data; }
#define DATA_BASE_VALUE_WITH_PARAMETER(type, data, param) type Get##data(param) const { return DataBase->data; }
#define DATA_BASE_BOOL(data) bool data() const { return DataBase->data; }

#if defined(WIN32) || defined(__DJGPP__)
#define GAME_DIR std::string("")
#define SAVE_DIR std::string("Save/")
#define HOME_DIR std::string("")
#endif

/* The program can only create directories to the deepness of one, no more... */

#ifdef USE_SDL
#define GAME_DIR (std::string(DATADIR) + "/ivan/")
#define SAVE_DIR (std::string(getenv("HOME")) + "/IvanSave/")
#define HOME_DIR (std::string(getenv("HOME")) + "/")
#endif

#define HAS_HIT 0
#define HAS_BLOCKED 1
#define HAS_DODGED 2
#define HAS_DIED 3
#define DID_NO_DAMAGE 4

#define BLOATED_LEVEL 100000
#define SATIATED_LEVEL 50000
#define NOT_HUNGER_LEVEL 10000
#define HUNGER_LEVEL 2500

#define OVER_LOADED 0
#define STRESSED 1
#define BURDENED 2
#define UNBURDENED 3

#define STARVING 0
#define HUNGRY 1
#define NOT_HUNGRY 2
#define SATIATED 3
#define BLOATED 4

#define STATES 14

#define POLYMORPHED 1
#define HASTE 2
#define SLOW 4
#define POLYMORPH_CONTROL 8
#define LIFE_SAVED 16
#define LYCANTHROPY 32
#define INVISIBLE 64
#define INFRA_VISION 128
#define ESP 256
#define POISONED 512
#define TELEPORT 1024 /* Teleports every now and then */
#define POLYMORPH 2048 /* Polymorph randomly every now and then */
#define TELEPORT_CONTROL 4096
#define PANIC 8192

#define TORSO 1
#define HEAD 2
#define RIGHT_ARM 4
#define LEFT_ARM 8
#define ARMS (RIGHT_ARM|LEFT_ARM)
#define GROIN 16
#define RIGHT_LEG 32
#define LEFT_LEG 64
#define LEGS (RIGHT_LEG|LEFT_LEG)
#define OTHER 128
#define ALL 255

#define PHYSICAL_DAMAGE 1
#define SOUND 2
#define ACID 3
#define ENERGY 4
#define FIRE 5
#define POISON 6
#define BULIMIA 7

#define UNDEFINED 0
#define MALE 1
#define FEMALE 2
#define TRANSSEXUAL 3

/* The maximum bodyparts a character can have */

#define MAX_BODYPARTS 7
#define HUMANOID_BODYPARTS 7

#define TORSO_INDEX 0
#define HEAD_INDEX 1
#define RIGHT_ARM_INDEX 2
#define LEFT_ARM_INDEX 3
#define GROIN_INDEX 4
#define RIGHT_LEG_INDEX 5
#define LEFT_LEG_INDEX 6

#define NONE_INDEX MAX_BODYPARTS

#define DIRECTION_COMMAND_KEYS 8
#define EXTENDED_DIRECTION_COMMAND_KEYS 9
#define YOURSELF 8
#define RANDOM_DIR 9

#define LIGHT_BORDER 80

#define ALPP 0
#define ALP 1
#define AL 2
#define ALM 3
#define ANP 4
#define AN 5
#define ANM 6
#define ACP 7
#define AC 8
#define ACM 9
#define ACMM 10

#define UNARTICLED 0
#define PLURAL 1
#define ARTICLE_BIT 2
#define DEFINITE 2
#define INDEFINE_BIT 4
#define INDEFINITE 6

#define NORMAL_ARTICLE 0
#define NO_ARTICLE 1
#define DEFINITE_ARTICLE 2

#define TRANSPARENT_COLOR 0xF81F // pink

#define RAW_TYPES 5

#define GR_GLTERRAIN 0
#define GR_OLTERRAIN 1
#define GR_ITEM 2
#define GR_CHARACTER 3
#define GR_HUMANOID 4

#define GRAPHIC_TYPES 5

#define GR_WTERRAIN 0
#define GR_FOW 1
#define GR_CURSOR 2
#define GR_SYMBOL 3
#define GR_MENU 4

#define ST_NORMAL (0 << 3)
#define ST_RIGHT_ARM (1 << 3)
#define ST_LEFT_ARM (2 << 3)
#define ST_GROIN (3 << 3)
#define ST_RIGHT_LEG (4 << 3)
#define ST_LEFT_LEG (5 << 3)
#define ST_FLAME 64 

#define SILHOUETTE_X_SIZE 64
#define SILHOUETTE_Y_SIZE 64

#define ITEM_CATEGORIES 18

#define ANY_CATEGORY 0
#define HELMET 1
#define AMULET 2
#define CLOAK 4
#define BODY_ARMOR 8
#define WEAPON 16
#define SHIELD 32
#define RING 64
#define GAUNTLET 128
#define BELT 256
#define BOOT 512
#define FOOD 1024
#define POTION 2048
#define SCROLL 4096
#define BOOK 8192
#define WAND 16384
#define TOOL 32768
#define VALUABLE 65536
#define MISC 131072

#define NUMBER_OF_LOCK_TYPES 3 // damaged lock type does not count

#define ROUND 0
#define SQUARE 1
#define TRIANGULAR 2
#define DAMAGED 3 // lock is too damaged to be used again

#define GOOD 1
#define NEUTRAL 2
#define EVIL 3

/* ConsumeTypes */

#define CT_FRUIT 1
#define CT_MEAT 2
#define CT_METAL 4
#define CT_MINERAL 8
#define CT_LIQUID 16
#define CT_BONE 32
#define CT_PROCESSED 64
#define CT_MISC_ORGANIC 128
#define CT_PLASTIC 256
#define CT_GAS 512

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3
#define CENTER 4
#define HIDDEN 5

#define HOSTILE 0
#define UNCARING 1
#define FRIEND 2

#define MARTIAL_SKILL_CATEGORIES 3
#define WEAPON_SKILL_CATEGORIES 16

#define UNARMED 0
#define KICK 1
#define BITE 2
#define UNCATEGORIZED 3
#define DAGGERS 4
#define SMALL_SWORDS 5
#define LARGE_SWORDS 6
#define CLUBS 7
#define HAMMERS 8
#define MACES 9
#define FLAILS 10
#define AXES 11
#define POLE_ARMS 12
#define SPEARS 13
#define WHIPS 14
#define SHIELDS 15

#define LOCKED 1

#define EFFECT_NOTHING 0
#define EFFECT_POISON 1
#define EFFECT_DARKNESS 2
#define EFFECT_OMMEL_URINE 3
#define EFFECT_PEPSI 4
#define EFFECT_KOBOLD_FLESH 5
#define EFFECT_HEAL 6
#define EFFECT_LYCANTHROPY 7
#define EFFECT_SCHOOL_FOOD 8
#define EFFECT_ANTIDOTE 9

/* CEM = Consume End Message */

#define CEM_NOTHING 0
#define CEM_SCHOOL_FOOD 1
#define CEM_BONE 2
#define CEM_FROG_FLESH 3
#define CEM_OMMEL_URINE 4
#define CEM_PEPSI 5
#define CEM_KOBOLD_FLESH 6
#define CEM_HEALING_LIQUID 7
#define CEM_ANTIDOTE 8

/* HM = Hit Message */

#define HM_NOTHING 0
#define HM_SCHOOL_FOOD 1
#define HM_FROG_FLESH 2
#define HM_OMMEL_URINE 3
#define HM_PEPSI 4
#define HM_KOBOLD_FLESH 5
#define HM_HEALING_LIQUID 6
#define HM_ANTIDOTE 7

#define MATERIAL_ID 4096

#define IRON (MATERIAL_ID + 1)
#define VALPURIUM (MATERIAL_ID + 2)
#define STONE (MATERIAL_ID + 3)
#define GRAVEL (MATERIAL_ID + 4)
#define MORAINE (MATERIAL_ID + 5)
#define WOOD (MATERIAL_ID + 6)
#define GLASS (MATERIAL_ID + 7)
#define PARCHMENT (MATERIAL_ID + 8)
#define CLOTH (MATERIAL_ID + 9)
#define MITHRIL (MATERIAL_ID + 10)
#define MARBLE (MATERIAL_ID + 11)
#define GOLD (MATERIAL_ID + 12)
#define GRASS (MATERIAL_ID + 13)
#define LEATHER (MATERIAL_ID + 14)
#define LEAF (MATERIAL_ID + 15)
#define FABRIC (MATERIAL_ID + 16)
#define PALM_LEAF (MATERIAL_ID + 17)
#define SULFUR (MATERIAL_ID + 18)
#define UNICORN_HORN (MATERIAL_ID + 19)
#define DIAMOND (MATERIAL_ID + 20)
#define SILVER (MATERIAL_ID + 21)
#define SAPPHIRE (MATERIAL_ID + 22)
#define RUBY (MATERIAL_ID + 23)
#define BRONZE (MATERIAL_ID + 24)
#define COPPER (MATERIAL_ID + 25)
#define TIN (MATERIAL_ID + 26)
#define STEEL (MATERIAL_ID + 27)
#define SPIDER_SILK (MATERIAL_ID + 28)
#define KEVLAR (MATERIAL_ID + 29)
#define OMMEL_HAIR (MATERIAL_ID + 30)
#define HARDENED_LEATHER (MATERIAL_ID + 31)
#define TROLL_HIDE (MATERIAL_ID + 32)
#define NYMPH_HAIR (MATERIAL_ID + 33)
#define ANGEL_HAIR (MATERIAL_ID + 34)
#define PHOENIX_FEATHER (MATERIAL_ID + 35)
#define GOLDEN_EAGLE_FEATHER (MATERIAL_ID + 36)

#define ORGANIC_SUBSTANCE_ID (4096 * 2)

#define BANANA_FLESH (ORGANIC_SUBSTANCE_ID + 1)
#define SCHOOL_FOOD (ORGANIC_SUBSTANCE_ID + 2)
#define BANANA_PEEL (ORGANIC_SUBSTANCE_ID + 3)
#define KIWI_FLESH (ORGANIC_SUBSTANCE_ID + 4)
#define PINEAPPLE_FLESH (ORGANIC_SUBSTANCE_ID + 5)
#define FIBER (ORGANIC_SUBSTANCE_ID + 6)
#define BONE (ORGANIC_SUBSTANCE_ID + 7)
#define BREAD (ORGANIC_SUBSTANCE_ID + 8)

#define GAS_ID (4096 * 3)

#define AIR (GAS_ID + 1)

#define LIQUID_ID (4096 * 4)

#define OMMEL_URINE (LIQUID_ID + 1)
#define PEPSI (LIQUID_ID + 2)
#define WATER (LIQUID_ID + 3)
#define HEALING_LIQUID (LIQUID_ID + 4)
#define BLOOD (LIQUID_ID + 5)
#define BROWN_SLIME (LIQUID_ID + 6)
#define POISON_LIQUID (LIQUID_ID + 7)
#define VALDEMAR (LIQUID_ID + 8)
#define ANTITODE_LIQUID (LIQUID_ID + 9)

#define FLESH_ID (4096 * 5)

#define GOBLINOID_FLESH (FLESH_ID + 1)
#define PORK (FLESH_ID + 2)
#define BEEF (FLESH_ID + 3)
#define FROG_FLESH (FLESH_ID + 4)
#define ELPURI_FLESH (FLESH_ID + 5)
#define HUMAN_FLESH (FLESH_ID + 6)
#define DOLPHIN_FLESH (FLESH_ID + 7)
#define POLAR_BEAR_FLESH (FLESH_ID + 8)
#define WOLF_FLESH (FLESH_ID + 9)
#define DOG_FLESH (FLESH_ID + 10)
#define ENNER_BEAST_FLESH (FLESH_ID + 11)
#define SPIDER_FLESH (FLESH_ID + 12)
#define JACKAL_FLESH (FLESH_ID + 13)
#define ASS_FLESH (FLESH_ID + 14)
#define BAT_FLESH (FLESH_ID + 15)
#define WERE_WOLF_FLESH (FLESH_ID + 16)
#define KOBOLD_FLESH (FLESH_ID + 17)
#define GIBBERLING_FLESH (FLESH_ID + 18)
#define CAT_FLESH (FLESH_ID + 19)
#define RAT_FLESH (FLESH_ID + 20)
#define ANGEL_FLESH (FLESH_ID + 21)
#define DWARF_FLESH (FLESH_ID + 22)
#define DAEMON_FLESH (FLESH_ID + 23)
#define MAMMOTH_FLESH (FLESH_ID + 24)
#define BLACK_UNICORN_FLESH (FLESH_ID + 25)
#define GRAY_UNICORN_FLESH (FLESH_ID + 26)
#define WHITE_UNICORN_FLESH (FLESH_ID + 27)
#define LION_FLESH (FLESH_ID + 28)
#define BUFFALO_FLESH (FLESH_ID + 29)
#define SNAKE_FLESH (FLESH_ID + 30)
#define ORC_FLESH (FLESH_ID + 31)

#define POWDER_ID (4096 * 6)

#define GUN_POWDER (POWDER_ID + 1)

#define UNARMED_ATTACK 0
#define WEAPON_ATTACK 1
#define KICK_ATTACK 2
#define BITE_ATTACK 3

#define USE_ARMS 1
#define USE_LEGS 2
#define USE_HEAD 4

#define ATTRIBUTES 10
#define BASE_ATTRIBUTES 6

#define ENDURANCE 0
#define PERCEPTION 1
#define INTELLIGENCE 2
#define WISDOM 3
#define CHARISMA 4
#define MANA 5

#define ARM_STRENGTH 6
#define LEG_STRENGTH 7
#define DEXTERITY 8
#define AGILITY 9

#define NO 0
#define YES 1
#define REQUIRES_ANSWER -1

#define DIR_ERROR 0xFF
#define DIR_ERROR_VECTOR vector2d(-666, -666)

#define GLOBAL_WEAK_BODYPART_HIT_MODIFIER 5.0f

#define HELMET_INDEX 0
#define AMULET_INDEX 1
#define CLOAK_INDEX 2
#define BODY_ARMOR_INDEX 3
#define BELT_INDEX 4
#define RIGHT_WIELDED_INDEX 5
#define LEFT_WIELDED_INDEX 6
#define RIGHT_RING_INDEX 7
#define LEFT_RING_INDEX 8
#define RIGHT_GAUNTLET_INDEX 9
#define LEFT_GAUNTLET_INDEX 10
#define RIGHT_BOOT_INDEX 11
#define LEFT_BOOT_INDEX 12

#define BROKEN 256

#define LONG_SWORD 1
#define TWO_HANDED_SWORD 2
#define TWO_HANDED_SCIMITAR 3
#define SPEAR 4
#define AXE 5
#define HALBERD 6
#define MACE 7
#define HAMMER 8
#define SICKLE 9
#define DAGGER 10
#define SHORT_SWORD 11

#define CHAIN_MAIL 1
#define PLATE_MAIL 2
#define ARMOR_OF_GREAT_HEALTH 3

#define CLOAK_OF_INVISIBILITY 1

#define BOOT_OF_STRENGTH 1
#define BOOT_OF_AGILITY 2

#define GAUNTLET_OF_STRENGTH 1
#define GAUNTLET_OF_DEXTERITY 2

#define RING_OF_FIRE_RESISTANCE 1
#define RING_OF_POLYMORPHCONTROL 2
#define RING_OF_INFRA_VISION 3
#define RING_OF_TELEPORTATION 4
#define RING_OF_TELEPORT_CONTROL 5
#define RING_OF_POLYMORPH 6
#define RING_OF_POISON_RESISTANCE 7
#define RING_OF_INVISIBILITY 8

#define AMULET_OF_LIFE_SAVING 1
#define AMULET_OF_ESP 2

#define HELM_OF_PERCEPTION 1
#define HELM_OF_UNDERSTANDING 2
#define HELM_OF_BRILLIANCE 3
#define HELM_OF_ATTRACTIVITY 4

#define BELT_OF_CARRYING 1

#define SMALL_CHEST 1
#define CHEST 2
#define LARGE_CHEST 3
#define STRONG_BOX 4

#define ROOKIE 1
#define VETERAN 2
#define ELITE 3
#define MASTER 4
#define GRAND_MASTER 5

#define WARRIOR 1
#define WAR_LORD 2

#define BERSERKER 1
#define BUTCHER 2
#define PRINCE 3
#define KING 4

#define CONICAL 1
#define FLAT 2

#define TORTURING_CHIEF 1
#define WHIP_CHAMPION 2
#define WAR_LADY 3
#define QUEEN 4

#define CHIEFTAIN 1
#define LORD 2

#define SLAUGHTERER 1
#define SQUAD_LEADER 2
#define OFFICER 3
#define GENERAL 4

#define PARQUET 1
#define FLOOR 2
#define GROUND 3
#define GRASS_TERRAIN 4

#define POOL 1

#define BRICK_FINE 1
#define BRICK_PROPAGANDA 2
#define BRICK_OLD 3
#define EARTH 4

#define PINE 1
#define SPRUCE 2
#define LINDEN 3
#define CARPET 4
#define COUCH 5
#define DOUBLE_BED 6
#define POOL_BORDER 7
#define POOL_CORNER 8
#define PALM 9

#define STAIRS_UP 1
#define STAIRS_DOWN 2

#define BOOK_CASE 1

#define WORLD_MAP 255

/* Prices */
#define PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR 50
#define PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR 100

/* How many turns it takes for a angel to attach a bodypart again */
#define LENGTH_OF_ANGELS_HEAL_COUNTER_LOOP 2500

/* Hard-coded teams */
#define MONSTER_TEAM 1
#define PLAYER_TEAM 0

#define LOAD 1
#define NO_PIC_UPDATE 2
#define NO_EQUIPMENT_PIC_UPDATE (NO_PIC_UPDATE << 1)
#define NO_MATERIALS 8
#define NO_EQUIPMENT 16

#define NOT_WALKABLE 1
#define HAS_CHARACTER 2
#define IN_ROOM 4
#define NOT_IN_ROOM 8
#define ATTACHABLE (16|NOT_IN_ROOM) /* overrides IN_ROOM */

#define ELPURI_CAVE 1
#define ATTNAM 2
#define NEW_ATTNAM 3
#define UNDER_WATER_TUNNEL 4
#define UNDER_WATER_TUNNEL_EXIT 5

#define DARK_LEVEL 4
#define OREE_LAIR 6

#define NO_SELECT 1
#define NO_MULTI_SELECT 2
#define NO_SPECIAL_INFO 4
#define REMEMBER_SELECTED 8

#define RECTANGLE 1
#define ROUND_CORNERS 2

/* Gods, 0 == none */

#define VALPURI 1
#define LEGIFER 2
#define ATAVUS 3
#define DULCIS 4
#define SEGES 5
#define SOPHOS 6
#define SILVA 7
#define LORICATUS 8
#define MELLIS 9
#define CLEPTIA 10
#define NEFAS 11
#define SCABIES 12
#define INFUSCOR 13
#define CRUENTUS 14
#define MORTIFER 15

#define MAX_PRICE 4294967295UL

#endif
