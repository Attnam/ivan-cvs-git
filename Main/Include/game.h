#ifndef __GAME_H__
#define __GAME_H__

#define DIRECTION_COMMAND_KEYS	8

#define UNDEFINED		0
#define MALE			1
#define FEMALE			2
#define TRANSSEXUAL		3

#include <string>
#include <fstream>

#include "command.h"

#include "dynarray.h"
#include "typedef.h"
#include "vector.h"

class area;
class material;
class level;
class character;
class vector;
class list;
class item;
class god;
class item;
class command;
class worldmap;
class square;
class dungeon;

/* Presentation of the game class */

class game
{
public:
	static void Init(std::string = "");
	static void DeInit(void);
	static void Run(void);
	static int Menu(std::string sMS); // hihi :-) (MENU)
	static int GetMoveCommandKey(uchar Index)	{ return MoveCommandKey[Index]; }
	static const vector GetMoveVector(uchar Index)	{ return MoveVector[Index]; }
	static area* GetCurrentArea(void);
	static level* GetCurrentLevel(void);
	static bool FlagHandler(ushort, ushort, ushort, ushort);
	static bool DoLine(int, int, int, int, bool (*Proc)(ushort, ushort, ushort, ushort));
	static class panel
	{
	public:
		void Draw(void) const;
	private:
	} Panel;
	static ushort*** GetLuxTable(void)		{ return LuxTable; }
	static ushort* GetLuxTableSize(void)		{ return LuxTableSize; }
	static void Quit(void);
	static character* GetPlayer(void) { return Player; }
	static void SetPlayer(character*);
	static vector GetCamera(void) { return Camera; }
	static void UpDateCameraX(void);
	static void UpDateCameraY(void);
	static bool Flag;
	static level* GetLevel(ushort);
	static void InitLuxTable(void);
	static void DeInitLuxTable(void);
	static const char* Insult(void);
	static bool BoolQuestion(std::string, char = 0, int = 0);
	static const char* PersonalPronoun(uchar Index);
	static const char* PossessivePronoun(uchar Index);
	static void DrawEverything(bool = true);
	static void StoryScreen(const char*, bool = true);
	static bool Save(std::string = game::SaveName());
	static bool Load(std::string = game::SaveName());
	static bool GetRunning(void) { return Running; }
	static void EnableWizardMode(void) { WizardMode = true; }
	static bool GetWizardMode(void) { return WizardMode; }
	static void SeeWholeMap(void) { SeeWholeMapCheat = !SeeWholeMapCheat; }
	static bool GetSeeWholeMapCheat(void) { return SeeWholeMapCheat; }
	static uchar EditGamma(short Value) {if(Value > 255 - Gamma) Gamma = 255; else if(Value + Gamma < 0) Gamma = 0; else Gamma += Value; return Gamma; }
	static bool EmitationHandler(ushort, ushort, ushort, ushort);
	static bool NoxifyHandler(ushort, ushort, ushort, ushort);
	static bool GetGoThroughWallsCheat(void) { return GoThroughWallsCheat; }
	static void GoThroughWalls(void) { GoThroughWallsCheat = !GoThroughWallsCheat; }
	static void UpdateCameraXWithPos(ushort);
	static void UpdateCameraYWithPos(ushort);
	static std::string StringQuestion(const char*, ushort);
	static bool KeyIsOK(char);
	static void SetCurrent(ushort What) { Current = What; }
	static ushort GetCurrent(void) { return Current; }
	//static ushort GetLevels(void) { return Levels; }
	static int GetMoveCommandKey(vector, vector);
	static void DrawEverythingNoBlit(bool = true);
	static god* GetGod(uchar Index) { return God[Index]; }
	static std::string CAlignment(uchar Index) { return Alignment[Index]; }
	static void ApplyDivineTick(ushort = 1);
	static void ApplyDivineAlignmentBonuses(god*, bool);
	static void SendToHell(character*);
	static void BurnHellsContents(void);
	static vector GetDirectionVectorForKey(ushort);
	static vector AskForDirectionVector(std::string = "");
	static std::string StringQuestionWithClear(std::string, ushort);
	static std::string GetPlayerName(void) { return PlayerName; }
	static void SetPlayerName(std::string What) { PlayerName = What; }
	static std::string SaveName(void);
	static bool EyeHandler(ushort, ushort, ushort, ushort);
	static long GodScore(void);
	static void ShowLevelMessage(void);
	static float Difficulty(void);
	static void TriggerQuestForMaakotkaShirt(void);
	static void CalculateGodNumber(void);
	static void SetGodNumber(uchar What) { GodNumber = What; }
	static uchar GetGodNumber(void) { return GodNumber; }
	static long GetBaseScore(void) { return BaseScore; }
	static void Turn(ushort Turn = 1) { Turns += Turn; }
	static float GetSoftGamma(void) { return SoftGamma; }
	static void EditSoftGamma(float E) { SoftGamma += E; if(SoftGamma < 0) SoftGamma = 0; if(SoftGamma > 2) SoftGamma = 2; }
	static void WhatToLoadMenu(void);
	static ulong GetTurns(void) { return Turns; }
	static std::string GetAutoSaveFileName(void) { return AutoSaveFileName; }
	static uchar DirectionQuestion(std::string, uchar = 8, bool = true, bool = false);
	static command* GetCommand(ushort Index) { return Command[Index]; }
	static void RemoveSaves(void);
	static bool GetInWilderness(void) { return InWilderness; }
	static void SetInWilderness(bool What) { InWilderness = What; }
	static worldmap* GetWorldMap(void) { return WorldMap; }
	static void SetWorldMap(worldmap* What) { WorldMap = What; }
	static void SetAreaInLoad(area* What) { AreaInLoad = What; }
	static void SetSquareInLoad(square* What) { SquareInLoad = What; }
	static area* GetAreaInLoad(void) { return AreaInLoad; }
	static square* GetSquareInLoad(void) { return SquareInLoad; }
	static character* GetPlayerBackup(void) { return PlayerBackup; }
	static void SetPlayerBackup(character* What) { PlayerBackup = What; }
	static ushort GetPolymorphCounter(void) { return PolymorphCounter; }
	static void SetPolymorphCounter(ushort What) { PolymorphCounter = What; }
	static dungeon* GetDungeon(void) { return Dungeon; }
	static uchar GetLevels(void);
private:
	static dynarray<character*> Hell;
	static std::string Alignment[];
	static god* God[];
	static unsigned int CountChars(char cSF,std::string sSH); // (MENU)
	//static level** Level;
	//static ushort Levels, Current;
	static ushort Current;
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
	static std::string PlayerName;
	static std::string LevelMsg[];
	static uchar GodNumber;
	static long BaseScore;
	static ulong Turns;
	static float SoftGamma;
	static std::string AutoSaveFileName;
	static command* Command[];
	static bool InWilderness;
	static worldmap* WorldMap;
	static area* AreaInLoad;
	static square* SquareInLoad;
	static character* PlayerBackup;
	static ushort PolymorphCounter;
	static dungeon* Dungeon;
};

#endif
