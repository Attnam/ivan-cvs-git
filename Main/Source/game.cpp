#include <ctime>
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
#include "save.h"
#include "miscitem.h"

#define SAVE_FILE_VERSION 113 // Increment this if changes make savefiles incompatible

#define LOADED 0
#define NEW_GAME 1
#define BACK 2

class quitrequest { };

ushort game::CurrentLevelIndex;
long game::BaseScore;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
dungeon** game::Dungeon;
ushort game::CurrentDungeonIndex;
ulong game::NextCharacterID = 1;
ulong game::NextItemID = 1;
ulong game::NextExplosionID = 1;
team** game::Team;
ulong game::LOSTurns;
vector2d game::CursorPos(-1, -1);
bool game::Zoom;
ushort** game::CurrentRedLuxTable;
ushort** game::CurrentGreenLuxTable;
ushort** game::CurrentBlueLuxTable;
long game::CurrentEmitterPosXMinus16;
long game::CurrentEmitterPosYMinus16;
vector2d game::CurrentEmitterPos;
bool game::Generating = false;
float game::AveragePlayerArmStrength;
float game::AveragePlayerLegStrength;
float game::AveragePlayerDexterity;
float game::AveragePlayerAgility;
uchar game::Teams;
uchar game::Dungeons;
uchar game::StoryState;
massacremap game::PlayerMassacreMap;
massacremap game::PetMassacreMap;
massacremap game::MiscMassacreMap;
ulong game::PlayerMassacreAmount = 0;
ulong game::PetMassacreAmount = 0;
ulong game::MiscMassacreAmount = 0;
boneidmap game::BoneItemIDMap;
boneidmap game::BoneCharacterIDMap;
bool game::TooGreatDangerFoundBool;

bool game::Loading = false;
bool game::InGetCommand = false;
character* game::Petrus = 0;
character* game::Haedlac = 0;

festring game::AutoSaveFileName = game::GetSaveDir() + "AutoSave";
const char* const game::Alignment[] = { "L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--" };
const char* const game::LockDescription[] = { "round", "square", "triangular", "broken" };
god** game::God;

const int game::MoveCommandKey[] = { KEY_HOME, KEY_UP, KEY_PAGE_UP, KEY_LEFT, KEY_RIGHT, KEY_END, KEY_DOWN, KEY_PAGE_DOWN, '.' };
const vector2d game::MoveVector[] = { vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1), vector2d(0, 0) };
const vector2d game::RelativeMoveVector[] = { vector2d(-1, -1), vector2d(1, 0), vector2d(1, 0), vector2d(-2, 1), vector2d(2, 0), vector2d(-2, 1), vector2d(1, 0), vector2d(1, 0), vector2d(-1, -1) };

bool game::LOSUpdateRequested = false;
ushort*** game::LuxTable = 0;
ushort* game::LuxTableSize = 0;
bool game::Running;
character* game::Player;
vector2d game::Camera(0, 0);
ulong game::Ticks;
gamescript* game::GameScript = 0;
valuemap game::GlobalValueMap;
dangermap game::DangerMap;
configid game::NextDangerID;
characteridmap game::CharacterIDMap;
const explosion* game::CurrentExplosion;
bool game::PlayerHurtByExplosion;
area* game::CurrentArea;
level* game::CurrentLevel;
wsquare*** game::CurrentWSquareMap;
lsquare*** game::CurrentLSquareMap;
festring game::DefaultPolymorphTo;
bool game::WizardMode;
uchar game::SeeWholeMapCheatMode;
bool game::GoThroughWallsCheat;
bool game::QuestMonsterFoundBool;
bitmap* game::BusyAnimationCache[48];

void game::AddCharacterID(character* Char, ulong ID) { CharacterIDMap.insert(std::pair<ulong, character*>(ID, Char)); }
void game::RemoveCharacterID(ulong ID) { CharacterIDMap.erase(CharacterIDMap.find(ID)); }
const dangermap& game::GetDangerMap() { return DangerMap; }

void game::InitScript()
{
  inputfile ScriptFile(GetGameDir() + "Script/dungeon.dat", &GlobalValueMap);
  GameScript = new gamescript;
  GameScript->ReadFrom(ScriptFile);
  GameScript->RandomizeLevels();
}

/*#include "materias.h"
#include "item.h"
#include "stack.h"*/

//#include "confdef.h"

bool game::Init(const festring& Name)
{
  festring PlayerName;

  if(!Name.GetSize())
    if(!configuration::GetDefaultName().GetSize())
      {
	PlayerName = iosystem::StringQuestion(CONST_S("What is your name? (1-20 letters)"), vector2d(30, 46), WHITE, 1, 20, true, true);

	if(!PlayerName.GetSize())
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
  mkdir(GetSaveDir().CStr(), S_IRWXU|S_IRWXG);
#endif

#ifdef WIN32
  _mkdir("Bones");
#endif

#ifdef __DJGPP__
  mkdir("Bones", S_IWUSR);
#endif

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
				      "between. Because it's winter, all Attnamian ships are trapped by ice and\n"
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
	InitScript();
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
	CurrentArea = WorldMap = new worldmap(128, 128);
	CurrentWSquareMap = WorldMap->GetMap();
	WorldMap->Generate();
	InWilderness = true;
	UpdateCamera();
	SendLOSUpdateRequest();
	Ticks = 0;
	InitPlayerAttributeAverage();
	StoryState = 0;
	PlayerMassacreMap.clear();
	PetMassacreMap.clear();
	MiscMassacreMap.clear();
	PlayerMassacreAmount = PetMassacreAmount = MiscMassacreAmount = 0;
	DefaultPolymorphTo.Empty();
	Player->GetStack()->AddItem(new encryptedscroll);
	Player->GetStack()->AddItem(new stethoscope);
	BaseScore = Player->GetScore();

	character* Doggie = new dog;
	Doggie->SetTeam(GetTeam(0));
	GetWorldMap()->GetPlayerGroup().push_back(Doggie);
	Doggie->SetAssignedName(configuration::GetDefaultPetName());

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
	WizardMode = false;
	SeeWholeMapCheatMode = MAP_HIDDEN;
	GoThroughWallsCheat = false;
	return true;
      }
    default:
      return false;
    }
}

void game::DeInit()
{
  delete WorldMap;
  WorldMap = 0;
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
  delete GameScript;
  msgsystem::Format();


  DangerMap.clear();
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
	      CurrentLevel->GenerateMonsters(); // Temporary place
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
      Alloc3D(LuxTable, 256, 33, 33);

      for(long c = 0; c < 0x100; ++c)
	for(long x = 0; x < 33; ++x)
	  for(long y = 0; y < 33; ++y)
	    LuxTable[c][x][y] = ushort(c / (float(HypotSquare(x - 16, y - 16)) / 128 + 1));

      atexit(DeInitLuxTable);
    }
}

void game::DeInitLuxTable()
{
  delete [] LuxTable;
  LuxTable = 0;
}

bool game::WorldMapLOSHandler(long X, long Y)
{
  CurrentWSquareMap[X][Y]->SetLastSeen(LOSTurns);
  return true;
}

bool game::LevelLOSHandler(long X, long Y)
{
  lsquare* Square = CurrentLSquareMap[X][Y];
  Square->SetLastSeen(LOSTurns);
  return Square->IsTransparent();
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
  switch(RAND() & 15)
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
    GetCurrentArea()->UpdateLOS();

  if(OnScreen(CursorPos))
    if(!IsInWilderness() || CurrentWSquareMap[CursorPos.X][CursorPos.Y]->GetLastSeen() || GetSeeWholeMapCheatMode())
      CurrentArea->GetSquare(CursorPos)->SendNewDrawRequest();
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

	  if(Square->GetLastSeen() != game::GetLOSTurns())
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
  SaveFile << ushort(SAVE_FILE_VERSION);
  SaveFile << GameScript << CurrentDungeonIndex << CurrentLevelIndex << Camera;
  SaveFile << WizardMode << SeeWholeMapCheatMode << GoThroughWallsCheat;
  SaveFile << BaseScore << Ticks << InWilderness << NextCharacterID << NextItemID;
  SaveFile << LOSTurns;
  ulong Seed = RAND();
  femath::SetSeed(Seed);
  SaveFile << Seed;
  SaveFile << AveragePlayerArmStrength << AveragePlayerLegStrength << AveragePlayerDexterity << AveragePlayerAgility;
  SaveFile << Teams << Dungeons << StoryState;
  SaveFile << PlayerMassacreMap << PetMassacreMap << MiscMassacreMap;
  SaveFile << PlayerMassacreAmount << PetMassacreAmount << MiscMassacreAmount;
  ushort c;

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
  SaveFile << DangerMap << NextDangerID << DefaultPolymorphTo;
  return true;
}

uchar game::Load(const festring& SaveName)
{
  inputfile SaveFile(SaveName + ".sav", 0, false);

  if(!SaveFile.IsOpen())
    return NEW_GAME;

  ushort Version;
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
  SaveFile >> BaseScore >> Ticks >> InWilderness >> NextCharacterID >> NextItemID;
  SaveFile >> LOSTurns;
  femath::SetSeed(ReadType<ulong>(SaveFile));
  SaveFile >> AveragePlayerArmStrength >> AveragePlayerLegStrength >> AveragePlayerDexterity >> AveragePlayerAgility;
  SaveFile >> Teams >> Dungeons >> StoryState;
  SaveFile >> PlayerMassacreMap >> PetMassacreMap >> MiscMassacreMap;
  SaveFile >> PlayerMassacreAmount >> PetMassacreAmount >> MiscMassacreAmount;
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
    {
      CurrentArea = LoadWorldMap(SaveName);
      CurrentWSquareMap = WorldMap->GetMap();
    }
  else
    {
      CurrentArea = CurrentLevel = GetCurrentDungeon()->LoadLevel(SaveName, CurrentLevelIndex);
      CurrentLSquareMap = CurrentLevel->GetMap();
    }

  vector2d Pos;
  SaveFile >> Pos;
  SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());
  msgsystem::Load(SaveFile);
  SaveFile >> DangerMap >> NextDangerID >> DefaultPolymorphTo;
  return LOADED;
}

festring game::SaveName(const festring& Base)
{
  festring SaveName = GetSaveDir();

  if(!Base.GetSize())
    SaveName << PLAYER->GetAssignedName();
  else
    SaveName << Base;

  for(ushort c = 0; c < SaveName.GetSize(); ++c)
    if(SaveName[c] == ' ')
      SaveName[c] = '_';

#if defined(WIN32) || defined(__DJGPP__)
  if(SaveName.GetSize() > 13)
    SaveName.Resize(13);
#endif

  return SaveName;
}

bool game::EmitationHandler(long X, long Y)
{
  long XVal = X - CurrentEmitterPosXMinus16;
  long YVal = Y - CurrentEmitterPosYMinus16;

  ulong Emit = XVal > 32 || XVal < 0 || YVal > 32 || YVal < 0 ? 0 :
	       MakeRGB24(CurrentRedLuxTable[XVal][YVal],
			 CurrentGreenLuxTable[XVal][YVal],
			 CurrentBlueLuxTable[XVal][YVal]);

  lsquare* Square = CurrentLSquareMap[X][Y];
  Square->AlterLuminance(CurrentEmitterPos, Emit);
  return Square->IsTransparent();
}

bool game::NoxifyHandler(long X, long Y)
{
  lsquare* Square = CurrentLSquareMap[X][Y];
  Square->NoxifyEmitter(CurrentEmitterPos);
  return Square->IsTransparent();
}

void game::UpdateCameraXWithPos(ushort Coord)
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

void game::UpdateCameraYWithPos(ushort Coord)
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
  for(ushort c = 0; c < EXTENDED_DIRECTION_COMMAND_KEYS; ++c)
    if((A + GetMoveVector(c)) == B)
      return MoveCommandKey[c];

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
    if(Key == GetMoveCommandKey(c))
      return GetMoveVector(c);

  return ERROR_VECTOR;
}

bool game::EyeHandler(long X, long Y)
{
  return CurrentLSquareMap[X][Y]->IsTransparent();
}

long game::GodScore()
{
  long Score = -1000;

  for(ushort c = 1; c <= GODS; ++c)
    if(GetGod(c)->GetRelation() > Score)
      Score = GetGod(c)->GetRelation();

  return Score;
}

float game::GetMinDifficulty()
{
  float Base = float(CurrentLevel->GetDifficulty()) / 5000;

  while(true)
    {
      uchar Dice = RAND() % 25;

      if(Dice < 5)
	{
	  Base /= 3;
	  continue;
	}

      if(Dice >= 20)
	{
	  Base *= 3;
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

uchar game::DirectionQuestion(const festring& Topic, bool RequireAnswer, bool AcceptYourself)
{
  while(true)
    {
      int Key = AskForKeyPress(Topic);

      if(AcceptYourself && Key == '.')
	return YOURSELF;

      for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
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

  for(ushort i = 1; i < Dungeons; ++i)
    for(ushort c = 0; c < GetDungeon(i)->GetLevels(); ++c)
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

  const std::list<std::pair<uchar, teamscript> >& TeamScript = GetGameScript()->GetTeam();

  for(std::list<std::pair<uchar, teamscript> >::const_iterator i = TeamScript.begin(); i != TeamScript.end(); ++i)
    {
      for(ushort c = 0; c < i->second.GetRelation().size(); ++c)
	GetTeam(i->second.GetRelation()[c].first)->SetRelation(GetTeam(i->first), i->second.GetRelation()[c].second);

      const ushort* KillEvilness = i->second.GetKillEvilness();

      if(KillEvilness)
	GetTeam(i->first)->SetKillEvilness(*KillEvilness);
    }
}

festring game::StringQuestion(const festring& Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters, bool AllowExit)
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0); // pos may be incorrect!
  festring Return = iosystem::StringQuestion(Topic, Pos, Color, MinLetters, MaxLetters, false, AllowExit);
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

long game::NumberQuestion(const festring& Topic, vector2d Pos, ushort Color)
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  long Return = iosystem::NumberQuestion(Topic, Pos, Color, false);
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

long game::ScrollBarQuestion(const festring& Topic, vector2d Pos, long BeginValue, long Step, long Min, long Max, long AbortValue, ushort TopicColor, ushort Color1, ushort Color2, void (*Handler)(long))
{
  DrawEverythingNoBlit();
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  long Return = iosystem::ScrollBarQuestion(Topic, Pos, BeginValue, Step, Min, Max, AbortValue, TopicColor, Color1, Color2, false, Handler);
  DOUBLE_BUFFER->Fill(16, 6, GetScreenXSize() << 4, 23, 0);
  return Return;
}

void game::LOSTurn()
{
  if(LOSTurns++ == 0xFFFFFFFF)
    ABORT("Suddenly the Universe explodes!");
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
	      GetPlayer()->AddScoreEntry(CONST_S("cowardly quit the game"), 0.75f);
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

uchar game::GetDirectionForVector(vector2d Vector)
{
  for(ushort c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
    if(Vector == GetMoveVector(c))
      return c;

  return DIR_ERROR;
}

const char* game::GetVerbalPlayerAlignment()
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
  static clock_t LastTime = 0;
  static ushort Frame = 0;
  static vector2d Pos((RES_X >> 1) - 100, (RES_Y << 1) / 3 - 100);

  if(clock() - LastTime > CLOCKS_PER_SEC >> 5)
    {
      BusyAnimationCache[Frame]->Blit(Buffer, 0, 0, Pos, 200, 200);

      if(Buffer == DOUBLE_BUFFER)
	graphics::BlitDBToScreen();

      if(++Frame == 48)
	Frame = 0;

      LastTime = clock();
    }
}

void game::CreateBusyAnimationCache()
{
  bitmap Elpuri(16, 16, TRANSPARENT_COLOR);
  ushort Color = MakeRGB16(60, 60, 60);
  igraph::GetCharacterRawGraphic()->MaskedBlit(&Elpuri, 64, 0, 0, 0, 16, 16, &Color);
  bitmap Circle(200, 200, TRANSPARENT_COLOR);

  for(ushort x = 0; x < 4; ++x)
    Circle.DrawPolygon(100, 100, 95 + x, 50, MakeRGB16(255 - 12 * x, 0, 0));

  for(ushort c = 0; c < 48; ++c)
    {
      bitmap* Bitmap = BusyAnimationCache[c] = new bitmap(200, 200, 0);
      Elpuri.MaskedBlit(Bitmap, 0, 0, 92, 92, 16, 16);
      double Rotation = 0.3f + c * FPI / 120;

      for(ushort x = 0; x < 10; ++x)
	Bitmap->DrawPolygon(100, 100, 95, 5, MakeRGB16(255 - 25 * (10 - x), 0, 0), false, true, Rotation + double(x) / 50);

      Circle.MaskedBlit(Bitmap);
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

  while(true)
    {
      square* Square = GetCurrentArea()->GetSquare(CursorPos);

      if(!Square->GetLastSeen() && (!Square->GetCharacter() || !Square->GetCharacter()->CanBeSeenByPlayer()) && !GetSeeWholeMapCheatMode())
	DOUBLE_BUFFER->Fill(CalculateScreenCoordinates(CursorPos), 16, 16, BLACK);
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

  if(Square->GetLastSeen() || GetSeeWholeMapCheatMode())
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

void game::TextScreen(const festring& Text, ushort Color, bool GKey, void (*BitmapEditor)(bitmap*))
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

  for(ushort c = 0; c < KeyNumber; ++c)
    Key[c] = va_arg(Arguments, int);

  va_end(Arguments);
  DrawEverythingNoBlit();
  FONT->Printf(DOUBLE_BUFFER, 16, 8, WHITE, "%s", Message.CStr());
  graphics::BlitDBToScreen();
  int Return = 0;

  while(!Return)
    {
      int k = GET_KEY();

      for(ushort c = 0; c < KeyNumber; ++c)
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

int game::Menu(bitmap* BackGround, vector2d Pos, const festring& Topic, const festring& sMS, ushort Color, const festring& SmallText1, const festring& SmallText2)
{
  globalwindowhandler::DisableControlLoops();
  int Return = iosystem::Menu(BackGround, Pos, Topic, sMS, Color, SmallText1, SmallText2);
  globalwindowhandler::EnableControlLoops();
  return Return;
}

void game::InitDangerMap()
{
  bool First = true;

  for(ushort c = 1; c < protocontainer<character>::GetProtoAmount(); ++c)
    {
      BusyAnimation();
      const character::prototype* Proto = protocontainer<character>::GetProto(c);
      const character::databasemap& Config = Proto->GetConfig();

      for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract && i->second.CanBeGenerated)
	  {
	    if(First)
	      {
		NextDangerID.Type = c;
		NextDangerID.Config = i->first;
		First = false;
	      }

	    character* Char = Proto->Clone(i->first, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	    float NakedDanger = Char->GetRelativeDanger(Player, true);
	    delete Char;
	    Char = Proto->Clone(i->first, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
	    float EquippedDanger = Char->GetRelativeDanger(Player, true);
	    delete Char;
	    DangerMap[configid(c, i->first)] = dangerid(NakedDanger, EquippedDanger);
	  }
    }
}

void game::CalculateNextDanger()
{
  if(IsInWilderness() || !*CurrentLevel->GetLevelScript()->GenerateMonsters())
    return;

  const character::prototype* Proto = protocontainer<character>::GetProto(NextDangerID.Type);
  const character::databasemap& Config = Proto->GetConfig();
  character::databasemap::const_iterator ConfigIterator = Config.find(NextDangerID.Config);
  dangermap::iterator DangerIterator = DangerMap.find(NextDangerID);

  if(ConfigIterator != Config.end() && DangerIterator != DangerMap.end())
    {
      character* Char = Proto->Clone(NextDangerID.Config, NO_EQUIPMENT|NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
      float CurrentDanger = Char->GetRelativeDanger(Player, true);

      if(DangerIterator->second.NakedDanger > CurrentDanger)
	DangerIterator->second.NakedDanger = (DangerIterator->second.NakedDanger * 9 + CurrentDanger) / 10;

      delete Char;
      Char = Proto->Clone(NextDangerID.Config, NO_PIC_UPDATE|NO_EQUIPMENT_PIC_UPDATE);
      CurrentDanger = Char->GetRelativeDanger(Player, true);

      if(DangerIterator->second.EquippedDanger > CurrentDanger)
	DangerIterator->second.EquippedDanger = (DangerIterator->second.EquippedDanger * 9 + CurrentDanger) / 10;

      delete Char;

      for(++ConfigIterator; ConfigIterator != Config.end(); ++ConfigIterator)
	if(!ConfigIterator->second.IsAbstract && ConfigIterator->second.CanBeGenerated)
	  {
	    NextDangerID.Config = ConfigIterator->first;
	    return;
	  }

      while(true)
	{
	  if(NextDangerID.Type < protocontainer<character>::GetProtoAmount() - 1)
	    ++NextDangerID.Type;
	  else
	    NextDangerID.Type = 1;

	  const character::databasemap& Config = protocontainer<character>::GetProto(NextDangerID.Type)->GetConfig();

	  for(ConfigIterator = Config.begin(); ConfigIterator != Config.end(); ++ConfigIterator)
	    if(!ConfigIterator->second.IsAbstract && ConfigIterator->second.CanBeGenerated)
	      {
		NextDangerID.Config = ConfigIterator->first;
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
      CurrentDungeonIndex = Dungeon;
      EnterArea(Group, Area, EntryIndex);
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
      SaveWorldMap();
    }

  return true;
}

/* Used always when the player enters an area. */

void game::EnterArea(std::vector<character*>& Group, uchar Area, uchar EntryIndex)
{
  if(Area != WORLD_MAP)
    {
      SetIsInWilderness(false);
      CurrentLevelIndex = Area;
      bool New = !PrepareRandomBone(Area) && !GetCurrentDungeon()->PrepareLevel(Area);
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

      const bool* AutoReveal = GetCurrentLevel()->GetLevelScript()->AutoReveal();

      if(New && AutoReveal && *AutoReveal)
	GetCurrentLevel()->Reveal();

      ShowLevelMessage();
      SendLOSUpdateRequest();
      UpdateCamera();
      GetCurrentArea()->UpdateLOS();

      if(configuration::GetAutoSaveInterval())
	Save(GetAutoSaveFileName().CStr());
    }
  else
    {
      SetIsInWilderness(true);
      LoadWorldMap();
      CurrentArea = WorldMap;
      CurrentWSquareMap = WorldMap->GetMap();
      GetWorldMap()->GetPlayerGroup().swap(Group);
      GetCurrentArea()->AddCharacter(GetWorldMap()->GetEntryPos(Player, EntryIndex), Player);
      SendLOSUpdateRequest();
      UpdateCamera();
      GetCurrentArea()->UpdateLOS();

      if(configuration::GetAutoSaveInterval())
	Save(GetAutoSaveFileName().CStr());
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

void game::SetStandardListAttributes(felist& List)
{
  List.SetPos(vector2d(26, 42));
  List.SetWidth(652);
  List.SetFlags(DRAW_BACKGROUND_AFTERWARDS);
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
  ushort Arms = 0;
  ushort Legs = 0;
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
  float PlayerArmStrength = 0;
  float PlayerLegStrength = 0;
  float PlayerDexterity = 0;
  float PlayerAgility = 0;
  ushort Arms = 0;
  ushort Legs = 0;
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

bool game::ExplosionHandler(long X, long Y)
{
  lsquare* Square = CurrentLSquareMap[X][Y];
  Square->GetHitByExplosion(CurrentExplosion);
  return Square->IsFlyable();
}

level* game::GetLevel(ushort Index)
{
  return GetCurrentDungeon()->GetLevel(Index);
}

ushort game::GetLevels()
{
  return GetCurrentDungeon()->GetLevels();
}

void game::InstallCurrentEmitter(vector2d Pos, ulong Emitation)
{
  CurrentEmitterPos = Pos;
  CurrentEmitterPosXMinus16 = Pos.X - 16;
  CurrentEmitterPosYMinus16 = Pos.Y - 16;
  CurrentRedLuxTable = LuxTable[GetRed24(Emitation)];
  CurrentGreenLuxTable = LuxTable[GetGreen24(Emitation)];
  CurrentBlueLuxTable = LuxTable[GetBlue24(Emitation)];
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
  else if(Murderer->GetTeam()->GetID() == PLAYER_TEAM)
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
    MassacreMap->insert(std::pair<configid, ushort>(CI, 1));
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
  bitmap** Picture;
  ushort Frames;
};

void game::DisplayMassacreList(const massacremap& MassacreMap, const char* Reason, ulong Amount)
{
  std::set<massacresetentry> MassacreSet;
  festring FirstPronoun;
  bool First = true;

  for(massacremap::const_iterator i1 = MassacreMap.begin(); i1 != MassacreMap.end(); ++i1)
    {
      character* Victim = protocontainer<character>::GetProto(i1->first.Type)->Clone(i1->first.Config);
      massacresetentry Entry;
      Entry.Frames = Victim->DrawBodyPartArray(Entry.Picture, 0);

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
      delete Victim;
    }

  ulong Total = PlayerMassacreAmount + PetMassacreAmount + MiscMassacreAmount;
  festring MainTopic;

  if(Total == 1)
    MainTopic << "One creature perished during your adventure.";
  else
    MainTopic << Total << " creatures perished during your adventure.";

  felist List(MainTopic);
  game::SetStandardListAttributes(List);
  List.SetPageLength(15);
  List.SetMaskColor(0);
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
    {
      List.AddEntry(i2->String, LIGHT_GRAY, 0, i2->Picture, i2->Frames);

      for(ushort c = 0; c < i2->Frames; ++c)
	delete i2->Picture[c];

      delete [] i2->Picture;
    }

  List.Draw();
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
  if(/**!WizardModeIsActive() && */!IsInWilderness() && GetCurrentLevel()->PreProcessForBone())
    {
      ushort BoneIndex;
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
	  BoneFile << PLAYER->GetAssignedName();
	  BoneFile << CurrentLevel;
	}
    }
}

bool game::PrepareRandomBone(ushort LevelIndex)
{
  /* Don't load a bone over a shop, etc. */

  if(!*GetCurrentDungeon()->GetLevelScript(LevelIndex)->CanGenerateBone())
    return false;

  ushort BoneIndex;
  festring BoneName;

  for(BoneIndex = 0; BoneIndex < 1000; ++BoneIndex)
    {
      BoneName = GetBoneDir() + "bon" + CurrentDungeonIndex + LevelIndex + BoneIndex;
      inputfile BoneFile(BoneName, 0, false);

      if(BoneFile.IsOpen())/// || RAND() & 1)
	{
	  festring PlayerName;
	  BoneFile >> PlayerName;
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
	  CurrentArea = NewLevel;
	  CurrentLevel = NewLevel;
	  CurrentLSquareMap = NewLevel->GetMap();
	  GetCurrentDungeon()->SetGenerated(LevelIndex, true);

	  if(PlayerName == PLAYER->GetAssignedName())
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

float game::CalculateAverageDanger(const std::vector<character*>& EnemyVector, character* Char)
{
  float DangerSum = 0;
  ushort Enemies = 0;

  for(ushort c = 0; c < EnemyVector.size(); ++c)
    {
      DangerSum += EnemyVector[c]->GetRelativeDanger(Char, true);
      ++Enemies;
    }

  return DangerSum / Enemies;
}

float game::CalculateAverageDangerOfAllNormalEnemies()
{
  float DangerSum = 0;
  ushort Enemies = 0;

  for(ushort c = 1; c < protocontainer<character>::GetProtoAmount(); ++c)
    {
      const character::prototype* Proto = protocontainer<character>::GetProto(c);
      const character::databasemap& Config = Proto->GetConfig();

      for(character::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	if(!i->second.IsAbstract && !i->second.IsUnique && i->second.CanBeGenerated)
	  {
	    DangerSum += DangerMap.find(configid(c, i->first))->second.EquippedDanger;
	    ++Enemies;
	  }
    }

  return DangerSum / Enemies;
}

character* game::CreateGhost()
{
  float AverageDanger = game::CalculateAverageDangerOfAllNormalEnemies();
  std::vector<character*> EnemyVector;
  protosystem::CreateEveryNormalEnemy(EnemyVector);
  ghost* Ghost = new ghost;
  Ghost->SetTeam(GetTeam(MONSTER_TEAM));
  Ghost->SetOwnerSoul(PLAYER->GetAssignedName());
  Ghost->SetIsActive(false);
  Ghost->EditAllAttributes(-4);
  PLAYER->SetSoulID(Ghost->GetID());
  while(game::CalculateAverageDanger(EnemyVector, Ghost) > AverageDanger && Ghost->EditAllAttributes(1));

  for(ushort c = 0; c < EnemyVector.size(); ++c)
    delete EnemyVector[c];

  return Ghost;
}
