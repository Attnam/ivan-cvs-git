#ifndef __GAME_H__
#define __GAME_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define DIRECTION_COMMAND_KEYS	8

#define UNDEFINED		0
#define MALE			1
#define FEMALE			2
#define TRANSSEXUAL		3

#define LIGHT_BORDER		160

#ifdef WIN32
#define GAME_DIR std::string("")
#define SAVE_DIR std::string("Save/")
#else

#define GAME_DIR std::string(DATADIR) + std::string("/ivan/")

#define SAVE_DIR (std::string(getenv("HOME")) + std::string("/IvanSave/"))
    // The program can only create directories to the deepness of 1 no more...
#endif
#include <string>
#include <vector>

#include "typedef.h"
#include "vector2d.h"

#include "command.h"

class area;
class material;
class level;
class character;
class felist;
class item;
class god;
class item;
class command;
class worldmap;
class square;
class dungeon;
class outputfile;
class inputfile;
class gamescript;
class team;
class bitmap;
class petrus;

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
	static bool LOSHandler(vector2d, vector2d);
	static void DrawPanel();
	static ushort*** GetLuxTable()		{ return LuxTable; }
	static ushort* GetLuxTableSize()		{ return LuxTableSize; }
	static void Quit();
	static character* GetPlayer() { return Player; }
	static void SetPlayer(character*);
	static vector2d GetCamera() { return Camera; }
	static void UpdateCameraX();
	static void UpdateCameraY();
	static bool GetIsLoading() { return IsLoading; }
	static void SetIsLoading(bool What) { IsLoading = What; }
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
	static bool EmitationHandler(vector2d, vector2d);
	static bool NoxifyHandler(vector2d, vector2d);
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
	static vector2d GetDirectionVectorForKey(ushort);
	static vector2d AskForDirectionVector(std::string = "");
	static std::string GetPlayerName() { return PlayerName; }
	static void SetPlayerName(std::string What) { PlayerName = What; }
	static std::string SaveName();
	static bool EyeHandler(vector2d, vector2d);
	static long GodScore();
	static void ShowLevelMessage();
	static float Difficulty();
	static void TriggerQuestForGoldenEagleShirt();
	static void CalculateGodNumber();
	static void SetGodNumber(uchar What) { GodNumber = What; }
	static uchar GetGodNumber() { return GodNumber; }
	static long GetBaseScore() { return BaseScore; }
	static void Turn(ushort Turn = 1) { Turns += Turn; }
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
	static uchar GetLevels();
	static dungeon* GetCurrentDungeon() { return Dungeon[CurrentDungeon]; }
	static dungeon* GetDungeon(ushort Index) { return Dungeon[Index]; }
	static void SetCurrentDungeon(uchar What) { CurrentDungeon = What; }
	static void InitDungeons();
	static bool OnScreen(vector2d);
	static gamescript& GetGameScript() { return GameScript; }
	static void DoGoodDeed(ushort);
	static void DoNeutralDeed(ushort);
	static void DoEvilDeed(ushort);
	static void InitScript();
	static void SaveWorldMap(std::string = SaveName(), bool = false);
	static void LoadWorldMap(std::string = SaveName());
	static void UpdateCamera();
	static ulong CreateNewObjectID();
	static void PopObjectID(ulong ID) { if(NextObjectID == ID + 1) --NextObjectID; }
	static team* GetTeam(ushort Index) { return Team[Index]; }
	static uchar GetTeams() { return Team.size(); }
	static void Hostility(team*, team*);
	static void CreateTeams();
	static bool IsValidPos(vector2d);
	static std::string StringQuestion(std::string, vector2d, ushort, ushort, ushort);
	static long NumberQuestion(std::string, vector2d, ushort);
	static void LOSTurn();
	static ulong GetLOSTurns() { return LOSTurns; }
	static void SendLOSUpdateRequest() { LOSUpdateRequested = true; }
	static void RemoveLOSUpdateRequest() { LOSUpdateRequested = false; }
	static petrus* GetPetrus() { return Petrus; }
	static void SetPetrus(petrus* What) { Petrus = What; }
	static bool HandleQuitMessage();
	static void Beep();
	static uchar GetDirectionForVector(vector2d);
	static void SetInGetCommand(bool What) { InGetCommand = What; }
	static bool GetInGetCommand() { return InGetCommand; }
	static std::string GetVerbalPlayerAlignment();
private:
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
	static bool GoThroughWallsCheat;
	static std::string PlayerName;
	static uchar GodNumber;
	static long BaseScore;
	static ulong Turns;
	static std::string AutoSaveFileName;
	static command* Command[];
	static bool InWilderness;
	static worldmap* WorldMap;
	static area* AreaInLoad;
	static square* SquareInLoad;
	static character* PlayerBackup;
	static std::vector<dungeon*> Dungeon;
	static gamescript GameScript;
	static ulong NextObjectID;
	static std::vector<team*> Team;
	static ulong LOSTurns;
	static bool LOSUpdateRequested;
	static petrus* Petrus;
	static bool InGetCommand;
	static bool IsLoading;
};

#endif



