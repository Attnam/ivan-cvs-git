#include <cmath>
#include <ctime>
#include <algorithm>
#include <cstdarg>

#ifdef WIN32
#include <direct.h>	// Needed for _mkdir
#include <windows.h>
#endif

#ifdef USE_SDL
#include <stdlib.h>
#include <sys/stat.h>
#define IDNO 1
#define IDYES 0
#define IDCANCEL 2
#endif

#ifdef __DJGPP__
#include <sys/stat.h>
#endif

#include "level.h"
#include "charde.h"
#include "itemba.h"
#include "godba.h"
#include "stdover.h"
#include "whandler.h"
#include "lsquare.h"
#include "lterraba.h"
#include "worldmap.h"
#include "message.h"
#include "dungeon.h"
#include "feio.h"
#include "script.h"
#include "team.h"
#include "colorbit.h"
#include "config.h"
#include "femath.h"
#include "hscore.h"
#include "error.h"
#include "command.h"
#include "save.h"
#include "stack.h"

#define SAVEFILE_VERSION 110 // Increment this if changes make savefiles incompatible

#define LOADED 0
#define NEWGAME 1
#define BACK 2

class quitrequest { };

ushort game::Current;
long game::BaseScore;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
std::vector<dungeon*> game::Dungeon;
character* game::PlayerBackup;
uchar game::CurrentDungeon;
ulong game::NextItemID = 0;
std::vector<team*> game::Team;
ulong game::LOSTurns;
vector2d game::ScreenSize(42, 26);
bool game::AnimationControllerActive = true;
vector2d game::CursorPos;
bool game::Zoom;

bool game::Loading = false, game::InGetCommand = false;
petrus* game::Petrus = 0;

std::string game::AutoSaveFileName = SAVE_DIR + "AutoSave";
std::string game::Alignment[] = { "L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--" };
std::string game::LockDescription[] = { "round", "square", "triangular" };
std::vector<god*> game::God;

command* game::Command[] =
{
  0,
  new command(&character::Apply, "apply", 'a', false),
  new command(&character::Talk, "chat", 'C', false),
  new command(&character::Close, "close", 'c', false),
  new command(&character::DecreaseContrast, "decrease contrast", 'B', true),
  new command(&character::Dip, "dip", 'D', true),
  new command(&character::Drop, "drop", 'd', false),
  new command(&character::Consume, "eat/drink", 'e', true),
  new command(&character::WhatToEngrave, "engrave", 'E', false),
  new command(&character::EqupmentScreen, "equipment menu", 'I', true),
  new command(&character::GainDivineKnowledge, "gain knowledge of all gods cheat", '7', true, true),
  new command(&character::GainAllItems, "give all items cheat", '8', true, true),
  new command(&character::Go, "go", 'g', false),
  new command(&character::GoDown, "go down", '>', true),
  new command(&character::GoUp, "go up", '<', true),
  new command(&character::IncreaseContrast, "increase contrast", 'b', true),
  new command(&character::Kick, "kick", 'k', false),
  new command(&character::Look, "look", 'l', true),
  new command(&character::LowerStats, "lower stats cheat", '2', true, true),
  new command(&character::LowerGodRelations, "lower your relations to the gods cheat", '6', true, true),
  new command(&character::AssignName, "name", 'n', false),
  new command(&character::Offer, "offer", 'O', false),
  new command(&character::Open, "open", 'o', false),
  new command(&character::OutlineCharacters, "outline characters", 'K', true),
  new command(&character::OutlineItems, "outline items", 'J', true),
  new command(&character::PickUp, "pick up", ',', false),
  new command(&character::Pray, "pray", 'p', false),
  new command(&character::Quit, "quit", 'q', true),
  new command(&character::RaiseStats, "raise stats cheat", '1', true, true),
  new command(&character::RaiseGodRelations, "raise your relations to the gods cheat", '5', true, true),
  new command(&character::Read, "read", 'r', false),
  new command(&character::RestUntilHealed, "rest/heal", 'h', true),
  new command(&character::Save, "save game", 's', true),
  new command(&character::SecretKnowledge, "reveal secret knowledge", '9', true, true),
  new command(&character::ScrollMessagesDown, "scroll messages down", '+', true),
  new command(&character::ScrollMessagesUp, "scroll messages up", '-', true),
  new command(&character::SeeWholeMap, "see whole map cheat", '3', true, true),
  new command(&character::ShowConfigScreen, "show config screen", '\\', true),
  new command(&character::ShowInventory, "show inventory", 'i', true),
  new command(&character::ShowKeyLayout, "show key layout", '?', true),
  new command(&character::DrawMessageHistory, "show message history", 'M', true),
  new command(&character::ShowWeaponSkills, "show weapon skills", '@', true),
  new command(&character::Sit, "sit", 'S', false),
  new command(&character::Throw, "throw", 't', false),
  new command(&character::WalkThroughWalls, "toggle walk through walls cheat", '4', true, true),
  new command(&character::ForceVomit, "vomit", 'v', false),
  new command(&character::NOP, "wait", '.', true),
  new command(&character::WizardMode, "wizard mode activation", 'X', true),
  new command(&character::Zap, "zap", 'z', false),
  0
};

int game::MoveCommandKey[EXTENDED_DIRECTION_COMMAND_KEYS] = { 0x147, 0x148, 0x149, 0x14B, 0x14D, 0x14F, 0x150, 0x151, '.' };
const vector2d game::MoveVector[EXTENDED_DIRECTION_COMMAND_KEYS] = { vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1), vector2d(0, 0) };

bool game::LOSUpdateRequested = false;
ushort*** game::LuxTable;
ushort* game::LuxTableSize;
bool game::Running;
character* game::Player;
vector2d game::Camera(0,0);
bool game::WizardMode;
bool game::SeeWholeMapCheat;
bool game::GoThroughWallsCheat;
bool KeyIsOK(char);
ulong game::Ticks;
gamescript* game::GameScript = 0;
valuemap game::GlobalValueMap;

void game::InitScript()
{
  inputfile ScriptFile(GAME_DIR + "Script/dungeon.dat");
  delete GameScript;
  GameScript = new gamescript;
  GameScript->SetValueMap(GetGlobalValueMap());
  GameScript->ReadFrom(ScriptFile);
}

/*#include "materde.h"
#include "itemde.h"
#include "stack.h"*/

void game::Init(const std::string& Name)
{
  std::string PlayerName;

  if(Name == "")
    if(configuration::GetDefaultName() == "")
      {
	PlayerName = iosystem::StringQuestion("What is your name? (3-20 letters)", vector2d(30, 46), WHITE, 3, 20, true, true);

	if(PlayerName == "")
	  return;
      }
    else
      PlayerName = configuration::GetDefaultName();
  else
    PlayerName = Name;

  static ushort Counter = 0;

  Ticks = 0;
  ADD_MESSAGE("Initialization of game number %d started...", ++Counter);
  msgsystem::Format();

  WizardMode = false;
  SeeWholeMapCheat = false;
  GoThroughWallsCheat = false;
  PlayerBackup = 0;

  LOSTurns = 1;
  WorldMap = 0;

#ifdef WIN32
  _mkdir("Save");
#endif

#ifdef __DJGPP__
  mkdir("Save", S_IWUSR);
#endif

#ifdef USE_SDL
  mkdir(SAVE_DIR.c_str(), S_IRWXU | S_IRWXG);
#endif

  switch(Load(SaveName(PlayerName)))
    {
    case LOADED:
      {
	SetIsRunning(true);
	SetIsLoading(true);
	GetCurrentArea()->SendNewDrawRequest();
	game::SendLOSUpdateRequest();
	ADD_MESSAGE("Game loaded successfully.");
	break;
      }
    case NEWGAME:
      {
	iosystem::TextScreen("For many days you have wandered through a thick and gloomy forest.\n"
			     "Constantly you have had to fight against ultra-violent bears and\n"
			     "goblins that roam freely in this area. Screams of enner beasts have\n"
			     "wailed in the distance and the cold air has almost freezed your body.\n"
			     "But now your eyes catch the destination: A small settlement of a few\n"
			     "houses build around a frog-shaped tower. The town has recently tried\n"
			     "to advertise itself as a perfect place for adventurous tourists who\n"
			     "seek to face an untouched nature. Unfortunately you were among those\n"
			     "few who didn't understand they really meant what they said.\n\n"
			     "You have arrived at Attnam, the Holy City of Valpurus the Great Frog.\n"
			     "And you know nothing about the adventures that await you here.");

	SetIsRunning(true);
	iosystem::TextScreen("Generating game...\n\nThis may take some time, please wait.", WHITE, false, &BusyAnimation);
	CreateTeams();
	CreateGods();
	SetPlayer(new human);
	Player->SetAssignedName(PlayerName);
	Player->SetTeam(GetTeam(0));
	GetTeam(0)->SetLeader(Player);
	Petrus = 0;
	InitDungeons();
	WorldMap = new worldmap(128, 128);
	WorldMap->Generate();
	InWilderness = true;
	UpdateCamera();
	game::SendLOSUpdateRequest();

	GetGod(1)->SetKnown(true);
	GetGod(2)->SetKnown(true);
	GetGod(4)->SetKnown(true);
	GetGod(5)->SetKnown(true);
	GetGod(6)->SetKnown(true);

	Ticks = 0;

	BaseScore = Player->GetScore();
	dog* Doggie = new dog;
	Doggie->SetTeam(GetTeam(0));
	GetWorldMap()->GetPlayerGroup().push_back(Doggie);

	/*for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
	  {
	    const material::prototype* Proto = protocontainer<material>::GetProto(c);
	    const material::databasemap& Config = Proto->GetConfig();

	    for(material::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	      Player->GetStack()->AddItem(new oillamp(MAKE_MATERIAL(i->first)));
	  }*/

	ADD_MESSAGE("Game generated successfully.");
	break;
      }
    case BACK:
      {
	SetIsRunning(false);
	break;
      }
    }
}

void game::DeInit()
{
  delete GetPlayerBackup();
  SetPlayerBackup(0);
  delete GetWorldMap();
  SetWorldMap(0);

  ushort c;

  for(c = 0; c < Dungeon.size(); ++c)
    delete Dungeon[c];

  Dungeon.clear();

  for(c = 1; c < GetGods(); ++c)
    delete GetGod(c); // sorry, Valpuri!

  God.clear();

  entitypool::BurnTheDead();

  for(c = 0; c < Team.size(); ++c)
    delete GetTeam(c);

  Team.clear();
}

void game::Run()
{
  while(IsRunning())
    {	
      if(!InWilderness)
	GetCurrentDungeon()->GetLevel(Current)->GenerateMonsters(); // Temporary place

      try
	{
	  entitypool::Be();

	  Tick();
	  ApplyDivineTick();
	}
      catch(quitrequest) { }

      if(!IsRunning())
	break;
    }
}

void game::InitLuxTable()
{
  static bool AlreadyInitialized = false;

  if(!AlreadyInitialized)
    {
      AlreadyInitialized = true;
      LuxTable = new ushort**[0x200];
      LuxTableSize = new ushort[0x200];

      for(ushort c = 0; c < 0x200; ++c)
	{
	  ushort MaxDist = c >= 160 ? ushort(2 * sqrt(c / 5 - 32)) : 0, MaxSize = (MaxDist << 1) + 1;
	  LuxTableSize[c] = MaxSize;
	  LuxTable[c] = new ushort*[MaxSize];

	  for(long x = 0; x < MaxSize; ++x)
	    {
	      LuxTable[c][x] = new ushort[MaxSize];

	      for(long y = 0; y < MaxSize; ++y)
		{
		  long xLSquare = abs((int)x - MaxDist), yLSquare = abs((int)y - MaxDist);
		  LuxTable[c][x][y] = ushort(float(c) / (float(xLSquare * xLSquare + yLSquare * yLSquare) / 128 + 1));
		}
	    }
	}

      atexit(game::DeInitLuxTable);
    }
}

void game::DeInitLuxTable()
{
  if(LuxTable)
    {
      for(ushort c = 0; c < 0x200; ++c)
	{
	  for(ushort x = 0; x < LuxTableSize[c]; ++x)
	    delete [] LuxTable[c][x];

	  delete [] LuxTable[c];
	}

      delete [] LuxTable;
      delete [] LuxTableSize;
      LuxTable = 0;
    }
}

bool game::LOSHandler(vector2d Pos, vector2d Origo)
{
  if(Pos.X < 0 || Pos.Y < 0 || Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
    return false;

  GetCurrentArea()->GetSquare(Pos)->SetLastSeen(LOSTurns);

  if(Pos == Origo)
    return true;
  else
    return GetCurrentArea()->GetSquare(Pos)->GetOTerrain()->IsWalkable();
}

void game::UpdateCameraX()
{
  if(Player->GetPos().X < GetScreenSize().X / 2)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else if(Player->GetPos().X > GetCurrentArea()->GetXSize() - GetScreenSize().X / 2)
    if(Camera.X == GetCurrentArea()->GetXSize() - GetScreenSize().X)
      return;
    else
      Camera.X = GetCurrentArea()->GetXSize() - GetScreenSize().X;
  else
    if(Camera.X == Player->GetPos().X - GetScreenSize().X / 2)
      return;
    else
      Camera.X = Player->GetPos().X - GetScreenSize().X / 2;

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraY()
{
  if(Player->GetPos().Y < GetScreenSize().Y / 2)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else if(Player->GetPos().Y > GetCurrentArea()->GetYSize() - GetScreenSize().Y / 2)
    if(Camera.Y == GetCurrentArea()->GetYSize() - GetScreenSize().Y)
      return;
    else
      Camera.Y = GetCurrentArea()->GetYSize() - GetScreenSize().Y;
  else
    if(Camera.Y == Player->GetPos().Y - GetScreenSize().Y / 2)
      return;
    else
      Camera.Y = Player->GetPos().Y - GetScreenSize().Y / 2;

  GetCurrentArea()->SendNewDrawRequest();
}

const char* game::Insult()
{
  switch(RAND() % 15)
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
    default : return "hugger-mugger";
    }
}

/* DefaultAnswer = REQUIRES_ANSWER the question requires an answer */

bool game::BoolQuestion(const std::string& String, int DefaultAnswer, int OtherKeyForTrue)
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
  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X >= Camera.X && Pos.Y >= Camera.Y && Pos.X < game::GetCamera().X + GetScreenSize().X && Pos.Y < game::GetCamera().Y + GetScreenSize().Y)
    return true;
  else
    return false;
}

void game::DrawEverythingNoBlit()
{
  if(LOSUpdateRequested)
    game::GetCurrentArea()->UpdateLOS();

  game::GetCurrentArea()->Draw();

  if(OnScreen(GetPlayer()->GetPos()))
    igraph::DrawCursor(CalculateScreenCoordinates(GetPlayer()->GetPos()));

  GetPlayer()->DrawPanel();
  msgsystem::Draw();
}

bool game::Save(const std::string& SaveName)
{
  outputfile SaveFile(SaveName + ".sav");

  SaveFile << ushort(SAVEFILE_VERSION);
  SaveFile << CurrentDungeon << Current << Camera << WizardMode << SeeWholeMapCheat;
  SaveFile << GoThroughWallsCheat << BaseScore << Ticks << InWilderness << NextItemID;
  SaveFile << LOSTurns;

  ulong Time = time(0);
  femath::SetSeed(Time);
  SaveFile << Time;

  SaveFile << Dungeon << Team;

  if(InWilderness)
    SaveWorldMap();
  else
    GetCurrentDungeon()->SaveLevel(SaveName, Current, false);

  SaveFile << God;
  SaveFile << game::GetPlayer()->GetPos();
  SaveFile << PlayerBackup;
  msgsystem::Save(SaveFile);
  return true;
}

uchar game::Load(const std::string& SaveName)
{
  inputfile SaveFile(SaveName + ".sav", false);

  if(!SaveFile.IsOpen())
    return NEWGAME;

  ushort Version;
  SaveFile >> Version;

  if(Version != SAVEFILE_VERSION)
    {
      if(!iosystem::Menu(0, RES >> 1, "Sorry, this save is incompatible with the new version.\rStart new game?\r","Yes\rNo\r", MAKE_SHADE_COL(LIGHTGRAY), LIGHTGRAY))
	  return NEWGAME;
      else
	  return BACK;
    }

  SaveFile >> CurrentDungeon >> Current >> Camera >> WizardMode >> SeeWholeMapCheat;
  SaveFile >> GoThroughWallsCheat >> BaseScore >> Ticks >> InWilderness >> NextItemID;
  SaveFile >> LOSTurns;

  ulong Time;
  SaveFile >> Time;
  femath::SetSeed(Time);

  SaveFile >> Dungeon >> Team;

  if(InWilderness)
    LoadWorldMap(SaveName);
  else
    GetCurrentDungeon()->LoadLevel(SaveName);

  SaveFile >> God;

  vector2d Pos;

  SaveFile >> Pos;

  SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());

  SaveFile >> PlayerBackup;
  msgsystem::Load(SaveFile);
  return LOADED;
}

std::string game::SaveName(const std::string& Base)
{
  std::string SaveName = SAVE_DIR;

  if(Base == "")
    SaveName += game::GetPlayer()->GetAssignedName();
  else
    SaveName += Base;

  for(ushort c = 0; c < SaveName.length(); ++c)
    if(SaveName[c] == ' ')
      SaveName[c] = '_';

#ifdef WIN32
  if(SaveName.length() > 13)
    SaveName.resize(13);
#endif

#ifdef __DJGPP__
  if(SaveName.length() > 13)
    SaveName.resize(13);
#endif

  return SaveName;
}

bool game::EmitationHandler(vector2d Pos, vector2d Origo)
{
  if(Pos.X < 0 || Pos.Y < 0 || Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
    return false;

  ushort Emit = GetLevel(Current)->GetLSquare(Origo)->GetEmitation();

  ushort MaxSize = game::GetLuxTableSize()[Emit] >> 1;

  if(long(Pos.X) - long(Origo.X) > MaxSize || long(Origo.X) - long(Pos.X) > MaxSize || long(Pos.Y) - long(Origo.Y) > MaxSize || long(Origo.Y) - long(Pos.Y) > MaxSize)
    Emit = 0;
  else
    Emit = game::GetLuxTable()[Emit][long(Pos.X) - long(Origo.X) + MaxSize][long(Pos.Y) - long(Origo.Y) + MaxSize];

  GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->AlterLuminance(Origo, Emit);

  if(Pos == Origo)
    return true;
  else
    return GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->GetOLTerrain()->IsWalkable();
}

bool game::NoxifyHandler(vector2d Pos, vector2d Origo)
{
  if(Pos.X < 0 || Pos.Y < 0 || Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
    return false;

  GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->NoxifyEmitter(Origo);

  if(Pos == Origo)
    return true;
  else
    return GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->GetOLTerrain()->IsWalkable();
}

void game::UpdateCameraXWithPos(ushort Coord)
{
  if(Coord < GetScreenSize().X / 2)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else if(Coord > GetCurrentArea()->GetXSize() - GetScreenSize().X / 2)
    if(Camera.X == GetCurrentArea()->GetXSize() - GetScreenSize().X)
      return;
    else
      Camera.X = GetCurrentArea()->GetXSize() - GetScreenSize().X;
  else
    if(Camera.X == Coord - GetScreenSize().X / 2)
      return;
    else
      Camera.X = Coord - GetScreenSize().X / 2;

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraYWithPos(ushort Coord)
{
  if(Coord < GetScreenSize().Y / 2)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else if(Coord > GetCurrentArea()->GetYSize() - GetScreenSize().Y / 2)
    if(Camera.Y == GetCurrentArea()->GetYSize() - GetScreenSize().Y)
      return;
    else
      Camera.Y = GetCurrentArea()->GetYSize() - GetScreenSize().Y;
  else
    if(Camera.Y == Coord - GetScreenSize().Y / 2)
      return;
    else
      Camera.Y = Coord - GetScreenSize().Y / 2;

  GetCurrentArea()->SendNewDrawRequest();
}

int game::GetMoveCommandKeyBetweenPoints(vector2d A, vector2d B)
{
  for(ushort c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if((A + game::GetMoveVector(c)) == B)
      return game::MoveCommandKey[c];

  return DIR_ERROR;
}

void game::ApplyDivineTick(ushort Ticks)
{
  for(ushort c = 1; c < GetGods(); ++c)
    GetGod(c)->ApplyDivineTick(Ticks);
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, bool Good, short Multiplier)
{
  for(ushort c = 1; c < GetGods(); ++c)
    if(GetGod(c) != CompareTarget)
      GetGod(c)->AdjustRelation(CompareTarget, Good, Multiplier);
}

/*uchar game::GetDirectionIndexForKey(int Key)
{
  for(ushort c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if(Key == game::GetMoveCommandKey(c))
      return c;

  return DIR_ERROR;
}*/

vector2d game::GetDirectionVectorForKey(int Key)
{
  for(ushort c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if(Key == game::GetMoveCommandKey(c))
      return game::GetMoveVector(c);

  return DIR_ERROR_VECTOR;
}

bool game::EyeHandler(vector2d Pos, vector2d Origo)
{
  if(Pos == Origo)
    return true;
  else
    return GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->GetOLTerrain()->IsWalkable();
}

long game::GodScore()
{
  long Score = -1000;

  for(ushort c = 1; c < GetGods(); ++c)
    if(GetGod(c)->GetRelation() > Score)
      Score = GetGod(c)->GetRelation();

  return Score;
}

float game::Difficulty()
{
  float Base = game::GetPlayer()->MaxDanger() * (0.075f + float(GetCurrent()) / 75);

  while(true)
    {
      uchar Dice = RAND() % 25;

      if(Dice == 0)
	{
	  Base /= 6;
	  continue;
	}

      if(Dice == 24)
	{
	  Base *= 6;
	  continue;
	}

      if(Dice < 5)
	{
	  Base /= 3;
	  continue;
	}

      if(Dice > 19)
	{
	  Base *= 3;
	  continue;
	}

      return Base;
    }
}

void game::ShowLevelMessage()
{
  if(GetLevel(GetCurrent())->GetLevelMessage().length())
    ADD_MESSAGE(GetLevel(GetCurrent())->GetLevelMessage().c_str());

  GetLevel(GetCurrent())->SetLevelMessage("");
}

void game::TriggerQuestForGoldenEagleShirt()
{
  GetDungeon(0)->PrepareLevel(6);
  GetDungeon(0)->GetLevel(6)->CreateStairs(false);

  if(GetDungeon(0)->GetLevel(6)->GetLevelMessage() == "")
    GetDungeon(0)->GetLevel(6)->SetLevelMessage("You feel something has changed since you were last here...");

  GetDungeon(0)->SaveLevel(SaveName(), 6);
}

uchar game::DirectionQuestion(const std::string& Topic, bool RequireAnswer, bool AcceptYourself)
{
  while(true)
    {
      int Key = AskForKeyPress(Topic);

      if(AcceptYourself && Key == '.')
	return YOURSELF;

      for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == game::GetMoveCommandKey(c))
	  return c;

      if(!RequireAnswer)
	return DIR_ERROR;
    }
}

void game::RemoveSaves(bool RealSavesAlso)
{
  if(RealSavesAlso)
    {
      remove((SaveName() + ".sav").c_str());
      remove((SaveName() + ".wm").c_str());
    }

  remove((AutoSaveFileName + ".sav").c_str());
  remove((AutoSaveFileName + ".wm").c_str());

  for(ushort i = 0; i < Dungeon.size(); ++i)
    for(ushort c = 0; c < GetDungeon(i)->GetLevels(); ++c)
      {
	/*
	 * This looks very odd. And it is very odd.
	 * Indeed, gcc is very odd to not compile this correctly with -O3
	 * if it is written in a less odd way.
	 */

	std::string File = SaveName() + "." + i;
	File += c;

	if(RealSavesAlso)
	  remove(File.c_str());

	File = AutoSaveFileName + "." + i;
	File += c;

	remove(File.c_str());
      }
}

void game::SetPlayer(character* NP)
{
  Player = NP;

  if(Player)
    Player->SetIsPlayer(true);
}

area* game::GetCurrentArea()
{
  return !InWilderness ? (area*)GetCurrentDungeon()->GetLevel(Current) : (area*)WorldMap;
}

level* game::GetCurrentLevel()
{
  return GetCurrentDungeon()->GetLevel(Current);
}

level* game::GetLevel(ushort Index)
{
  return GetCurrentDungeon()->GetLevel(Index);
}

uchar game::GetLevels()
{
  return GetCurrentDungeon()->GetLevels();
}

void game::SeeWholeMap()
{
  SeeWholeMapCheat = !SeeWholeMapCheat;
  GetCurrentArea()->SendNewDrawRequest();
}

void game::InitDungeons()
{
  Dungeon.resize(*GetGameScript()->GetDungeons());

  for(ushort c = 0; c < Dungeon.size(); ++c)
    {
      Dungeon[c] = new dungeon(c);
      Dungeon[c]->SetIndex(c);
    }
}

void game::DoGoodDeed(ushort Amount)
{
  if(!Amount)
    return;
	
  for(ushort c = 1; c < game::GetGods(); ++c)
    {
      short Change = Amount - Amount * GetGod(c)->Alignment() / 5;

      if(!IsInWilderness() && GetPlayer()->GetLSquareUnder()->GetDivineMaster() == c)
	if(GetGod(c)->GetRelation() + Change * 2 < -750)
	  {
	    if(GetGod(c)->GetRelation() > -750)
	      GetGod(c)->SetRelation(-750);
	  }
	else if(GetGod(c)->GetRelation() + Change * 2 > 750)
	  {
	    if(GetGod(c)->GetRelation() < 750)
	      GetGod(c)->SetRelation(750);
	  }
	else
	  GetGod(c)->SetRelation(GetGod(c)->GetRelation() + Change * 2);
      else
	if(GetGod(c)->GetRelation() + Change < -500)
	  {
	    if(GetGod(c)->GetRelation() > -500)
	      GetGod(c)->SetRelation(-500);
	  }
	else if(GetGod(c)->GetRelation() + Change > 500)
	  {
	    if(GetGod(c)->GetRelation() < 500)
	      GetGod(c)->SetRelation(500);
	  }
	else
	  GetGod(c)->SetRelation(GetGod(c)->GetRelation() + Change);
    }
}

void game::DoEvilDeed(ushort Amount)
{
  if(!Amount)
    return;

  for(ushort c = 1; c < game::GetGods(); ++c)
    {
      short Change = Amount - Amount * GetGod(c)->Alignment() / 5;

      if(!IsInWilderness() && GetPlayer()->GetLSquareUnder()->GetDivineMaster() == c)
	if(GetGod(c)->GetRelation() - Change * 2 < -750)
	  {
	    if(GetGod(c)->GetRelation() > -750)
	      GetGod(c)->SetRelation(-750);
	  }
	else if(GetGod(c)->GetRelation() - Change * 2 > 750)
	  {
	    if(GetGod(c)->GetRelation() < 750)
	      GetGod(c)->SetRelation(750);
	  }
	else
	  GetGod(c)->SetRelation(GetGod(c)->GetRelation() - Change * 2);
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

void game::DoNeutralDeed(ushort Amount)
{
  if(!Amount)
    return;

  ADD_MESSAGE("If you are a coder, you could help us make game::DoNeutralDeed!");
}

void game::SaveWorldMap(const std::string& SaveName, bool DeleteAfterwards)
{
  outputfile SaveFile(SaveName + ".wm");
  SaveFile << WorldMap;

  if(DeleteAfterwards)
    delete WorldMap;
}

void game::LoadWorldMap(const std::string& SaveName)
{
  inputfile SaveFile(SaveName + ".wm");
  SaveFile >> WorldMap;
}

ulong game::CreateNewItemID()
{
  if(NextItemID == 0xFFFFFFFF)
    ABORT("Suddenly the Universe ends!");

  return NextItemID++;
}

void game::Hostility(team* Attacker, team* Defender)
{
  for(ushort c = 0; c < Team.size(); ++c)
    if(GetTeam(c) != Attacker && GetTeam(c) != Defender)
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
  ushort c;

  for(c = 0; c < *GetGameScript()->GetTeams(); ++c)
    {
      team* NewTeam = new team(c);

      for(ushort i = 0; i < Team.size(); ++i)
	GetTeam(i)->SetRelation(NewTeam, UNCARING);

      Team.push_back(NewTeam);
    }

  for(c = 0; c < Team.size(); ++c)
    if(c != 1)
      GetTeam(1)->SetRelation(GetTeam(c), HOSTILE);

  for(c = 0; c < GetGameScript()->GetTeam().size(); ++c)
  {
    for(ushort i = 0; i < GetGameScript()->GetTeam()[c].second->GetRelation().size(); ++i)
      GetTeam(GetGameScript()->GetTeam()[c].second->GetRelation()[i].first)->SetRelation(GetTeam(GetGameScript()->GetTeam()[c].first), GetGameScript()->GetTeam()[c].second->GetRelation()[i].second);

    if(GetGameScript()->GetTeam()[c].second->GetAttackEvilness(false))
      GetTeam(GetGameScript()->GetTeam()[c].first)->SetAttackEvilness(*GetGameScript()->GetTeam()[c].second->GetAttackEvilness());
  }
}

bool game::IsValidPos(vector2d Pos)
{
  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < GetCurrentArea()->GetXSize() && Pos.Y < GetCurrentArea()->GetYSize())
    return true;
  else
    return false;
}

std::string game::StringQuestion(const std::string& Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool AllowExit)
{
  DrawEverythingNoBlit();
  std::string Return = iosystem::StringQuestion(Topic, Pos, Color, MinLetters, MaxLetters, false, AllowExit);
  DOUBLEBUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

long game::NumberQuestion(const std::string& Topic, vector2d Pos, ushort Color)
{
  DrawEverythingNoBlit();
  long Return = iosystem::NumberQuestion(Topic, Pos, Color, false);
  DOUBLEBUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

void game::LOSTurn()
{
  if(LOSTurns == 0xFFFFFFFF)
    ABORT("Suddenly the Universe explodes!");

  ++LOSTurns;
}

void game::UpdateCamera()
{
  if(Player->GetPos().X < GetScreenSize().X / 2)
    Camera.X = 0;
  else if(Player->GetPos().X > GetCurrentArea()->GetXSize() - GetScreenSize().X / 2)
    Camera.X = GetCurrentArea()->GetXSize() - GetScreenSize().X;
  else
    Camera.X = Player->GetPos().X - GetScreenSize().X / 2;

  if(Player->GetPos().Y < GetScreenSize().Y / 2)
    Camera.Y = 0;
  else if(Player->GetPos().Y > GetCurrentArea()->GetYSize() - GetScreenSize().Y / 2)
    Camera.Y = GetCurrentArea()->GetYSize() - GetScreenSize().Y;
  else
    Camera.Y = Player->GetPos().Y - GetScreenSize().Y / 2;

  GetCurrentArea()->SendNewDrawRequest();
}

#ifndef __DJGPP__

bool game::HandleQuitMessage()
{
  if(IsRunning())
    {
      if(IsInGetCommand())
	{
#ifndef WIN32
	  switch(iosystem::Menu(0, RES >> 1, "Do you want to save your game before quitting?\r","Yes\rNo\rCancel\r", MAKE_SHADE_COL(LIGHTGRAY), LIGHTGRAY))
#else
	  switch(MessageBox(NULL, "Do you want to save your game before quitting?", "Save before quitting?", MB_YESNOCANCEL | MB_ICONQUESTION))
#endif
	    {
	    case IDYES:
	      Save();
	      game::RemoveSaves(false);
	      break;
	    case IDCANCEL:
#ifndef WIN32
	      GetCurrentArea()->SendNewDrawRequest();
#endif
	      return false;
	    default:
	      GetPlayer()->AddScoreEntry("cowardly quit the game", 0.75f);
	      End(true, false);
	      break;
	    }
	}
      else
#ifdef WIN32
	if(MessageBox(NULL, "You can't save at this point. Are you sure you still want to do this?", "Exit confirmation request", MB_YESNO | MB_ICONWARNING) == IDYES)
#else
	if(iosystem::Menu(0, RES >> 1, "You can't save at this point. Are you sure you still want to do this?", "Yes\rNo\r", MAKE_SHADE_COL(LIGHTGRAY), LIGHTGRAY))
#endif
	  {
	    RemoveSaves();
	  }
	else
	  return false;
    }

  configuration::Save();
  return true;
}

#endif

void game::Beep()
{
#ifdef WIN32
  if(configuration::GetBeepOnCritical())
    ::Beep(400, 1000);
#endif
}

uchar game::GetDirectionForVector(vector2d Vector)
{
  for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
    if(Vector == game::GetMoveVector(c))
      return c;

  return DIR_ERROR;
}

std::string game::GetVerbalPlayerAlignment()
{
  long Sum = 0;

  for(ushort c = 1; c < game::GetGods(); ++c)
    {
      if(GetGod(c)->GetRelation() > 0)
	Sum += GetGod(c)->GetRelation() * (5 - GetGod(c)->Alignment());
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
  God.resize(1, 0);

  for(ushort c = 1; c < protocontainer<god>::GetProtoAmount(); ++c)
    God.push_back(protocontainer<god>::GetProto(c)->Clone());
}

vector2d game::CalculateScreenCoordinates(vector2d Pos)
{
  return (Pos - Camera + vector2d(1, 2)) << 4;
}

void game::BusyAnimation(bitmap* Buffer)
{
  static bitmap Elpuri(16, 16, TRANSPARENTCOL);
  static bool ElpuriLoaded = false;
  static double Rotation = 0;
  static clock_t LastTime = 0;

  if(!ElpuriLoaded)
    {
      ushort Color = MAKE_RGB(48, 48, 48);
      igraph::GetCharacterRawGraphic()->MaskedBlit(&Elpuri, 64, 0, 0, 0, 16, 16, &Color);
      ElpuriLoaded = true;
    }

  if(clock() - LastTime > CLOCKS_PER_SEC >> 5)
    {
      vector2d Pos(RES.X / 2, RES.Y * 2 / 3);
      Buffer->Fill(Pos.X - 100, Pos.Y - 100, 200, 200, 0);
      Rotation += 0.02;

      if(Rotation > 2 * PI)
	Rotation -= 2 * PI;

      Elpuri.MaskedBlit(DOUBLEBUFFER, 0, 0, Pos.X - 8, Pos.Y - 7, 16, 16);

      ushort x;

      for(x = 0; x < 10; ++x)
	Buffer->DrawPolygon(Pos, 100, 5, MAKE_RGB(255 - 25 * (10 - x),0,0), false, true, Rotation + double(x) / 50);

      for(x = 0; x < 4; ++x)
	Buffer->DrawPolygon(Pos, 100 + x, 50, MAKE_RGB(255 - 12 * x,0,0));

      if(Buffer == DOUBLEBUFFER)
	graphics::BlitDBToScreen();

      LastTime = clock();
    }
}

int game::AskForKeyPress(const std::string& Topic)
{
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLEBUFFER, 16, 8, WHITE, "%s", Topic.c_str());
  graphics::BlitDBToScreen();
  int Key = GETKEY();
  DOUBLEBUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Key;
}

/* 
   Handler is called when the key has been identified as a movement key 
   BadKeyHandler is called when the key has NOT been identified as a movement key
   Both can be deactivated by passing 0 as parameter
 */  

vector2d game::PositionQuestion(const std::string& Topic, vector2d CursorPos, void (*Handler)(vector2d), void (*BadKeyHandler)(vector2d, int), bool Zoom)
{
  int Key = 0;
  FONT->Printf(DOUBLEBUFFER, 16, 8, WHITE, "%s", Topic.c_str());
  graphics::BlitDBToScreen();
  globalwindowhandler::InstallControlLoop(PositionQuestionController);
  SetDoZoom(Zoom);
  vector2d Return;

  while(true)
    {
      if(!GetCurrentArea()->GetSquare(CursorPos)->GetLastSeen() && !GetSeeWholeMapCheat())
	DOUBLEBUFFER->Fill(CalculateScreenCoordinates(CursorPos), vector2d(16, 16), 0);
      else
	GetCurrentArea()->GetSquare(CursorPos)->SendNewDrawRequest();

      if(Key == ' ')
	{
	  Return = CursorPos;
	  break;
	}

      if(Key == 0x1B)
	{
	  Return = vector2d(-1, -1);
	  break;
	}

      vector2d DirectionVector = GetDirectionVectorForKey(Key);

      if(DirectionVector != DIR_ERROR_VECTOR)
	{
	  CursorPos += DirectionVector;

	  if(short(CursorPos.X) > GetCurrentArea()->GetXSize() - 1) CursorPos.X = 0;
	  if(short(CursorPos.X) < 0) CursorPos.X = GetCurrentArea()->GetXSize() - 1;
	  if(short(CursorPos.Y) > GetCurrentArea()->GetYSize() - 1) CursorPos.Y = 0;
	  if(short(CursorPos.Y) < 0) CursorPos.Y = GetCurrentArea()->GetYSize() - 1;
	}
      else if(BadKeyHandler)
	BadKeyHandler(CursorPos, Key);

      if(CursorPos.X < GetCamera().X + 2 || CursorPos.X > GetCamera().X + GetScreenSize().X - 2)
	UpdateCameraXWithPos(CursorPos.X);

      if(CursorPos.Y < GetCamera().Y + 2 || CursorPos.Y > GetCamera().Y + GetScreenSize().Y - 2)
	UpdateCameraYWithPos(CursorPos.Y);

      if(Handler)
	Handler(CursorPos);

      DrawEverythingNoBlit();
      SetCursorPos(CursorPos);
      PositionQuestionController();
      graphics::BlitDBToScreen();
      Key = GETKEY();
    }

  DOUBLEBUFFER->Fill(16, 6, GetScreenSize().X << 4, 23, BLACK);
  DOUBLEBUFFER->Fill(RES.X - 96, RES.Y - 96, 80, 80, BLACK);
  globalwindowhandler::DeInstallControlLoop(PositionQuestionController);
  DrawEverythingNoBlit();
  return Return;
}

bool game::PositionQuestionController()
{
  vector2d ScreenCordinates = CalculateScreenCoordinates(GetCursorPos());
  igraph::DrawCursor(ScreenCordinates);

  if(DoZoom())
    DOUBLEBUFFER->StretchBlit(DOUBLEBUFFER, ScreenCordinates, RES.X - 96, RES.Y - 96, 16, 16, 5);

  return true;
}

void game::LookHandler(vector2d CursorPos)
{
  std::string OldMemory;

  if(game::GetSeeWholeMapCheat())
    {
      OldMemory = game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();
      game::GetCurrentArea()->GetSquare(CursorPos)->UpdateMemorizedDescription(true);
    }

  if(game::GetCurrentArea()->GetSquare(CursorPos)->GetLastSeen() || game::GetSeeWholeMapCheat())
    {
      std::string Msg;

      if(game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeen(true) || game::GetSeeWholeMapCheat())
	Msg = "You see here ";
      else
	Msg = "You remember here ";

      Msg += game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();

      ADD_MESSAGE("%s.", Msg.c_str());

      character* Character = game::GetCurrentArea()->GetSquare(CursorPos)->GetCharacter();

      if(Character && (game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeen() || game::GetSeeWholeMapCheat()))
	Character->DisplayInfo();

      if(!IsInWilderness())
	{
	  lsquare* LSquare = game::GetCurrentLevel()->GetLSquare(CursorPos);

	  if(LSquare->CanBeSeen() && LSquare->GetStack()->GetItems() > 1)
	    ADD_MESSAGE("[press i to see all items]");
	}
    }
  else
    ADD_MESSAGE("You have no idea what might lie here.");

  if(game::GetSeeWholeMapCheat())
    {
      game::GetCurrentArea()->GetSquare(CursorPos)->SetMemorizedDescription(OldMemory);
      game::GetCurrentArea()->GetSquare(CursorPos)->SetDescriptionChanged(true);
    }

  if(game::WizardModeActivated())
    ADD_MESSAGE("(%d, %d)", CursorPos.X, CursorPos.Y);
}

bool game::AnimationController()
{
  if(IsRunning() && AnimationControllerIsActive())
    {
      game::DrawEverythingNoBlit();
      return true;
    }
  else
    return false;
}

void game::InitGlobalValueMap()
{
  inputfile SaveFile(GAME_DIR + "Script/define.dat");
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word != "#" || SaveFile.ReadWord() != "define")
	ABORT("Illegal datafile define!");

      SaveFile.ReadWord(Word);
      GlobalValueMap[Word] = SaveFile.ReadNumber(GlobalValueMap);
    }
}

void game::TextScreen(const std::string& Text, ushort Color, bool GKey, void (*BitmapEditor)(bitmap*))
{
  SetAnimationControllerActive(false);
  iosystem::TextScreen(Text, Color, GKey, BitmapEditor);
  SetAnimationControllerActive(true);
}

/* ... all the keys that are acceptable 
   DefaultAnswer = REQUIRES_ANSWER if this question requires an answer */

int game::KeyQuestion(const std::string& Message, int DefaultAnswer, int KeyNumber, ...)
{
  std::vector<int> KeyVector;
  va_list Arguments;
  va_start(Arguments, KeyNumber);

  for(ushort c = 0; c < KeyNumber; ++c)
    {
      KeyVector.push_back(va_arg(Arguments,int));
    }

  va_end(Arguments);

  DrawEverythingNoBlit();
  FONT->Printf(DOUBLEBUFFER, 16, 8, WHITE, "%s", Message.c_str());
  graphics::BlitDBToScreen();
  int Return;

  while(true)
    {
      int k = GETKEY();
      std::vector<int>::iterator i = std::find(KeyVector.begin(), KeyVector.end(), k);

      if(i != KeyVector.end())
	{
	  Return = k;
	  break;
	}
      else if(DefaultAnswer != REQUIRES_ANSWER)
	{
	  Return = DefaultAnswer;
	  break;
	}
    }

  DOUBLEBUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

void game::LookBadKeyHandler(vector2d CursorPos, int Key)
{
  switch(Key)
    {
    case 'i':
      if(!IsInWilderness() && (GetCurrentLevel()->GetLSquare(CursorPos)->CanBeSeen() || game::GetSeeWholeMapCheat()))
	{
	  stack* Stack = game::GetCurrentLevel()->GetLSquare(CursorPos)->GetStack();

	  if(Stack->GetItems() > 1)
	    {
	      Stack->DrawContents(game::GetPlayer(), "Items here", false);
	    }
	}
      break;
    default:
      /* Bad Key */
      break;
    }
}

void game::End(bool Permanently, bool AndGoToMenu)
{
  SetIsRunning(false);
  RemoveSaves(Permanently);

  if(Permanently && !WizardModeActivated())
    {
      highscore HScore;
      HScore.Draw();
    }

  if(AndGoToMenu)
    {
      /* This prevents monster movement etc. after death. */

      throw quitrequest();
    }
}

uchar game::CalculateRoughDirection(vector2d Vector)
{
  if(!Vector.X && !Vector.Y)
    return YOURSELF;

  float Angle = femath::CalculateAngle(Vector);

  if(Angle < PI / 8)
    return 4;
  else if(Angle < 3 * PI / 8)
    return 7;
  else if(Angle < 5 * PI / 8)
    return 6;
  else if(Angle < 7 * PI / 8)
    return 5;
  else if(Angle < 9 * PI / 8)
    return 3;
  else if(Angle < 11 * PI / 8)
    return 0;
  else if(Angle < 13 * PI / 8)
    return 1;
  else if(Angle < 15 * PI / 8)
    return 2;
  else
    return 4;
}
