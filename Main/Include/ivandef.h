#ifndef __IVANDEF_H__
#define __IVANDEF_H__

/*
 * Global defines for the project IVAN.
 * This file is created to decrease the need of including headers in other headers just for the sake of some silly macros,
 * because it decreases compilation efficiency and may even cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple .cpp or .h files, except those #defined in confdef.h
 * 2. all inline functions used in multiple .cpp or .h files and independent enough (do not require other headers)
 * 3. class construction macros used in multiple .h files
 *
 * DO NOT INCLUDE ANY FILES IN THIS HEADER.
 */

#define IVAN_VERSION "0.410"

#define DATA_BASE_VALUE(type, data) type Get##data() const { return DataBase->data; }
#define DATA_BASE_VALUE_WITH_PARAMETER(type, data, param) type Get##data(param) const { return DataBase->data; }
#define DATA_BASE_BOOL(data) bool data() const { return DataBase->data; }

#define HAS_HIT 0
#define HAS_BLOCKED 1
#define HAS_DODGED 2
#define HAS_DIED 3
#define DID_NO_DAMAGE 4
#define HAS_FAILED 5

#define OVER_FED_LEVEL 175000
#define BLOATED_LEVEL 150000
#define SATIATED_LEVEL 100000
#define NOT_HUNGER_LEVEL 25000
#define HUNGER_LEVEL 5000

#define OVER_LOADED 0
#define STRESSED 1
#define BURDENED 2
#define UNBURDENED 3

#define STARVING 0
#define HUNGRY 1
#define NOT_HUNGRY 2
#define SATIATED 3
#define BLOATED 4
#define OVER_FED 5

#define STATES 18

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
#define CONFUSED 16384
#define PARASITIZED 32768
#define SEARCHING 65536
#define GAS_IMMUNITY 131072

#define TORSO 1
#define HEAD 2
#define RIGHT_ARM 4
#define LEFT_ARM 8
#define ARMS 12
#define GROIN 16
#define RIGHT_LEG 32
#define LEFT_LEG 64
#define LEGS 96
#define OTHER 128
#define ALL 255

#define PHYSICAL_DAMAGE 1
#define SOUND 2
#define ACID 3
#define FIRE 4
#define ELECTRICITY 5
#define ENERGY 6
#define POISON 7
#define DRAIN 8

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

#ifndef LIGHT_BORDER
#define LIGHT_BORDER 80
#endif

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
#define GR_EFFECT 5

#define GRAPHIC_TYPES 5

#define GR_WTERRAIN 0
#define GR_FOW 1
#define GR_CURSOR 2
#define GR_SYMBOL 3
#define GR_MENU 4
#define GR_TRANSPARENT_COLOR_TILE 5

#define ST_NORMAL 0
#define ST_RIGHT_ARM 8
#define ST_LEFT_ARM 16
#define ST_GROIN 24
#define ST_RIGHT_LEG 32
#define ST_LEFT_LEG 40
#define ST_FLAME 64
#define ST_LIGHTNING 128

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

#define HOSTILE 1
#define UNCARING 2
#define FRIEND 4

#define MARTIAL_SKILL_CATEGORIES 3
#define WEAPON_SKILL_CATEGORIES 11

#define UNARMED 0
#define KICK 1
#define BITE 2
#define UNCATEGORIZED 3
#define SMALL_SWORDS 4
#define LARGE_SWORDS 5
#define BLUNT_WEAPONS 6
#define AXES 7
#define POLE_ARMS 8
#define WHIPS 9
#define SHIELDS 10

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
#define EFFECT_CONFUSE 10
#define EFFECT_POLYMORPH 11
#define EFFECT_ESP 12
#define EFFECT_SKUNK_SMELL 13
#define EFFECT_MAGIC_VAPOUR 14

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
#define CEM_ESP 12

/* HM = Hit Message */

#define HM_NOTHING 0
#define HM_SCHOOL_FOOD 1
#define HM_FROG_FLESH 2
#define HM_OMMEL_URINE 3
#define HM_PEPSI 4
#define HM_KOBOLD_FLESH 5
#define HM_HEALING_LIQUID 6
#define HM_ANTIDOTE 7
#define HM_CONFUSE 8

#define UNARMED_ATTACK 0
#define WEAPON_ATTACK 1
#define KICK_ATTACK 2
#define BITE_ATTACK 3
#define THROW_ATTACK 4

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

#define GLOBAL_WEAK_BODYPART_HIT_MODIFIER 10.0f

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

#define WORLD_MAP 255

/* Prices */
#define PRICE_TO_ATTACH_OLD_LIMB_AT_ALTAR 50
#define PRICE_TO_ATTACH_NEW_LIMB_AT_ALTAR 100

/* Hard-coded teams */
#define PLAYER_TEAM 0
#define MONSTER_TEAM 1
#define ATTNAM_TEAM 2
#define FINAL_BATTLE_TEAM 3
#define NEW_ATTNAM_TEAM 7
#define COLONIST_TEAM 8

#define LOAD 1
#define NO_PIC_UPDATE 2
#define NO_EQUIPMENT_PIC_UPDATE (NO_PIC_UPDATE << 1)
#define NO_MATERIALS 8
#define NO_EQUIPMENT 16
#define NO_SIGNALS 32

#define NOT_WALKABLE 1
#define HAS_CHARACTER 2
#define IN_ROOM 4
#define NOT_IN_ROOM 8
#define ATTACHABLE (16|NOT_IN_ROOM) /* overrides IN_ROOM */

#define RANDOM 0
#define ELPURI_CAVE 1
#define ATTNAM 2
#define NEW_ATTNAM 3
#define UNDER_WATER_TUNNEL 4
#define MONDEDR 5
#define UNDER_WATER_TUNNEL_EXIT 0x80

#define DARK_LEVEL 6
#define OREE_LAIR 9

#define NO_SELECT 1
#define NO_MULTI_SELECT 2
#define NO_SPECIAL_INFO 4
#define REMEMBER_SELECTED 8
#define NONE_AS_CHOICE 16
#define SELECT_PAIR 32

#define RECTANGLE 1
#define ROUND_CORNERS 2

/* Gods, 0 == none */

#define GODS 15

#define VALPURUS 1
#define LEGIFER 2
#define ATAVUS 3
#define DULCIS 4
#define SEGES 5
#define SOPHOS 6
#define TERRA 7
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

#define PERMANENT 0xFFFF

#define MISSED 0
#define HIT 1
#define CATCHED 2

#define BEAM_EFFECTS 11

#define BEAM_POLYMORPH 0
#define BEAM_STRIKE 1
#define BEAM_FIRE_BALL 2
#define BEAM_TELEPORT 3
#define BEAM_HASTE 4
#define BEAM_SLOW 5
#define BEAM_RESURRECT 6
#define BEAM_INVISIBILITY 7
#define BEAM_CLONE 8
#define BEAM_LIGHTNING 9
#define BEAM_DOOR_CREATION 10

#define BEAM_STYLES 3

#define PARTICLE_BEAM 0
#define LIGHTNING_BEAM 1
#define SHIELD_BEAM 2

#define RANDOM_COLOR 0x10000

/* Entry indices, not actual config defines */

#define STAIRS_UP 1
#define STAIRS_DOWN 2

#define NO_LIMIT 0xFFFF

#define ALL_ITEMS 0xFFFF

#endif
