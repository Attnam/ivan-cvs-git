#ifndef __GAME_H__
#define __GAME_H__

#define NUMB_OF_SOLID_MATERIALS 9

#define DIRECTION_COMMAND_KEYS	8

#define UNDEFINED		0
#define MALE			1
#define FEMALE			2
#define TRANSSEXUAL		3

#define ADD_MESSAGE game::GlobalMessagingSystem.AddMessage
#define DRAW_MESSAGES game::GlobalMessagingSystem.Draw()
#define EMPTY_MESSAGES game::GlobalMessagingSystem.Empty()

#include <ctime>
#include <string>
#include <fstream>

#include "dynarray.h"
#include "typedef.h"
#include "vector.h"

#include "command.h"
#include "list.h"
#include "proto.h"

class material;
class level;
class character;
class vector;
class list;
class item;
class god;
class item;
class command;
class groundterrain;
class overterrain;

/* Presentation of the game class */

class game
{
public:
	static void Init(std::string = "");
	static void DeInit(void);
	static void Run(void);
	static const int Menu(std::string sMS); // hihi :-) (MENU)
	static int* CMoveCommandKey(void)	{return MoveCommandKey;}
	static const vector* CMoveVector(void)	{return MoveVector;}
	static level* CCurrentLevel(void)		{return Level[Current];}
	static bool FlagHandler(ushort, ushort, ushort, ushort);
	static bool DoLine(int, int, int, int, bool (*Proc)(ushort, ushort, ushort, ushort));
	static class globalmessagingsystem
	{
	public:
		globalmessagingsystem(void) : MessageBuffer(0), BufferLength(0) {}
		void AddMessage(const char*, ...);
		void Draw(void);
		void Empty(void);
		const char* CBuffer(void)	{return MessageBuffer;}
		void DrawMessageHistory(void);
		void Format(void);
	private:
		char* MessageBuffer;
		ushort BufferLength;
		list MessageHistory;
	} GlobalMessagingSystem;
	static class panel
	{
	public:
		void Draw(void);
	private:
	} Panel;
	static ushort*** CLuxTable(void)		{return LuxTable;}
	static ushort* CLuxTableSize(void)		{return LuxTableSize;}
	static void Quit(void);
	static character* CPlayer(void) {return Player;}
	static void SPlayer(character* NP) {Player = NP;}
	static vector CCamera(void) {return Camera;}
	static void UpDateCameraX(void);
	static void UpDateCameraY(void);
	static bool Flag;
	static level* CLevel(ushort Index) {return Level[Index];}
	static character* CreateMonster(ushort);
	static item* CreateItem(ushort);
	static void InitLuxTable(void);
	static void DeInitLuxTable(void);
	static character* BalancedCreateMonster(void);
	static item* BalancedCreateItem(void);
	static const char* Insult(void);
	static bool BoolQuestion(std::string, char = 0, int = 0, int (*)(void) = game::GetKey);
	static const char* PersonalPronoun(uchar Index);
	static const char* PossessivePronoun(uchar Index);
	static void DrawEverything(bool = true);
	static void DrawEverythingWithDebug(bool = true);
	static void StoryScreen(const char*, bool = true);
	static bool Save(std::string = game::SaveName());
	static bool Load(std::string = game::SaveName());
	static material* CreateRandomSolidMaterial(ulong);
	static material* CreateMaterial(ushort, ulong);
	static groundterrain* LoadGroundTerrain(std::ifstream*);
	static overterrain* LoadOverTerrain(std::ifstream*);
	static material* LoadMaterial(std::ifstream*);
	static item* LoadItem(std::ifstream*);
	static character* LoadCharacter(std::ifstream*);
	static bool CRunning(void) {return Running;}
	static void EnableWizardMode(void) { WizardMode = true; }
	static bool CWizardMode(void) {return WizardMode;}
	static void SeeWholeMap(void) { SeeWholeMapCheat = !SeeWholeMapCheat; }
	static bool CSeeWholeMapCheat(void) {return SeeWholeMapCheat;}
	static uchar EditGamma(short Value) {if(Value > 255 - Gamma) Gamma = 255; else if(Value + Gamma < 0) Gamma = 0; else Gamma += Value; return Gamma;}
	static bool EmitationHandler(ushort, ushort, ushort, ushort);
	static bool NoxifyHandler(ushort, ushort, ushort, ushort);
	static clock_t HandlePlayTim; //Public for debug
	static int GetKey(void);
	static int MonsGetKey(void);
	static bool CGoThroughWallsCheat(void) { return GoThroughWallsCheat; }
	static void GoThroughWalls(void) { GoThroughWallsCheat = !GoThroughWallsCheat; }
	static void UpdateCameraXWithPos(ushort);
	static void UpdateCameraYWithPos(ushort);
	static std::string StringQuestion(const char*, ushort);
	static bool KeyIsOK(char);
	static void SCurrent(ushort What) { Current = What; }
	static ushort CCurrent(void) { return Current; }
	static ushort CLevels(void) { return Levels; }
	static int GetMoveCommandKey(vector, vector);
	static void DrawEverythingNoBlit(bool = true);
	static god* CGod(uchar Index) { return God[Index]; }
	static std::string CAlignment(uchar Index) { return Alignment[Index]; }
	static void ApplyDivineTick(ushort = 1);
	static void ApplyDivineAlignmentBonuses(god*, bool);
	static void SendToHell(character*);
	static void BurnHellsContents(void);
	static vector GetDirectionVectorForKey(ushort);
	static vector AskForDirectionVector(std::string = "");
	static std::string StringQuestionWithClear(std::string, ushort);
	static std::string CPlayerName(void) { return PlayerName; }
	static void SPlayerName(std::string What) { PlayerName = What; }
	static std::string SaveName(void);
	static bool EyeHandler(ushort, ushort, ushort, ushort);
	static long GodScore(void);
	static void ShowLevelMessage(void);
	static float Difficulty(void);
	static void TriggerQuestForMaakotkaShirt(void);
	static void CalculateGodNumber(void);
	static void SGodNumber(uchar What) { GodNumber = What; }
	static uchar CGodNumber(void) { return GodNumber; }
	static long CBaseScore(void) { return BaseScore; }
	static void Turn(ushort Turn = 1) { Turns += Turn; }
	static float CSoftGamma(void) { return SoftGamma; }
	static void EditSoftGamma(float E) { SoftGamma += E; if(SoftGamma < 0) SoftGamma = 0; if(SoftGamma > 2) SoftGamma = 2; }
	static void WhatToLoadMenu(void);
	static ulong CTurns(void) { return Turns; }
	static std::string CAutoSaveFileName(void) { return AutoSaveFileName; }
	static uchar DirectionQuestion(std::string, uchar = 8, bool = true);
	static command** CCommand(void) { return Command; }
	static const character* const CCharacterPrototype(const ushort Index) { return CharacterPrototype[Index]; }
	static prototypecontainer<character>& CCharacterPrototype(void) { return CharacterPrototype; }
	static const item* const CItemPrototype(const ushort Index) { return ItemPrototype[Index]; }
	static prototypecontainer<item>& CItemPrototype(void) { return ItemPrototype; }
	static void SaveLevel(std::string = SaveName(), ushort = CCurrent(), bool = true);
	static void LoadLevel(std::string = SaveName(), ushort = CCurrent());
	static void RemoveSaves(void);
	static item* game::CreateItem(std::string);
private:
	static dynarray<character*> Hell;
	static std::string Alignment[];
	static prototypecontainer<character> CharacterPrototype;
	static prototypecontainer<item> ItemPrototype;
	static god* God[];
	static const unsigned int CountChars(const char cSF,std::string sSH); // (MENU)
	static level** Level;
	static ushort Levels, Current;
	static int MoveCommandKey[DIRECTION_COMMAND_KEYS];
	static const vector MoveVector[DIRECTION_COMMAND_KEYS];
	static ushort*** LuxTable;
	static ushort* LuxTableSize;
	static bool Running;
	static character* Player;
	static vector Camera;
	static bool WizardMode;
	static bool SeeWholeMapCheat;
	static uchar Gamma;
	static bool GoThroughWallsCheat;
	static clock_t HandleCharTim;
	static std::string PlayerName;
	static std::string LevelMsg[];
	static uchar GodNumber;
	static long BaseScore;
	static ulong Turns;
	static float SoftGamma;
	static std::string AutoSaveFileName;
	static command* Command[];
};

#endif
