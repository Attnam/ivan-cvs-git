#include <ctime>
#include <algorithm>
#include <cstdarg>

#ifdef LINUX
//#include <cstdlib>
#include <sys/stat.h>
#endif

#ifdef __DJGPP__
#include <sys/stat.h>
#endif

#ifdef WIN32
#include <direct.h>
#endif

#include "lterra.h"
#include "whandler.h"
#include "festring.h"
#include "hscore.h"
#include "colorbit.h"
#include "message.h"
#include "feio.h"
#include "team.h"
#include "config.h"
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

#define SAVE_FILE_VERSION 111 // Increment this if changes make savefiles incompatible

#define LOADED 0
#define NEW_GAME 1
#define BACK 2

class quitrequest { };

bool configid::operator<(const configid& CI) const
{
  return femath::CompareBits(this, &CI, sizeof(configid));
}

uchar game::CurrentLevelIndex;
long game::BaseScore;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
dungeon** game::Dungeon;
uchar game::CurrentDungeonIndex;
ulong game::NextCharacterID = 1;
ulong game::NextItemID = 1;
ulong game::NextExplosionID = 1;
team** game::Team;
ulong game::LOSTurns;
vector2d game::ScreenSize(42, 26);
vector2d game::CursorPos(-1, -1);
bool game::Zoom;
ulong game::CurrentEmitterEmitation;
long game::CurrentEmitterPosX;
long game::CurrentEmitterPosY;
vector2d game::CurrentEmitterPos;
bool game::Generating = false;
float game::AveragePlayerArmStrength;
float game::AveragePlayerLegStrength;
float game::AveragePlayerDexterity;
float game::AveragePlayerAgility;
uchar game::Teams;
uchar game::Dungeons;
uchar game::StoryState;

bool game::Loading = false;
bool game::InGetCommand = false;
character* game::Petrus = 0;
character* game::Haedlac = 0;

std::string game::AutoSaveFileName = game::GetSaveDir() + "AutoSave";
std::string game::Alignment[] = { "L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--" };
std::string game::LockDescription[] = { "round", "square", "triangular", "broken" };
god** game::God;

int game::MoveCommandKey[] = { KEY_HOME, KEY_UP, KEY_PAGE_UP, KEY_LEFT, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, '.' };
const vector2d game::MoveVector[] = { vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1), vector2d(0, 0) };
const vector2d game::RelativeMoveVector[] = { vector2d(-1, -1), vector2d(1, 0), vector2d(1, 0), vector2d(-2, 1), vector2d(2, 0), vector2d(-2, 1), vector2d(1, 0), vector2d(1, 0), vector2d(-1, -1) };

bool game::LOSUpdateRequested = false;
ushort*** game::LuxTable = 0;
ushort* game::LuxTableSize = 0;
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
dangermap game::DangerMap;
configid game::NextDangerId;
characteridmap game::CharacterIDMap;
const explosion* game::CurrentExplosion;
bool game::PlayerHurtByExplosion;

vector2d game::CalculateScreenCoordinates(vector2d Pos) { return (Pos - Camera + vector2d(1, 2)) << 4; }
void game::AddCharacterID(character* Char, ulong ID) { CharacterIDMap.insert(std::pair<ulong, character*>(ID, Char)); }
void game::RemoveCharacterID(ulong ID) { CharacterIDMap.erase(CharacterIDMap.find(ID)); }

void game::InitScript()
{
  inputfile ScriptFile(game::GetGameDir() + "Script/dungeon.dat", &GlobalValueMap);
  delete GameScript;
  GameScript = new gamescript;
  GameScript->ReadFrom(ScriptFile);
}

/*#include "materias.h"
#include "item.h"
#include "stack.h"*/

bool game::Init(const std::string& Name)
{
  std::string PlayerName;

  if(!Name.length())
    if(!configuration::GetDefaultName().length())
      {
	PlayerName = iosystem::StringQuestion("What is your name? (1-20 letters)", vector2d(30, 46), WHITE, 1, 20, true, true);

	if(!PlayerName.length())
	  return false;
      }
    else
      PlayerName = configuration::GetDefaultName();
  else
    PlayerName = Name;

#ifdef WIN32
  _mkdir("Save");
#endif

#ifdef __DJGPP__
  mkdir("Save", S_IWUSR);
#endif

#ifdef LINUX
  mkdir(GetSaveDir().c_str(), S_IRWXU | S_IRWXG);
#endif

  switch(Load(SaveName(PlayerName)))
    {
    case LOADED:
      {
	globalwindowhandler::InstallControlLoop(AnimationController);
	SetIsRunning(true);
	SetIsLoading(true);
	GetCurrentArea()->SendNewDrawRequest();
	game::SendLOSUpdateRequest();
	ADD_MESSAGE("Game loaded successfully.");
	return true;
      }
    case NEW_GAME:
      {
	iosystem::TextScreen( "You couldn't possibly have guessed this day would differ from any other.\n"
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
			      "colony, and were led directly to him.");

	iosystem::TextScreen( "\"I have a task for you, citizen\", said the viceroy picking his golden\n"
			      "teeth, \"The market price of bananas has taken a deep dive and yet the\n"
			      "central government is about to raise taxes. I have sent appeals to high\n"
			      "priest Petrus but received no response. I fear my enemies in Attnam are\n"
			      "plotting against me and intercepting my messages before they reach him!\"\n\n"
			      "\"That is why you must travel to Attnam with a letter I'll give you and\n"
			      "deliver it to Petrus directly. Alas, you somehow have to cross the sea\n"
			      "between. Because it's winter, all Attnamian ships are trapped by ice and\n"
			      "I have none. Therefore you must venture through the small underwater tunnel\n"
			      "connecting our islands. It is infested with monsters, but since you have\n"
			      "stayed alive here so long, the trip will surely cause you no trouble.\"\n\n"
			      "You have never been so happy! According to the mansion's traveling\n"
			      "brochures, Attnam is a peaceful but bustling world city on a beautiful\n"
			      "snowy fell surrounded by frozen lakes glittering in the arctic sun just\n"
			      "like the diamonds of the imperial treasury. Not that you would believe a\n"
			      "word. The point is that tomorrow you can finally forget your home and\n"
			      "face the untold adventures ahead.");

	globalwindowhandler::InstallControlLoop(AnimationController);
	SetIsRunning(true);
	iosystem::TextScreen("Generating game...\n\nThis may take some time, please wait.", WHITE, false, &BusyAnimation);
	msgsystem::Format();
	WizardMode = false;
	SeeWholeMapCheat = false;
	GoThroughWallsCheat = false;
	LOSTurns = 1;
	CreateTeams();
	CreateGods();
	SetPlayer(new human);
	Player->SetAssignedName(PlayerName);
	Player->SetTeam(GetTeam(0));
	Player->SetNP(SATIATED_LEVEL);
	GetTeam(0)->SetLeader(Player);
	InitDangerMap();
	Petrus = 0;
	Haedlac = 0;
	InitDungeons();
	WorldMap = new worldmap(128, 128);
	WorldMap->Generate();
	InWilderness = true;
	UpdateCamera();
	game::SendLOSUpdateRequest();
	Ticks = 0;
	InitPlayerAttributeAverage();
	StoryState = 0;

	BaseScore = Player->GetScore();
	character* Doggie = new dog;
	Doggie->SetTeam(GetTeam(0));
	GetWorldMap()->GetPlayerGroup().push_back(Doggie);

	/*for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
	  {
	    const material::prototype* Proto = protocontainer<material>::GetProto(c);
	    const material::databasemap& Config = Proto->GetConfig();

	    for(material::databasemap::const_iterator i = ++Config.begin(); i != Config.end(); ++i)
	      {
		oillamp* Lamp = new oillamp(0, NO_MATERIALS);
		Lamp->InitMaterials(MAKE_MATERIAL(i->first));
		Player->GetStack()->AddItem(Lamp);
	      }
	  }*/

	ADD_MESSAGE("Game generated successfully.");
	return true;
      }
    default:
      return false;
    }
}

void game::DeInit()
{
  delete GetWorldMap();
  SetWorldMap(0);
  ushort c;

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
}

void game::Run()
{
  while(true)
    {
      if(!InWilderness)
	{
	  static ushort Counter = 0;

	  if(++Counter == 10)
	    {
	      GetCurrentDungeon()->GetLevel(CurrentLevelIndex)->GenerateMonsters(); // Temporary place
	      Counter = 0;
	    }
	}

      try
	{
	  pool::Be();
	  pool::BurnHell();

	  Tick();
	  ApplyDivineTick();
	}
      catch(quitrequest)
	{
	  break;
	}
    }
}

void game::InitLuxTable()
{
  if(!LuxTable)
    {
      LuxTable = Alloc3D<ushort>(256, 33, 33);

      for(long c = 0; c < 0x100; ++c)
	for(long x = 0; x < 33; ++x)
	  for(long y = 0; y < 33; ++y)
	    LuxTable[c][x][y] = ushort(c / (float(HypotSquare(x - 16, y - 16)) / 128 + 1));

      atexit(game::DeInitLuxTable);
    }
}

void game::DeInitLuxTable()
{
  delete [] LuxTable;
  LuxTable = 0;
}

bool game::WorldMapLOSHandler(long X, long Y)
{
  GetWorldMap()->GetWSquare(X, Y)->SetLastSeen(LOSTurns);
  return true;
}

bool game::LevelLOSHandler(long X, long Y)
{
  lsquare* Square = GetCurrentDungeon()->GetLevel(CurrentLevelIndex)->GetLSquare(X, Y);
  Square->SetLastSeen(LOSTurns);
  return Square->GetOLTerrain()->IsWalkable();
}

void game::UpdateCameraX()
{
  if(GetCurrentArea()->GetXSize() <= GetScreenSize().X || Player->GetPos().X < GetScreenSize().X >> 1)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else if(Player->GetPos().X > GetCurrentArea()->GetXSize() - (GetScreenSize().X >> 1))
    if(Camera.X == GetCurrentArea()->GetXSize() - GetScreenSize().X)
      return;
    else
      Camera.X = GetCurrentArea()->GetXSize() - GetScreenSize().X;
  else
    if(Camera.X == Player->GetPos().X - (GetScreenSize().X >> 1))
      return;
    else
      Camera.X = Player->GetPos().X - (GetScreenSize().X >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraY()
{
  if(GetCurrentArea()->GetYSize() <= GetScreenSize().Y || Player->GetPos().Y < GetScreenSize().Y >> 1)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else if(Player->GetPos().Y > GetCurrentArea()->GetYSize() - (GetScreenSize().Y >> 1))
    if(Camera.Y == GetCurrentArea()->GetYSize() - GetScreenSize().Y)
      return;
    else
      Camera.Y = GetCurrentArea()->GetYSize() - GetScreenSize().Y;
  else
    if(Camera.Y == Player->GetPos().Y - (GetScreenSize().Y >> 1))
      return;
    else
      Camera.Y = Player->GetPos().Y - (GetScreenSize().Y >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

const char* game::Insult()
{
  switch(RAND() % 16)
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

void game::DrawEverythingNoBlit(bool AnimationDraw)
{
  if(LOSUpdateRequested)
    GetCurrentArea()->UpdateLOS();

  if(OnScreen(CursorPos))
    if(!IsInWilderness() || GetWorldMap()->GetSquare(CursorPos)->GetLastSeen() || SeeWholeMapCheatIsActive())
      GetCurrentArea()->GetSquare(CursorPos)->SendNewDrawRequest();
    else
      DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), 16, 16, 0);

  globalwindowhandler::UpdateTick();
  GetCurrentArea()->Draw();

  if(OnScreen(GetPlayer()->GetPos()))
    igraph::DrawCursor(CalculateScreenCoordinates(GetPlayer()->GetPos()));

  GetPlayer()->DrawPanel(AnimationDraw);

  if(!AnimationDraw)
    msgsystem::Draw();

  if(OnScreen(CursorPos))
    {
      vector2d ScreenCordinates = CalculateScreenCoordinates(CursorPos);

      if(DoZoom())
	DOUBLE_BUFFER->StretchBlit(DOUBLE_BUFFER, ScreenCordinates, RES.X - 96, RES.Y - 96, 16, 16, 5);

      igraph::DrawCursor(ScreenCordinates);
    }
}

bool game::Save(const std::string& SaveName)
{
  outputfile SaveFile(SaveName + ".sav");
  SaveFile << ushort(SAVE_FILE_VERSION);
  SaveFile << CurrentDungeonIndex << CurrentLevelIndex << Camera << WizardMode << SeeWholeMapCheat;
  SaveFile << GoThroughWallsCheat << BaseScore << Ticks << InWilderness << NextCharacterID << NextItemID;
  SaveFile << LOSTurns << femath::GetSeed();
  SaveFile << AveragePlayerArmStrength << AveragePlayerLegStrength << AveragePlayerDexterity << AveragePlayerAgility;
  SaveFile << Teams << Dungeons << StoryState;
  ushort c;

  for(c = 1; c < Dungeons; ++c)
    SaveFile << Dungeon[c];

  for(c = 1; c <= GODS; ++c)
    SaveFile << God[c];

  for(c = 0; c < Teams; ++c)
    SaveFile << Team[c];

  if(InWilderness)
    SaveWorldMap(SaveName);
  else
    GetCurrentDungeon()->SaveLevel(SaveName, CurrentLevelIndex, false);

  SaveFile << PLAYER->GetPos();
  msgsystem::Save(SaveFile);
  SaveFile << DangerMap << NextDangerId;
  return true;
}

uchar game::Load(const std::string& SaveName)
{
  inputfile SaveFile(SaveName + ".sav", 0, false);

  if(!SaveFile.IsOpen())
    return NEW_GAME;

  ushort Version;
  SaveFile >> Version;

  if(Version != SAVE_FILE_VERSION)
    {
      if(!iosystem::Menu(0, RES >> 1, "Sorry, this save is incompatible with the new version.\rStart new game?\r","Yes\rNo\r", LIGHT_GRAY))
	return NEW_GAME;
      else
	return BACK;
    }

  SaveFile >> CurrentDungeonIndex >> CurrentLevelIndex >> Camera >> WizardMode >> SeeWholeMapCheat;
  SaveFile >> GoThroughWallsCheat >> BaseScore >> Ticks >> InWilderness >> NextCharacterID >> NextItemID;
  SaveFile >> LOSTurns;
  femath::SetSeed(ReadType<ulonglong>(SaveFile));
  SaveFile >> AveragePlayerArmStrength >> AveragePlayerLegStrength >> AveragePlayerDexterity >> AveragePlayerAgility;
  SaveFile >> Teams >> Dungeons >> StoryState;
  ushort c;

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
    LoadWorldMap(SaveName);
  else
    GetCurrentDungeon()->LoadLevel(SaveName, CurrentLevelIndex);

  vector2d Pos;
  SaveFile >> Pos;
  SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());
  msgsystem::Load(SaveFile);
  SaveFile >> DangerMap >> NextDangerId;
  return LOADED;
}

std::string game::SaveName(const std::string& Base)
{
  std::string SaveName = GetSaveDir();

  if(!Base.length())
    SaveName << PLAYER->GetAssignedName();
  else
    SaveName << Base;

  for(ushort c = 0; c < SaveName.length(); ++c)
    if(SaveName[c] == ' ')
      SaveName[c] = '_';

#if defined(WIN32) || defined(__DJGPP__)
  if(SaveName.length() > 13)
    SaveName.resize(13);
#endif

  return SaveName;
}

bool game::EmitationHandler(long X, long Y)
{
  ulong Emit;

  if(X - CurrentEmitterPosX > 16 || CurrentEmitterPosX - X > 16 || Y - CurrentEmitterPosY > 16 || CurrentEmitterPosY - Y > 16)
    Emit = 0;
  else
    {
      Emit = CurrentEmitterEmitation;
      uchar Red = LuxTable[GetRed24(Emit)][X - CurrentEmitterPosX + 16][Y - CurrentEmitterPosY + 16];
      uchar Green = LuxTable[GetGreen24(Emit)][X - CurrentEmitterPosX + 16][Y - CurrentEmitterPosY + 16];
      uchar Blue = LuxTable[GetBlue24(Emit)][X - CurrentEmitterPosX + 16][Y - CurrentEmitterPosY + 16];
      Emit = MakeRGB24(Red, Green, Blue);
    }

  lsquare* Square = GetCurrentDungeon()->GetLevel(CurrentLevelIndex)->GetLSquare(X, Y);
  Square->AlterLuminance(CurrentEmitterPos, Emit);
  return Square->GetOLTerrain()->IsWalkable();
}

bool game::NoxifyHandler(long X, long Y)
{
  lsquare* Square = GetCurrentDungeon()->GetLevel(CurrentLevelIndex)->GetLSquare(X, Y);
  Square->NoxifyEmitter(CurrentEmitterPos);
  return Square->GetOLTerrain()->IsWalkable();
}

void game::UpdateCameraXWithPos(ushort Coord)
{
  if(GetCurrentArea()->GetXSize() <= GetScreenSize().X || Coord < GetScreenSize().X >> 1)
    if(!Camera.X)
      return;
    else
      Camera.X = 0;
  else if(Coord > GetCurrentArea()->GetXSize() - (GetScreenSize().X >> 1))
    if(Camera.X == GetCurrentArea()->GetXSize() - GetScreenSize().X)
      return;
    else
      Camera.X = GetCurrentArea()->GetXSize() - GetScreenSize().X;
  else
    if(Camera.X == Coord - (GetScreenSize().X >> 1))
      return;
    else
      Camera.X = Coord - (GetScreenSize().X >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraYWithPos(ushort Coord)
{
  if(GetCurrentArea()->GetYSize() <= GetScreenSize().Y || Coord < GetScreenSize().Y >> 1)
    if(!Camera.Y)
      return;
    else
      Camera.Y = 0;
  else if(Coord > GetCurrentArea()->GetYSize() - (GetScreenSize().Y >> 1))
    if(Camera.Y == GetCurrentArea()->GetYSize() - GetScreenSize().Y)
      return;
    else
      Camera.Y = GetCurrentArea()->GetYSize() - GetScreenSize().Y;
  else
    if(Camera.Y == Coord - (GetScreenSize().Y >> 1))
      return;
    else
      Camera.Y = Coord - (GetScreenSize().Y >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

int game::GetMoveCommandKeyBetweenPoints(vector2d A, vector2d B)
{
  for(ushort c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if((A + game::GetMoveVector(c)) == B)
      return game::MoveCommandKey[c];

  return DIR_ERROR;
}

void game::ApplyDivineTick()
{
  for(ushort c = 1; c <= GODS; ++c)
    GetGod(c)->ApplyDivineTick();
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, bool Good, short Multiplier)
{
  for(ushort c = 1; c <= GODS; ++c)
    if(GetGod(c) != CompareTarget)
      GetGod(c)->AdjustRelation(CompareTarget, Good, Multiplier);
}

vector2d game::GetDirectionVectorForKey(int Key)
{
  if(Key == KEY_NUMPAD_5)
    return vector2d(0,0);

  for(ushort c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if(Key == game::GetMoveCommandKey(c))
      return game::GetMoveVector(c);

  return ERROR_VECTOR;
}

bool game::EyeHandler(long X, long Y)
{
  return GetCurrentDungeon()->GetLevel(CurrentLevelIndex)->GetLSquare(X, Y)->GetOLTerrain()->IsWalkable();
}

long game::GodScore()
{
  long Score = -1000;

  for(ushort c = 1; c <= GODS; ++c)
    if(GetGod(c)->GetRelation() > Score)
      Score = GetGod(c)->GetRelation();

  return Score;
}

float game::Difficulty()
{
  float Base = float(GetCurrentLevel()->GetDifficulty()) / 1000;

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
  if(GetLevel(GetCurrentLevelIndex())->GetLevelMessage().length())
    ADD_MESSAGE(GetLevel(GetCurrentLevelIndex())->GetLevelMessage().c_str());

  GetLevel(GetCurrentLevelIndex())->SetLevelMessage("");
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
  std::string File;

  for(ushort i = 1; i < Dungeons; ++i)
    for(ushort c = 0; c < GetDungeon(i)->GetLevels(); ++c)
      {
	/*
	 * This looks very odd. And it is very odd.
	 * Indeed, gcc is very odd to not compile this correctly with -O3
	 * if it is written in a less odd way.
	 */

	File = SaveName() + "." + i;
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
  return !InWilderness ? static_cast<area*>(GetCurrentDungeon()->GetLevel(CurrentLevelIndex)) : static_cast<area*>(WorldMap);
}

level* game::GetCurrentLevel()
{
  return GetCurrentDungeon()->GetLevel(CurrentLevelIndex);
}

level* game::GetLevel(ushort Index)
{
  return GetCurrentDungeon()->GetLevel(Index);
}

ushort game::GetLevels()
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
  Dungeons = *GetGameScript()->GetDungeons() + 1;
  Dungeon = new dungeon*[Dungeons];
  Dungeon[0] = 0;

  for(ushort c = 1; c < Dungeons; ++c)
    {
      Dungeon[c] = new dungeon(c);
      Dungeon[c]->SetIndex(c);
    }
}

void game::DoEvilDeed(ushort Amount)
{
  if(!Amount)
    return;

  for(ushort c = 1; c <= GODS; ++c)
    {
      short Change = Amount - Amount * GetGod(c)->GetAlignment() / 5;

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

void game::SaveWorldMap(const std::string& SaveName, bool DeleteAfterwards)
{
  outputfile SaveFile(SaveName + ".wm");
  SaveFile << WorldMap;

  if(DeleteAfterwards)
    {
      delete WorldMap;
      WorldMap = 0;
    }
}

void game::LoadWorldMap(const std::string& SaveName)
{
  inputfile SaveFile(SaveName + ".wm");
  SaveFile >> WorldMap;
}

void game::Hostility(team* Attacker, team* Defender)
{
  for(ushort c = 0; c < Teams; ++c)
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
  Teams = *GetGameScript()->GetTeams();
  Team = new team*[Teams];
  ushort c;

  for(c = 0; c < Teams; ++c)
    {
      Team[c] = new team(c);

      for(ushort i = 0; i < c; ++i)
	Team[i]->SetRelation(Team[c], UNCARING);
    }

  for(c = 0; c < Teams; ++c)
    if(c != 1)
      Team[1]->SetRelation(Team[c], HOSTILE);

  const std::vector<std::pair<uchar, teamscript*> >& TeamScript = GetGameScript()->GetTeam();

  for(c = 0; c < TeamScript.size(); ++c)
  {
    for(ushort i = 0; i < TeamScript[c].second->GetRelation().size(); ++i)
      GetTeam(TeamScript[c].second->GetRelation()[i].first)->SetRelation(GetTeam(TeamScript[c].first), TeamScript[c].second->GetRelation()[i].second);

    ushort* KillEvilness = TeamScript[c].second->GetKillEvilness();

    if(KillEvilness)
      GetTeam(TeamScript[c].first)->SetKillEvilness(*KillEvilness);
  }
}

std::string game::StringQuestion(const std::string& Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool AllowExit)
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0); // pos may be incorrect!
  std::string Return = iosystem::StringQuestion(Topic, Pos, Color, MinLetters, MaxLetters, false, AllowExit);
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

long game::NumberQuestion(const std::string& Topic, vector2d Pos, ushort Color)
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  long Return = iosystem::NumberQuestion(Topic, Pos, Color, false);
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

long game::ScrollBarQuestion(const std::string& Topic, vector2d Pos, long BeginValue, long Step, long Min, long Max, long AbortValue, ushort TopicColor, ushort Color1, ushort Color2, void (*Handler)(long))
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  long Return = iosystem::ScrollBarQuestion(Topic, Pos, BeginValue, Step, Min, Max, AbortValue, TopicColor, Color1, Color2, false, Handler);
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

void game::LOSTurn()
{
  if(LOSTurns++ == 0xFFFFFFFF)
    ABORT("Suddenly the Universe explodes!");
}

void game::UpdateCamera()
{
  if(GetCurrentArea()->GetXSize() <= GetScreenSize().X || Player->GetPos().X < GetScreenSize().X >> 1)
    Camera.X = 0;
  else if(Player->GetPos().X > GetCurrentArea()->GetXSize() - (GetScreenSize().X >> 1))
    Camera.X = GetCurrentArea()->GetXSize() - GetScreenSize().X;
  else
    Camera.X = Player->GetPos().X - (GetScreenSize().X >> 1);

  if(GetCurrentArea()->GetYSize() <= GetScreenSize().Y || Player->GetPos().Y < GetScreenSize().Y >> 1)
    Camera.Y = 0;
  else if(Player->GetPos().Y > GetCurrentArea()->GetYSize() - (GetScreenSize().Y >> 1))
    Camera.Y = GetCurrentArea()->GetYSize() - GetScreenSize().Y;
  else
    Camera.Y = Player->GetPos().Y - (GetScreenSize().Y >> 1);

  GetCurrentArea()->SendNewDrawRequest();
}

bool game::HandleQuitMessage()
{
  if(IsRunning())
    {
      if(IsInGetCommand())
	{
	  switch(game::Menu(0, RES >> 1, "Do you want to save your game before quitting?\r","Yes\rNo\rCancel\r", LIGHT_GRAY))
	    {
	    case 0:
	      Save();
	      game::RemoveSaves(false);
	      break;
	    case 2:
	      GetCurrentArea()->SendNewDrawRequest();
	      game::DrawEverything();
	      return false;
	    default:
	      GetPlayer()->AddScoreEntry("cowardly quit the game", 0.75f);
	      End(true, false);
	      break;
	    }
	}
      else
	if(game::Menu(0, RES >> 1, "You can't save at this point. Are you sure you still want to do this?", "Yes\rNo\r", LIGHT_GRAY))
	  RemoveSaves();
	else
	  return false;
    }

  return true;
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

  for(ushort c = 1; c <= GODS; ++c)
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

  for(ushort c = 1; c < protocontainer<god>::GetProtoAmount(); ++c)
    God[c] = protocontainer<god>::GetProto(c)->Clone();
}

void game::BusyAnimation()
{
  BusyAnimation(DOUBLE_BUFFER);
}

void game::BusyAnimation(bitmap* Buffer)
{
  static bitmap Elpuri(16, 16, TRANSPARENT_COLOR);
  static bool ElpuriLoaded = false;
  static double Rotation = 0;
  static clock_t LastTime = 0;

  if(clock() - LastTime > CLOCKS_PER_SEC >> 5)
    {
      if(!ElpuriLoaded)
	{
	  ushort Color = MakeRGB16(60, 60, 60);
	  igraph::GetCharacterRawGraphic()->MaskedBlit(&Elpuri, 64, 0, 0, 0, 16, 16, &Color);
	  ElpuriLoaded = true;
	}

      vector2d Pos(RES.X >> 1, (RES.Y << 1) / 3);
      Buffer->Fill(Pos.X - 100, Pos.Y - 100, 200, 200, 0);
      Rotation += 0.02;

      if(Rotation > 2 * FPI)
	Rotation -= 2 * FPI;

      Elpuri.MaskedBlit(Buffer, 0, 0, Pos.X - 8, Pos.Y - 7, 16, 16);
      ushort x;

      for(x = 0; x < 10; ++x)
	Buffer->DrawPolygon(Pos, 100, 5, MakeRGB16(255 - 25 * (10 - x),0,0), false, true, Rotation + double(x) / 50);

      for(x = 0; x < 4; ++x)
	Buffer->DrawPolygon(Pos, 100 + x, 50, MakeRGB16(255 - 12 * x,0,0));

      if(Buffer == DOUBLE_BUFFER)
	graphics::BlitDBToScreen();

      LastTime = clock();
    }
}

int game::AskForKeyPress(const std::string& Topic)
{
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", festring::CapitalizeCopy(Topic).c_str());
  graphics::BlitDBToScreen();
  int Key = GET_KEY();
  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Key;
}

/* 
 * Handler is called when the key has been identified as a movement key 
 * KeyHandler is called when the key has NOT been identified as a movement key
 * Both can be deactivated by passing 0 as parameter
 */  

vector2d game::PositionQuestion(const std::string& Topic, vector2d CursorPos, void (*Handler)(vector2d), void (*KeyHandler)(vector2d, int), bool Zoom)
{
  int Key = 0;
  graphics::BlitDBToScreen();
  SetDoZoom(Zoom);
  vector2d Return;

  while(true)
    {
      square* Square = GetCurrentArea()->GetSquare(CursorPos);

      if(!Square->GetLastSeen() && (!Square->GetCharacter() || !Square->GetCharacter()->CanBeSeenByPlayer()) && !SeeWholeMapCheatIsActive())
	DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), vector2d(16, 16), BLACK);
      else
	GetCurrentArea()->GetSquare(CursorPos)->SendNewDrawRequest();

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

	  if(short(CursorPos.X) > GetCurrentArea()->GetXSize() - 1) CursorPos.X = 0;
	  if(short(CursorPos.X) < 0) CursorPos.X = GetCurrentArea()->GetXSize() - 1;
	  if(short(CursorPos.Y) > GetCurrentArea()->GetYSize() - 1) CursorPos.Y = 0;
	  if(short(CursorPos.Y) < 0) CursorPos.Y = GetCurrentArea()->GetYSize() - 1;

	  if(Handler)
	    Handler(CursorPos);
	}
      else if(KeyHandler)
	KeyHandler(CursorPos, Key);

      if(CursorPos.X < GetCamera().X + 2 || CursorPos.X >= GetCamera().X + GetScreenSize().X - 2)
	UpdateCameraXWithPos(CursorPos.X);

      if(CursorPos.Y < GetCamera().Y + 2 || CursorPos.Y >= GetCamera().Y + GetScreenSize().Y - 2)
	UpdateCameraYWithPos(CursorPos.Y);

      FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", Topic.c_str());
      SetCursorPos(CursorPos);
      DrawEverythingNoBlit();
      graphics::BlitDBToScreen();
      Key = GET_KEY();
    }

  DOUBLE_BUFFER->Fill(16, 6, GetScreenSize().X << 4, 23, BLACK);
  DOUBLE_BUFFER->Fill(RES.X - 96, RES.Y - 96, 80, 80, BLACK);
  SetDoZoom(false);
  SetCursorPos(vector2d(-1, -1));
  DrawEverythingNoBlit();
  return Return;
}

void game::LookHandler(vector2d CursorPos)
{
  std::string OldMemory;

  if(game::SeeWholeMapCheatIsActive())
    {
      OldMemory = game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription();

      if(IsInWilderness())
	GetWorldMap()->GetWSquare(CursorPos)->UpdateMemorizedDescription(true);
      else
	GetCurrentLevel()->GetLSquare(CursorPos)->UpdateMemorizedDescription(true);
    }

  std::string Msg;

  if(game::GetCurrentArea()->GetSquare(CursorPos)->GetLastSeen() || game::SeeWholeMapCheatIsActive())
    {
      if(game::GetCurrentArea()->GetSquare(CursorPos)->CanBeSeenByPlayer(true) || game::SeeWholeMapCheatIsActive())
	Msg = "You see here ";
      else
	Msg = "You remember here ";

      Msg << game::GetCurrentArea()->GetSquare(CursorPos)->GetMemorizedDescription() << ".";
    }
  else
    Msg = "You have never been here.";

  character* Character = game::GetCurrentArea()->GetSquare(CursorPos)->GetCharacter();

  if(Character && (Character->CanBeSeenByPlayer() || game::SeeWholeMapCheatIsActive()))
    Character->DisplayInfo(Msg);

  if(!(RAND() % 10000))
    Msg << " You see here a frog eating a magnolia.";

  ADD_MESSAGE("%s", Msg.c_str());

  if(game::SeeWholeMapCheatIsActive())
    game::GetCurrentArea()->GetSquare(CursorPos)->SetMemorizedDescription(OldMemory);
}

bool game::AnimationController()
{
  game::DrawEverythingNoBlit(true);
  return true;
}

void game::InitGlobalValueMap()
{
  inputfile SaveFile(GetGameDir() + "Script/define.dat", &GlobalValueMap);
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word != "#" || SaveFile.ReadWord() != "define")
	ABORT("Illegal datafile define on line %d!", SaveFile.TellLine());

      SaveFile.ReadWord(Word);
      GlobalValueMap.insert(std::pair<std::string, long>(Word, SaveFile.ReadNumber()));
    }
}

void game::TextScreen(const std::string& Text, ushort Color, bool GKey, void (*BitmapEditor)(bitmap*))
{
  globalwindowhandler::DeInstallControlLoop(AnimationController);
  iosystem::TextScreen(Text, Color, GKey, BitmapEditor);
  globalwindowhandler::InstallControlLoop(AnimationController);
}

/* ... all the keys that are acceptable 
   DefaultAnswer = REQUIRES_ANSWER if this question requires an answer 
   Not surprisingly KeyNumber is the number of keys at ...
*/

int game::KeyQuestion(const std::string& Message, int DefaultAnswer, int KeyNumber, ...)
{
  std::vector<int> KeyVector;
  va_list Arguments;
  va_start(Arguments, KeyNumber);

  for(ushort c = 0; c < KeyNumber; ++c)
    KeyVector.push_back(va_arg(Arguments, int));

  va_end(Arguments);

  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", Message.c_str());
  graphics::BlitDBToScreen();
  int Return;

  while(true)
    {
      int k = GET_KEY();
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

  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenSize().X << 4, 23, 0);
  return Return;
}

void game::LookKeyHandler(vector2d CursorPos, int Key)
{
  square* Square = GetCurrentArea()->GetSquare(CursorPos);

  switch(Key)
    {
    case 'i':
      if(!IsInWilderness())
	if(Square->CanBeSeenByPlayer() || CursorPos == GetPlayer()->GetPos() || game::SeeWholeMapCheatIsActive())
	  {
	    stack* Stack = game::GetCurrentLevel()->GetLSquare(CursorPos)->GetStack();

	    if(Square->GetOTerrain()->IsWalkable() && Stack->GetVisibleItems(PLAYER))
	      Stack->DrawContents(PLAYER, "Items here", NO_SELECT|(SeeWholeMapCheatIsActive() ? 0 : NO_SPECIAL_INFO));
	    else
	      ADD_MESSAGE("You see no items here.");
	  }
	else
	  ADD_MESSAGE("You should perhaps move a bit closer.");

      break;
    case 'c':
      if(Square->CanBeSeenByPlayer() || CursorPos == GetPlayer()->GetPos() || game::SeeWholeMapCheatIsActive())
	{
	  character* Char = Square->GetCharacter();

	  if(Char && (Char->CanBeSeenByPlayer() || Char->IsPlayer() || game::SeeWholeMapCheatIsActive()))
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
      character* Character = game::GetCurrentArea()->GetSquare(CursorPos)->GetCharacter();

      if(Character && Character->CanBeSeenByPlayer())
	{
	  if(Character->GetTeam() != GetPlayer()->GetTeam())
	    ADD_MESSAGE("%s refuses to let YOU decide what %s's called.", Character->CHAR_NAME(DEFINITE), Character->GetPersonalPronoun().c_str());
	  else if(Character->IsPlayer())
	    ADD_MESSAGE("You can't rename yourself!");
	  else if(!Character->IsNameable())
	    ADD_MESSAGE("%s refuses to be called anything else but %s.", Character->CHAR_NAME(DEFINITE), Character->CHAR_NAME(DEFINITE));
	  else
	    {
	      std::string Topic = "What name will you give to " + Character->GetName(DEFINITE) + "?";
	      std::string Name = StringQuestion(Topic, vector2d(16, 6), WHITE, 0, 80, true);

	      if(Name.length())
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

  if(Permanently || !WizardModeIsActive())
    RemoveSaves(Permanently);

  if(Permanently && !WizardModeIsActive())
    {
      highscore HScore;

      if(HScore.LastAddFailed())
	iosystem::TextScreen("You didn't manage to get onto the high score list.");
      else
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

void game::SetCurrentEmitterPos(vector2d What)
{
  CurrentEmitterPos = What;
  CurrentEmitterPosX = What.X;
  CurrentEmitterPosY = What.Y;
}

int game::Menu(bitmap* BackGround, vector2d Pos, const std::string& Topic, const std::string& sMS, ushort Color, const std::string& SmallText1, const std::string& SmallText2)
{
  globalwindowhandler::DeInstallControlLoop(AnimationController);
  int Return = iosystem::Menu(BackGround, Pos, Topic, sMS, Color, SmallText1, SmallText2);
  globalwindowhandler::InstallControlLoop(AnimationController);
  return Return;
}

void game::InitDangerMap()
{
  bool First = true;

  for(ushort c = 1; c < protocontainer<character>::GetProtoAmount(); ++c)
    {
      game::BusyAnimation();
      const character::prototype* Proto = protocontainer<character>::GetProto(c);
      const character::databasemap& Config = Proto->GetConfig();

      for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract && i->second.CanBeGenerated)
	  {
	    if(First)
	      {
		NextDangerId.Type = c;
		NextDangerId.Config = i->first;
		First = false;
	      }

	    character* Char = Proto->Clone(i->first, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	    DangerMap.insert(std::pair<configid, dangerid>(configid(c, i->first), dangerid(Char->GetRelativeDanger(Player, true))));
	    delete Char;
	  }
    }
}

void game::CalculateNextDanger()
{
  if(IsInWilderness() || !*GetCurrentLevel()->GetLevelScript()->GenerateMonsters())
    return;

  const character::prototype* Proto = protocontainer<character>::GetProto(NextDangerId.Type);
  const character::databasemap& Config = Proto->GetConfig();
  character::databasemap::const_iterator ConfigIterator = Config.find(NextDangerId.Config);
  dangermap::iterator DangerIterator = DangerMap.find(NextDangerId);

  if(ConfigIterator != Config.end() && DangerIterator != DangerMap.end())
    {
      character* Char = Proto->Clone(NextDangerId.Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
      DangerIterator->second.Danger = (DangerIterator->second.Danger * 9 + Char->GetRelativeDanger(Player, true)) / 10;
      delete Char;

      for(++ConfigIterator; ConfigIterator != Config.end(); ++ConfigIterator)
	if(!ConfigIterator->second.IsAbstract && ConfigIterator->second.CanBeGenerated)
	  {
	    NextDangerId.Config = ConfigIterator->first;
	    return;
	  }

      while(true)
	{
	  if(NextDangerId.Type < protocontainer<character>::GetProtoAmount() - 1)
	    ++NextDangerId.Type;
	  else
	    NextDangerId.Type = 1;

	  const character::databasemap& Config = protocontainer<character>::GetProto(NextDangerId.Type)->GetConfig();

	  for(ConfigIterator = Config.begin(); ConfigIterator != Config.end(); ++ConfigIterator)
	    if(!ConfigIterator->second.IsAbstract && ConfigIterator->second.CanBeGenerated)
	      {
		NextDangerId.Config = ConfigIterator->first;
		return;
	      }
	}
    }
  else
    ABORT("It is dangerous to go ice fishing in the summer.");
}

bool game::TryTravel(uchar Dungeon, uchar Area, uchar EntryIndex, bool AllowHostiles)
{
  std::vector<character*> Group;

  if(LeaveArea(Group, AllowHostiles))
    {
      game::SetCurrentDungeonIndex(Dungeon);
      game::EnterArea(Group, Area, EntryIndex);
      return true;
    }
  else
    return false;
}

bool game::LeaveArea(std::vector<character*>& Group, bool AllowHostiles)
{
  if(!IsInWilderness())
    {
      if(!GetCurrentLevel()->CollectCreatures(Group, Player, AllowHostiles))
	return false;

      GetCurrentLevel()->RemoveCharacter(Player->GetPos());
      GetCurrentDungeon()->SaveLevel(SaveName(), CurrentLevelIndex);
    }
  else
    {
      GetWorldMap()->RemoveCharacter(Player->GetPos());
      GetWorldMap()->GetPlayerGroup().swap(Group);
      SaveWorldMap(SaveName(), true);
    }

  return true;
}

/* Used when the player enters an area. */

void game::EnterArea(std::vector<character*>& Group, uchar Area, uchar EntryIndex)
{
  if(Area != WORLD_MAP)
    {
      SetIsInWilderness(false);
      SetCurrentLevelIndex(Area);
      bool New = !GetCurrentDungeon()->PrepareLevel(Area);
      vector2d Pos = GetCurrentLevel()->GetEntryPos(Player, EntryIndex);
      GetCurrentLevel()->GetLSquare(Pos)->KickAnyoneStandingHereAway();
      GetCurrentLevel()->AddCharacter(Pos, Player);
      GetCurrentLevel()->FiatLux();

      for(ushort c = 0; c < Group.size(); ++c)
	{
	  vector2d NPCPos = GetCurrentLevel()->GetNearestFreeSquare(Group[c], Pos);

	  if(NPCPos == ERROR_VECTOR)
	    NPCPos = GetCurrentLevel()->GetRandomSquare(Group[c]);

	  GetCurrentLevel()->AddCharacter(NPCPos, Group[c]);
	}

      bool* AutoReveal = GetCurrentLevel()->GetLevelScript()->AutoReveal();

      if(New && AutoReveal && *AutoReveal)
	GetCurrentLevel()->Reveal();

      ShowLevelMessage();
      SendLOSUpdateRequest();
      UpdateCamera();
      GetCurrentArea()->UpdateLOS();

      if(configuration::GetAutoSaveInterval())
	Save(GetAutoSaveFileName().c_str());
    }
  else
    {
      SetIsInWilderness(true);
      LoadWorldMap();
      GetWorldMap()->GetPlayerGroup().swap(Group);
      GetCurrentArea()->AddCharacter(GetWorldMap()->GetEntryPos(Player, EntryIndex), Player);
      SendLOSUpdateRequest();
      UpdateCamera();
      GetCurrentArea()->UpdateLOS();

      if(configuration::GetAutoSaveInterval())
	Save(GetAutoSaveFileName().c_str());
    }
}

char game::CompareLights(ulong L1, ulong L2)
{
  if(GetRed24(L1) > GetRed24(L2) || GetGreen24(L1) > GetGreen24(L2) || GetBlue24(L1) > GetBlue24(L2))
    return 1;
  else if(GetRed24(L1) == GetRed24(L2) || GetGreen24(L1) == GetGreen24(L2) || GetBlue24(L1) == GetBlue24(L2))
    return 0;
  else
    return -1;
}

char game::CompareLightToInt(ulong L, uchar Int)
{
  if(GetRed24(L) > Int || GetGreen24(L) > Int || GetBlue24(L) > Int)
    return 1;
  else if(GetRed24(L) == Int || GetGreen24(L) == Int || GetBlue24(L) == Int)
    return 0;
  else
    return -1;
}

void game::CombineLights(ulong& L1, ulong L2)
{
  L1 = MakeRGB24(Max(GetRed24(L1), GetRed24(L2)), Max(GetGreen24(L1), GetGreen24(L2)), Max(GetBlue24(L1), GetBlue24(L2)));
}

bool game::IsDark(ulong Light)
{
  return !Light || (GetRed24(Light) < LIGHT_BORDER && GetGreen24(Light) < LIGHT_BORDER && GetBlue24(Light) < LIGHT_BORDER);
}

void game::SetStandardListAttributes(felist& List)
{
  List.SetPos(vector2d(26, 42));
  List.SetWidth(652);
  List.SetBackColor(MakeRGB16(16, 16, 40));
  List.SetFlags(DRAW_BACKGROUND_AFTERWARDS);
}

void game::SignalGeneration(ushort Type, ushort Config)
{
  dangermap::iterator Iterator = DangerMap.find(configid(Type, Config));

  if(Iterator != DangerMap.end())
    Iterator->second.HasBeenGenerated = true;
}

void game::InitPlayerAttributeAverage()
{
  AveragePlayerArmStrength = AveragePlayerLegStrength = AveragePlayerDexterity = AveragePlayerAgility = 0;

  if(!GetPlayer()->IsHumanoid())
    return;

  humanoid* Player = static_cast<humanoid*>(GetPlayer());
  ushort Arms = 0;
  ushort Legs = 0;

  if(Player->GetRightArm() && Player->GetRightArm()->IsAlive())
    {
      AveragePlayerArmStrength += Player->GetRightArm()->GetStrength();
      AveragePlayerDexterity += Player->GetRightArm()->GetDexterity();
      ++Arms;
    }

  if(Player->GetLeftArm() && Player->GetLeftArm()->IsAlive())
    {
      AveragePlayerArmStrength += Player->GetLeftArm()->GetStrength();
      AveragePlayerDexterity += Player->GetLeftArm()->GetDexterity();
      ++Arms;
    }

  if(Player->GetRightLeg() && Player->GetRightLeg()->IsAlive())
    {
      AveragePlayerLegStrength += Player->GetRightLeg()->GetStrength();
      AveragePlayerAgility += Player->GetRightLeg()->GetAgility();
      ++Legs;
    }

  if(Player->GetLeftLeg() && Player->GetLeftLeg()->IsAlive())
    {
      AveragePlayerLegStrength += Player->GetLeftLeg()->GetStrength();
      AveragePlayerAgility += Player->GetLeftLeg()->GetAgility();
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
  float PlayerArmStrength = 0;
  float PlayerLegStrength = 0;
  float PlayerDexterity = 0;
  float PlayerAgility = 0;
  ushort Arms = 0;
  ushort Legs = 0;

  if(Player->GetRightArm() && Player->GetRightArm()->IsAlive())
    {
      PlayerArmStrength += Player->GetRightArm()->GetStrength();
      PlayerDexterity += Player->GetRightArm()->GetDexterity();
      ++Arms;
    }

  if(Player->GetLeftArm() && Player->GetLeftArm()->IsAlive())
    {
      PlayerArmStrength += Player->GetLeftArm()->GetStrength();
      PlayerDexterity += Player->GetLeftArm()->GetDexterity();
      ++Arms;
    }

  if(Player->GetRightLeg() && Player->GetRightLeg()->IsAlive())
    {
      PlayerLegStrength += Player->GetRightLeg()->GetStrength();
      PlayerAgility += Player->GetRightLeg()->GetAgility();
      ++Legs;
    }

  if(Player->GetLeftLeg() && Player->GetLeftLeg()->IsAlive())
    {
      PlayerLegStrength += Player->GetLeftLeg()->GetStrength();
      PlayerAgility += Player->GetLeftLeg()->GetAgility();
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

void game::CallForAttention(vector2d Pos, ushort Range)
{
  for(ushort c = 0; c < GetTeams(); ++c)
    {
      if(GetTeam(c)->HasEnemy())
	for(std::list<character*>::const_iterator i = GetTeam(c)->GetMember().begin(); i != GetTeam(c)->GetMember().end(); ++i)
	  if((*i)->IsEnabled())
	    {
	      ulong ThisDistance = HypotSquare(long((*i)->GetPos().X) - Pos.X, long((*i)->GetPos().Y) - Pos.Y);

	      if(ThisDistance <= Range)
		(*i)->SetWayPoint(Pos);
	    }
    }
}

outputfile& operator<<(outputfile& SaveFile, homedata* HomeData)
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
  CharacterIDMap.insert(std::pair<ulong, character*>(ID, NewChar));
  return ID;
}

character* game::SearchCharacter(ulong ID)
{
  characteridmap::iterator Iterator = CharacterIDMap.find(ID);
  return Iterator != CharacterIDMap.end() ? Iterator->second : 0;
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
  SaveFile << Value.Danger << Value.HasBeenGenerated;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, dangerid& Value)
{
  SaveFile >> Value.Danger >> Value.HasBeenGenerated;
  return SaveFile;
}

/* The program can only create directories to the deepness of one, no more... */

std::string game::GetHomeDir()
{
#ifdef LINUX
  return std::string(getenv("HOME")) + "/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

std::string game::GetSaveDir()
{
#ifdef LINUX
  return std::string(getenv("HOME")) + "/IvanSave/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "Save/";
#endif
}

std::string game::GetGameDir()
{
#ifdef LINUX
  return DATADIR "/ivan/";
#endif

#if defined(WIN32) || defined(__DJGPP__)
  return "";
#endif
}

bool game::ExplosionHandler(long X, long Y)
{
  lsquare* Square = GetCurrentDungeon()->GetLevel(CurrentLevelIndex)->GetLSquare(X, Y);
  Square->GetHitByExplosion(*CurrentExplosion);
  return Square->GetOLTerrain()->IsWalkable();
}

