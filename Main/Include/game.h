#ifndef __GAME_H__
#define __GAME_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>
#include <vector>
#include <string>

#include "vector2d.h"
#include "felibdef.h"
#include "femath.h"

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
struct explosion;

typedef std::map<std::string, long> valuemap;

struct homedata
{
  vector2d Pos;
  uchar Dungeon;
  uchar Level;
  uchar Room;
};

outputfile& operator<<(outputfile&, const homedata*);
inputfile& operator>>(inputfile&, homedata*&);

#ifdef VC
#pragma pack(1)
#endif

struct configid
{
  configid() { }
  configid(ushort Type, ushort Config) : Type(Type), Config(Config) { }
  bool operator<(const configid&) const;
  ushort Type NO_ALIGNMENT;
  ushort Config NO_ALIGNMENT;
};

#ifdef VC
#pragma pack()
#endif

outputfile& operator<<(outputfile&, const configid&);
inputfile& operator>>(inputfile&, configid&);

struct dangerid
{
  dangerid() { }
  dangerid(float NakedDanger, float EquippedDanger) : NakedDanger(NakedDanger), EquippedDanger(EquippedDanger), HasBeenGenerated(false) { }
  float NakedDanger;
  float EquippedDanger;
  bool HasBeenGenerated;
};

outputfile& operator<<(outputfile&, const dangerid&);
inputfile& operator>>(inputfile&, dangerid&);

typedef std::map<configid, dangerid> dangermap;
typedef std::map<ulong, character*> characteridmap;
typedef std::map<configid, ushort> massacremap;

class game
{
 public:
  static bool Init(const std::string& = "");
  static void DeInit();
  static void Run();
  static int GetMoveCommandKey(ushort Index) { return MoveCommandKey[Index]; }
  static const vector2d GetMoveVector(ushort Index) { return MoveVector[Index]; }
  static const vector2d GetRelativeMoveVector(ushort Index) { return RelativeMoveVector[Index]; }
  static area* GetCurrentArea() { return CurrentArea; }
  static level* GetCurrentLevel() { return CurrentLevel; }
  static bool WorldMapLOSHandler(long, long);
  static bool LevelLOSHandler(long, long);
  static ushort*** GetLuxTable() { return LuxTable; }
  static ushort* GetLuxTableSize() { return LuxTableSize; }
  static character* GetPlayer() { return Player; }
  static void SetPlayer(character*);
  static vector2d GetCamera() { return Camera; }
  static void UpdateCameraX();
  static void UpdateCameraY();
  static bool IsLoading() { return Loading; }
  static void SetIsLoading(bool What) { Loading = What; }
  static level* GetLevel(ushort);
  static void InitLuxTable();
  static void DeInitLuxTable();
  static const char* Insult();
  static bool BoolQuestion(const std::string&, int = 0, int = 0);
  static void DrawEverything();
  static bool Save(const std::string& = SaveName(""));
  static uchar Load(const std::string& = SaveName(""));
  static bool IsRunning() { return Running; }
  static void SetIsRunning(bool What) { Running = What; }
  static void ActivateWizardMode() { WizardMode = true; }
  static bool WizardModeIsActive() { return WizardMode; }
  static void SeeWholeMap();
  static bool SeeWholeMapCheatIsActive() { return SeeWholeMapCheat; }
  static bool EmitationHandler(long, long);
  static bool NoxifyHandler(long, long);
  static bool GoThroughWallsCheatIsActive() { return GoThroughWallsCheat; }
  static void GoThroughWalls() { GoThroughWallsCheat = !GoThroughWallsCheat; }
  static void UpdateCameraXWithPos(ushort);
  static void UpdateCameraYWithPos(ushort);
  static bool KeyIsOK(char);
  static uchar GetCurrentLevelIndex() { return CurrentLevelIndex; }
  static int GetMoveCommandKeyBetweenPoints(vector2d, vector2d);
  static void DrawEverythingNoBlit(bool = false);
  static god* GetGod(ushort Index) { return God[Index]; }
  static const std::string& GetAlignment(ushort Index) { return Alignment[Index]; }
  static void ApplyDivineTick();
  static void ApplyDivineAlignmentBonuses(god*, bool, short = 10);
  static vector2d GetDirectionVectorForKey(int);
  static std::string SaveName(const std::string& = "");
  static bool EyeHandler(long, long);
  static long GodScore();
  static void ShowLevelMessage();
  static float Difficulty();
  static void TriggerQuestForGoldenEagleShirt();
  static void CalculateGodNumber();
  static long GetBaseScore() { return BaseScore; }
  static void Tick(ushort Tick = 1) { Ticks += Tick; }
  static ulong GetTicks() { return Ticks; }
  static std::string GetAutoSaveFileName() { return AutoSaveFileName; }
  static uchar DirectionQuestion(const std::string&, bool = true, bool = false);
  static void RemoveSaves(bool = true);
  static bool IsInWilderness() { return InWilderness; }
  static void SetIsInWilderness(bool What) { InWilderness = What; }
  static worldmap* GetWorldMap() { return WorldMap; }
  static void SetAreaInLoad(area* What) { AreaInLoad = What; }
  static void SetSquareInLoad(square* What) { SquareInLoad = What; }
  static area* GetAreaInLoad() { return AreaInLoad; }
  static square* GetSquareInLoad() { return SquareInLoad; }
  static ushort GetLevels();
  static dungeon* GetCurrentDungeon() { return Dungeon[CurrentDungeonIndex]; }
  static dungeon* GetDungeon(ushort Index) { return Dungeon[Index]; }
  static uchar GetCurrentDungeonIndex() { return CurrentDungeonIndex; }
  static void InitDungeons();
  static bool OnScreen(vector2d);
  static void DoEvilDeed(ushort);
  static void SaveWorldMap(const std::string& = SaveName(""), bool = false);
  static worldmap* LoadWorldMap(const std::string& = SaveName(""));
  static void UpdateCamera();
  static ulong CreateNewCharacterID(character*);
  static ulong CreateNewItemID() { return NextItemID++; }
  static team* GetTeam(ushort Index) { return Team[Index]; }
  static uchar GetTeams() { return Teams; }
  static void Hostility(team*, team*);
  static void CreateTeams();
  static std::string StringQuestion(const std::string&, vector2d, ushort, ushort, ushort, bool);
  static long NumberQuestion(const std::string&, vector2d, ushort);
  static void LOSTurn();
  static ulong GetLOSTurns() { return LOSTurns; }
  static void SendLOSUpdateRequest() { LOSUpdateRequested = true; }
  static void RemoveLOSUpdateRequest() { LOSUpdateRequested = false; }
  static character* GetPetrus() { return Petrus; }
  static void SetPetrus(character* What) { Petrus = What; }
  static bool HandleQuitMessage();
  static uchar GetDirectionForVector(vector2d);
  static std::string GetVerbalPlayerAlignment();
  static void CreateGods();
  static vector2d GetScreenSize() { return ScreenSize; }
  static void SetScreenSize(vector2d What) { ScreenSize = What; }
  static vector2d CalculateScreenCoordinates(vector2d);
  static void BusyAnimation();
  static void BusyAnimation(bitmap*);
  static vector2d PositionQuestion(const std::string&, vector2d, void (*)(vector2d) = 0, void (*)(vector2d, int) = 0, bool = true);
  static void LookHandler(vector2d);
  static int AskForKeyPress(const std::string&);
  static bool AnimationController();
  static gamescript* GetGameScript() { return GameScript; }
  static void InitScript();
  static valuemap& GetGlobalValueMap() { return GlobalValueMap; }
  static void InitGlobalValueMap();
  static void TextScreen(const std::string&, ushort = 0xFFFF, bool = true, void (*)(bitmap*) = 0);
  static vector2d GetCursorPos() { return CursorPos; }
  static void SetCursorPos(vector2d What) { CursorPos = What; }
  static bool DoZoom() { return Zoom; }
  static void SetDoZoom(bool What) { Zoom = What; }
  static int KeyQuestion(const std::string&, int, int, ...);
  static void LookKeyHandler(vector2d, int);
  static void NameKeyHandler(vector2d, int);
  static const std::string& GetLockDescription(ushort Index) { return LockDescription[Index]; }
  static void End(bool = true, bool = true);
  static uchar CalculateRoughDirection(vector2d);
  static void InstallCurrentEmitter(vector2d, ulong);
  static void InstallCurrentNoxifier(vector2d Pos) { CurrentEmitterPos = Pos; }
  static long ScrollBarQuestion(const std::string&, vector2d, long, long, long, long, long, ushort, ushort, ushort, void (*)(long) = 0);
  static bool IsGenerating() { return Generating; }
  static void SetIsGenerating(bool What) { Generating = What; }
  static void CalculateNextDanger();
  static int Menu(bitmap*, vector2d, const std::string&, const std::string&, ushort, const std::string& = "", const std::string& = "");
  static void InitDangerMap();
  static const dangermap& GetDangerMap() { return DangerMap; }
  static bool TryTravel(uchar, uchar, uchar, bool = false);
  static bool LeaveArea(std::vector<character*>&, bool);
  static void EnterArea(std::vector<character*>&, uchar, uchar);
  static char CompareLights(ulong, ulong);
  static char CompareLightToInt(ulong, uchar);
  static void CombineLights(ulong&, ulong);
  static bool IsDark(ulong);
  static void SetStandardListAttributes(felist&);
  static void SignalGeneration(configid);
  static float GetAveragePlayerArmStrength() { return AveragePlayerArmStrength; }
  static float GetAveragePlayerLegStrength() { return AveragePlayerLegStrength; }
  static float GetAveragePlayerDexterity() { return AveragePlayerDexterity; }
  static float GetAveragePlayerAgility() { return AveragePlayerAgility; }
  static void InitPlayerAttributeAverage();
  static void UpdatePlayerAttributeAverage();
  static void CallForAttention(vector2d, ushort);
  static character* GetHaedlac() { return Haedlac; }
  static void SetHaedlac(character* What) { Haedlac = What; }
  static character* SearchCharacter(ulong);
  static void AddCharacterID(character*, ulong);
  static void RemoveCharacterID(ulong);
  static uchar GetStoryState() { return StoryState; }
  static void SetStoryState(uchar What) { StoryState = What; }
  static void SetIsInGetCommand(bool What) { InGetCommand = What; }
  static bool IsInGetCommand() { return InGetCommand; }
  static std::string GetHomeDir();
  static std::string GetSaveDir();
  static std::string GetGameDir();
  static bool ExplosionHandler(long, long);
  static ulong CreateNewExplosionID() { return NextExplosionID++; }
  static void SetCurrentExplosion(const explosion* What) { CurrentExplosion = What; }
  static bool PlayerWasHurtByExplosion() { return PlayerHurtByExplosion; }
  static void SetPlayerWasHurtByExplosion(bool What) { PlayerHurtByExplosion = What; }
  static void SetCurrentArea(area* What) { CurrentArea = What; }
  static void SetCurrentLevel(level* What) { CurrentLevel = What; }
  static void SetCurrentWSquareMap(wsquare*** What) { CurrentWSquareMap = What; }
  static void SetCurrentLSquareMap(lsquare*** What) { CurrentLSquareMap = What; }
  static const std::string& GetDefaultPolymorphTo() { return DefaultPolymorphTo; }
  static void SetDefaultPolymorphTo(const std::string What) { DefaultPolymorphTo = What; }
  static void SignalDeath(const character*, const character*);
  static void DisplayMassacreLists();
  static void DisplayMassacreList(const massacremap&, const std::string&, ulong);
  static bool MassacreListsEmpty();
 private:
  static std::string Alignment[];
  static god** God;
  static uchar CurrentLevelIndex;
  static uchar CurrentDungeonIndex;
  static int MoveCommandKey[];
  static const vector2d MoveVector[];
  static const vector2d RelativeMoveVector[];
  static ushort*** LuxTable;
  static ushort* LuxTableSize;
  static bool Running;
  static character* Player;
  static vector2d Camera;
  static bool WizardMode;
  static bool SeeWholeMapCheat;
  static bool GoThroughWallsCheat;
  static long BaseScore;
  static ulong Ticks;
  static std::string AutoSaveFileName;
  static bool InWilderness;
  static worldmap* WorldMap;
  static area* AreaInLoad;
  static square* SquareInLoad;
  static dungeon** Dungeon;
  static ulong NextCharacterID;
  static ulong NextItemID;
  static ulong NextExplosionID;
  static team** Team;
  static ulong LOSTurns;
  static bool LOSUpdateRequested;
  static character* Petrus;
  static bool Loading;
  static vector2d ScreenSize;
  static gamescript* GameScript;
  static valuemap GlobalValueMap;
  static vector2d CursorPos;
  static bool Zoom;
  static std::string LockDescription[];
  static ushort** CurrentRedLuxTable;
  static ushort** CurrentGreenLuxTable;
  static ushort** CurrentBlueLuxTable;
  static long CurrentEmitterPosXMinus16;
  static long CurrentEmitterPosYMinus16;
  static vector2d CurrentEmitterPos;
  static bool Generating;
  static dangermap DangerMap;
  static configid NextDangerId;
  static float AveragePlayerArmStrength;
  static float AveragePlayerLegStrength;
  static float AveragePlayerDexterity;
  static float AveragePlayerAgility;
  static character* Haedlac;
  static characteridmap CharacterIDMap;
  static uchar Teams;
  static uchar Dungeons;
  static uchar StoryState;
  static bool InGetCommand;
  static const explosion* CurrentExplosion;
  static bool PlayerHurtByExplosion;
  static area* CurrentArea;
  static level* CurrentLevel;
  static wsquare*** CurrentWSquareMap;
  static lsquare*** CurrentLSquareMap;
  static std::string DefaultPolymorphTo;
  static massacremap PlayerMassacreMap;
  static massacremap PetMassacreMap;
  static massacremap MiscMassacreMap;
  static ulong PlayerMassacreAmount;
  static ulong PetMassacreAmount;
  static ulong MiscMassacreAmount;
};

inline void game::CombineLights(ulong& L1, ulong L2)
{
  L1 = MakeRGB24(Max(GetRed24(L1), GetRed24(L2)), Max(GetGreen24(L1), GetGreen24(L2)), Max(GetBlue24(L1), GetBlue24(L2)));
}

inline bool game::IsDark(ulong Light)
{
  return !Light || (GetRed24(Light) < LIGHT_BORDER && GetGreen24(Light) < LIGHT_BORDER && GetBlue24(Light) < LIGHT_BORDER);
}

#endif
