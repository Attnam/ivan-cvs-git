#include <cmath>
#include <ctime>

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
#include "strover.h"
#include "felist.h"
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

gamescript game::GameScript;

bool game::IsLoading = false, game::InGetCommand = false;
petrus* game::Petrus = 0;

std::string game::AutoSaveFileName = SAVE_DIR + "Autosave";
std::string game::Alignment[] = {"L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--"};
std::vector<god*> game::God;

command* game::Command[] = {0,
			    new command(&character::Apply, "apply", 'a', false),
			    new command(&character::Talk, "chat", 'C', false),
			    new command(&character::Close, "close", 'c', false),
			    new command(&character::DecreaseContrast, "decrease contrast", 'B', true),
			    new command(&character::Dip, "dip", 'D', true),
			    new command(&character::Drop, "drop", 'd', false),
			    new command(&character::Consume, "eat/drink", 'e', true),
			    new command(&character::WhatToEngrave, "engrave", 'E', false),
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
			    new command(&character::OutlineItems, "outline items", 'I', true),
			    new command(&character::PickUp, "pick up", ',', false),
			    new command(&character::Pray, "pray", 'p', false),
			    new command(&character::Quit, "quit", 'q', true),
			    new command(&character::RaiseStats, "raise stats cheat", '1', true, true),
			    new command(&character::RaiseGodRelations, "raise your relations to the gods cheat", '5', true, true),
			    new command(&character::Read, "read", 'r', false),
			    new command(&character::RestUntilHealed, "rest/heal", 'h', true),
			    new command(&character::Save, "save game", 's', true),
			    new command(&character::SecretKnowledge, "reveal secret knowledge", '9', true, true),
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
			    new command(&character::WearArmor, "wear", 'W', true),
			    new command(&character::Wield, "wield", 'w', true),
			    new command(&character::WizardMode, "wizard mode activation", 'X', true),
			    new command(&character::Zap, "zap", 'z', false),
			    0};

int game::MoveCommandKey[DIRECTION_COMMAND_KEYS] = {0x147, 0x148, 0x149, 0x14B, 0x14D, 0x14F, 0x150, 0x151};
const vector2d game::MoveVector[DIRECTION_COMMAND_KEYS] = {vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1)};

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
std::string game::PlayerName;
ulong game::Ticks;

void game::InitScript()
{
  femath::SetSeed(time(0));
  inputfile ScriptFile((GAME_DIR + std::string("Script/dungeon.dat")).c_str());
  GameScript.ReadFrom(ScriptFile);
}

#include "stack.h"
#include "itemde.h"

void game::Init(std::string Name)
{
  if(Name == "")
    if(configuration::GetDefaultName() == "")
      {
	SetPlayerName(iosystem::StringQuestion("What is your name? (3-20 letters)", vector2d(30, 46), WHITE, 3, 20, true, true));

	if(PlayerName == "")
	  return;
      }
    else
      SetPlayerName(configuration::GetDefaultName());
  else
    SetPlayerName(Name);

  static ushort Counter = 0;

  Ticks = 0;
  EMPTY_MESSAGES();
  ADD_MESSAGE("Initialization of game number %d started...", ++Counter);
  globalmessagingsystem::Format();

  WizardMode = false;
  SeeWholeMapCheat = false;
  GoThroughWallsCheat = false;
  PlayerBackup = 0;

  femath::SetSeed(time(0));
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

  switch(Load())
    {
    case LOADED:
      {
	Running = true;
	IsLoading = true;
	GetCurrentArea()->SendNewDrawRequest();
	game::SendLOSUpdateRequest();
	ADD_MESSAGE("Game loaded successfully.");
	break;
      }
    case NEWGAME:
      {
	iosystem::TextScreen("For many days you have wandered through a thick and gloomy forest.\n"
			     "Constantly you have had to fight against ultra-violent bears and\n"
			     "goblins that roam freely in this area. Screams of Enner Beasts have\n"
			     "wailed in the distance and the cold air has almost freezed your body.\n"
			     "But now your eyes catch the destination: A small settlement of a few\n"
			     "houses build around a frog-shaped tower. The town has recently tried\n"
			     "to advertise itself as a perfect place for adventurous tourists who\n"
			     "seek to face an untouched nature. Unfortunately you were among those\n"
			     "few who didn't understand they really meant what they said.\n\n"
			     "You have arrived at Attnam, the Holy City of Valpurus the Great Frog.\n"
			     "And you know nothing about the adventures that await you here.");

	Running = true;
	iosystem::TextScreen("Generating game...\n\nThis may take some time, please wait.", WHITE, false);
	CreateTeams();
	CreateGods();
	SetPlayer(new human);
	Player->SetTeam(GetTeam(0));
	GetTeam(0)->SetLeader(Player);
	Petrus = 0;
	InitDungeons();
	WorldMap = new worldmap(128, 128);
	WorldMap->Generate();
	GetDungeon(0)->PrepareLevel(6);
	GetDungeon(0)->SaveLevel(SaveName(), 6);
	InWilderness = true;
	UpdateCamera();
	game::SendLOSUpdateRequest();

	GetGod(1)->SetKnown(true);
	GetGod(2)->SetKnown(true);
	GetGod(4)->SetKnown(true);
	GetGod(6)->SetKnown(true);
	GetGod(7)->SetKnown(true);

	Ticks = 0;

	BaseScore = Player->Score();
	dog* Doggie = new dog;
	Doggie->SetTeam(GetTeam(0));
	GetWorldMap()->GetPlayerGroup().push_back(Doggie);
	ADD_MESSAGE("Game generated successfully.");
	break;
      }
    case BACK:
      {
	Running = false;
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
  while(GetRunning())
    {	
      if(!InWilderness)
	GetCurrentDungeon()->GetLevel(Current)->HandleCharacters(); // Temporary

      try
	{
	  entitypool::Be();

	  Tick();
	  ApplyDivineTick();
	}
      catch(quitrequest) { }

      if(!GetRunning())
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

void game::Quit()
{
  Running = false;

  /* This prevents monster movement etc. after death. */

  throw quitrequest();
}

bool game::LOSHandler(vector2d Pos, vector2d Origo)
{
  if(Pos.X < 0 || Pos.Y < 0 || Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
    return false;

  GetCurrentArea()->GetSquare(Pos)->SetLastSeen(LOSTurns);

  if(Pos == Origo)
    return true;
  else
    return GetCurrentArea()->GetSquare(Pos)->GetOTerrain()->GetIsWalkable();
}

void game::DrawPanel()
{
  DOUBLEBUFFER->Fill(0, 512, 800, 88, 0);

  character* Player = GetPlayer();

  FONT->Printf(DOUBLEBUFFER, 16, 524, WHITE, "%s the %s %s", GetPlayerName().c_str(), GetVerbalPlayerAlignment().c_str(), Player->CNAME(UNARTICLED));

  FONT->Printf(DOUBLEBUFFER, 16, 534, WHITE, "Strength: %d", Player->GetStrength());
  FONT->Printf(DOUBLEBUFFER, 16, 544, WHITE, "Endurance: %d", Player->GetEndurance());
  FONT->Printf(DOUBLEBUFFER, 16, 554, WHITE, "Agility: %d", Player->GetAgility());
  FONT->Printf(DOUBLEBUFFER, 16, 564, WHITE, "Perception: %d", Player->GetPerception());
  FONT->Printf(DOUBLEBUFFER, 16, 574, WHITE, "Size: %d", Player->GetSize());
  FONT->Printf(DOUBLEBUFFER, 16, 584, Player->GetHP() < Player->GetMaxHP() / 3 ? RED : WHITE, "HP: %d/%d", Player->GetHP(), Player->GetMaxHP());

  if(Player->GetWielded())
    FONT->Printf(DOUBLEBUFFER, 200, 574, WHITE, "Wielded: %s", Player->GetWielded()->CNAME(INDEFINITE));

  if(Player->GetTorsoArmor())
    FONT->Printf(DOUBLEBUFFER, 200, 584, WHITE, "Worn: %s", Player->GetTorsoArmor()->CNAME(INDEFINITE));

  FONT->Printf(DOUBLEBUFFER, 200, 534, WHITE, "Weapon Strength: %.0f", Player->GetAttackStrength() / 100);
  FONT->Printf(DOUBLEBUFFER, 200, 544, WHITE, "To Hit Value: %.0f", Player->GetToHitValue());
  FONT->Printf(DOUBLEBUFFER, 200, 554, WHITE, "Damage: %d-%d", ushort(Player->GetAttackStrength() * Player->GetStrength() / 66667), ushort(Player->GetAttackStrength() * Player->GetStrength() / 40000 + 1));
  FONT->Printf(DOUBLEBUFFER, 200, 564, WHITE, "Money: %d", Player->GetMoney());

  FONT->Printf(DOUBLEBUFFER, 440, 534, WHITE, "Armor Value: %d", Player->CalculateArmorModifier());
  FONT->Printf(DOUBLEBUFFER, 440, 544, WHITE, "Dodge Value: %.0f", Player->GetDodgeValue());

  if(GetWizardMode())
    {
      FONT->Printf(DOUBLEBUFFER, 440, 554, WHITE, "Danger: %d", Player->CurrentDanger());
      FONT->Printf(DOUBLEBUFFER, 440, 564, WHITE, "NP: %d", Player->GetNP());
    }
  else
    FONT->Printf(DOUBLEBUFFER, 440, 554, WHITE, "Danger Level: %d", Player->DangerLevel());

  if(GetInWilderness())
    FONT->Printf(DOUBLEBUFFER, 620, 534, WHITE, "Worldmap");
  else
    FONT->Printf(DOUBLEBUFFER, 620, 534, WHITE, "%s", GetCurrentDungeon()->GetLevelDescription(GetCurrent()).c_str());

  FONT->Printf(DOUBLEBUFFER, 620, 544, WHITE, "Time: %d", GetTicks() / 10);

  if(Player->StateIsActivated(FAINTED))
    FONT->Printf(DOUBLEBUFFER, 620, 554, RED, "Fainted");
  else if(Player->GetNP() < CRITICALHUNGERLEVEL)
    FONT->Printf(DOUBLEBUFFER, 620, 554, RED, "Fainting");
  else
    if(Player->GetNP() < HUNGERLEVEL)
      FONT->Printf(DOUBLEBUFFER, 620, 554, BLUE, "Hungry");

  switch(Player->GetBurdenState())
    {
    case OVERLOADED:
      FONT->Printf(DOUBLEBUFFER, 620, 564, RED, "Overload!");
      break;
    case STRESSED:
      FONT->Printf(DOUBLEBUFFER, 620, 564, BLUE, "Stressed");
      break;
    case BURDENED:
      FONT->Printf(DOUBLEBUFFER, 620, 564, BLUE, "Burdened!");
    case UNBURDENED:
      break;
    }
}

void game::UpdateCameraX()
{
  if(Player->GetPos().X < 25)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else
    if(Camera.X == Player->GetPos().X - 25)
      return;
    else
      Camera.X = Player->GetPos().X - 25;

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraY()
{
  if(Player->GetPos().Y < 15)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else
    if(Camera.Y == Player->GetPos().Y - 15)
      return;
    else
      Camera.Y = Player->GetPos().Y - 15;

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

bool game::BoolQuestion(std::string String, char DefaultAnswer, int OtherKeyForTrue)
{
  int ch;
  ADD_MESSAGE(String.c_str());
  DRAW_MESSAGES();
  graphics::BlitDBToScreen();
  for(;;)
    {
      ch = GETKEY();
      if (ch == 0x00d || ch == 'y' || ch == 'Y' || ch == OtherKeyForTrue) { EMPTY_MESSAGES(); graphics::BlitDBToScreen(); return true;  }
      if (ch == 'n' || ch == 'N') { EMPTY_MESSAGES(); graphics::BlitDBToScreen(); return false; }
      if(DefaultAnswer == 2) continue;
      EMPTY_MESSAGES();
      graphics::BlitDBToScreen();
      return DefaultAnswer ? true : false;
    }
}

const char* game::PersonalPronoun(uchar Sex)
{
  switch(Sex)
    {
    case UNDEFINED:
      return "it";
    case MALE:
      return "he";
    case FEMALE:
      return "she";
    default:
      ABORT("Illegal sex encountered.");
      return "xxx";
    }
}

const char* game::PossessivePronoun(uchar Sex)
{
  switch(Sex)
    {
    case UNDEFINED:
      return "its";
    case MALE:
      return "his";
    case FEMALE:
      return "her";
    default:
      ABORT("Illegal sex encountered.");
      return "xxx";
    }
}

void game::DrawEverything(bool EmptyMsg)
{
  DrawEverythingNoBlit(EmptyMsg);
  graphics::BlitDBToScreen();
}

bool game::OnScreen(vector2d Pos)
{
  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X >= Camera.X && Pos.Y >= Camera.Y && Pos.X < game::GetCamera().X + 50 && Pos.Y < game::GetCamera().Y + 30)
    return true;
  else
    return false;
}

void game::DrawEverythingNoBlit(bool EmptyMsg)
{
  if(LOSUpdateRequested)
    game::GetCurrentArea()->UpdateLOS();

  game::GetCurrentArea()->Draw();

  if(OnScreen(GetPlayer()->GetPos()))
    igraph::DrawCursor((GetPlayer()->GetPos() - GetCamera() + vector2d(0,2)) << 4);

  DrawPanel();

  if(EmptyMsg)
    {
      DRAW_MESSAGES();
      EMPTY_MESSAGES();
    }
}

bool game::Save(std::string SaveName)
{
  outputfile SaveFile(SaveName + ".sav");

  SaveFile << ushort(SAVEFILE_VERSION) << PlayerName;
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
  globalmessagingsystem::GetMessageHistory()->Save(SaveFile);
  return true;
}

uchar game::Load(std::string SaveName)
{
  inputfile SaveFile(SaveName + ".sav", false);

  if(!SaveFile.IsOpen())
    return NEWGAME;

  ushort Version;
  SaveFile >> Version;

  if(Version != SAVEFILE_VERSION)
    {
      if(!iosystem::Menu(0, "Sorry, this save is incompatible with the new version.\rStart new game?\r","Yes\rNo\r", BLUE, WHITE, false))
	  return NEWGAME;
      else
	  return BACK;
    }

  SaveFile >> PlayerName;
  SaveFile >> CurrentDungeon >> Current >> Camera >> WizardMode >> SeeWholeMapCheat;
  SaveFile >> GoThroughWallsCheat >> BaseScore >> Ticks >> InWilderness >> NextItemID;
  SaveFile >> LOSTurns;

  ulong Time;
  SaveFile >> Time;
  femath::SetSeed(Time);

  SaveFile >> Dungeon >> Team;

  if(InWilderness)
    LoadWorldMap();
  else
    GetCurrentDungeon()->LoadLevel(SaveName);

  SaveFile >> God;

  vector2d Pos;

  SaveFile >> Pos;

  SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());

  SaveFile >> PlayerBackup;
  globalmessagingsystem::GetMessageHistory()->Load(SaveFile);
  return LOADED;
}

std::string game::SaveName()
{
  std::string SaveName = SAVE_DIR + PlayerName;

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
    return GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->GetOLTerrain()->GetIsWalkable();
}

bool game::NoxifyHandler(vector2d Pos, vector2d Origo)
{
  if(Pos.X < 0 || Pos.Y < 0 || Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
    return false;

  GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->NoxifyEmitter(Origo);

  if(Pos == Origo)
    return true;
  else
    return GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->GetOLTerrain()->GetIsWalkable();
}

void game::UpdateCameraXWithPos(ushort Coord)
{
  if(Coord < 25)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else
    if(Camera.X == Coord - 25)
      return;
    else
      Camera.X = Coord - 25;

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraYWithPos(ushort Coord)
{
  if(Coord < 15)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else
    if(Camera.Y == Coord - 15)
      return;
    else
      Camera.Y = Coord - 15;

  GetCurrentArea()->SendNewDrawRequest();
}

int game::GetMoveCommandKey(vector2d A, vector2d B)
{
  for(ushort c = 0; c < 8; ++c)
    if((A + game::GetMoveVector(c)) == B)
      return game::MoveCommandKey[c];

  return 0xFF;
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

vector2d game::GetDirectionVectorForKey(ushort Key)
{
  for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
    if(Key == game::GetMoveCommandKey(c))
      return game::GetMoveVector(c);

  return vector2d(0,0);
}

vector2d game::AskForDirectionVector(std::string String)
{
  if(String != "")
    {
      ADD_MESSAGE("%s", String.c_str());
      DRAW_MESSAGES();
      graphics::BlitDBToScreen();
      EMPTY_MESSAGES();
    }

  return GetDirectionVectorForKey(GETKEY());
}

bool game::EyeHandler(vector2d Pos, vector2d Origo)
{
  if(Pos == Origo)
    return true;
  else
    return GetCurrentDungeon()->GetLevel(Current)->GetLSquare(Pos)->GetOLTerrain()->GetIsWalkable();
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

uchar game::DirectionQuestion(std::string Topic, uchar DefaultAnswer, bool RequireAnswer, bool AcceptYourself)
{
  ADD_MESSAGE(Topic.c_str());

  DRAW_MESSAGES();

  EMPTY_MESSAGES();

  graphics::BlitDBToScreen();

  while(true)
    {
      int Key = GETKEY();

      if(AcceptYourself && Key == '.')
	return '.';

      for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
	if(Key == game::GetMoveCommandKey(c))
	  return c;

      if(DefaultAnswer < 8) return DefaultAnswer;
      if(!RequireAnswer) return 0xFF;
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
  Dungeon.resize(2);

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

      if(!GetInWilderness() && GetPlayer()->GetLSquareUnder()->GetDivineOwner() == c)
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

      if(!GetInWilderness() && GetPlayer()->GetLSquareUnder()->GetDivineOwner() == c)
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

void game::SaveWorldMap(std::string SaveName, bool DeleteAfterwards)
{
  outputfile SaveFile(SaveName + ".wm");
  SaveFile << WorldMap;

  if(DeleteAfterwards)
    delete WorldMap;
}

void game::LoadWorldMap(std::string SaveName)
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
	    if(GetTeam(c)->GetRelation(Attacker) == NEUTRAL)
	      GetTeam(c)->SetRelation(Attacker, FRIEND);

	    break;
	  }
	case NEUTRAL:
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

  for(c = 0; c < *GameScript.GetTeams(); ++c)
    {
      team* NewTeam = new team(c);

      for(ushort i = 0; i < Team.size(); ++i)
	GetTeam(i)->SetRelation(NewTeam, NEUTRAL);

      Team.push_back(NewTeam);
    }

  for(c = 0; c < Team.size(); ++c)
    if(c != 1)
      GetTeam(1)->SetRelation(GetTeam(c), HOSTILE);

  for(c = 0; c < GameScript.GetTeam().size(); ++c)
  {
    for(ushort i = 0; i < GameScript.GetTeam()[c].second->GetRelation().size(); ++i)
      GetTeam(GameScript.GetTeam()[c].second->GetRelation()[i].first)->SetRelation(GetTeam(GameScript.GetTeam()[c].first), GameScript.GetTeam()[c].second->GetRelation()[i].second);

    if(GameScript.GetTeam()[c].second->GetAttackEvilness(false))
      GetTeam(GameScript.GetTeam()[c].first)->SetAttackEvilness(*GameScript.GetTeam()[c].second->GetAttackEvilness());
  }
}

bool game::IsValidPos(vector2d Pos)
{
  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < GetCurrentArea()->GetXSize() && Pos.Y < GetCurrentArea()->GetYSize())
    return true;
  else
    return false;
}

std::string game::StringQuestion(std::string Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool AllowExit)
{
  EMPTY_MESSAGES();
  DrawEverythingNoBlit();
  return iosystem::StringQuestion(Topic, Pos, Color, MinLetters, MaxLetters, false, AllowExit);
}

long game::NumberQuestion(std::string Topic, vector2d Pos, ushort Color)
{
  EMPTY_MESSAGES();
  DrawEverythingNoBlit();
  return iosystem::NumberQuestion(Topic, Pos, Color, false);
}

void game::LOSTurn()
{
  if(LOSTurns == 0xFFFFFFFF)
    ABORT("Suddenly the Universe explodes!");

  ++LOSTurns;
}

void game::UpdateCamera()
{
  if(Player->GetPos().X < 25)
    Camera.X = 0;
  else
    Camera.X = Player->GetPos().X - 25;

  if(Player->GetPos().Y < 15)
    Camera.Y = 0;
  else
    Camera.Y = Player->GetPos().Y - 15;

  GetCurrentArea()->SendNewDrawRequest();
}

#ifndef __DJGPP__

bool game::HandleQuitMessage()
{
  if(GetRunning())
    {
      if(GetInGetCommand())
	{
#ifndef WIN32
	  switch(iosystem::Menu(0, "Do you want to save your game before quitting?\r","Yes\rNo\rCancel\r", BLUE, WHITE, false))
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
	      RemoveSaves();
	      Running = false;

	      if(!GetWizardMode())
		{
		  GetPlayer()->AddScoreEntry("cowardly quit the game", 0.75f);
		  highscore HScore;
		  HScore.Draw();
		}

	      break;
	    }
	}
      else
#ifdef WIN32
	if(MessageBox(NULL, "You can't save at this point. Are you sure you still want to do this?", "Exit confirmation request", MB_YESNO | MB_ICONWARNING) == IDYES)
#else
	if(iosystem::Menu(0, "You can't save at this point. Are you sure you still want to do this?", "Yes\rNo\r", BLUE, WHITE, false))
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

  return DIRECTION_COMMAND_KEYS;
}


std::string game::GetVerbalPlayerAlignment()
{
  long Sum = 0;

  for(ushort c = 1; c < game::GetGods(); c++)
    {
      if(GetGod(c)->GetRelation() > 0)
	Sum += GetGod(c)->GetRelation() * (5 - GetGod(c)->Alignment());
    }

  if(Sum > 15000)
    return std::string("extremely lawful");
  if(Sum > 10000)
    return std::string("very lawful");
  if(Sum > 5000)
    return std::string("lawful");
  if(Sum > 1000)
    return std::string("mildly lawful");
  if(Sum > -1000)
    return std::string("neutral");
  if(Sum > -5000)
    return std::string("mildly chaotic");
  if(Sum > -10000)
    return std::string("chaotic");
  if(Sum > -15000)
    return std::string("very chaotic");

  return std::string("extremely chaotic");
}

void game::CreateGods()
{
  God.resize(1, 0);

  for(ushort c = 1; c <= protocontainer<god>::GetProtoAmount(); ++c)
    God.push_back(protocontainer<god>::GetProto(c)->Clone());
}

/* 
   This function gets a position from the current map and then returns it.
   However, if the player presses ESC then it returns vector2d(-1,-1)
*/
vector2d game::PositionQuestion(std::string Question, vector2d CurrentPos)
{
  int Key = 0;
  game::DrawEverythingNoBlit();
  EMPTY_MESSAGES();
  ADD_MESSAGE(Question.c_str());
  DRAW_MESSAGES();
  
  while(true)
    {
      DOUBLEBUFFER->Fill((CurrentPos.X - game::GetCamera().X) << 4, (CurrentPos.Y - game::GetCamera().Y + 2) << 4, 16, 16, 0);
      graphics::BlitDBToScreen();
      Key = GETKEY();
      if(Key == FK_ESC)
	return vector2d(-1,-1);
      if(Key == ' ')
	return CurrentPos;

      vector2d DirectionVector = GetDirectionVectorForKey(Key); 

      if(DirectionVector != vector2d(0,0))
	{
	  CurrentPos += DirectionVector;
	  if(short(CurrentPos.X) > game::GetCurrentArea()->GetXSize()-1) CurrentPos.X = 0;
	  if(short(CurrentPos.X) < 0) 			CurrentPos.X = game::GetCurrentArea()->GetXSize()-1;
	  if(short(CurrentPos.Y) > game::GetCurrentArea()->GetYSize()-1) CurrentPos.Y = 0;
	  if(short(CurrentPos.Y) < 0) 			CurrentPos.Y = game::GetCurrentArea()->GetYSize()-1;
	}
      if(CurrentPos.X < game::GetCamera().X + 2 || CurrentPos.X > game::GetCamera().X + 48)
	game::UpdateCameraXWithPos(CurrentPos.X);

      if(CurrentPos.Y < game::GetCamera().Y + 2 || CurrentPos.Y > game::GetCamera().Y + 27)
	game::UpdateCameraYWithPos(CurrentPos.Y);
    }
	


}
