#ifndef __IVANDEF_H__
#define __IVANDEF_H__

/* Global defines for the project IVAN.
   This file is created to decrease the need of including headers in other
   headers just for the sake of some silly macros, because it decreases
   compilation efficiency and may cause cross-including

   List of macros that should be gathered here:
   1. all numeric defines used in multiple .cpp or .h files, except those
      #defined in confdef.h
   2. all inline functions used in multiple .cpp or .h files and independent
      enough (do not require other headers)
   3. class construction macros used in multiple .h files */

#define IVAN_VERSION "0.440"

struct databasebase { int Config; };

#define DATA_BASE_VALUE(type, data) type Get##data() const { return DataBase->data; }
#define DATA_BASE_VALUE_WITH_PARAMETER(type, data, param) type Get##data(param) const { return DataBase->data; }
#define DATA_BASE_BOOL(data) bool data() const { return DataBase->data; }
#define DATA_BASE_BOOL_WITH_PARAMETER(data, param) bool data(param) const { return DataBase->data; }

#define HAS_HIT 0
#define HAS_BLOCKED 1
#define HAS_DODGED 2
#define HAS_DIED 3
#define DID_NO_DAMAGE 4
#define HAS_FAILED 5

#define OVER_FED_LEVEL 175000
#define BLOATED_LEVEL 150000
#define SATIATED_LEVEL 100000
#define NOT_HUNGER_LEVEL 30000
#define HUNGER_LEVEL 20000
#define VERY_HUNGER_LEVEL 10000

#define OVER_LOADED 0
#define STRESSED 1
#define BURDENED 2
#define UNBURDENED 3

#define STARVING 0
#define VERY_HUNGRY 1
#define HUNGRY 2
#define NOT_HUNGRY 3
#define SATIATED 4
#define BLOATED 5
#define OVER_FED 6

#define STATES 20

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
#define TELEPORT 1024
#define POLYMORPH 2048
#define TELEPORT_CONTROL 4096
#define PANIC 8192
#define CONFUSED 16384
#define PARASITIZED 32768
#define SEARCHING 65536
#define GAS_IMMUNITY 131072
#define LEVITATION 262144
#define LEPROSY 524288

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
#define ACID 4
#define FIRE 8
#define ELECTRICITY 16
#define ENERGY 32
#define POISON 64
#define DRAIN 128
#define THROW 32768

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

#define RAW_TYPES 6

#define GR_GLTERRAIN 0
#define GR_OLTERRAIN 1
#define GR_ITEM 2
#define GR_CHARACTER 3
#define GR_HUMANOID 4
#define GR_EFFECT 5

#define GRAPHIC_TYPES 4

#define GR_WTERRAIN 0
#define GR_FOW 1
#define GR_CURSOR 2
#define GR_SYMBOL 3

/* SpecialFlags for graphics system. No one knows what "ST_" means... */

#define ST_NORMAL 0
#define ST_RIGHT_ARM 8
#define ST_LEFT_ARM 16
#define ST_GROIN 24
#define ST_RIGHT_LEG 32
#define ST_LEFT_LEG 40
#define ST_OTHER_BODYPART 48
#define ST_WIELDED 56
#define ST_CLOAK 64
#define ST_LIGHTNING 128
#define ST_DISALLOW_R_COLORS 256
#define ST_WOBBLE 512
#define ST_WOBBLE_HORIZONTALLY_BIT 1024
#define ST_WOBBLE_VERTICALLY ST_WOBBLE
#define ST_WOBBLE_HORIZONTALLY (ST_WOBBLE|ST_WOBBLE_HORIZONTALLY_BIT)
#define ST_FLAME_1 2048
#define ST_FLAME_2 4096
#define ST_FLAME_3 8192
#define ST_FLAME_4 16384
#define ST_FLAMES (ST_FLAME_1|ST_FLAME_2|ST_FLAME_3|ST_FLAME_4)
#define ST_FLAME_SHIFT 11

#define SILHOUETTE_X_SIZE 48
#define SILHOUETTE_Y_SIZE 64

#define ITEM_CATEGORIES 18

#define ANY_CATEGORY 0x7FFFFFFF
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
#define HEXAGONAL 4

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

/* Possible square positions for item. The first four are used for items
   on walls */

#define LEFT 0
#define DOWN 1
#define UP 2
#define RIGHT 3
#define CENTER 4 // item on ground

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
#define EFFECT_MAGIC_MUSHROOM 14
#define EFFECT_TRAIN_PERCEPTION 15
#define EFFECT_HOLY_BANANA 16
#define EFFECT_EVIL_WONDER_STAFF_VAPOUR 17
#define EFFECT_GOOD_WONDER_STAFF_VAPOUR 18
#define EFFECT_PEA_SOUP 19
#define EFFECT_BLACK_UNICORN_FLESH 20
#define EFFECT_GRAY_UNICORN_FLESH 21
#define EFFECT_WHITE_UNICORN_FLESH 22
#define EFFECT_TELEPORT_CONTROL 23
#define EFFECT_MUSHROOM 24
#define EFFECT_OMMEL_CERUMEN 25
#define EFFECT_OMMEL_SWEAT 26
#define EFFECT_OMMEL_TEARS 27
#define EFFECT_OMMEL_SNOT 28

/* CEM = Consume End Message */

#define CEM_NOTHING 0
#define CEM_SCHOOL_FOOD 1
#define CEM_BONE 2
#define CEM_FROG_FLESH 3
#define CEM_OMMEL 4
#define CEM_PEPSI 5
#define CEM_KOBOLD_FLESH 6
#define CEM_HEALING_LIQUID 7
#define CEM_ANTIDOTE 8
#define CEM_ESP 9
#define CEM_HOLY_BANANA 10
#define CEM_PEA_SOUP 11
#define CEM_BLACK_UNICORN_FLESH 12
#define CEM_GRAY_UNICORN_FLESH 13
#define CEM_WHITE_UNICORN_FLESH 14

/* HM = Hit Message */

#define HM_NOTHING 0
#define HM_SCHOOL_FOOD 1
#define HM_FROG_FLESH 2
#define HM_OMMEL 3
#define HM_PEPSI 4
#define HM_KOBOLD_FLESH 5
#define HM_HEALING_LIQUID 6
#define HM_ANTIDOTE 7
#define HM_CONFUSE 8
#define HM_HOLY_BANANA 9

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

#define GLOBAL_WEAK_BODYPART_HIT_MODIFIER 10.0

#define MAX_EQUIPMENT_SLOTS 13

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

#define DEFAULT_TEAM 0xFF

/* Hard-coded teams */

#define PLAYER_TEAM 0
#define MONSTER_TEAM 1
#define ATTNAM_TEAM 2
#define SUMO_TEAM 3
#define IVAN_TEAM 6
#define NEW_ATTNAM_TEAM 7
#define COLONIST_TEAM 8
#define TOURIST_GUIDE_TEAM 9
#define TOURIST_TEAM 10
#define NO_TEAM 0xFFFF

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

#define DEFAULT_ATTACHED_AREA 0xFE
#define DEFAULT_ATTACHED_ENTRY 0xFE
#define NO_ENTRY 0

#define RANDOM 0
#define ELPURI_CAVE 1
#define ATTNAM 2
#define NEW_ATTNAM 3
#define UNDER_WATER_TUNNEL 4
#define UNDER_WATER_TUNNEL_EXIT 0x80

#define VESANA_LEVEL 2
#define CRYSTAL_LEVEL 3
#define ENNER_BEAST_LEVEL 4
#define IVAN_LEVEL 5
#define DARK_LEVEL 6
#define OREE_LAIR 9

/* stack::DrawContents flags */

#define NO_SELECT 1	      // only show items
#define NO_MULTI_SELECT 2     // select only one item
#define NO_SPECIAL_INFO 4     // show only name and amount
#define REMEMBER_SELECTED 8   // if DrawContents will be called multiple times,
			      // remember the selected item
#define NONE_AS_CHOICE 16     // "none" is a choice, for instance when wielding
#define SELECT_PAIR 32	      // if NO_MULTI_SELECT is on, selects a pair if
			      // appropriate

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
#define ATHEIST 16

#define MAX_PRICE 2147483647L

#define PERMANENT 0xFFFF

#define MISSED 0
#define HIT 1
#define CATCHED 2

#define BEAM_EFFECTS 12

#define BEAM_POLYMORPH 0
#define BEAM_STRIKE 1
#define BEAM_FIRE_BALL 2
#define BEAM_TELEPORT 3
#define BEAM_HASTE 4
#define BEAM_SLOW 5
#define BEAM_RESURRECT 6
#define BEAM_INVISIBILITY 7
#define BEAM_DUPLICATE 8
#define BEAM_LIGHTNING 9
#define BEAM_DOOR_CREATION 10
#define BEAM_ACID_RAIN 11

#define BEAM_STYLES 3

#define PARTICLE_BEAM 0
#define LIGHTNING_BEAM 1
#define SHIELD_BEAM 2

#define RANDOM_COLOR 0x10000

/* Entry indices, not actual config defines */

#define STAIRS_UP 1
#define STAIRS_DOWN 2
#define FOUNTAIN 0xFFFF

#define NO_LIMIT 0xFFFF

#define ALL_ITEMS 0xFFFF

/* StateData flags */

#define NO_FLAGS 0
#define SECRET 1
#define DUR_TEMPORARY 2
#define DUR_PERMANENT 4
#define DUR_FLAGS (2|4)
#define SRC_FOUNTAIN 8
#define SRC_MUSHROOM 16
#define SRC_MAGIC_MUSHROOM 32
#define SRC_CONFUSE_READ 64
#define SRC_EVIL 128
#define SRC_GOOD 256
#define SRC_FLAGS (8|16|32|64|128|256)
#define RANDOMIZABLE (2|4|8|16|32|64|128|256)

#define MAP_HIDDEN 0
#define SHOW_MAP_IN_TRUE_LIGHT 1
#define SHOW_MAP_IN_UNIFORM_LIGHT 2

#define DIM_LUMINANCE 0x6E6E6E

#define BROKEN 128
#define WINDOW 1024

/* item flags */

#define CANNIBALIZED 4
#define SQUARE_POSITION_BITS (16|32|64)
#define SQUARE_POSITION_SHIFT 4

#define NO_BROKEN 1
#define IGNORE_BROKEN_PRICE 2

#define MAX_SQUARES_UNDER 16
#define MAX_NEIGHBOUR_SQUARES 20

#define N_LOCK_ID 1024
#define S_LOCK_ID 16384
#define LOCK_DELTA 1024

#define LOCK_BITS 0xFC00

#define BROKEN_LOCK S_LOCK_ID

/* Normal lock types, which can be randomized */

#define ROUND_LOCK (N_LOCK_ID + LOCK_DELTA * 1)
#define SQUARE_LOCK (N_LOCK_ID + LOCK_DELTA * 2)
#define TRIANGULAR_LOCK (N_LOCK_ID + LOCK_DELTA * 3)

/* Special lock types, which must be generated in the script */

#define HEXAGONAL_LOCK (S_LOCK_ID + LOCK_DELTA * 1)

#define DESERT 1
#define JUNGLE 2
#define STEPPE 3
#define LEAFY_FOREST 4
#define EVERGREEN_FOREST 5
#define TUNDRA 6
#define GLACIER 7

#define NO_MOVE 0
#define WALK 1
#define SWIM 2
#define FLY  4
#define ETHEREAL 8
#define ANY_MOVE 15

#define KEY_UP_INDEX 1
#define KEY_LEFT_INDEX 3
#define KEY_RIGHT_INDEX 4
#define KEY_DOWN_INDEX 6

#define NO_ACTION 0
#define SUCCESS 1
#define BLOCKED 2

#define STACK_SLOT 1
#define CHARACTER_SLOT 2
#define GEAR_SLOT 3

#define NOT_RUSTED 0
#define SLIGHTLY_RUSTED 1
#define RUSTED 2
#define VERY_RUSTED 3

#define HUMAN_BODY_ARMOR_PIXELS 68

#define ARMOR_OUTLINE_PRIORITY ((7 << 4) + 7)
#define CLOAK_PRIORITY ((8 << 4) + 7)
#define DRIP_PRIORITY ((9 << 4) + 9)

#define BODY_ARMOR_PARTS 6

#define SUMO_ROOM_POS vector2d(25, 35)
#define SUMO_ARENA_POS vector2d(19, 12)

#define MAX_RAIN_DROPS 32

#define WON 0
#define LOST 1
#define DISQUALIFIED 2

#define EMITTER_IDENTIFIER_BITS 0xFFFF
#define EMITTER_SQUARE_PART_BITS 0xF000000
#define EMITTER_SHADOW_BITS 0xF0000000
#define EMITTER_SQUARE_PART_SHIFT 24
#define EMITTER_SHADOW_SHIFT 28

#define RE_SUN_EMITATED 0x200000
#define ID_X_COORDINATE 0x400000
#define ID_BEGIN 0x800000

#define FORCE_ADD 0x400000
#define SECONDARY_SUN_LIGHT 0x800000

/* square & lsquare flags */

#define ALLOW_EMITATION_CONTINUE 1
#define FREEZED 2 /* also a stack flag */
#define INSIDE 4
#define NEW_DRAW_REQUEST 8
#define STRONG_BIT 16
#define STRONG_NEW_DRAW_REQUEST (NEW_DRAW_REQUEST|STRONG_BIT)
#define DESCRIPTION_CHANGE 32
#define MEMORIZED_UPDATE_REQUEST 128
#define IN_SQUARE_STACK 256
#define CHECK_SUN_LIGHT_NEEDED 512
#define IS_TRANSPARENT 1024
#define PERFECTLY_QUADRI_HANDLED 2048

/* Slows down protosystem::BalancedCreateItem() but makes it produce more
   accurate results */

#define BALANCED_CREATE_ITEM_ITERATIONS 50

#define CONFIG_TABLE_SIZE 256

#define SPARKLE_POS_X_ERROR 128

#define SKIN_COLOR 1
#define CAP_COLOR 2
#define HAIR_COLOR 4
#define EYE_COLOR 8
#define TORSO_MAIN_COLOR 16
#define BELT_COLOR 32
#define BOOT_COLOR 64
#define TORSO_SPECIAL_COLOR 128
#define ARM_MAIN_COLOR 256
#define GAUNTLET_COLOR 512
#define ARM_SPECIAL_COLOR 1024
#define LEG_MAIN_COLOR 2048
#define LEG_SPECIAL_COLOR 4096
#define CLOTH_COLOR (CAP_COLOR\
		    |TORSO_MAIN_COLOR\
		    |ARM_MAIN_COLOR\
		    |GAUNTLET_COLOR\
		    |LEG_MAIN_COLOR)

/* contentscript<character> flags */

#define IS_LEADER 1
#define IS_MASTER 2

/* stack flags */

/* If set, all items are always considered visible, so CanBeSeenBy calls
   become unneeded */

#define HIDDEN 1

/* All costly updates (like emitation's) are avoided if this is set.
   Allows much faster removing and adding items, but make sure the stack is
   returned to the original state (excluding item order) before switching
   this off. Note: also an lsquare flag */

#define FREEZED 2

/* End stack Flags */

#define SUN_BEAM_DIRECTIONS 48

/* Square part flags */

#define SP_TOP_LEFT 1
#define SP_TOP_RIGHT 2
#define SP_BOTTOM_LEFT 4
#define SP_BOTTOM_RIGHT 8
#define SP_TOP (SP_TOP_LEFT|SP_TOP_RIGHT)
#define SP_LEFT (SP_TOP_LEFT|SP_BOTTOM_LEFT)
#define SP_RIGHT (SP_TOP_RIGHT|SP_BOTTOM_RIGHT)
#define SP_BOTTOM (SP_BOTTOM_LEFT|SP_BOTTOM_RIGHT)

#define CONDITION_COLORS 5

#define NATURAL_MATERIAL_FORM 0x7FFF

#define EXP_DIVISOR 2e-8
#define EXP_MULTIPLIER 5e+7
#define MIN_EXP 5e+7
#define MAX_EXP 5e+10

#define HAS_BEEN_GENERATED 1
#define HAS_BEEN_SEEN 2

#define DEPENDS_ON_DANGER 0xFFFF

/* Tiredness states */

#define FAINTING 0
#define EXHAUSTED 1
#define UNTIRED 2

#define DEFAULT_GENERATION_DANGER 0.05
#define ANGEL_GENERATION_DANGER 0.10

/* Duplication flags */

#define MIRROR_IMAGE 1
#define IGNORE_PROHIBITIONS 2
#define CHANGE_TEAM 4
#define LE_BASE_SHIFT 3
#define LE_BASE_RANGE 0x7FFF
#define LE_RAND_SHIFT 18
#define LE_RAND_RANGE 0x3FFF

/* Action flags */

#define IN_DND_MODE 1
#define TERMINATING 2

#endif
