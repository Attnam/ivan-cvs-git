#include <algorithm>
#include <cstdarg>

#if defined(LINUX) || defined(__DJGPP__)
#include <sys/stat.h>
#endif

#ifdef WIN32
#include <direct.h>
#endif

#include "whandler.h"
#include "hscore.h"
#include "colorbit.h"
#include "message.h"
#include "feio.h"
#include "team.h"
#include "iconf.h"
#include "allocate.h"
#include "pool.h"
#include "god.h"
#include "proto.h"
#include "stack.h"
#include "felist.h"
#include "human.h"
#include "nonhuman.h"
#include "wsquare.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"
#include "save.h"
#include "miscitem.h"
#include "room.h"
#include "materias.h"
#include "rain.h"
#include "gear.h"
//#include "wskill.h"

#define SAVE_FILE_VERSION 115 // Increment this if changes make savefiles incompatible
#define BONE_FILE_VERSION 102 // Increment this if changes make bonefiles incompatible

#define LOADED 0
#define NEW_GAME 1
#define BACK 2

int game::CurrentLevelIndex;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
dungeon** game::Dungeon;
int game::CurrentDungeonIndex;
ulong game::NextCharacterID = 1;
ulong game::NextItemID = 1;
team** game::Team;
ulong game::LOSTick;
vector2d game::CursorPos(-1, -1);
bool game::Zoom;
bool game::Generating = false;
double game::AveragePlayerArmStrength;
double game::AveragePlayerLegStrength;
double game::AveragePlayerDexterity;
double game::AveragePlayerAgility;
int game::Teams;
int game::Dungeons;
int game::StoryState;
massacremap game::PlayerMassacreMap;
massacremap game::PetMassacreMap;
massacremap game::MiscMassacreMap;
long game::PlayerMassacreAmount = 0;
long game::PetMassacreAmount = 0;
long game::MiscMassacreAmount = 0;
boneidmap game::BoneItemIDMap;
boneidmap game::BoneCharacterIDMap;
bool game::TooGreatDangerFoundBool;
itemvector game::ItemDrawVector;
charactervector game::CharacterDrawVector;
bool game::SumoWrestling;
liquid* game::GlobalRainLiquid;
vector2d game::GlobalRainSpeed;
long game::GlobalRainTimeModifier;
bool game::PlayerSumoChampion;
ulong game::SquarePartEmitationTick = 0;
long game::Turn;

bool game::Loading = false;
bool game::InGetCommand = false;
character* game::Petrus = 0;

festring game::AutoSaveFileName = game::GetSaveDir() + "AutoSave";
const char* const game::Alignment[] = { "L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--" };
god** game::God;

const int game::MoveNormalCommandKey[] = { KEY_HOME, KEY_UP, KEY_PAGE_UP, KEY_LEFT, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, '.' };
const int game::MoveAbnormalCommandKey[] = { '7','8','9','u','o','j','k','l','.' };

const vector2d game::MoveVector[] = { vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1), vector2d(0, 0) };
const vector2d game::RelativeMoveVector[] = { vector2d(-1, -1), vector2d(1, 0), vector2d(1, 0), vector2d(-2, 1), vector2d(2, 0), vector2d(-2, 1), vector2d(1, 0), vector2d(1, 0), vector2d(-1, -1) };
const vector2d game::BasicMoveVector[] = { vector2d(-1, 0), vector2d(1, 0), vector2d(0, -1), vector2d(0, 1) };
const vector2d game::LargeMoveVector[] = { vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(2, -1), vector2d(-1, 0), vector2d(2, 0), vector2d(-1, 1), vector2d(2, 1), vector2d(-1, 2), vector2d(0, 2), vector2d(1, 2), vector2d(2, 2), vector2d(0, 0), vector2d(1, 0), vector2d(0, 1), vector2d(1, 1) };
const int game::LargeMoveDirection[] = { 0, 1, 1, 2, 3, 4, 3, 4, 5, 6, 6, 7, 8, 8, 8, 8 };

bool game::LOSUpdateRequested = false;
uchar*** game::LuxTable = 0;
bool game::Running;
character* game::Player;
vector2d game::Camera(0, 0);
ulong game::Tick;
gamescript* game::GameScript = 0;
valuemap game::GlobalValueMap;
dangermap game::DangerMap;
int game::NextDangerIDType;
int game::NextDangerIDConfigIndex;
characteridmap game::CharacterIDMap;
itemidmap game::ItemIDMap;
bool game::PlayerHurtByExplosion;
area* game::CurrentArea;
level* game::CurrentLevel;
wsquare*** game::CurrentWSquareMap;
lsquare*** game::CurrentLSquareMap;
festring game::DefaultPolymorphTo;
festring game::DefaultSummonMonster;
festring game::DefaultWish;
festring game::DefaultChangeMaterial;
festring game::DefaultDetectMaterial;
bool game::WizardMode;
int game::SeeWholeMapCheatMode;
bool game::GoThroughWallsCheat;
int game::QuestMonstersFound;
bitmap* game::BusyAnimationCache[32];
festring game::PlayerName;

void game::AddCharacterID(character* Char, ulong ID) {
if(CharacterIDMap.find(ID) != CharacterIDMap.end())
  int esko = 2;
CharacterIDMap.insert(std::pair<ulong, character*>(ID, Char));
}
void game::RemoveCharacterID(ulong ID) {
if(CharacterIDMap.find(ID) == CharacterIDMap.end())
  int esko = 2;
CharacterIDMap.erase(CharacterIDMap.find(ID));
}
void game::AddItemID(item* Item, ulong ID) {
if(ID == 1)
  int esko = 2;

if(ItemIDMap.find(ID) != ItemIDMap.end())
  int esko = 2;
ItemIDMap.insert(std::pair<ulong, item*>(ID, Item));
}
void game::RemoveItemID(ulong ID)
{
if(ID && ItemIDMap.find(ID) == ItemIDMap.end())
  int esko = 2;

if(ID) ItemIDMap.erase(ItemIDMap.find(ID));
}
void game::UpdateItemID(item* Item, ulong ID) {
if(ItemIDMap.find(ID) == ItemIDMap.end())
  int esko = 2;
ItemIDMap.find(ID)->second = Item;
}
const dangermap& game::GetDangerMap() { return DangerMap; }
void game::ClearItemDrawVector() { ItemDrawVector.clear(); }
void game::ClearCharacterDrawVector() { CharacterDrawVector.clear(); }

void game::InitScript()
{
  inputfile ScriptFile(GetGameDir() + "Script/dungeon.dat", &GlobalValueMap);
  GameScript = new gamescript;
  GameScript->ReadFrom(ScriptFile);
  GameScript->RandomizeLevels();
}

#include "confdef.h"

bool game::Init(const festring& Name)
{
  if(Name.IsEmpty())
    if(ivanconfig::GetDefaultName().IsEmpty())
      {
	PlayerName.Empty();

	if(iosystem::StringQuestion(PlayerName, CONST_S("What is your name? (1-20 letters)"), vector2d(30, 46), WHITE, 1, 20, true, true) == ABORTED || PlayerName.IsEmpty())
	  return false;
      }
    else
      PlayerName = ivanconfig::GetDefaultName();
  else
    PlayerName = Name;

#ifdef WIN32
  _mkdir("Save");
#endif

#ifdef __DJGPP__
  mkdir("Save", S_IWUSR);
#endif

#ifdef LINUX
  mkdir(GetSaveDir().CStr(), S_IRWXU|S_IRWXG);
#endif

#ifdef WIN32
  _mkdir("Bones");
#endif

#ifdef __DJGPP__
  mkdir("Bones", S_IWUSR);
#endif

  LOSTick = 2;

  switch(Load(SaveName(PlayerName)))
    {
    case LOADED:
      {
	globalwindowhandler::InstallControlLoop(AnimationController);
	SetIsRunning(true);
	SetIsLoading(true);
	GetCurrentArea()->SendNewDrawRequest();
	SendLOSUpdateRequest();
	ADD_MESSAGE("Game loaded successfully.");
	PlayerName = PLAYER->GetAssignedName();
	return true;
      }
    case NEW_GAME:
      {
	iosystem::TextScreen(CONST_S( "You couldn't possibly have guessed this day would differ from any other.\n"
				      "It began just as always. You woke up at dawn and drove off the giant spider\n"
				      "resting on your face. On your way to work you had serious trouble avoiding\n"
				      "the lions and pythons roaming wild around the village. After getting kicked\n"
				      "by colony masters for being late you performed your twelve-hour routine of\n"
				      "climbing trees, gathering bananas, climbing trees, gathering bananas, chasing\n"
				      "monkeys that stole the first gathered bananas, carrying bananas to the village\n"
				      "and trying to look happy when real food was distributed.\n\n"
				      "Finally you were about to enjoy your free time by taking a quick dip in the\n"
				      "nearby crocodile bay. However, at this point something unusual happened.\n"
				      "You were summoned to the mansion of Richel Decos, the viceroy of the\n"
				      "colony, and were led directly to him."));

	iosystem::TextScreen(CONST_S( "\"I have a task for you, citizen\", said the viceroy picking his golden\n"
				      "teeth, \"The market price of bananas has taken a deep dive and yet the\n"
				      "central government is about to raise taxes. I have sent appeals to high\n"
				      "priest Petrus but received no response. I fear my enemies in Attnam are\n"
				      "plotting against me and intercepting my messages before they reach him!\"\n\n"
				      "\"That is why you must travel to Attnam with a letter I'll give you and\n"
				      "deliver it to Petrus directly. Alas, you somehow have to cross the sea\n"
				      "between. Because it's winter, all Attnamese ships are trapped by ice and\n"
				      "I have none. Therefore you must venture through the small underwater tunnel\n"
				      "connecting our islands. It is infested with monsters, but since you have\n"
				      "stayed alive here so long, the trip will surely cause you no trouble.\"\n\n"
				      "You have never been so happy! According to the mansion's traveling\n"
				      "brochures, Attnam is a peaceful but bustling world city on a beautiful\n"
				      "snowy fell surrounded by frozen lakes glittering in the arctic sun just\n"
				      "like the diamonds of the imperial treasury. Not that you would believe a\n"
				      "word. The point is that tomorrow you can finally forget your home and\n"
				      "face the untold adventures ahead."));

	globalwindowhandler::InstallControlLoop(AnimationController);
	SetIsRunning(true);
	iosystem::TextScreen(CONST_S("Generating game...\n\nThis may take some time, please wait."), WHITE, false, &BusyAnimation);
	NextCharacterID = 1;
	NextItemID = 1;
	InitScript();
	CreateTeams();
	CreateGods();
	SetPlayer(new playerkind);
	Player->SetAssignedName(PlayerName);
	Player->SetTeam(GetTeam(0));
	Player->SetNP(SATIATED_LEVEL);
	GetTeam(0)->SetLeader(Player);
	InitDangerMap();
	Petrus = 0;
	InitDungeons();
	SetCurrentArea(WorldMap = new worldmap(128, 128));
	CurrentWSquareMap = WorldMap->GetMap();
	WorldMap->Generate();
	InWilderness = true;
	UpdateCamera();
	SendLOSUpdateRequest();
	Tick = 0;
	Turn = 0;
	InitPlayerAttributeAverage();
	StoryState = 0;
	PlayerMassacreMap.clear();
	PetMassacreMap.clear();
	MiscMassacreMap.clear();
	PlayerMassacreAmount = PetMassacreAmount = MiscMassacreAmount = 0;
	DefaultPolymorphTo.Empty();
	DefaultSummonMonster.Empty();
	DefaultWish.Empty();
	DefaultChangeMaterial.Empty();
	DefaultDetectMaterial.Empty();
	Player->GetStack()->AddItem(new encryptedscroll);
	character* Doggie = new dog;
	Doggie->SetTeam(GetTeam(0));
	GetWorldMap()->GetPlayerGroup().push_back(Doggie);
	Doggie->SetAssignedName(ivanconfig::GetDefaultPetName());
	ADD_MESSAGE("Game generated successfully.");
	WizardMode = false;
	SeeWholeMapCheatMode = MAP_HIDDEN;
	GoThroughWallsCheat = false;
	SumoWrestling = false;
	GlobalRainTimeModifier = RAND() & 1023;
	PlayerSumoChampion = false;

	if(game::IsXMas())
	  {
	    item* Present = new banana;
	    game::GetPlayer()->GetStack()->AddItem(Present);
	    ADD_MESSAGE("Atavus is happy today! He gives you %s.", Present->CHAR_NAME(INDEFINITE));
	  }

	return true;
      }
    default:
      return false;
    }
}

void game::DeInit()
{
  /*_CrtMemState sta;
  _CrtMemCheckpoint(&sta);

  InitDungeons();

  for(int c = 1; c < Dungeons; ++c)
    delete Dungeon[c];

  delete [] Dungeon;
  _CrtMemDumpAllObjectsSince(&sta);*/


  delete WorldMap;
  WorldMap = 0;
  int c;

  for(c = 1; c < Dungeons; ++c)
    delete Dungeon[c];

  delete [] Dungeon;

  for(c = 1; c <= GODS; ++c)
    delete God[c]; // sorry, Valpuri!

  delete [] God;
  pool::BurnHell();

  for(c = 0; c < Teams; ++c)
    delete Team[c];

  delete [] Team;
  delete GameScript;
  msgsystem::Format();
  DangerMap.clear();
}

void game::Run()
{
  for(;;)
    {
      if(!InWilderness)
	{
	  /* Temporary places */
	  static int Counter = 0;

	  if(++Counter == 10)
	    {
	      CurrentLevel->GenerateMonsters();
	      Counter = 0;
	    }

	  if(!(game::GetTick() % 1000))
	    CurrentLevel->CheckSunLight();

	  if((CurrentDungeonIndex == NEW_ATTNAM
	   || CurrentDungeonIndex == ATTNAM)
	  && CurrentLevelIndex == 0)
	    {
	      long OldVolume = GlobalRainLiquid->GetVolume();
	      long NewVolume = Max(long(sin((Tick + GlobalRainTimeModifier) * 0.0003) * 300 - 150), 0L);

	      if(NewVolume && !OldVolume)
		CurrentLevel->EnableGlobalRain();
	      else if(!NewVolume && OldVolume)
		CurrentLevel->DisableGlobalRain();

	      GlobalRainLiquid->SetVolumeNoSignals(NewVolume);
	    }
	}

      try
	{
	  pool::Be();
	  pool::BurnHell();
	  IncreaseTick();
	  ApplyDivineTick();
	}
      catch(quitrequest)
	{
	  break;
	}
      catch(areachangerequest)
	{
	}
    }
}

void game::InitLuxTable()
{
  if(!LuxTable)
    {
      Alloc3D(LuxTable, 256, 33, 33);

      for(int c = 0; c < 0x100; ++c)
	for(int x = 0; x < 33; ++x)
	  for(int y = 0; y < 33; ++y)
	    {
	      int X = x - 16, Y = y - 16;
	      LuxTable[c][x][y] = int(c / (double(X * X + Y * Y) / 128 + 1));
	    }

      atexit(DeInitLuxTable);
    }
}

void game::DeInitLuxTable()
{
  delete [] LuxTable;
  LuxTable = 0;
}

void game::UpdateCameraX()
{
  if(GetCurrentArea()->GetXSize() <= GetScreenXSize() || Player->GetPos().X < GetScreenXSize() >> 1)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else if(Player->GetPos().X > GetCurrentArea()->GetXSize() - (GetScreenXSize() >> 1))
    if(Camera.X == GetCurrentArea()->GetXSize() - GetScreenXSize())
      return;
    else
      Camera.X = GetCurrentArea()->GetXSize() - GetScreenXSize();
  else
    if(Camera.X == Player->GetPos().X - (GetScreenXSize() >> 1))
      return;
    else
      Camera.X = Player->GetPos().X - (GetScreenXSize() >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraY()
{
  if(GetCurrentArea()->GetYSize() <= GetScreenYSize() || Player->GetPos().Y < GetScreenYSize() >> 1)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else if(Player->GetPos().Y > GetCurrentArea()->GetYSize() - (GetScreenYSize() >> 1))
    if(Camera.Y == GetCurrentArea()->GetYSize() - GetScreenYSize())
      return;
    else
      Camera.Y = GetCurrentArea()->GetYSize() - GetScreenYSize();
  else
    if(Camera.Y == Player->GetPos().Y - (GetScreenYSize() >> 1))
      return;
    else
      Camera.Y = Player->GetPos().Y - (GetScreenYSize() >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

const char* game::Insult() // convert to array
{
  switch(RAND_N(18))
    {
    case 0  : return "moron";
    case 1  : return "silly";
    case 2  : return "idiot";
    case 3  : return "airhead";
    case 4  : return "jerk";
    case 5  : return "dork";
    case 6  : return "Mr. Mole";
    case 7  : return "navastater";
    case 8  : return "potatoes-for-eyes";
    case 9  : return "lamer";
    case 10 : return "mommo-for-brains";
    case 11 : return "pinhead";
    case 12 : return "stupid-headed person";
    case 13 : return "software abuser";
    case 14 : return "loser";
    case 15 : return "peaballs";
    case 16 : return "person-with-problems";
    case 17 : return "unimportant user";
    default : return "hugger-mugger";
    }
}

/* DefaultAnswer = REQUIRES_ANSWER the question requires an answer */

bool game::BoolQuestion(const festring& String, int DefaultAnswer, int OtherKeyForTrue)
{
  if(DefaultAnswer == NO)
    DefaultAnswer = 'n';
  else if(DefaultAnswer == YES)
    DefaultAnswer = 'y';
  else if(DefaultAnswer != REQUIRES_ANSWER)
    ABORT("Illegal BoolQuestion DefaultAnswer send!");
  
  int FromKeyQuestion = KeyQuestion(String, DefaultAnswer, 5, 'y', 'Y', 'n', 'N', OtherKeyForTrue);
  return FromKeyQuestion == 'y' || FromKeyQuestion == 'Y' || FromKeyQuestion == OtherKeyForTrue;
}

void game::DrawEverything()
{
  DrawEverythingNoBlit();
  graphics::BlitDBToScreen();
}

bool game::OnScreen(vector2d Pos)
{
  return Pos.X >= 0 && Pos.Y >= 0 && Pos.X >= Camera.X && Pos.Y >= Camera.Y && Pos.X < GetCamera().X + GetScreenXSize() && Pos.Y < GetCamera().Y + GetScreenYSize();
}

void game::DrawEverythingNoBlit(bool AnimationDraw)
{
  if(LOSUpdateRequested)
    if(!IsInWilderness())
      GetCurrentLevel()->UpdateLOS();
    else
      GetWorldMap()->UpdateLOS();

  if(OnScreen(CursorPos))
    if(!IsInWilderness() || CurrentWSquareMap[CursorPos.X][CursorPos.Y]->GetLastSeen() || GetSeeWholeMapCheatMode())
      CurrentArea->GetSquare(CursorPos)->SendStrongNewDrawRequest();
    else
      DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), 16, 16, 0);

  globalwindowhandler::UpdateTick();
  GetCurrentArea()->Draw(AnimationDraw);
  GetPlayer()->DrawPanel(AnimationDraw);

  if(!AnimationDraw)
    msgsystem::Draw();

  if(OnScreen(CursorPos))
    {
      if(!IsInWilderness() && !GetSeeWholeMapCheatMode())
	{
	  lsquare* Square = CurrentLSquareMap[CursorPos.X][CursorPos.Y];

	  if(Square->GetLastSeen() != game::GetLOSTick())
	    Square->DrawMemorized();
	}

      vector2d ScreenCordinates = CalculateScreenCoordinates(CursorPos);

      if(DoZoom())
	DOUBLE_BUFFER->StretchBlit(DOUBLE_BUFFER, ScreenCordinates, RES_X - 96, RES_Y - 96, 16, 16, 5);

      igraph::DrawCursor(ScreenCordinates);
    }

  if(OnScreen(GetPlayer()->GetPos()))
    igraph::DrawCursor(CalculateScreenCoordinates(GetPlayer()->GetPos()));
}

bool game::Save(const festring& SaveName)
{
  outputfile SaveFile(SaveName + ".sav");
  SaveFile << int(SAVE_FILE_VERSION);
  SaveFile << GameScript << CurrentDungeonIndex << CurrentLevelIndex << Camera;
  SaveFile << WizardMode << SeeWholeMapCheatMode << GoThroughWallsCheat;
  SaveFile << Tick << Turn << InWilderness << NextCharacterID << NextItemID;
  SaveFile << SumoWrestling << PlayerSumoChampion << GlobalRainTimeModifier;
  long Seed = RAND();
  femath::SetSeed(Seed);
  SaveFile << Seed;
  SaveFile << AveragePlayerArmStrength << AveragePlayerLegStrength << AveragePlayerDexterity << AveragePlayerAgility;
  SaveFile << Teams << Dungeons << StoryState;
  SaveFile << PlayerMassacreMap << PetMassacreMap << MiscMassacreMap;
  SaveFile << PlayerMassacreAmount << PetMassacreAmount << MiscMassacreAmount;
  int c;

  for(c = 1; c < Dungeons; ++c)
    SaveFile << Dungeon[c];

  for(c = 1; c <= GODS; ++c)
    SaveFile << God[c];

  for(c = 0; c < Teams; ++c)
    SaveFile << Team[c];

  if(InWilderness)
    SaveWorldMap(SaveName, false);
  else
    GetCurrentDungeon()->SaveLevel(SaveName, CurrentLevelIndex, false);

  SaveFile << PLAYER->GetPos();
  msgsystem::Save(SaveFile);
  SaveFile << DangerMap << NextDangerIDType << NextDangerIDConfigIndex;
  SaveFile << DefaultPolymorphTo << DefaultSummonMonster;
  SaveFile << DefaultWish << DefaultChangeMaterial << DefaultDetectMaterial;
  return true;
}

int game::Load(const festring& SaveName)
{
  inputfile SaveFile(SaveName + ".sav", 0, false);

  if(!SaveFile.IsOpen())
    return NEW_GAME;

  int Version;
  SaveFile >> Version;

  if(Version != SAVE_FILE_VERSION)
    {
      if(!iosystem::Menu(0, vector2d(RES_X >> 1, RES_Y >> 1), CONST_S("Sorry, this save is incompatible with the new version.\rStart new game?\r"), CONST_S("Yes\rNo\r"), LIGHT_GRAY))
	return NEW_GAME;
      else
	return BACK;
    }

  SaveFile >> GameScript >> CurrentDungeonIndex >> CurrentLevelIndex >> Camera;
  SaveFile >> WizardMode >> SeeWholeMapCheatMode >> GoThroughWallsCheat;
  SaveFile >> Tick >> Turn >> InWilderness >> NextCharacterID >> NextItemID;
  SaveFile >> SumoWrestling >> PlayerSumoChampion >> GlobalRainTimeModifier;
  femath::SetSeed(ReadType<long>(SaveFile));
  SaveFile >> AveragePlayerArmStrength >> AveragePlayerLegStrength >> AveragePlayerDexterity >> AveragePlayerAgility;
  SaveFile >> Teams >> Dungeons >> StoryState;
  SaveFile >> PlayerMassacreMap >> PetMassacreMap >> MiscMassacreMap;
  SaveFile >> PlayerMassacreAmount >> PetMassacreAmount >> MiscMassacreAmount;
  int c;

  Dungeon = new dungeon*[Dungeons];
  Dungeon[0] = 0;

  for(c = 1; c < Dungeons; ++c)
    SaveFile >> Dungeon[c];

  God = new god*[GODS + 1];
  God[0] = 0;

  for(c = 1; c <= GODS; ++c)
    SaveFile >> God[c];

  Team = new team*[Teams];

  for(c = 0; c < Teams; ++c)
    SaveFile >> Team[c];

  if(InWilderness)
    {
      SetCurrentArea(LoadWorldMap(SaveName));
      CurrentWSquareMap = WorldMap->GetMap();
    }
  else
    {
      SetCurrentArea(CurrentLevel = GetCurrentDungeon()->LoadLevel(SaveName, CurrentLevelIndex));
      CurrentLSquareMap = CurrentLevel->GetMap();
    }

  vector2d Pos;
  SaveFile >> Pos;
  SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());
  msgsystem::Load(SaveFile);
  SaveFile >> DangerMap >> NextDangerIDType >> NextDangerIDConfigIndex;
  SaveFile >> DefaultPolymorphTo >> DefaultSummonMonster;
  SaveFile >> DefaultWish >> DefaultChangeMaterial >> DefaultDetectMaterial;
  return LOADED;
}

festring game::SaveName(const festring& Base)
{
  festring SaveName = GetSaveDir();

  if(!Base.GetSize())
    SaveName << PlayerName;
  else
    SaveName << Base;

  for(festring::sizetype c = 0; c < SaveName.GetSize(); ++c)
    if(SaveName[c] == ' ')
      SaveName[c] = '_';

#if defined(WIN32) || defined(__DJGPP__)
  if(SaveName.GetSize() > 13)
    SaveName.Resize(13);
#endif

  return SaveName;
}

void game::UpdateCameraXWithPos(int Coord)
{
  if(GetCurrentArea()->GetXSize() <= GetScreenXSize() || Coord < GetScreenXSize() >> 1)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else if(Coord > GetCurrentArea()->GetXSize() - (GetScreenXSize() >> 1))
    if(Camera.X == GetCurrentArea()->GetXSize() - GetScreenXSize())
      return;
    else
      Camera.X = GetCurrentArea()->GetXSize() - GetScreenXSize();
  else
    if(Camera.X == Coord - (GetScreenXSize() >> 1))
      return;
    else
      Camera.X = Coord - (GetScreenXSize() >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraYWithPos(int Coord)
{
  if(GetCurrentArea()->GetYSize() <= GetScreenYSize() || Coord < GetScreenYSize() >> 1)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else if(Coord > GetCurrentArea()->GetYSize() - (GetScreenYSize() >> 1))
    if(Camera.Y == GetCurrentArea()->GetYSize() - GetScreenYSize())
      return;
    else
      Camera.Y = GetCurrentArea()->GetYSize() - GetScreenYSize();
  else
    if(Camera.Y == Coord - (GetScreenYSize() >> 1))
      return;
    else
      Camera.Y = Coord - (GetScreenYSize() >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

int game::GetMoveCommandKeyBetweenPoints(vector2d A, vector2d B)
{
  for(int c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if((A + GetMoveVector(c)) == B)
      return GetMoveCommandKey(c);

  return DIR_ERROR;
}

void game::ApplyDivineTick()
{
  for(int c = 1; c <= GODS; ++c)
    GetGod(c)->ApplyDivineTick();
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, int Multiplier, bool Good)
{
  for(int c = 1; c <= GODS; ++c)
    if(GetGod(c) != CompareTarget)
      GetGod(c)->AdjustRelation(CompareTarget, Multiplier, Good);
}

vector2d game::GetDirectionVectorForKey(int Key)
{
  if(Key == KEY_NUMPAD_5)
    return vector2d(0,0);

  for(int c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if(Key == GetMoveCommandKey(c))
      return GetMoveVector(c);

  return ERROR_VECTOR;
}

double game::GetMinDifficulty()
{
  double Base = double(CurrentLevel->GetDifficulty()) / 5000;
  long MultiplierExponent = 0;

  for(;;)
    {
      int Dice = RAND() % 25;

      if(Dice < 5 && MultiplierExponent > -3)
	{
	  Base /= 3;
	  --MultiplierExponent;
	  continue;
	}

      if(Dice >= 20 && MultiplierExponent < 3)
	{
	  Base *= 3;
	  ++MultiplierExponent;
	  continue;
	}

      return Base;
    }
}

void game::ShowLevelMessage()
{
  if(CurrentLevel->GetLevelMessage().GetSize())
    ADD_MESSAGE(CurrentLevel->GetLevelMessage().CStr());

  CurrentLevel->SetLevelMessage("");
}

int game::DirectionQuestion(const festring& Topic, bool RequireAnswer, bool AcceptYourself)
{
  for(;;)
    {
      int Key = AskForKeyPress(Topic);

      if(AcceptYourself && Key == '.')
	return YOURSELF;

      for(int c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == GetMoveCommandKey(c))
	  return c;

      if(!RequireAnswer)
	return DIR_ERROR;
    }
}

void game::RemoveSaves(bool RealSavesAlso)
{
  if(RealSavesAlso)
    {
      remove(festring(SaveName() + ".sav").CStr());
      remove(festring(SaveName() + ".wm").CStr());
    }

  remove(festring(AutoSaveFileName + ".sav").CStr());
  remove(festring(AutoSaveFileName + ".wm").CStr());
  festring File;

  for(int i = 1; i < Dungeons; ++i)
    for(int c = 0; c < GetDungeon(i)->GetLevels(); ++c)
      {
	/* This looks very odd. And it is very odd.
	 * Indeed, gcc is very odd to not compile this correctly with -O3
	 * if it is written in a less odd way. */

	File = SaveName() + '.' + i;
	File << c;

	if(RealSavesAlso)
	  remove(File.CStr());

	File = AutoSaveFileName + '.' + i;
	File << c;

	remove(File.CStr());
      }
}

void game::SetPlayer(character* NP)
{
  Player = NP;

  if(Player)
    Player->SetIsPlayer(true);
}

void game::InitDungeons()
{
  Dungeons = *GetGameScript()->GetDungeons() + 1;
  Dungeon = new dungeon*[Dungeons];
  Dungeon[0] = 0;

  for(int c = 1; c < Dungeons; ++c)
    {
      Dungeon[c] = new dungeon(c);
      Dungeon[c]->SetIndex(c);
    }
}

void game::DoEvilDeed(int Amount)
{
  if(!Amount)
    return;

  for(int c = 1; c <= GODS; ++c)
    {
      int Change = Amount - Amount * GetGod(c)->GetAlignment() / 5;

      if(!IsInWilderness() && GetPlayer()->GetLSquareUnder()->GetDivineMaster() == c)
	if(GetGod(c)->GetRelation() - (Change << 1) < -750)
	  {
	    if(GetGod(c)->GetRelation() > -750)
	      GetGod(c)->SetRelation(-750);
	  }
	else if(GetGod(c)->GetRelation() - (Change << 1) > 750)
	  {
	    if(GetGod(c)->GetRelation() < 750)
	      GetGod(c)->SetRelation(750);
	  }
	else
	  GetGod(c)->SetRelation(GetGod(c)->GetRelation() - Change << 1);
      else
	if(GetGod(c)->GetRelation() - Change < -500)
	  {
	    if(GetGod(c)->GetRelation() > -500)
	      GetGod(c)->SetRelation(-500);
	  }
	else if(GetGod(c)->GetRelation() - Change > 500)
	  {
	    if(GetGod(c)->GetRelation() < 500)
	      GetGod(c)->SetRelation(500);
	  }
	else
	  GetGod(c)->SetRelation(GetGod(c)->GetRelation() - Change);
    }
}

void game::SaveWorldMap(const festring& SaveName, bool DeleteAfterwards)
{
  outputfile SaveFile(SaveName + ".wm");
  SaveFile << WorldMap;

  if(DeleteAfterwards)
    {
      delete WorldMap;
      WorldMap = 0;
    }
}

worldmap* game::LoadWorldMap(const festring& SaveName)
{
  inputfile SaveFile(SaveName + ".wm");
  SaveFile >> WorldMap;
  return WorldMap;
}

void game::Hostility(team* Attacker, team* Defender)
{
  for(int c = 0; c < Teams; ++c)
    if(GetTeam(c) != Attacker && GetTeam(c) != Defender && c != NEW_ATTNAM_TEAM) // gum solution
      switch(GetTeam(c)->GetRelation(Defender))
	{
	case HOSTILE:
	  {
	    if(GetTeam(c)->GetRelation(Attacker) == UNCARING)
	      GetTeam(c)->SetRelation(Attacker, FRIEND);

	    break;
	  }
	case UNCARING:
	  {
	    if(GetTeam(c)->GetRelation(Attacker) == HOSTILE)
	      GetTeam(c)->SetRelation(Defender, FRIEND);

	    break;
	  }
	case FRIEND:
	  {
	    GetTeam(c)->SetRelation(Attacker, HOSTILE);
	    break;
	  }
	default:
	  ABORT("Enemy unknown!");
	}
}

void game::CreateTeams()
{
  Teams = *GetGameScript()->GetTeams();
  Team = new team*[Teams];
  int c;

  for(c = 0; c < Teams; ++c)
    {
      Team[c] = new team(c);

      for(int i = 0; i < c; ++i)
	Team[i]->SetRelation(Team[c], UNCARING);
    }

  for(c = 0; c < Teams; ++c)
    if(c != 1)
      Team[1]->SetRelation(Team[c], HOSTILE);

  const std::list<std::pair<int, teamscript> >& TeamScript = GetGameScript()->GetTeam();

  for(std::list<std::pair<int, teamscript> >::const_iterator i = TeamScript.begin(); i != TeamScript.end(); ++i)
    {
      for(uint c = 0; c < i->second.GetRelation().size(); ++c)
	GetTeam(i->second.GetRelation()[c].first)->SetRelation(GetTeam(i->first), i->second.GetRelation()[c].second);

      const int* KillEvilness = i->second.GetKillEvilness();

      if(KillEvilness)
	GetTeam(i->first)->SetKillEvilness(*KillEvilness);
    }
}

/* vector2d Pos should be removed from xxxQuestion()s? */

festring game::StringQuestion(const festring& Topic, vector2d Pos, color16 Color, festring::sizetype MinLetters, festring::sizetype MaxLetters, bool AllowExit)
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0); // pos may be incorrect!
  festring Return;
  iosystem::StringQuestion(Return, Topic, Pos, Color, MinLetters, MaxLetters, false, AllowExit);
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

long game::NumberQuestion(const festring& Topic, vector2d Pos, color16 Color)
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  long Return = iosystem::NumberQuestion(Topic, Pos, Color, false);
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

long game::ScrollBarQuestion(const festring& Topic, vector2d Pos, long BeginValue, long Step, long Min, long Max, long AbortValue, color16 TopicColor, color16 Color1, color16 Color2, void (*Handler)(long))
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  long Return = iosystem::ScrollBarQuestion(Topic, Pos, BeginValue, Step, Min, Max, AbortValue, TopicColor, Color1, Color2, GetMoveCommandKey(KEY_LEFT_INDEX), GetMoveCommandKey(KEY_RIGHT_INDEX), false, Handler);
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

ulong game::IncreaseLOSTick()
{
  if(LOSTick != 0xFE)
    return LOSTick += 2;
  else
    {
      CurrentLevel->InitLastSeen();
      return LOSTick = 4;
    }
}

void game::UpdateCamera()
{
  if(GetCurrentArea()->GetXSize() <= GetScreenXSize() || Player->GetPos().X < GetScreenXSize() >> 1)
    Camera.X = 0;
  else if(Player->GetPos().X > GetCurrentArea()->GetXSize() - (GetScreenXSize() >> 1))
    Camera.X = GetCurrentArea()->GetXSize() - GetScreenXSize();
  else
    Camera.X = Player->GetPos().X - (GetScreenXSize() >> 1);

  if(GetCurrentArea()->GetYSize() <= GetScreenYSize() || Player->GetPos().Y < GetScreenYSize() >> 1)
    Camera.Y = 0;
  else if(Player->GetPos().Y > GetCurrentArea()->GetYSize() - (GetScreenYSize() >> 1))
    Camera.Y = GetCurrentArea()->GetYSize() - GetScreenYSize();
  else
    Camera.Y = Player->GetPos().Y - (GetScreenYSize() >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

bool game::HandleQuitMessage()
{
#ifdef USE_SDL

  if(IsRunning())
    {
      if(IsInGetCommand())
	{
	  switch(Menu(0, vector2d(RES_X >> 1, RES_Y >> 1), CONST_S("Do you want to save your game before quitting?\r"), CONST_S("Yes\rNo\rCancel\r"), LIGHT_GRAY))
	    {
	    case 0:
	      Save();
	      RemoveSaves(false);
	      break;
	    case 2:
	      GetCurrentArea()->SendNewDrawRequest();
	      DrawEverything();
	      return false;
	    default:
	      GetPlayer()->AddScoreEntry(CONST_S("cowardly quit the game"), 0.75);
	      End(true, false);
	      break;
	    }
	}
      else
	if(!Menu(0, vector2d(RES_X >> 1, RES_Y >> 1), CONST_S("You can't save at this point. Are you sure you still want to do this?\r"), CONST_S("Yes\rNo\r"), LIGHT_GRAY))
	  RemoveSaves();
	else
	  {
	    GetCurrentArea()->SendNewDrawRequest();
	    DrawEverything();
	    return false;
	  }
    }

#endif /* USE_SDL */

  return true;
}

int game::GetDirectionForVector(vector2d Vector)
{
  for(int c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
    if(Vector == GetMoveVector(c))
      return c;

  return DIR_ERROR;
}

const char* game::GetVerbalPlayerAlignment()
{
  long Sum = 0;

  for(int c = 1; c <= GODS; ++c)
    {
      if(GetGod(c)->GetRelation() > 0)
	Sum += GetGod(c)->GetRelation() * (5 - GetGod(c)->GetAlignment());
    }

  if(Sum > 15000)
    return "extremely lawful";
  if(Sum > 10000)
    return "very lawful";
  if(Sum > 5000)
    return "lawful";
  if(Sum > 1000)
    return "mildly lawful";
  if(Sum > -1000)
    return "neutral";
  if(Sum > -5000)
    return "mildly chaotic";
  if(Sum > -10000)
    return "chaotic";
  if(Sum > -15000)
    return "very chaotic";

  return "extremely chaotic";
}

void game::CreateGods()
{
  God = new god*[GODS + 1];
  God[0] = 0;

  for(int c = 1; c < protocontainer<god>::GetSize(); ++c)
    God[c] = protocontainer<god>::GetProto(c)->Clone();
}

void game::BusyAnimation()
{
  BusyAnimation(DOUBLE_BUFFER);
}

void game::BusyAnimation(bitmap* Buffer)
{
  static clock_t LastTime = 0;
  static int Frame = 0;
  static vector2d Pos((RES_X >> 1) - 100, (RES_Y << 1) / 3 - 100);

  if(clock() - LastTime > CLOCKS_PER_SEC / 25)
    {
      BusyAnimationCache[Frame]->NormalBlit(Buffer, 0, 0, Pos, 200, 200);

      if(Buffer == DOUBLE_BUFFER)
	graphics::BlitDBToScreen();

      if(++Frame == 32)
	Frame = 0;

      LastTime = clock();
    }
}

void game::CreateBusyAnimationCache()
{
  bitmap Elpuri(16, 16, TRANSPARENT_COLOR);
  Elpuri.ActivateFastFlag();
  packedcolor16 Color = MakeRGB16(60, 60, 60);
  igraph::GetCharacterRawGraphic()->MaskedBlit(&Elpuri, 64, 0, 0, 0, 16, 16, &Color);
  bitmap Circle(200, 200, TRANSPARENT_COLOR);
  Circle.ActivateFastFlag();

  for(int x = 0; x < 4; ++x)
    Circle.DrawPolygon(100, 100, 95 + x, 50, MakeRGB16(255 - 12 * x, 0, 0));

  for(int c = 0; c < 32; ++c)
    {
      bitmap* Bitmap = BusyAnimationCache[c] = new bitmap(200, 200, 0);
      Bitmap->ActivateFastFlag();
      Elpuri.NormalMaskedBlit(Bitmap, 0, 0, 92, 92, 16, 16);
      double Rotation = 0.3 + c * FPI / 80;

      for(int x = 0; x < 10; ++x)
	Bitmap->DrawPolygon(100, 100, 95, 5, MakeRGB16(5 + 25 * x, 0, 0), false, true, Rotation + double(x) / 50);

      Circle.NormalMaskedBlit(Bitmap);
    }
}

int game::AskForKeyPress(const festring& Topic)
{
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", Topic.CapitalizeCopy().CStr());
  graphics::BlitDBToScreen();
  int Key = GET_KEY();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Key;
}

/* Handler is called when the key has been identified as a movement key 
 * KeyHandler is called when the key has NOT been identified as a movement key
 * Both can be deactivated by passing 0 as parameter */  

vector2d game::PositionQuestion(const festring& Topic, vector2d CursorPos, void (*Handler)(vector2d), void (*KeyHandler)(vector2d, int), bool Zoom)
{
  int Key = 0;
  SetDoZoom(Zoom);
  vector2d Return;

  for(;;)
    {
      square* Square = GetCurrentArea()->GetSquare(CursorPos);

      if(!Square->HasBeenSeen() && (!Square->GetCharacter() || !Square->GetCharacter()->CanBeSeenByPlayer()) && !GetSeeWholeMapCheatMode())
	DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), 16, 16, BLACK);
      else
	GetCurrentArea()->GetSquare(CursorPos)->SendStrongNewDrawRequest();

      if(Key == ' ' || Key == '.')
	{
	  Return = CursorPos;
	  break;
	}

      if(Key == KEY_ESC)
	{
	  Return = vector2d(-1, -1);
	  break;
	}

      vector2d DirectionVector = GetDirectionVectorForKey(Key);

      if(DirectionVector != ERROR_VECTOR)
	{
	  CursorPos += DirectionVector;

	  if(CursorPos.X > GetCurrentArea()->GetXSize() - 1) CursorPos.X = 0;
	  if(CursorPos.X < 0) CursorPos.X = GetCurrentArea()->GetXSize() - 1;
	  if(CursorPos.Y > GetCurrentArea()->GetYSize() - 1) CursorPos.Y = 0;
	  if(CursorPos.Y < 0) CursorPos.Y = GetCurrentArea()->GetYSize() - 1;

	  if(Handler)
	    Handler(CursorPos);
	}
      else if(KeyHandler)
	KeyHandler(CursorPos, Key);

      if(CursorPos.X < GetCamera().X + 3 || CursorPos.X >= GetCamera().X + GetScreenXSize() - 3)
	UpdateCameraXWithPos(CursorPos.X);

      if(CursorPos.Y < GetCamera().Y + 3 || CursorPos.Y >= GetCamera().Y + GetScreenYSize() - 3)
	UpdateCameraYWithPos(CursorPos.Y);

      FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", Topic.CStr());
      SetCursorPos(CursorPos);
      DrawEverything();
      Key = GET_KEY();
    }

  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, BLACK);
  DOUBLE_BUFFER->Fill(RES_X - 96, RES_Y - 96, 80, 80, BLACK);
  SetDoZoom(false);
  SetCursorPos(vector2d(-1, -1));
  return Return;
}

void game::LookHandler(vector2d CursorPos)
{
  square* Square = GetCurrentArea()->GetSquare(CursorPos);
  festring OldMemory;

  if(GetSeeWholeMapCheatMode())
    {
      OldMemory = Square->GetMemorizedDescription();

      if(IsInWilderness())
	GetWorldMap()->GetWSquare(CursorPos)->UpdateMemorizedDescription(true);
      else
	GetCurrentLevel()->GetLSquare(CursorPos)->UpdateMemorizedDescription(true);
    }

  festring Msg;

  if(Square->HasBeenSeen() || GetSeeWholeMapCheatMode())
    {
      if(!IsInWilderness() && !Square->CanBeSeenByPlayer() && GetCurrentLevel()->GetLSquare(CursorPos)->CanBeFeltByPlayer())
	Msg = CONST_S("You feel here ");
      else if(Square->CanBeSeenByPlayer(true) || GetSeeWholeMapCheatMode())
	Msg = CONST_S("You see here ");
      else
	Msg = CONST_S("You remember here ");

      Msg << Square->GetMemorizedDescription() << '.';

      if(!IsInWilderness() && (Square->CanBeSeenByPlayer() || GetSeeWholeMapCheatMode()))
	{
	  lsquare* LSquare = GetCurrentLevel()->GetLSquare(CursorPos);

	  if(LSquare->FluidIsVisible())
	    LSquare->DisplayFluidInfo(Msg);

	  LSquare->DisplaySmokeInfo(Msg);

	  if(LSquare->HasEngravings() && LSquare->IsTransparent())
	    if(LSquare->EngravingsCanBeReadByPlayer() || GetSeeWholeMapCheatMode())
	      LSquare->DisplayEngravedInfo(Msg);
	    else
	      Msg << " Something has been engraved here.";
	}
    }
  else
    Msg = CONST_S("You have never been here.");

  character* Character = Square->GetCharacter();

  if(Character && (Character->CanBeSeenByPlayer() || GetSeeWholeMapCheatMode()))
    Character->DisplayInfo(Msg);

  if(!(RAND() % 10000) && (Square->CanBeSeenByPlayer() || GetSeeWholeMapCheatMode()))
    Msg << " You see here a frog eating a magnolia.";

  ADD_MESSAGE("%s", Msg.CStr());

  if(GetSeeWholeMapCheatMode())
    Square->SetMemorizedDescription(OldMemory);
}

bool game::AnimationController()
{
  game::DrawEverythingNoBlit(true);
  return true;
}

void game::InitGlobalValueMap()
{
  inputfile SaveFile(GetGameDir() + "Script/define.dat", &GlobalValueMap);
  festring Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word != "#" || SaveFile.ReadWord() != "define")
	ABORT("Illegal datafile define on line %d!", SaveFile.TellLine());

      SaveFile.ReadWord(Word);
      GlobalValueMap.insert(std::pair<festring, long>(Word, SaveFile.ReadNumber()));
    }
}

void game::TextScreen(const festring& Text, color16 Color, bool GKey, void (*BitmapEditor)(bitmap*))
{
  globalwindowhandler::DisableControlLoops();
  iosystem::TextScreen(Text, Color, GKey, BitmapEditor);
  globalwindowhandler::EnableControlLoops();
}

/* ... all the keys that are acceptable 
   DefaultAnswer = REQUIRES_ANSWER if this question requires an answer 
   Not surprisingly KeyNumber is the number of keys at ...
*/

int game::KeyQuestion(const festring& Message, int DefaultAnswer, int KeyNumber, ...)
{
  int* Key = new int[KeyNumber];
  va_list Arguments;
  va_start(Arguments, KeyNumber);

  for(int c = 0; c < KeyNumber; ++c)
    Key[c] = va_arg(Arguments, int);

  va_end(Arguments);
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", Message.CStr());
  graphics::BlitDBToScreen();
  int Return = 0;

  while(!Return)
    {
      int k = GET_KEY();

      for(int c = 0; c < KeyNumber; ++c)
	if(Key[c] == k)
	  {
	    Return = k;
	    break;
	  }

      if(!Return && DefaultAnswer != REQUIRES_ANSWER)
	Return = DefaultAnswer;
    }

  delete [] Key;
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

void game::LookKeyHandler(vector2d CursorPos, int Key)
{
  square* Square = GetCurrentArea()->GetSquare(CursorPos);

  switch(Key)
    {
    case 'i':
      if(!IsInWilderness())
	if(Square->CanBeSeenByPlayer() || CursorPos == GetPlayer()->GetPos() || GetSeeWholeMapCheatMode())
	  {
	    lsquare* LSquare = GetCurrentLevel()->GetLSquare(CursorPos);
	    stack* Stack = LSquare->GetStack();

	    if(LSquare->IsTransparent() && Stack->GetVisibleItems(PLAYER))
	      Stack->DrawContents(PLAYER, "Items here", NO_SELECT|(GetSeeWholeMapCheatMode() ? 0 : NO_SPECIAL_INFO));
	    else
	      ADD_MESSAGE("You see no items here.");
	  }
	else
	  ADD_MESSAGE("You should perhaps move a bit closer.");

      break;
    case 'c':
      if(Square->CanBeSeenByPlayer() || CursorPos == GetPlayer()->GetPos() || GetSeeWholeMapCheatMode())
	{
	  character* Char = Square->GetCharacter();

	  if(Char && (Char->CanBeSeenByPlayer() || Char->IsPlayer() || GetSeeWholeMapCheatMode()))
	    Char->PrintInfo();
	  else
	    ADD_MESSAGE("You see no one here.");
	}
      else
	ADD_MESSAGE("You should perhaps move a bit closer.");

      break;
    }
}

void game::NameKeyHandler(vector2d CursorPos, int Key)
{
  switch(Key)
    {
    case 'n':
      character* Character = GetCurrentArea()->GetSquare(CursorPos)->GetCharacter();

      if(Character && Character->CanBeSeenByPlayer())
	{
	  if(Character->GetTeam() != GetPlayer()->GetTeam())
	    ADD_MESSAGE("%s refuses to let YOU decide what %s's called.", Character->CHAR_NAME(DEFINITE), Character->GetPersonalPronoun().CStr());
	  else if(Character->IsPlayer())
	    ADD_MESSAGE("You can't rename yourself!");
	  else if(!Character->IsNameable())
	    ADD_MESSAGE("%s refuses to be called anything else but %s.", Character->CHAR_NAME(DEFINITE), Character->CHAR_NAME(DEFINITE));
	  else
	    {
	      festring Topic = CONST_S("What name will you give to ") + Character->GetName(DEFINITE) + '?';
	      festring Name = StringQuestion(Topic, vector2d(16, 6), WHITE, 0, 80, true);

	      if(Name.GetSize())
		Character->SetAssignedName(Name);
	    }
	}

      break;
    }
}

void game::End(bool Permanently, bool AndGoToMenu)
{
  globalwindowhandler::DeInstallControlLoop(AnimationController);
  SetIsRunning(false);

  if(Permanently || !WizardModeIsReallyActive())
    RemoveSaves(Permanently);

  if(Permanently && !WizardModeIsReallyActive())
    {
      highscore HScore;

      if(HScore.LastAddFailed())
	iosystem::TextScreen(CONST_S("You didn't manage to get onto the high score list."));
      else
	HScore.Draw();
    }

  if(AndGoToMenu)
    {
      /* This prevents monster movement etc. after death. */

      throw quitrequest();
    }
}

int game::CalculateRoughDirection(vector2d Vector)
{
  if(!Vector.X && !Vector.Y)
    return YOURSELF;

  double Angle = femath::CalculateAngle(Vector);

  if(Angle < FPI / 8)
    return 4;
  else if(Angle < 3 * FPI / 8)
    return 7;
  else if(Angle < 5 * FPI / 8)
    return 6;
  else if(Angle < 7 * FPI / 8)
    return 5;
  else if(Angle < 9 * FPI / 8)
    return 3;
  else if(Angle < 11 * FPI / 8)
    return 0;
  else if(Angle < 13 * FPI / 8)
    return 1;
  else if(Angle < 15 * FPI / 8)
    return 2;
  else
    return 4;
}

int game::Menu(bitmap* BackGround, vector2d Pos, const festring& Topic, const festring& sMS, color16 Color, const festring& SmallText1, const festring& SmallText2)
{
  globalwindowhandler::DisableControlLoops();
  int Return = iosystem::Menu(BackGround, Pos, Topic, sMS, Color, SmallText1, SmallText2);
  globalwindowhandler::EnableControlLoops();
  return Return;
}

void game::InitDangerMap()
{
  bool First = true;

  for(int c1 = 1; c1 < protocontainer<character>::GetSize(); ++c1)
    {
      BusyAnimation();
      const character::prototype* Proto = protocontainer<character>::GetProto(c1);
      const character::database*const* ConfigData = Proto->GetConfigData();
      int ConfigSize = Proto->GetConfigSize();

      for(int c2 = 0; c2 < ConfigSize; ++c2)
	if(!ConfigData[c2]->IsAbstract && ConfigData[c2]->CanBeGenerated)
	  {
	    int Config = ConfigData[c2]->Config;

	    if(First)
	      {
		NextDangerIDType = c1;
		NextDangerIDConfigIndex = c2;
		First = false;
	      }

	    character* Char = Proto->Clone(Config, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	    double NakedDanger = Char->GetRelativeDanger(Player, true);
	    delete Char;
	    Char = Proto->Clone(Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	    double EquippedDanger = Char->GetRelativeDanger(Player, true);
	    delete Char;
	    DangerMap[configid(c1, Config)] = dangerid(NakedDanger, EquippedDanger);
	  }
    }
}

void game::CalculateNextDanger()
{
  if(IsInWilderness() || !*CurrentLevel->GetLevelScript()->GenerateMonsters())
    return;

  const character::prototype* Proto = protocontainer<character>::GetProto(NextDangerIDType);
  const character::database*const* ConfigData = Proto->GetConfigData();
  const character::database* DataBase = ConfigData[NextDangerIDConfigIndex];

  //character::databasemap::const_iterator ConfigIterator = Config.find(NextDangerID.Config);
  dangermap::iterator DangerIterator = DangerMap.find(configid(NextDangerIDType, DataBase->Config));

  if(DataBase && DangerIterator != DangerMap.end())
    {
      character* Char = Proto->Clone(DataBase->Config, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
      double CurrentDanger = Char->GetRelativeDanger(Player, true);
      double NakedDanger = DangerIterator->second.NakedDanger;

      if(NakedDanger > CurrentDanger)
	DangerIterator->second.NakedDanger = (NakedDanger * 9 + CurrentDanger) / 10;

      delete Char;
      Char = Proto->Clone(DataBase->Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
      CurrentDanger = Char->GetRelativeDanger(Player, true);
      double EquippedDanger = DangerIterator->second.EquippedDanger;

      if(EquippedDanger > CurrentDanger)
	DangerIterator->second.EquippedDanger = (EquippedDanger * 9 + CurrentDanger) / 10;

      delete Char;
      int ConfigSize = Proto->GetConfigSize();

      for(int c = NextDangerIDConfigIndex + 1; c < ConfigSize; ++c)
	if(ConfigData[c]->CanBeGenerated)
	  {
	    NextDangerIDConfigIndex = c;
	    return;
	  }

      for(;;)
	{
	  if(++NextDangerIDType >= protocontainer<character>::GetSize())
	    NextDangerIDType = 1;

	  Proto = protocontainer<character>::GetProto(NextDangerIDType);
	  ConfigData = Proto->GetConfigData();
	  ConfigSize = Proto->GetConfigSize();

	  for(int c = 0; c < ConfigSize; ++c)
	    if(!ConfigData[c]->IsAbstract && ConfigData[c]->CanBeGenerated)
	      {
		NextDangerIDConfigIndex = c;
		return;
	      }
	}
    }
  else
    ABORT("It is dangerous to go ice fishing in the summer.");
}

bool game::TryTravel(int Dungeon, int Area, int EntryIndex, bool AllowHostiles)
{
  charactervector Group;

  if(LeaveArea(Group, AllowHostiles))
    {
      CurrentDungeonIndex = Dungeon;
      EnterArea(Group, Area, EntryIndex);
      return true;
    }
  else
    return false;
}

bool game::LeaveArea(charactervector& Group, bool AllowHostiles)
{
  if(!IsInWilderness())
    {
      if(!GetCurrentLevel()->CollectCreatures(Group, Player, AllowHostiles))
	return false;

      Player->Remove();
      GetCurrentDungeon()->SaveLevel(SaveName(), CurrentLevelIndex);
    }
  else
    {
      Player->Remove();
      GetWorldMap()->GetPlayerGroup().swap(Group);
      SaveWorldMap();
    }

  return true;
}

/* Used always when the player enters an area. */

void game::EnterArea(charactervector& Group, int Area, int EntryIndex)
{
  if(Area != WORLD_MAP)
    {
      SetIsInWilderness(false);
      CurrentLevelIndex = Area;
      bool New = !PrepareRandomBone(Area) && !GetCurrentDungeon()->PrepareLevel(Area);
      vector2d Pos = GetCurrentLevel()->GetEntryPos(Player, EntryIndex);

      if(Player)
	{
	  GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();
	  Player->PutToOrNear(Pos);
	}
      else
	SetPlayer(GetCurrentLevel()->GetLSquare(Pos)->GetCharacter());

      uint c;

      for(c = 0; c < Group.size(); ++c)
	{
	  vector2d NPCPos = GetCurrentLevel()->GetNearestFreeSquare(Group[c], Pos);

	  if(NPCPos == ERROR_VECTOR)
	    NPCPos = GetCurrentLevel()->GetRandomSquare(Group[c]);

	  Group[c]->PutTo(NPCPos);
	}

      GetCurrentLevel()->FiatLux();
      const bool* AutoReveal = GetCurrentLevel()->GetLevelScript()->AutoReveal();

      if(New && AutoReveal && *AutoReveal)
	GetCurrentLevel()->Reveal();

      ShowLevelMessage();
      SendLOSUpdateRequest();
      UpdateCamera();
      GetCurrentLevel()->UpdateLOS();
      Player->SignalStepFrom(0);

      for(c = 0; c < Group.size(); ++c)
	Group[c]->SignalStepFrom(0);

      /* Gum solution! */

      if(New && CurrentDungeonIndex == ATTNAM && Area == 0)
	{
	  GlobalRainLiquid = new powder(SNOW);
	  GlobalRainSpeed = vector2d(-64, 128);
	  CurrentLevel->CreateGlobalRain(GlobalRainLiquid, GlobalRainSpeed);
	}

      if(New && CurrentDungeonIndex == NEW_ATTNAM && Area == 0)
	{
	  GlobalRainLiquid = new liquid(WATER);
	  GlobalRainSpeed = vector2d(256, 512);
	  CurrentLevel->CreateGlobalRain(GlobalRainLiquid, GlobalRainSpeed);
	}

      if(ivanconfig::GetAutoSaveInterval())
	Save(GetAutoSaveFileName().CStr());
    }
  else
    {
      SetIsInWilderness(true);
      LoadWorldMap();
      SetCurrentArea(WorldMap);
      CurrentWSquareMap = WorldMap->GetMap();
      GetWorldMap()->GetPlayerGroup().swap(Group);
      Player->PutTo(GetWorldMap()->GetEntryPos(Player, EntryIndex));
      SendLOSUpdateRequest();
      UpdateCamera();
      GetWorldMap()->UpdateLOS();

      if(ivanconfig::GetAutoSaveInterval())
	Save(GetAutoSaveFileName().CStr());
    }
}

int game::CompareLightToInt(color24 L, color24 Int)
{
  if((L & 0xFF0000) > Int || (L & 0xFF00) > Int || (L & 0xFF) > Int)
    return 1;
  else if((L & 0xFF0000) == Int || (L & 0xFF00) == Int || (L & 0xFF) == Int)
    return 0;
  else
    return -1;
}

void game::SetStandardListAttributes(felist& List)
{
  List.SetPos(vector2d(26, 42));
  List.SetWidth(652);
  List.SetFlags(DRAW_BACKGROUND_AFTERWARDS);
  List.SetUpKey(game::GetMoveCommandKey(KEY_UP_INDEX));
  List.SetDownKey(game::GetMoveCommandKey(KEY_DOWN_INDEX));
}

void game::SignalGeneration(configid ConfigID)
{
  dangermap::iterator Iterator = DangerMap.find(ConfigID);

  if(Iterator != DangerMap.end())
    Iterator->second.HasBeenGenerated = true;
}

void game::InitPlayerAttributeAverage()
{
  AveragePlayerArmStrength = AveragePlayerLegStrength = AveragePlayerDexterity = AveragePlayerAgility = 0;

  if(!GetPlayer()->IsHumanoid())
    return;

  humanoid* Player = static_cast<humanoid*>(GetPlayer());
  int Arms = 0;
  int Legs = 0;
  rightarm* RightArm = Player->GetRightArm();

  if(RightArm && !RightArm->UseMaterialAttributes())
    {
      AveragePlayerArmStrength += RightArm->GetStrength();
      AveragePlayerDexterity += RightArm->GetDexterity();
      ++Arms;
    }

  leftarm* LeftArm = Player->GetLeftArm();

  if(LeftArm && !LeftArm->UseMaterialAttributes())
    {
      AveragePlayerArmStrength += LeftArm->GetStrength();
      AveragePlayerDexterity += LeftArm->GetDexterity();
      ++Arms;
    }

  rightleg* RightLeg = Player->GetRightLeg();

  if(RightLeg && !RightLeg->UseMaterialAttributes())
    {
      AveragePlayerLegStrength += RightLeg->GetStrength();
      AveragePlayerAgility += RightLeg->GetAgility();
      ++Legs;
    }

  leftleg* LeftLeg = Player->GetLeftLeg();

  if(LeftLeg && !LeftLeg->UseMaterialAttributes())
    {
      AveragePlayerLegStrength += LeftLeg->GetStrength();
      AveragePlayerAgility += LeftLeg->GetAgility();
      ++Legs;
    }

  if(Arms)
    {
      AveragePlayerArmStrength = AveragePlayerArmStrength / Arms;
      AveragePlayerDexterity = AveragePlayerDexterity / Arms;
    }

  if(Legs)
    {
      AveragePlayerLegStrength = AveragePlayerLegStrength / Legs;
      AveragePlayerAgility = AveragePlayerAgility / Legs;
    }
}

void game::UpdatePlayerAttributeAverage()
{
  if(!GetPlayer()->IsHumanoid())
    return;

  humanoid* Player = static_cast<humanoid*>(GetPlayer());
  double PlayerArmStrength = 0;
  double PlayerLegStrength = 0;
  double PlayerDexterity = 0;
  double PlayerAgility = 0;
  int Arms = 0;
  int Legs = 0;
  rightarm* RightArm = Player->GetRightArm();

  if(RightArm && !RightArm->UseMaterialAttributes())
    {
      PlayerArmStrength += RightArm->GetStrength();
      PlayerDexterity += RightArm->GetDexterity();
      ++Arms;
    }

  leftarm* LeftArm = Player->GetLeftArm();

  if(LeftArm && !LeftArm->UseMaterialAttributes())
    {
      PlayerArmStrength += LeftArm->GetStrength();
      PlayerDexterity += LeftArm->GetDexterity();
      ++Arms;
    }

  rightleg* RightLeg = Player->GetRightLeg();

  if(RightLeg && !RightLeg->UseMaterialAttributes())
    {
      PlayerLegStrength += RightLeg->GetStrength();
      PlayerAgility += RightLeg->GetAgility();
      ++Legs;
    }

  leftleg* LeftLeg = Player->GetLeftLeg();

  if(LeftLeg && !LeftLeg->UseMaterialAttributes())
    {
      PlayerLegStrength += LeftLeg->GetStrength();
      PlayerAgility += LeftLeg->GetAgility();
      ++Legs;
    }

  if(Arms)
    {
      AveragePlayerArmStrength = (49 * AveragePlayerArmStrength + PlayerArmStrength / Arms) / 50;
      AveragePlayerDexterity = (49 * AveragePlayerDexterity + PlayerDexterity / Arms) / 50;
    }

  if(Legs)
    {
      AveragePlayerLegStrength = (49 * AveragePlayerLegStrength + PlayerLegStrength / Legs) / 50;
      AveragePlayerAgility = (49 * AveragePlayerAgility + PlayerAgility / Legs) / 50;
    }
}

void game::CallForAttention(vector2d Pos, int RangeSquare)
{
  for(int c = 0; c < GetTeams(); ++c)
    {
      if(GetTeam(c)->HasEnemy())
	for(std::list<character*>::const_iterator i = GetTeam(c)->GetMember().begin(); i != GetTeam(c)->GetMember().end(); ++i)
	  if((*i)->IsEnabled())
	    {
	      long ThisDistance = HypotSquare(long((*i)->GetPos().X) - Pos.X, long((*i)->GetPos().Y) - Pos.Y);

	      if(ThisDistance <= RangeSquare && !(*i)->IsGoingSomeWhere())
		(*i)->SetGoingTo(Pos);
	    }
    }
}

outputfile& operator<<(outputfile& SaveFile, const homedata* HomeData)
{
  if(HomeData)
    {
      SaveFile.Put(1);
      SaveFile << HomeData->Pos << HomeData->Dungeon << HomeData->Level << HomeData->Room;
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, homedata*& HomeData)
{
  if(SaveFile.Get())
    {
      HomeData = new homedata;
      SaveFile >> HomeData->Pos >> HomeData->Dungeon >> HomeData->Level >> HomeData->Room;
    }

  return SaveFile;
}

ulong game::CreateNewCharacterID(character* NewChar)
{
  ulong ID = NextCharacterID++;

if(CharacterIDMap.find(ID) != CharacterIDMap.end())
  int esko = 2;

  CharacterIDMap.insert(std::pair<ulong, character*>(ID, NewChar));
  return ID;
}

ulong game::CreateNewItemID(item* NewItem)
{
  ulong ID = NextItemID++;

if(ItemIDMap.find(ID) != ItemIDMap.end())
  int esko = 2;

  if(NewItem)
    ItemIDMap.insert(std::pair<ulong, item*>(ID, NewItem));

  return ID;
}

character* game::SearchCharacter(ulong ID)
{
  characteridmap::iterator Iterator = CharacterIDMap.find(ID);
  return Iterator != CharacterIDMap.end() ? Iterator->second : 0;
}

item* game::SearchItem(ulong ID)
{
  itemidmap::iterator Iterator = ItemIDMap.find(ID);
  return Iterator != ItemIDMap.end() ? Iterator->second : 0;
}

outputfile& operator<<(outputfile& SaveFile, const configid& Value)
{
  SaveFile.Write(reinterpret_cast<const char*>(&Value), sizeof(Value));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, configid& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const dangerid& Value)
{
  SaveFile << Value.NakedDanger << Value.EquippedDanger << Value.HasBeenGenerated;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, dangerid& Value)
{
  SaveFile >> Value.NakedDanger >> Value.EquippedDanger >> Value.HasBeenGenerated;
  return SaveFile;
}

/* The program can only create directories to the deepness of one, no more... */

festring game::GetHomeDir()
{
#ifdef LINUX
  festring Dir;
  Dir << getenv("HOME") << '/';
  return Dir;
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

festring game::GetSaveDir()
{
#ifdef LINUX
  festring Dir;
  Dir << getenv("HOME") << "/IvanSave/";
  return Dir;
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Save/";
#endif
}

festring game::GetGameDir()
{
#ifdef LINUX
  return DATADIR "/ivan/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

festring game::GetBoneDir()
{
#ifdef LINUX
  return LOCAL_STATE_DIR "/Bones/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Bones/";
#endif
}

level* game::GetLevel(int I)
{
  return GetCurrentDungeon()->GetLevel(I);
}

int game::GetLevels()
{
  return GetCurrentDungeon()->GetLevels();
}

void game::SignalDeath(const character* Ghost, const character* Murderer)
{
  massacremap* MassacreMap;

  if(!Murderer)
    {
      ++MiscMassacreAmount;
      MassacreMap = &MiscMassacreMap;
    }
  else if(Murderer->IsPlayer())
    {
      ++PlayerMassacreAmount;
      MassacreMap = &PlayerMassacreMap;
    }
  else if(Murderer->IsPet())
    {
      ++PetMassacreAmount;
      MassacreMap = &PetMassacreMap;
    }
  else
    {
      ++MiscMassacreAmount;
      MassacreMap = &MiscMassacreMap;
    }

  configid CI(Ghost->GetType(), Ghost->GetConfig());
  massacremap::iterator i = MassacreMap->find(CI);

  if(i == MassacreMap->end())
    MassacreMap->insert(std::pair<configid, int>(CI, 1));
  else
    ++i->second;
}

void game::DisplayMassacreLists()
{
  DisplayMassacreList(PlayerMassacreMap, "directly by you.", PlayerMassacreAmount);
  DisplayMassacreList(PetMassacreMap, "by your allies.", PetMassacreAmount);
  DisplayMassacreList(MiscMassacreMap, "by some other reason.", MiscMassacreAmount);
}

struct massacresetentry
{
  bool operator<(const massacresetentry& MSE) const { return festring::IgnoreCaseCompare(Key, MSE.Key); }
  festring Key;
  festring String;
  int ImageKey;
};

void game::DisplayMassacreList(const massacremap& MassacreMap, const char* Reason, long Amount)
{
  std::set<massacresetentry> MassacreSet;
  festring FirstPronoun;
  bool First = true;
  charactervector GraveYard;

  for(massacremap::const_iterator i1 = MassacreMap.begin(); i1 != MassacreMap.end(); ++i1)
    {
      character* Victim = protocontainer<character>::GetProto(i1->first.Type)->Clone(i1->first.Config);
      massacresetentry Entry;
      GraveYard.push_back(Victim);
      Entry.ImageKey = AddToCharacterDrawVector(Victim);

      if(i1->second == 1)
	{
	  Victim->AddName(Entry.Key, UNARTICLED);
	  Victim->AddName(Entry.String, INDEFINITE);
	}
      else
	{
	  Victim->AddName(Entry.Key, PLURAL);
	  Entry.String << i1->second << ' ' << Entry.Key;
	}

      if(First)
	{
	  FirstPronoun = Victim->GetSex() == UNDEFINED ? "it" : Victim->GetSex() == MALE ? "he" : "she";
	  First = false;
	}

      MassacreSet.insert(Entry);
    }

  long Total = PlayerMassacreAmount + PetMassacreAmount + MiscMassacreAmount;
  festring MainTopic;

  if(Total == 1)
    MainTopic << "One creature perished during your adventure.";
  else
    MainTopic << Total << " creatures perished during your adventure.";

  felist List(MainTopic);
  SetStandardListAttributes(List);
  List.SetPageLength(15);
  List.SetEntryDrawer(CharacterEntryDrawer);
  List.AddDescription(CONST_S(""));
  festring SideTopic;

  if(Amount != Total)
    {
      SideTopic = CONST_S("The following ");

      if(Amount == 1)
	SideTopic << "one was killed " << Reason;
      else
	SideTopic << Amount << " were killed " << Reason;
    }
  else
    {
      if(Amount == 1)
	{
	  FirstPronoun.Capitalize();
	  SideTopic << FirstPronoun << " was killed " << Reason;
	}
      else
	SideTopic << "They were all killed " << Reason;
    }

  List.AddDescription(SideTopic);

  for(std::set<massacresetentry>::const_iterator i2 = MassacreSet.begin(); i2 != MassacreSet.end(); ++i2)
    List.AddEntry(i2->String, LIGHT_GRAY, 0, i2->ImageKey);

  List.Draw();
  ClearCharacterDrawVector();

  for(uint c = 0; c < GraveYard.size(); ++c)
    delete GraveYard[c];
}

bool game::MassacreListsEmpty()
{
  return PlayerMassacreMap.empty() && PetMassacreMap.empty() && MiscMassacreMap.empty();
}

#ifdef WIZARD

void game::SeeWholeMap()
{
  if(SeeWholeMapCheatMode < 2)
    ++SeeWholeMapCheatMode;
  else
    SeeWholeMapCheatMode = 0;
    
  GetCurrentArea()->SendNewDrawRequest();
}

#endif

void game::CreateBone()
{
  if(!WizardModeIsActive() && !IsInWilderness() && RAND() & 3 && GetCurrentLevel()->PreProcessForBone())
    {
      int BoneIndex;
      festring BoneName;

      for(BoneIndex = 0; BoneIndex < 1000; ++BoneIndex)
	{
	  BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + CurrentLevelIndex + BoneIndex;
	  inputfile BoneFile(BoneName, 0, false);

	  if(!BoneFile.IsOpen())
	    break;
	}

      if(BoneIndex != 1000)
	{
	  festring BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + CurrentLevelIndex + BoneIndex;
	  outputfile BoneFile(BoneName);
	  BoneFile << int(BONE_FILE_VERSION) << PlayerName << CurrentLevel;
	}
    }
}

bool game::PrepareRandomBone(int LevelIndex)
{
  if(WizardModeIsActive() || GetCurrentDungeon()->IsGenerated(LevelIndex) || !*GetCurrentDungeon()->GetLevelScript(LevelIndex)->CanGenerateBone())
    return false;

  int BoneIndex;
  festring BoneName;

  for(BoneIndex = 0; BoneIndex < 1000; ++BoneIndex)
    {
      BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + LevelIndex + BoneIndex;
      inputfile BoneFile(BoneName, 0, false);

      if(BoneFile.IsOpen() && !(RAND() & 7))
	{
	  if(ReadType<int>(BoneFile) != BONE_FILE_VERSION)
	    {
	      BoneFile.Close();
	      remove(BoneName.CStr());
	      continue;
	    }

	  festring Name;
	  BoneFile >> Name;
	  level* NewLevel = GetCurrentDungeon()->LoadLevel(BoneFile, LevelIndex);

	  if(!NewLevel->PostProcessForBone())
	    {
	      delete NewLevel;
	      GetBoneItemIDMap().clear();
	      GetBoneCharacterIDMap().clear();
	      continue;
	    }

	  NewLevel->FinalProcessForBone();
	  GetBoneItemIDMap().clear();
	  GetBoneCharacterIDMap().clear();
	  SetCurrentArea(NewLevel);
	  CurrentLevel = NewLevel;
	  CurrentLSquareMap = NewLevel->GetMap();
	  GetCurrentDungeon()->SetIsGenerated(LevelIndex, true);

	  if(Name == PlayerName)
	    ADD_MESSAGE("This place is oddly familiar. Like you had been here in one of your past lives.");
	  else
	    ADD_MESSAGE("You smell the stench of death.");

	  break;
	}
    }

  if(BoneIndex != 1000)
    {
      remove(BoneName.CStr());
      return true;
    }
  else
    return false;
}

double game::CalculateAverageDanger(const charactervector& EnemyVector, character* Char)
{
  double DangerSum = 0;
  int Enemies = 0;

  for(uint c = 0; c < EnemyVector.size(); ++c)
    {
      DangerSum += EnemyVector[c]->GetRelativeDanger(Char, true);
      ++Enemies;
    }

  return DangerSum / Enemies;
}

double game::CalculateAverageDangerOfAllNormalEnemies()
{
  double DangerSum = 0;
  int Enemies = 0;

  for(int c1 = 1; c1 < protocontainer<character>::GetSize(); ++c1)
    {
      const character::prototype* Proto = protocontainer<character>::GetProto(c1);
      const character::database*const* ConfigData = Proto->GetConfigData();
      int ConfigSize = Proto->GetConfigSize();

      for(int c2 = 0; c2 < ConfigSize; ++c2)
	if(!ConfigData[c2]->IsAbstract
	&& !ConfigData[c2]->IsUnique
	&& ConfigData[c2]->CanBeGenerated)
	  {
	    DangerSum += DangerMap.find(configid(c1, ConfigData[c2]->Config))->second.EquippedDanger;
	    ++Enemies;
	  }
    }

  return DangerSum / Enemies;
}

character* game::CreateGhost()
{
  double AverageDanger = game::CalculateAverageDangerOfAllNormalEnemies();
  charactervector EnemyVector;
  protosystem::CreateEveryNormalEnemy(EnemyVector);
  ghost* Ghost = new ghost;
  Ghost->SetTeam(GetTeam(MONSTER_TEAM));
  Ghost->SetOwnerSoul(PlayerName);
  Ghost->SetIsActive(false);
  Ghost->EditAllAttributes(-4);
  PLAYER->SetSoulID(Ghost->GetID());
  while(game::CalculateAverageDanger(EnemyVector, Ghost) > AverageDanger && Ghost->EditAllAttributes(1));

  for(uint c = 0; c < EnemyVector.size(); ++c)
    delete EnemyVector[c];

  return Ghost;
}

int game::GetMoveCommandKey(int I) 
{
  if(!ivanconfig::GetUseAlternativeKeys())
    return MoveNormalCommandKey[I];
  else
    return MoveAbnormalCommandKey[I];
}

long game::GetScore()
{
  double Counter = 0;
  massacremap::const_iterator i;
  massacremap SumMap = PlayerMassacreMap;

  for(i = PetMassacreMap.begin(); i != PetMassacreMap.end(); ++i)
    SumMap[i->first] += i->second;

  for(i = SumMap.begin(); i != SumMap.end(); ++i)
    {
      character* Char = protocontainer<character>::GetProto(i->first.Type)->Clone(i->first.Config);
      int SumOfAttributes = Char->GetSumOfAttributes();
      delete Char;
      Counter += sqrt(i->second) * SumOfAttributes * SumOfAttributes;
    }

  return long(0.01 * Counter);
}

/* Only works if New Attnam is loaded */

bool game::TweraifIsFree()
{
  for(std::list<character*>::const_iterator i = GetTeam(COLONIST_TEAM)->GetMember().begin(); i != GetTeam(COLONIST_TEAM)->GetMember().end(); ++i)
    if((*i)->IsEnabled())
      return false;

  return true;
}

bool game::IsXMas() // returns true if date is christmaseve or day
{
  time_t Time = time(0);
  struct tm* TM = localtime(&Time);
  return (TM->tm_mon == 11 && (TM->tm_mday == 24 || TM->tm_mday == 25));
}

int game::AddToItemDrawVector(item* What)
{
  ItemDrawVector.push_back(What);
  return ItemDrawVector.size() - 1;
}

void game::ItemEntryDrawer(bitmap* Bitmap, vector2d Pos, uint I)
{
  item* Item = ItemDrawVector[I];

  if(Item)
    Item->Draw(Bitmap, Pos, NORMAL_LUMINANCE, 0, true, Item->AllowAlphaEverywhere());
}

int game::AddToCharacterDrawVector(character* What)
{
  CharacterDrawVector.push_back(What);
  return CharacterDrawVector.size() - 1;
}

void game::CharacterEntryDrawer(bitmap* Bitmap, vector2d Pos, uint I)
{
  if(CharacterDrawVector[I])
    CharacterDrawVector[I]->DrawBodyParts(Bitmap, Pos, NORMAL_LUMINANCE, 0, true, false);
}

void game::GodEntryDrawer(bitmap* Bitmap, vector2d Pos, uint I)
{
  igraph::GetSymbolGraphic()->NormalMaskedBlit(Bitmap, I << 4, 0, Pos, 16, 16);
}

character* game::GetSumo()
{
  return GetCurrentLevel()->GetLSquare(SUMO_ROOM_POS)->GetRoom()->GetMaster();
}

bool game::TryToEnterSumoArena()
{
  character* Sumo = GetSumo();

  if(!Sumo || !Sumo->IsEnabled() || Sumo->GetRelation(PLAYER) == HOSTILE || !PLAYER->CanBeSeenBy(Sumo))
    return true;

  if(TweraifIsFree())
    {
      ADD_MESSAGE("\"You started this stupid revolution, after which I've been constantly hungry. Get lost!\"");
      return false;
    }

  if(PlayerIsSumoChampion())
    {
      ADD_MESSAGE("\"I don't really enjoy losing, especially many times to the same guy. Go away.\"");
      return false;
    }

  if(PLAYER->IsPolymorphed())
    {
      ADD_MESSAGE("\"Don't try to cheat. Come back when you're normal again.\"");
      return false;
    }

  if(PLAYER->GetHungerState() < SATIATED)
    {
      ADD_MESSAGE("\"Your figure is too slender for this sport. Eat a lot more and come back.\"");
      return false;
    }

  if(PLAYER->GetHungerState() < BLOATED)
    {
      ADD_MESSAGE("\"You're still somewhat too thin. Eat some more and we'll compete.\"");
      return false;
    }

  ADD_MESSAGE("\"So you want to compete? Okay, I'll explain the rules. First, I'll make a mirror image out of us both. We'll enter the arena and fight till one is knocked out. Use of any equipment is not allowed. Note that we will not gain experience from fighting as a mirror image, but won't get really hurt, either. However, controlling the image is exhausting and you can get hungry very quickly.\"");

  if(!BoolQuestion("Do you want to challenge him? [y/N]"))
    return false;

  SumoWrestling = true;
  character* MirrorPlayer = PLAYER->Duplicate();
  character* MirrorSumo = Sumo->Duplicate();
  SetPlayer(MirrorPlayer);
  charactervector Spectators;

  if(PLAYER->GetTeam()->GetRelation(GetTeam(TOURIST_GUIDE_TEAM)) != HOSTILE
  && PLAYER->GetTeam()->GetRelation(GetTeam(TOURIST_TEAM)) != HOSTILE)
    {
      GetTeam(TOURIST_GUIDE_TEAM)->MoveMembersTo(Spectators);
      GetTeam(TOURIST_TEAM)->MoveMembersTo(Spectators);
    }

  GetCurrentDungeon()->SaveLevel(SaveName(), 0);
  charactervector test;
  EnterArea(test, 1, STAIRS_UP);
  MirrorSumo->PutTo(SUMO_ARENA_POS + vector2d(6, 7));
  MirrorSumo->ChangeTeam(GetTeam(SUMO_TEAM));
  GetCurrentLevel()->GetLSquare(SUMO_ARENA_POS)->GetRoom()->SetMasterID(MirrorSumo->GetID());

  for(uint c = 0; c < Spectators.size(); ++c)
    Spectators[c]->PutToOrNear(SUMO_ARENA_POS + vector2d(6, 10));

  throw areachangerequest();
  return true;
}

bool game::TryToExitSumoArena()
{
  if(GetTeam(PLAYER_TEAM)->GetRelation(GetTeam(NEW_ATTNAM_TEAM)) == HOSTILE)
    return true;

  itemvector IVector;
  charactervector CVector;

  if(IsSumoWrestling())
    {
      if(BoolQuestion("Do you really wish to give up? [y/N]"))
	return EndSumoWrestling(LOST);
      else
	return false;
    }
  else
    {
      PLAYER->Remove();
      GetCurrentLevel()->CollectEverything(IVector, CVector);
      GetCurrentDungeon()->SaveLevel(SaveName(), 1);
      std::vector<character*> test;
      EnterArea(test, 0, STAIRS_DOWN);
      PLAYER->GetStackUnder()->AddItems(IVector);

      if(!IVector.empty())
	{
	  character* Sumo = GetSumo();

	  if(Sumo && Sumo->GetRelation(PLAYER) != HOSTILE && PLAYER->CanBeSeenBy(Sumo))
	    ADD_MESSAGE("\"Don't leave anything there, please.\"");
	}

      vector2d PlayerPos = PLAYER->GetPos();

      for(uint c = 0; c < CVector.size(); ++c)
	CVector[c]->PutNear(PlayerPos);

      throw areachangerequest();
      return true;
    }
}

bool game::EndSumoWrestling(int Result)
{
  msgsystem::LeaveBigMessageMode();

  if(Result == LOST)
    AskForKeyPress("You lose. [press any key to continue]");
  else if(Result == WON)
    AskForKeyPress("You win! [press any key to continue]");
  else if(Result == DISQUALIFIED)
    AskForKeyPress("You are disqualified! [press any key to continue]");

  character* Sumo = GetCurrentLevel()->GetLSquare(SUMO_ARENA_POS)->GetRoom()->GetMaster();

  /* We'll make a throw soon so deletes are allowed */

  if(Sumo)
    {
      Sumo->Remove();
      delete Sumo;
    }

  PLAYER->Remove();
  delete PLAYER;
  SetPlayer(0);
  itemvector IVector;
  charactervector CVector;
  GetCurrentLevel()->CollectEverything(IVector, CVector);
  GetCurrentDungeon()->SaveLevel(SaveName(), 1);
  charactervector test;
  EnterArea(test, 0, STAIRS_DOWN);
  SumoWrestling = false;
  PLAYER->GetStackUnder()->AddItems(IVector);
  vector2d PlayerPos = PLAYER->GetPos();

  for(uint c = 0; c < CVector.size(); ++c)
    CVector[c]->PutNear(PlayerPos);

  if(Result == LOST)
    ADD_MESSAGE("\"I hope you've learned your lesson now!\"");
  else if(Result == DISQUALIFIED)
    ADD_MESSAGE("\"Don't do that again or I'll be really angry!\"");
  else
    {
      PlayerSumoChampion = true;
      character* Sumo = GetSumo();
      item* Belt = Sumo && Sumo->IsHumanoid() ? static_cast<humanoid*>(Sumo)->GetBelt() : 0;
      festring Msg = Sumo->GetName(DEFINITE) + " seems humbler than before. \"Darn. You bested me.\n";

      if(Belt)
	{
	  Msg << "Here's a little something as a reward\", " << Sumo->GetPersonalPronoun() << " says and hands you " << Belt->GetName(INDEFINITE) << ".\n\"";
	  Belt->MoveTo(PLAYER->GetStack());
	}

      Msg << "Allow me to ";

      if(Belt)
	Msg << "also ";

      Msg << "teach you a few nasty martial art tricks the years have taught me.\"";
      PLAYER->GetCWeaponSkill(UNARMED)->AddHit(1000);
      PLAYER->GetCWeaponSkill(KICK)->AddHit(1000);
      character* Imperialist = GetCurrentLevel()->GetLSquare(5, 5)->GetRoom()->GetMaster();

      if(Imperialist && Imperialist->GetRelation(PLAYER) != HOSTILE)
	{
	  vector2d Pos = PLAYER->GetPos() + vector2d(0, 1);
	  GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();
	  Imperialist->Remove();
	  Imperialist->PutTo(Pos);
	  Msg << "\n\nSuddenly you notice " << Imperialist->GetName(DEFINITE) << " has also entered.\n"
		 "\"I see we have a promising fighter among us. I had already heard of your\n"
		 "adventures outside the village, but hardly could I believe that one day you\n"
		 "would defeat even the mighty Huang Ming Pong! A hero such as you is bound\n"
		 "to become world famous, and can earn a fortune if wealthy sponsors are behind\n"
		 "him. May I therefore propose a mutually profitable contract: I'll give you this\n"
		 "nice shirt with my company's ad, and you'll wear it as you journey bravely to\n"
		 "the unknown and fight epic battles against the limitless minions of evil. I'll\n"
		 "reward you well when you return, depending on how much you have used it.\"";
	  PLAYER->GetStack()->AddItem(new decosadshirt);
	}

      TextScreen(Msg);
      GetCurrentArea()->SendNewDrawRequest();
      DrawEverything();
    }

  PLAYER->EditNP(-25000);
  PLAYER->CheckStarvationDeath(CONST_S("exhausted after controlling a mirror image for too long"));
  throw areachangerequest();
  return true;
}

rain* game::ConstructGlobalRain()
{
  return new rain(GlobalRainLiquid, static_cast<lsquare*>(game::GetSquareInLoad()), GlobalRainSpeed, MONSTER_TEAM, false);
}

vector2d game::GetSunLightDirectionVector()
{
  int Index = Tick % 48000 / 1000;

  /* Should have the same sign as sin(PI * Index / 24) and XTable[Index] /
     YTable[Index] should equal roughly -tan(PI * Index / 24). Also, vector
     (XTable[Index], YTable[Index]) + P should not be a valid position of
     any possible level L for any P belonging to L. */

  static int XTable[48] = {     0,  1000,  1000,  1000,  1000,  1000,
			     1000,  1303,  1732,  2414,  3732,  7596,
			     1000,  7596,  3732,  2414,  1732,  1303,
			     1000,  1000,  1000,  1000,  1000,  1000,
			        0, -1000, -1000, -1000, -1000, -1000,
			    -1000, -1303, -1732, -2414, -3732, -7596,
			    -1000, -7596, -3732, -2414, -1732, -1303,
			    -1000, -1000, -1000, -1000, -1000, -1000 };

  /* Should have the same sign as -cos(PI * Index / 24) */

  static int YTable[48] = { -1000, -7596, -3732, -2414, -1732, -1303,
			    -1000, -1000, -1000, -1000, -1000, -1000,
			        0,  1000,  1000,  1000,  1000,  1000,
			     1000,  1303,  1732,  2414,  3732,  7596,
			     1000,  7596,  3732,  2414,  1732,  1303,
			     1000,  1000,  1000,  1000,  1000,  1000,
			        0, -1000, -1000, -1000, -1000, -1000,
			    -1000, -1303, -1732, -2414, -3732, -7596 };

  return vector2d(XTable[Index], YTable[Index]);
}

int game::CalculateMinimumEmitationRadius(color24 E)
{
  int MaxElement = Max(GetRed24(E), GetGreen24(E), GetBlue24(E));
  return int(sqrt(double(MaxElement << 7) / LIGHT_BORDER - 120.));
}

ulong game::IncreaseSquarePartEmitationTicks()
{
  if((SquarePartEmitationTick += 2) == 0x100)
    {
      CurrentLevel->InitSquarePartEmitationTicks();
      SquarePartEmitationTick = 2;
    }

  return SquarePartEmitationTick;
}

void game::Wish(character* Wisher, const char* MsgSingle, const char* MsgPair)
{
  for(;;)
    {
      festring Temp = game::DefaultQuestion(CONST_S("What do you want to wish for?"),
					    DefaultWish);
      item* TempItem = protosystem::CreateItem(Temp, Wisher->IsPlayer());

      if(TempItem)
	{
	  Wisher->GetStack()->AddItem(TempItem);
	  TempItem->SpecialGenerationHandler();

	  if(TempItem->HandleInPairs())
	    ADD_MESSAGE(MsgPair, TempItem->CHAR_NAME(PLURAL));
	  else
	    ADD_MESSAGE(MsgSingle, TempItem->CHAR_NAME(INDEFINITE));

	  return;
	}
    }
}

festring game::DefaultQuestion(festring Topic, festring& Default)
{
  if(!Default.IsEmpty())
    Topic << " [" << Default << ']';

  festring Answer = StringQuestion(Topic, vector2d(16, 6), WHITE, 0, 80, false);

  if(Answer.IsEmpty())
    Answer = Default;

  return Default = Answer;
}

void game::GetTime(ivantime& Time)
{
  Time.Hour = 12 + Tick / 2000;
  Time.Day = Time.Hour / 24 + 1;
  Time.Hour %= 24;
  Time.Min = Tick % 2000 * 60 / 2000;
}
