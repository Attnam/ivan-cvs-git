#ifndef __GAME_H__
#define __GAME_H__

#define DIRECTION_COMMAND_KEYS	8

#define UNDEFINED		0
#define MALE			1
#define FEMALE			2
#define TRANSSEXUAL		3

#include <string>
#include <vector>

#include "command.h"

//#include "dynarray.h"
#include "typedef.h"
#include "vector2d.h"

class area;
class material;
class level;
class character;
class list;
class item;
class god;
class item;
class command;
class worldmap;
class square;
class dungeon;
class outputfile;
class inputfile;

/* Presentation of the game class */

class game
{
public:
	static void Init(std::string = "");
	static void DeInit();
	static void Run();
	static int GetMoveCommandKey(uchar Index)	{ return MoveCommandKey[Index]; }
	static const vector2d GetMoveVector(uchar Index)	{ return MoveVector[Index]; }
	static area* GetCurrentArea();
	static level* GetCurrentLevel();
	static bool FlagHandler(ushort, ushort, ushort, ushort);
	static bool DoLine(int, int, int, int, bool (*Proc)(ushort, ushort, ushort, ushort));
	static class panel
	{
	public:
		void Draw() const;
	private:
	} Panel;
	static ushort*** GetLuxTable()		{ return LuxTable; }
	static ushort* GetLuxTableSize()		{ return LuxTableSize; }
	static void Quit();
	static character* GetPlayer() { return Player; }
	static void SetPlayer(character*);
	static vector2d GetCamera() { return Camera; }
	static void UpDateCameraX();
	static void UpDateCameraY();
	static bool Flag;
	static level* GetLevel(ushort);
	static void InitLuxTable();
	static void DeInitLuxTable();
	static const char* Insult();
	static bool BoolQuestion(std::string, char = 0, int = 0);
	static const char* PersonalPronoun(uchar Index);
	static const char* PossessivePronoun(uchar Index);
	static void DrawEverything(bool = true);
	static bool Save(std::string = game::SaveName());
	static bool Load(std::string = game::SaveName());
	static bool GetRunning() { return Running; }
	static void EnableWizardMode() { WizardMode = true; }
	static bool GetWizardMode() { return WizardMode; }
	static void SeeWholeMap();
	static bool GetSeeWholeMapCheat() { return SeeWholeMapCheat; }
	static uchar EditGamma(short Value) {if(Value > 255 - Gamma) Gamma = 255; else if(Value + Gamma < 0) Gamma = 0; else Gamma += Value; return Gamma; }
	static bool EmitationHandler(ushort, ushort, ushort, ushort);
	static bool NoxifyHandler(ushort, ushort, ushort, ushort);
	static bool GetGoThroughWallsCheat() { return GoThroughWallsCheat; }
	static void GoThroughWalls() { GoThroughWallsCheat = !GoThroughWallsCheat; }
	static void UpdateCameraXWithPos(ushort);
	static void UpdateCameraYWithPos(ushort);
	static bool KeyIsOK(char);
	static void SetCurrent(ushort What) { Current = What; }
	static ushort GetCurrent() { return Current; }
	static int GetMoveCommandKey(vector2d, vector2d);
	static void DrawEverythingNoBlit(bool = true);
	static god* GetGod(uchar Index) { return God[Index]; }
	static std::string CAlignment(uchar Index) { return Alignment[Index]; }
	static void ApplyDivineTick(ushort = 1);
	static void ApplyDivineAlignmentBonuses(god*, bool);
	//static void SendToHell(character*);
	//static void BurnHellsContents();
	static vector2d GetDirectionVectorForKey(ushort);
	static vector2d AskForDirectionVector(std::string = "");
	static std::string GetPlayerName() { return PlayerName; }
	static void SetPlayerName(std::string What) { PlayerName = What; }
	static std::string SaveName();
	static bool EyeHandler(ushort, ushort, ushort, ushort);
	static long GodScore();
	static void ShowLevelMessage();
	static float Difficulty();
	static void TriggerQuestForMaakotkaShirt();
	static void CalculateGodNumber();
	static void SetGodNumber(uchar What) { GodNumber = What; }
	static uchar GetGodNumber() { return GodNumber; }
	static long GetBaseScore() { return BaseScore; }
	static void Turn(ushort Turn = 1) { Turns += Turn; }
	static float GetSoftGamma() { return SoftGamma; }
	static void EditSoftGamma(float E) { SoftGamma += E; if(SoftGamma < 0) SoftGamma = 0; if(SoftGamma > 2) SoftGamma = 2; }
	static ulong GetTurns() { return Turns; }
	static std::string GetAutoSaveFileName() { return AutoSaveFileName; }
	static uchar DirectionQuestion(std::string, uchar = 8, bool = true, bool = false);
	static command* GetCommand(ushort Index) { return Command[Index]; }
	static void RemoveSaves();
	static bool GetInWilderness() { return InWilderness; }
	static void SetInWilderness(bool What) { InWilderness = What; }
	static worldmap* GetWorldMap() { return WorldMap; }
	static void SetWorldMap(worldmap* What) { WorldMap = What; }
	static void SetAreaInLoad(area* What) { AreaInLoad = What; }
	static void SetSquareInLoad(square* What) { SquareInLoad = What; }
	static area* GetAreaInLoad() { return AreaInLoad; }
	static square* GetSquareInLoad() { return SquareInLoad; }
	static character* GetPlayerBackup() { return PlayerBackup; }
	static void SetPlayerBackup(character* What) { PlayerBackup = What; }
	static ushort GetPolymorphCounter() { return PolymorphCounter; }
	static void SetPolymorphCounter(ushort What) { PolymorphCounter = What; }
	static uchar GetLevels();
	static dungeon* GetCurrentDungeon() { return Dungeon[CurrentDungeon]; }
	static dungeon* GetDungeon(ushort Index) { return Dungeon[Index]; }
	static void SetCurrentDungeon(uchar What) { CurrentDungeon = What; }
	static void InitDungeons();
	static bool OnScreen(vector2d);
private:
	//static dynarray<character*> Hell;
	static std::string Alignment[];
	static god* God[];
	static ushort Current;
	static uchar CurrentDungeon;
	static int MoveCommandKey[DIRECTION_COMMAND_KEYS];
	static const vector2d MoveVector[DIRECTION_COMMAND_KEYS];
	static ushort*** LuxTable;
	static ushort* LuxTableSize;
	static bool Running;
	static character* Player;
	static vector2d Camera;
	static bool WizardMode;
	static bool SeeWholeMapCheat;
	static uchar Gamma;
	static bool GoThroughWallsCheat;
	static std::string PlayerName;
	//static std::string LevelMsg[];
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
	static std::vector<dungeon*> Dungeon;
};

#endif
