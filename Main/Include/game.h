/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __GAME_H__
#define __GAME_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>
#include <vector>

#include "vector2d.h"
#include "femath.h"
#include "festring.h"
#include "ivandef.h"

#ifndef LIGHT_BORDER
#define LIGHT_BORDER 80
#endif

#define PLAYER game::GetPlayer()

class area;
class level;
class dungeon;
class felist;
class team;
class character;
class gamescript;
class item;
class outputfile;
class inputfile;
class worldmap;
class god;
class square;
class wsquare;
class lsquare;
class bitmap;
class festring;
class rain;
class liquid;
class entity;
class olterrain;
struct explosion;

typedef std::map<festring, long> valuemap;
typedef bool (*stringkeyhandler)(int, festring&);
typedef vector2d (*positionkeyhandler)(vector2d, int);
typedef void (*positionhandler)(vector2d);

struct homedata
{
  vector2d Pos;
  int Dungeon;
  int Level;
  int Room;
};

outputfile& operator<<(outputfile&, const homedata*);
inputfile& operator>>(inputfile&, homedata*&);

#ifdef VC
#pragma pack(1)
#endif

struct configid
{
  configid() { }
  configid(int Type, int Config) : Type(Type), Config(Config) { }
  bool operator<(const configid& CI) const { return memcmp(this, &CI, sizeof(configid)) < 0; }
  int Type NO_ALIGNMENT;
  int Config NO_ALIGNMENT;
};

#ifdef VC
#pragma pack()
#endif

outputfile& operator<<(outputfile&, const configid&);
inputfile& operator>>(inputfile&, configid&);

struct dangerid
{
  dangerid() { }
  dangerid(double NakedDanger, double EquippedDanger) : NakedDanger(NakedDanger), EquippedDanger(EquippedDanger) { }
  double NakedDanger;
  double EquippedDanger;
};

outputfile& operator<<(outputfile&, const dangerid&);
inputfile& operator>>(inputfile&, dangerid&);

struct ivantime
{
  int Day;
  int Hour;
  int Min;
};

struct killreason
{
  killreason() { }
  killreason(const festring& String, int Amount) : String(String), Amount(Amount) { }
  festring String;
  int Amount;
};

outputfile& operator<<(outputfile&, const killreason&);
inputfile& operator>>(inputfile&, killreason&);

struct killdata
{
  killdata(int Amount = 0, double DangerSum = 0) : Amount(Amount), DangerSum(DangerSum) { }
  int Amount;
  double DangerSum;
  std::vector<killreason> Reason;
};

outputfile& operator<<(outputfile&, const killdata&);
inputfile& operator>>(inputfile&, killdata&);

typedef std::map<configid, dangerid> dangermap;
typedef std::map<ulong, character*> characteridmap;
typedef std::map<ulong, item*> itemidmap;
typedef std::map<ulong, entity*> trapidmap;
typedef std::map<configid, killdata> massacremap;
typedef std::map<ulong, ulong> boneidmap;
typedef std::vector<item*> itemvector;
typedef std::vector<itemvector> itemvectorvector;
typedef std::vector<character*> charactervector;

class quitrequest { };
class areachangerequest { };

class game
{
 public:
  static bool Init(const festring& = CONST_S(""));
  static void DeInit();
  static void Run();
  static int GetMoveCommandKey(int);
  static const vector2d GetMoveVector(int I) { return MoveVector[I]; }
  static const vector2d GetRelativeMoveVector(int I) { return RelativeMoveVector[I]; }
  static const vector2d GetBasicMoveVector(int I) { return BasicMoveVector[I]; }
  static const vector2d GetLargeMoveVector(int I) { return LargeMoveVector[I]; }
  static area* GetCurrentArea() { return CurrentArea; }
  static level* GetCurrentLevel() { return CurrentLevel; }
  static uchar*** GetLuxTable() { return LuxTable; }
  static character* GetPlayer() { return Player; }
  static void SetPlayer(character*);
  static vector2d GetCamera() { return Camera; }
  static void UpdateCameraX();
  static void UpdateCameraY();
  static bool IsLoading() { return Loading; }
  static void SetIsLoading(bool What) { Loading = What; }
  static level* GetLevel(int);
  static void InitLuxTable();
  static void DeInitLuxTable();
  static const char* Insult();
  static bool BoolQuestion(const festring&, int = 0, int = 0);
  static void DrawEverything();
  static bool Save(const festring& = SaveName(""));
  static int Load(const festring& = SaveName(""));
  static bool IsRunning() { return Running; }
  static void SetIsRunning(bool What) { Running = What; }
  static void UpdateCameraX(int);
  static void UpdateCameraY(int);
  static int GetCurrentLevelIndex() { return CurrentLevelIndex; }
  static int GetMoveCommandKeyBetweenPoints(vector2d, vector2d);
  static void DrawEverythingNoBlit(bool = false);
  static god* GetGod(int I) { return God[I]; }
  static const char* GetAlignment(int I) { return Alignment[I]; }
  static void ApplyDivineTick();
  static void ApplyDivineAlignmentBonuses(god*, int, bool);
  static vector2d GetDirectionVectorForKey(int);
  static festring SaveName(const festring& = CONST_S(""));
  static void ShowLevelMessage();
  static double GetMinDifficulty();
  static void TriggerQuestForGoldenEagleShirt();
  static void CalculateGodNumber();
  static void IncreaseTick() { ++Tick; }
  static ulong GetTick() { return Tick; }
  static festring GetAutoSaveFileName() { return AutoSaveFileName; }
  static int DirectionQuestion(const festring&, bool = true, bool = false);
  static void RemoveSaves(bool = true);
  static bool IsInWilderness() { return InWilderness; }
  static void SetIsInWilderness(bool What) { InWilderness = What; }
  static worldmap* GetWorldMap() { return WorldMap; }
  static void SetAreaInLoad(area* What) { AreaInLoad = What; }
  static void SetSquareInLoad(square* What) { SquareInLoad = What; }
  static area* GetAreaInLoad() { return AreaInLoad; }
  static square* GetSquareInLoad() { return SquareInLoad; }
  static int GetLevels();
  static dungeon* GetCurrentDungeon() { return Dungeon[CurrentDungeonIndex]; }
  static dungeon* GetDungeon(int I) { return Dungeon[I]; }
  static int GetCurrentDungeonIndex() { return CurrentDungeonIndex; }
  static void InitDungeons();
  static bool OnScreen(vector2d);
  static void DoEvilDeed(int);
  static void SaveWorldMap(const festring& = SaveName(""), bool = true);
  static worldmap* LoadWorldMap(const festring& = SaveName(""));
  static void UpdateCamera();
  static ulong CreateNewCharacterID(character*);
  static ulong CreateNewItemID(item*);
  static ulong CreateNewTrapID(entity*);
  static team* GetTeam(int I) { return Team[I]; }
  static int GetTeams() { return Teams; }
  static void Hostility(team*, team*);
  static void CreateTeams();
  static festring StringQuestion(const festring&, color16, festring::sizetype, festring::sizetype, bool, stringkeyhandler = 0);
  static long NumberQuestion(const festring&, int, bool = false);
  static ulong IncreaseLOSTick();
  static ulong GetLOSTick() { return LOSTick; }
  static void SendLOSUpdateRequest() { LOSUpdateRequested = true; }
  static void RemoveLOSUpdateRequest() { LOSUpdateRequested = false; }
  static character* GetPetrus() { return Petrus; }
  static void SetPetrus(character* What) { Petrus = What; }
  static bool HandleQuitMessage();
  static int GetDirectionForVector(vector2d);
  static const char* GetVerbalPlayerAlignment();
  static void CreateGods();
  static int GetScreenXSize() { return 42; }
  static int GetScreenYSize() { return 26; }
  static vector2d CalculateScreenCoordinates(vector2d);
  static void BusyAnimation();
  static void BusyAnimation(bitmap*);
  static vector2d PositionQuestion(const festring&, vector2d, positionhandler = 0, positionkeyhandler = 0, bool = true);
  static void LookHandler(vector2d);
  static int AskForKeyPress(const festring&);
  static bool AnimationController();
  static gamescript* GetGameScript() { return GameScript; }
  static void InitScript();
  static valuemap& GetGlobalValueMap() { return GlobalValueMap; }
  static void InitGlobalValueMap();
  static void TextScreen(const festring&, color16 = 0xFFFF, bool = true, void (*)(bitmap*) = 0);
  static void SetCursorPos(vector2d What) { CursorPos = What; }
  static bool DoZoom() { return Zoom; }
  static void SetDoZoom(bool What) { Zoom = What; }
  static int KeyQuestion(const festring&, int, int, ...);
  static vector2d LookKeyHandler(vector2d, int);
  static vector2d NameKeyHandler(vector2d, int);
  static void End(festring, bool = true, bool = true);
  static int CalculateRoughDirection(vector2d);
  static long ScrollBarQuestion(const festring&, long, long, long, long, long, color16, color16, color16, void (*)(long) = 0);
  static bool IsGenerating() { return Generating; }
  static void SetIsGenerating(bool What) { Generating = What; }
  static void CalculateNextDanger();
  static int Menu(bitmap*, vector2d, const festring&, const festring&, color16, const festring& = "", const festring& = "");
  static void InitDangerMap();
  static const dangermap& GetDangerMap();
  static bool TryTravel(int, int, int, bool = false, bool = true);
  static bool LeaveArea(charactervector&, bool, bool);
  static void EnterArea(charactervector&, int, int);
  static int CompareLights(color24, color24);
  static int CompareLightToInt(color24, color24);
  static void CombineLights(color24&, color24);
  static color24 CombineConstLights(color24, color24);
  static bool IsDark(color24);
  static void SetStandardListAttributes(felist&);
  static double GetAveragePlayerArmStrengthExperience() { return AveragePlayerArmStrengthExperience; }
  static double GetAveragePlayerLegStrengthExperience() { return AveragePlayerLegStrengthExperience; }
  static double GetAveragePlayerDexterityExperience() { return AveragePlayerDexterityExperience; }
  static double GetAveragePlayerAgilityExperience() { return AveragePlayerAgilityExperience; }
  static void InitPlayerAttributeAverage();
  static void UpdatePlayerAttributeAverage();
  static void CallForAttention(vector2d, int);
  static character* SearchCharacter(ulong);
  static item* SearchItem(ulong);
  static entity* SearchTrap(ulong);
  static void AddCharacterID(character*, ulong);
  static void RemoveCharacterID(ulong);
  static void AddItemID(item*, ulong);
  static void RemoveItemID(ulong);
  static void UpdateItemID(item*, ulong);
  static void AddTrapID(entity*, ulong);
  static void RemoveTrapID(ulong);
  static void UpdateTrapID(entity*, ulong);
  static int GetStoryState() { return StoryState; }
  static void SetStoryState(int What) { StoryState = What; }
  static void SetIsInGetCommand(bool What) { InGetCommand = What; }
  static bool IsInGetCommand() { return InGetCommand; }
  static festring GetHomeDir();
  static festring GetSaveDir();
  static festring GetGameDir();
  static festring GetBoneDir();
  static bool PlayerWasHurtByExplosion() { return PlayerHurtByExplosion; }
  static void SetPlayerWasHurtByExplosion(bool What) { PlayerHurtByExplosion = What; }
  static void SetCurrentArea(area* What) { CurrentArea = What; }
  static void SetCurrentLevel(level* What) { CurrentLevel = What; }
  static void SetCurrentWSquareMap(wsquare*** What) { CurrentWSquareMap = What; }
  static void SetCurrentLSquareMap(lsquare*** What) { CurrentLSquareMap = What; }
  static festring& GetDefaultPolymorphTo() { return DefaultPolymorphTo; }
  static festring& GetDefaultSummonMonster() { return DefaultSummonMonster; }
  static festring& GetDefaultChangeMaterial() { return DefaultChangeMaterial; }
  static festring& GetDefaultDetectMaterial() { return DefaultDetectMaterial; }
  static void SignalDeath(const character*, const character*, const festring&);
  static void DisplayMassacreLists();
  static void DisplayMassacreList(const massacremap&, const char*, long);
  static bool MassacreListsEmpty();
#ifdef WIZARD
  static void ActivateWizardMode() { WizardMode = true; }
  static bool WizardModeIsActive() { return WizardMode; }
  static void SeeWholeMap();
  static int GetSeeWholeMapCheatMode() { return SeeWholeMapCheatMode; }
  static bool GoThroughWallsCheatIsActive() { return GoThroughWallsCheat; }
  static void GoThroughWalls() { GoThroughWallsCheat = !GoThroughWallsCheat; }
#else
  static bool WizardModeIsActive() { return false; }
  static int GetSeeWholeMapCheatMode() { return 0; }
  static bool GoThroughWallsCheatIsActive() { return false; }
#endif
  static bool WizardModeIsReallyActive() { return WizardMode; }
  static void CreateBone();
  static int GetQuestMonstersFound() { return QuestMonstersFound; }
  static void SignalQuestMonsterFound() { ++QuestMonstersFound; }
  static void SetQuestMonstersFound(int What) { QuestMonstersFound = What; }
  static bool PrepareRandomBone(int);
  static boneidmap& GetBoneItemIDMap() { return BoneItemIDMap; }
  static boneidmap& GetBoneCharacterIDMap() { return BoneCharacterIDMap; }
  static double CalculateAverageDanger(const charactervector&, character*);
  static double CalculateAverageDangerOfAllNormalEnemies();
  static character* CreateGhost();
  static bool TooGreatDangerFound() { return TooGreatDangerFoundBool; }
  static void SetTooGreatDangerFound(bool What) { TooGreatDangerFoundBool = What; }
  static void CreateBusyAnimationCache();
  static long GetScore();
  static bool TweraifIsFree();
  static bool IsXMas();
  static int AddToItemDrawVector(const itemvector&);
  static void ClearItemDrawVector();
  static void ItemEntryDrawer(bitmap*, vector2d, uint);
  static int AddToCharacterDrawVector(character*);
  static void ClearCharacterDrawVector();
  static void CharacterEntryDrawer(bitmap*, vector2d, uint);
  static void GodEntryDrawer(bitmap*, vector2d, uint);
  static itemvectorvector& GetItemDrawVector() { return ItemDrawVector; }
  static charactervector& GetCharacterDrawVector() { return CharacterDrawVector; }
  static bool IsSumoWrestling() { return SumoWrestling; }
  static void SetIsSumoWrestling(bool What) { SumoWrestling = What; }
  static bool AllowHostilities() { return !SumoWrestling; }
  static bool AllBodyPartsVanish() { return SumoWrestling; }
  static bool TryToEnterSumoArena();
  static bool TryToExitSumoArena();
  static bool EndSumoWrestling(int);
  static character* GetSumo();
  static const festring& GetPlayerName() { return PlayerName; }
  static rain* ConstructGlobalRain();
  static void SetGlobalRainLiquid(liquid* What) { GlobalRainLiquid = What; }
  static void SetGlobalRainSpeed(vector2d What) { GlobalRainSpeed = What; }
  static bool PlayerIsSumoChampion() { return PlayerSumoChampion; }
  static vector2d GetSunLightDirectionVector();
  static int CalculateMinimumEmitationRadius(color24);
  static ulong IncreaseSquarePartEmitationTicks();
  static const int GetLargeMoveDirection(int I) { return LargeMoveDirection[I]; }
  static void Wish(character*, const char*, const char*);
  static festring DefaultQuestion(festring, festring&, stringkeyhandler = 0);
  static void GetTime(ivantime&);
  static long GetTurn() { return Turn; }
  static void IncreaseTurn() { ++Turn; }
  static int GetTotalMinutes() { return Tick * 60 / 2000; }
  static bool PolymorphControlKeyHandler(int, festring&);
  static ulong* GetEquipmentMemory() { return EquipmentMemory; }
  static bool PlayerIsRunning() { return PlayerRunning; }
  static void SetPlayerIsRunning(bool What) { PlayerRunning = What; }
  static bool FillPetVector(const char*);
  static bool CommandQuestion();
  static void NameQuestion();
  static vector2d CommandKeyHandler(vector2d, int);
  static void CommandScreen(const festring&, ulong, ulong, ulong&, ulong&);
  static bool CommandAll();
  static double GetDangerFound() { return DangerFound; }
  static void SetDangerFound(double What) { DangerFound = What; }
  static color16 GetAttributeColor(int);
  static void UpdateAttributeMemory();
  static void InitAttributeMemory();
  static void TeleportHandler(vector2d);
  static void PetHandler(vector2d);
  static bool SelectPet(int);
  static double GetGameSituationDanger();
  static olterrain* GetMonsterPortal() { return MonsterPortal; }
  static void SetMonsterPortal(olterrain* What) { MonsterPortal = What; }
  static bool GetCausePanicFlag() { return CausePanicFlag; }
  static void SetCausePanicFlag(bool What) { CausePanicFlag = What; }
  static long GetTimeSpent();
 private:
  static void UpdateCameraCoordinate(int&, int, int, int);
  static const char* const Alignment[];
  static god** God;
  static int CurrentLevelIndex;
  static int CurrentDungeonIndex;
  static const int MoveNormalCommandKey[];
  static const int MoveAbnormalCommandKey[];
  static const vector2d MoveVector[];
  static const vector2d RelativeMoveVector[];
  static const vector2d BasicMoveVector[];
  static const vector2d LargeMoveVector[];
  static uchar*** LuxTable;
  static bool Running;
  static character* Player;
  static vector2d Camera;
  static ulong Tick;
  static festring AutoSaveFileName;
  static bool InWilderness;
  static worldmap* WorldMap;
  static area* AreaInLoad;
  static square* SquareInLoad;
  static dungeon** Dungeon;
  static ulong NextCharacterID;
  static ulong NextItemID;
  static ulong NextTrapID;
  static team** Team;
  static ulong LOSTick;
  static bool LOSUpdateRequested;
  static character* Petrus;
  static bool Loading;
  static gamescript* GameScript;
  static valuemap GlobalValueMap;
  static vector2d CursorPos;
  static bool Zoom;
  static bool Generating;
  static dangermap DangerMap;
  static int NextDangerIDType;
  static int NextDangerIDConfigIndex;
  static double AveragePlayerArmStrengthExperience;
  static double AveragePlayerLegStrengthExperience;
  static double AveragePlayerDexterityExperience;
  static double AveragePlayerAgilityExperience;
  static characteridmap CharacterIDMap;
  static itemidmap ItemIDMap;
  static trapidmap TrapIDMap;
  static int Teams;
  static int Dungeons;
  static int StoryState;
  static bool InGetCommand;
  static bool PlayerHurtByExplosion;
  static area* CurrentArea;
  static level* CurrentLevel;
  static wsquare*** CurrentWSquareMap;
  static lsquare*** CurrentLSquareMap;
  static festring DefaultPolymorphTo;
  static festring DefaultSummonMonster;
  static festring DefaultWish;
  static festring DefaultChangeMaterial;
  static festring DefaultDetectMaterial;
  static massacremap PlayerMassacreMap;
  static massacremap PetMassacreMap;
  static massacremap MiscMassacreMap;
  static long PlayerMassacreAmount;
  static long PetMassacreAmount;
  static long MiscMassacreAmount;
  static bool WizardMode;
  static int SeeWholeMapCheatMode;
  static bool GoThroughWallsCheat;
  static int QuestMonstersFound;
  static boneidmap BoneItemIDMap;
  static boneidmap BoneCharacterIDMap;
  static bool TooGreatDangerFoundBool;
  static bitmap* BusyAnimationCache[32];
  static itemvectorvector ItemDrawVector;
  static charactervector CharacterDrawVector;
  static bool SumoWrestling;
  static festring PlayerName;
  static liquid* GlobalRainLiquid;
  static vector2d GlobalRainSpeed;
  static long GlobalRainTimeModifier;
  static bool PlayerSumoChampion;
  static ulong SquarePartEmitationTick;
  static const int LargeMoveDirection[];
  static long Turn;
  static ulong EquipmentMemory[MAX_EQUIPMENT_SLOTS];
  static bool PlayerRunning;
  static character* LastPetUnderCursor;
  static charactervector PetVector;
  static double DangerFound;
  static int OldAttribute[ATTRIBUTES];
  static int NewAttribute[ATTRIBUTES];
  static int LastAttributeChangeTick[ATTRIBUTES];
  static int NecroCounter;
  static int CursorData;
  static olterrain* MonsterPortal;
  static bool CausePanicFlag;
  static time_t TimePlayedBeforeLastLoad;
  static time_t LastLoad;
  static time_t GameBegan;
};

inline void game::CombineLights(color24& L1, color24 L2)
{
  if(L2)
    {
      if(L1)
	{
	  long Red1 = L1 & 0xFF0000, Red2 = L2 & 0xFF0000;
	  long New = Red1 >= Red2 ? Red1 : Red2;
	  long Green1 = L1 & 0xFF00, Green2 = L2 & 0xFF00;
	  New |= Green1 >= Green2 ? Green1 : Green2;
	  long Blue1 = L1 & 0xFF, Blue2 = L2 & 0xFF;
	  L1 = Blue1 >= Blue2 ? New | Blue1 : New | Blue2;
	}
      else
	L1 = L2;
    }
}

inline color24 game::CombineConstLights(color24 L1, color24 L2)
{
  CombineLights(L1, L2);
  return L1;
}

inline bool game::IsDark(color24 Light)
{
  return !Light
      || ((Light & 0xFF0000) < (LIGHT_BORDER << 16)
       && (Light & 0x00FF00) < (LIGHT_BORDER << 8)
       && (Light & 0x0000FF) < LIGHT_BORDER);
}

inline int game::CompareLights(color24 L1, color24 L2)
{
  if(L1)
    {
      if((L1 & 0xFF0000) > (L2 & 0xFF0000)
      || (L1 & 0x00FF00) > (L2 & 0x00FF00)
      || (L1 & 0x0000FF) > (L2 & 0x0000FF))
	return 1;
      else if((L1 & 0xFF0000) == (L2 & 0xFF0000)
	   || (L1 & 0x00FF00) == (L2 & 0x00FF00)
	   || (L1 & 0x0000FF) == (L2 & 0x0000FF))
	return 0;
      else
	return -1;
    }
  else
    return -int(!!L2);
}

inline vector2d game::CalculateScreenCoordinates(vector2d Pos)
{
  return vector2d((Pos.X - Camera.X + 1) << 4, (Pos.Y - Camera.Y + 2) << 4);
}

#endif
