#ifndef __IVANDEF_H__
#define __IVANDEF_H__

/*
 * Global defines for the project IVAN.
 * This file is created to decrease the need of including headers in other headers just for the sake of some silly macros,
 * because it decreases compilation efficiency and may even cause cross-including
 *
 * List of macros that should be gathered here:
 * 1. all numeric defines used in multiple source files
 * 2. all functional macros used in multiple source files and independent enough (do not require other headers)
 * 3. class construction macros used in multiple header files
 *
 * DO NOT INCLUDE ANY FILES IN THIS HEADER.
 */

#ifndef VERSION
#define VERSION "0.40d"
#endif

#define DATABASEVALUE(type, data) virtual type Get##data() const { return DataBase->data; }
#define DATABASEVALUEWITHPARAMETER(type, data, param) virtual type Get##data(param) const { return DataBase->data; }
#define DATABASEBOOL(data) virtual bool data() const { return DataBase->data; }

#define PROTODATABASEVALUE(type, data) virtual type Get##data() const { return DataBase.data; }
#define PROTODATABASEVALUEWITHPARAMETER(type, data, param) virtual type Get##data(param) const { return DataBase.data; }
#define PROTODATABASEBOOL(data) virtual bool data() const { return DataBase.data; }

#define DATAVALUE(type, data)\
 public:\
  type Get##data() const { return data; }\
  void Set##data(type What) { data = What; }\
 protected:\
  type data;

#define COMPLEXDATAVALUE(type, data)\
 public:\
  const type& Get##data() const { return data; }\
  void Set##data(const type& What) { data = What; }\
 protected:\
  type data;

#define EDITABLEDATAVALUE(type, data)\
 public:\
  type Get##data() const { return data; }\
  void Set##data(type What) { data = What; }\
  void Edit##data(type What) { data += What; }\
 protected:\
  type data;

#define DATABOOL(data)\
 public:\
  bool data() const { return data##Bool; }\
  void Set##data(bool What) { data##Bool = What; }\
 protected:\
  type data##Bool;

#if defined WIN32 || defined __DJGPP__
#define GAME_DIR std::string("")
#define SAVE_DIR std::string("Save/")
#endif

// The program can only create directories to the deepness of one, no more...

#ifdef USE_SDL
#define GAME_DIR (std::string(DATADIR) + "/ivan/")
#define SAVE_DIR (std::string(getenv("HOME")) + "/IvanSave/")
#endif

#define HASHIT 0
#define HASBLOCKED 1
#define HASDODGED 2
#define HASDIED 3

#define BLOATEDLEVEL 40000
#define SATIATEDLEVEL 30000
#define NOTHUNGERLEVEL 5000
#define HUNGERLEVEL 1000

#define OVERLOADED 0
#define STRESSED 1
#define BURDENED 2
#define UNBURDENED 3

#define VERYHUNGRY 0
#define HUNGRY 1
#define NOTHUNGRY 2
#define SATIATED 3
#define BLOATED 4

#define STATES 10

#define POLYMORPHED 1
#define HASTE 2
#define SLOW 4
#define POLYMORPH_CONTROL 8
#define LIFE_SAVED 16
#define LYCANTHROPY 32
#define INVISIBLE 64
#define INFRAVISION 128
#define ESP 256
#define POISONED 512

#define TORSO 1
#define HEAD 2
#define RIGHTARM 4
#define LEFTARM 8
#define ARMS RIGHTARM|LEFTARM
#define GROIN 16
#define RIGHTLEG 32
#define LEFTLEG 64
#define LEGS RIGHTLEG|LEFTLEG
#define OTHER 128
#define ALL 255

#define PHYSICALDAMAGE 1
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

#define HUMANOID_BODYPARTS 7

#define TORSOINDEX 0
#define HEADINDEX 1
#define RIGHTARMINDEX 2
#define LEFTARMINDEX 3
#define GROININDEX 4
#define RIGHTLEGINDEX 5
#define LEFTLEGINDEX 6

#define NONEINDEX 0xFF

#define DIRECTION_COMMAND_KEYS 8
#define EXTENDED_DIRECTION_COMMAND_KEYS 9
#define YOURSELF 8

#define LIGHT_BORDER 160

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
#define ARTICLEBIT 2
#define DEFINITE 2
#define INDEFINEBIT 4
#define INDEFINITE 6

#define NORMALARTICLE 0
#define NOARTICLE 1
#define DEFINITEARTICLE 2

#define TRANSPARENTCOL 0xF81F // pink

#define RAW_TYPES 5

#define GRGLTERRAIN 0
#define GROLTERRAIN 1
#define GRITEM 2
#define GRCHARACTER 3
#define GRHUMANOID 4

#define GRAPHIC_TYPES 5

#define GRWTERRAIN 0
#define GRFOW 1
#define GRCURSOR 2
#define GRSYMBOL 3
#define GRMENU 4

#define STNORMAL 0 << 3
#define STRIGHTARM 1 << 3
#define STLEFTARM 2 << 3
#define STGROIN 3 << 3
#define STRIGHTLEG 4 << 3
#define STLEFTLEG 5 << 3

#define SILHOUETTE_X_SIZE 64
#define SILHOUETTE_Y_SIZE 64

#define ITEM_CATEGORIES 18

#define HELMET 0
#define AMULET 1
#define CLOAK 2
#define BODYARMOR 3
#define WEAPON 4
#define SHIELD 5
#define RING 6
#define GAUNTLET 7
#define BELT 8
#define BOOT 9
#define FOOD 10
#define POTION 11
#define SCROLL 12
#define BOOK 13
#define WAND 14
#define TOOL 15
#define VALUABLE 16
#define MISC 17

#define NUMBER_OF_LOCK_TYPES 3

#define ROUND 0
#define SQUARE 1
#define TRIANGULAR 2

#define GOOD 1
#define NEUTRAL 2
#define EVIL 3

/* ConsumeTypes */

#define ODD 0
#define FRUIT 1
#define MEAT 2
#define METAL 4
#define MINERAL 8
#define LIQUID 16
#define BONY 32
#define PROCESSED 64
#define MISC_ORGANIC 128
#define GAS 256

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
#define WEAPON_SKILL_CATEGORIES 15

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
#define HALBERDS 12
#define SPEARS 13
#define WHIPS 14

#define LOCKED 1

#define ENOTHING 0
#define EPOISON 1
#define EDARKNESS 2
#define EOMLEURINE 3
#define EPEPSI 4
#define EKOBOLDFLESH 5
#define EHEAL 6
#define ELYCANTHROPY 7

#define CEM_NOTHING 0
#define CEM_SCHOOLFOOD 1
#define CEM_BONE 2
#define CEM_FROGFLESH 3
#define CEM_OMLEURINE 4
#define CEM_PEPSI 5
#define CEM_KOBOLDFLESH 6
#define CEM_HEALINGLIQUID 7

#define HM_NOTHING 0
#define HM_SCHOOLFOOD 1
#define HM_FROGFLESH 2
#define HM_OMLEURINE 3
#define HM_PEPSI 4
#define HM_KOBOLDFLESH 5
#define HM_HEALINGLIQUID 6

#define FIRSTMATERIAL 4096

#define IRON FIRSTMATERIAL + 1
#define VALPURIUM FIRSTMATERIAL + 2
#define STONE FIRSTMATERIAL + 3
#define GRAVEL FIRSTMATERIAL + 4
#define MORAINE FIRSTMATERIAL + 5
#define WOOD FIRSTMATERIAL + 6
#define BONE FIRSTMATERIAL + 7
#define GLASS FIRSTMATERIAL + 8
#define PARCHMENT FIRSTMATERIAL + 9
#define CLOTH FIRSTMATERIAL + 10
#define MITHRIL FIRSTMATERIAL + 11
#define MARBLE FIRSTMATERIAL + 12
#define GOLD FIRSTMATERIAL + 13
#define GRASS FIRSTMATERIAL + 14
#define LEATHER FIRSTMATERIAL + 15
#define LEAF FIRSTMATERIAL + 16
#define FABRIC FIRSTMATERIAL + 17
#define PALMLEAF FIRSTMATERIAL + 18
#define SULFUR FIRSTMATERIAL + 19
#define GUNPOWDER FIRSTMATERIAL + 20
#define BREAD FIRSTMATERIAL + 21
#define UNICORNHORN FIRSTMATERIAL + 22
#define DIAMOND FIRSTMATERIAL + 23
#define SILVER FIRSTMATERIAL + 24
#define SAPPHIRE FIRSTMATERIAL + 25
#define RUBY FIRSTMATERIAL + 26
#define BRONZE FIRSTMATERIAL + 27
#define COPPER FIRSTMATERIAL + 28
#define TIN FIRSTMATERIAL + 29
#define STEEL FIRSTMATERIAL + 30

#define FIRSTORGANICSUBSTANCE 4096 * 2

#define BANANAFLESH FIRSTORGANICSUBSTANCE + 1
#define SCHOOLFOOD FIRSTORGANICSUBSTANCE + 2
#define BANANAPEAL FIRSTORGANICSUBSTANCE + 3
#define KIWIFLESH FIRSTORGANICSUBSTANCE + 4
#define PINEAPPLEFLESH FIRSTORGANICSUBSTANCE + 5

#define FIRSTGAS 4096 * 3

#define AIR FIRSTGAS + 1

#define FIRSTLIQUID 4096 * 4

#define OMLEURINE FIRSTLIQUID + 1
#define PEPSI FIRSTLIQUID + 2
#define WATER FIRSTLIQUID + 3
#define HEALINGLIQUID FIRSTLIQUID + 4
#define BLOOD FIRSTLIQUID + 5
#define BROWNSLIME FIRSTLIQUID + 6

#define FIRSTFLESH 4096 * 5

#define GOBLINOIDFLESH FIRSTFLESH + 1
#define PORK FIRSTFLESH + 2
#define BEEF FIRSTFLESH + 3
#define FROGFLESH FIRSTFLESH + 4
#define ELPURIFLESH FIRSTFLESH + 5
#define HUMANFLESH FIRSTFLESH + 6
#define DOLPHINFLESH FIRSTFLESH + 7
#define POLARBEARFLESH FIRSTFLESH + 8
#define WOLFFLESH FIRSTFLESH + 9
#define DOGFLESH FIRSTFLESH + 10
#define ENNERBEASTFLESH FIRSTFLESH + 11
#define SPIDERFLESH FIRSTFLESH + 12
#define JACKALFLESH FIRSTFLESH + 13
#define DONKEYFLESH FIRSTFLESH + 14
#define BATFLESH FIRSTFLESH + 15
#define WEREWOLFFLESH FIRSTFLESH + 16
#define KOBOLDFLESH FIRSTFLESH + 17
#define GIBBERLINGFLESH FIRSTFLESH + 18
#define CATFLESH FIRSTFLESH + 19
#define RATFLESH FIRSTFLESH + 20
#define ANGELFLESH FIRSTFLESH + 21
#define DWARFFLESH FIRSTFLESH + 22
#define DAEMONFLESH FIRSTFLESH + 23
#define MAMMOTHFLESH FIRSTFLESH + 24
#define BLACKUNICORNFLESH FIRSTFLESH + 25
#define GRAYUNICORNFLESH FIRSTFLESH + 26
#define WHITEUNICORNFLESH FIRSTFLESH + 27
#define LIONFLESH FIRSTFLESH + 28
#define BUFFALOFLESH FIRSTFLESH + 29
#define SNAKEFLESH FIRSTFLESH + 29

#define UNARMEDATTACK 0
#define WEAPONATTACK 1
#define KICKATTACK 2
#define BITEATTACK 3

#define USE_ARMS 1
#define USE_LEGS 2
#define USE_HEAD 4

#define ATTRIBUTES 10
#define BASEATTRIBUTES 6

#define ENDURANCE 0
#define PERCEPTION 1
#define INTELLIGENCE 2
#define WISDOM 3
#define CHARISMA 4
#define MANA 5

#define ARMSTRENGTH 6
#define LEGSTRENGTH 7
#define DEXTERITY 8
#define AGILITY 9

#define NO 0
#define YES 1
#define REQUIRES_ANSWER -1

#define DIR_ERROR 0xFF
#define DIR_ERROR_VECTOR vector2d(666, 666)

#define GLOBAL_WEAK_BODYPART_HIT_MODIFIER 0.10f

#define HELMETINDEX 0
#define AMULETINDEX 1
#define CLOAKINDEX 2
#define BODYARMORINDEX 3
#define BELTINDEX 4
#define RIGHTWIELDEDINDEX 5
#define LEFTWIELDEDINDEX 6
#define RIGHTRINGINDEX 7
#define LEFTRINGINDEX 8
#define RIGHTGAUNTLETINDEX 9
#define LEFTGAUNTLETINDEX 10
#define RIGHTBOOTINDEX 11
#define LEFTBOOTINDEX 12

#define LONGSWORD 1
#define TWOHANDEDSWORD 2
#define CURVEDTWOHANDEDSWORD 3
#define SPEAR 4
#define AXE 5
#define POLEAXE 6
#define SPIKEDMACE 7

#define CHAINMAIL 1

#define BOOTOFSPEED 1

#define RINGOFFIRERESISTANCE 1
#define RINGOFPOLYMORPHCONTROL 2
#define RINGOFINFRAVISION 3

#define AMULETOFLIFESAVING 1
#define AMULETOFESP 2

#define CONICALMOMMO 1
#define FLATMOMMO 2

#define PARQUET 1
#define FLOOR 2
#define GROUND 3
#define GRASSTERRAIN 4

#define POOL 1

#define BRICK_FINE 1
#define BRICK_OLD 2
#define EARTH 3

#define PINE 1
#define SPRUCE 2
#define LINDEN 3
#define CARPET 4
#define COUCH 5
#define DOUBLEBED 6
#define POOLBORDER 7
#define POOLCORNER 8

#define BOOKCASE 1

/* Prices */
/* The logic will go bad, if a new limb cost less than an old one */
#define PRICE_TO_ATTACH_OLD_LIMB_IN_ALTAR 50
#define PRICE_TO_ATTACH_NEW_LIMB_IN_ALTAR 100

/* Sorry about the name. This means how many turns it takes for a angel to attach a bodypart again */
#define LENGTH_OF_ANGELS_HEAL_COUNTER_LOOP 2500

#endif
