#include <cmath>
#include <ctime>
#include <direct.h>	// Needed for _mkdir

#include "game.h"
#include "level.h"
#include "charde.h"
#include "error.h"
#include "itemde.h"
#include "god.h"
#include "igraph.h"
#include "strover.h"
#include "felist.h"
#include "whandler.h"
#include "lsquare.h"
#include "stack.h"
#include "lterraba.h"
#include "worldmap.h"
#include "message.h"
#include "pool.h"
#include "proto.h"
#include "dungeon.h"
#include "save.h"
#include "feio.h"
#include "graphics.h"
#include "script.h"
#include "team.h"
#include "colorbit.h"

ushort game::Current;
long game::BaseScore;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
std::vector<dungeon*> game::Dungeon;
character* game::PlayerBackup;
uchar game::CurrentDungeon;
ulong game::NextObjectID = 0;
std::vector<team*> game::Team;
ulong game::LOSTurns;

gamescript game::GameScript;

bool game::Flag;
perttu* game::Perttu = 0;

std::string game::AutoSaveFileName = "Save/Autosave";
std::string game::Alignment[] = {"L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--"};
god* game::God[] = {0, new valpuri, new venius, new atavus, new dulcis, new inasnum, new seges, new consummo, new silva, new loricatus, new mellis, new calamus, new pestifer, new macellarius, new scabies, new infuscor, new cruentus, new erado, 0};

command* game::Command[] = {	0,
				new command(&character::Apply, "apply", 'a', false),
				new command(&character::Close, "close", 'c', false),
				new command(&character::Consume, "eat/drink", 'e', true),
				new command(&character::DecreaseSoftGamma, "decrease software gamma", 'F', true),
				new command(&character::Dip, "dip", 'D', true),
				new command(&character::Drop, "drop", 'd', false),
				new command(&character::WhatToEngrave, "engrave", 'E', false),
				new command(&character::GainAllItems, "give all items cheat", 'A', true, true),
				new command(&character::GainDivineKnowledge, "gain knowledge of all gods cheat", 'G', true, true),
				new command(&character::GoDown, "go down", '>', true),
				new command(&character::GoUp, "go up", '<', true),
				new command(&character::IncreaseSoftGamma, "increase software gamma", 'f', true),
				new command(&character::Kick, "kick", 'k', false),
				new command(&character::Look, "look", 'l', true),
				new command(&character::LowerGodRelations, "lower your relations to the gods cheat", 'L', true, true),
				new command(&character::LowerStats, "lower stats cheat", 'T', true, true),
				new command(&character::Offer, "offer", 'O', false),
				new command(&character::Open, "open", 'o', false),
				new command(&character::OutlineCharacters, "outline characters", 'K', true),
				new command(&character::OutlineItems, "outline items", 'I', true),
				new command(&character::PickUp, "pick up", ',', false),
				new command(&character::Pray, "pray", 'p', false),
				new command(&character::Quit, "quit", 'q', true),
				new command(&character::RaiseGodRelations, "raise your relations to the gods cheat", 'P', true, true),
				new command(&character::RaiseStats, "raise stats cheat", 'R', true, true),
				new command(&character::Read, "read", 'r', false),
				new command(&character::RestUntilHealed, "rest/heal", 'h', true),
				new command(&character::Save, "save game", 's', true),
				new command(&character::SeeWholeMap, "see whole map cheat", 'Y', true, true),
				new command(&character::ShowInventory, "show inventory", 'i', true),
				new command(&character::ShowKeyLayout, "show key layout", '?', true),
				new command(&character::ShowWeaponSkills, "show weapon skills", '@', true),
				new command(&character::Sit, "sit", 'S', false),
				new command(&character::DrawMessageHistory, "show message history", 'M', true),
				new command(&character::Talk, "chat with someone", 'C', false),
				new command(&character::Throw, "throw", 't', false),
				new command(&character::WalkThroughWalls, "toggle walk through walls cheat", 'U', true, true),
				new command(&character::ForceVomit, "vomit", 'v', false),
				new command(&character::NOP, "wait", '.', true),
				new command(&character::WearArmor, "wear", 'W', true),
				new command(&character::Wield, "wield", 'w', true),
				new command(&character::WizardMode, "activate wizard mode", 'X', true),
				new command(&character::Zap, "zap", 'z', false),
				0};

int game::MoveCommandKey[DIRECTION_COMMAND_KEYS] = {0x147, 0x148, 0x149, 0x14B, 0x14D, 0x14F, 0x150, 0x151};
const vector2d game::MoveVector[DIRECTION_COMMAND_KEYS] = {vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1)};

game::panel game::Panel;

bool game::LOSUpdateRequested = false;
ushort*** game::LuxTable;
ushort* game::LuxTableSize;
bool game::Running;
character* game::Player;
vector2d game::Camera(0, 0);
bool game::WizardMode;
bool game::SeeWholeMapCheat;
bool game::GoThroughWallsCheat;
bool KeyIsOK(char);
std::string game::PlayerName;
uchar game::GodNumber;
ulong game::Turns;
float game::SoftGamma = 1;
bool game::OutlineItems = false, game::OutlineCharacters = false;

void game::InitScript()
{
	srand(time(0));
	inputfile ScriptFile("Script/Dungeon.dat");
	GameScript.ReadFrom(ScriptFile);
}

void game::Init(std::string Name)
{
	static ushort Counter = 0;

	Turns = 0;
	EMPTY_MESSAGES();
	ADD_MESSAGE("Initialization of game number %d started...", ++Counter);
	globalmessagingsystem::Format();

	Running = true;
	WizardMode = false;
	SeeWholeMapCheat = false;
	GoThroughWallsCheat = false;
	InWilderness = false;
	PlayerBackup = 0;
	srand(time(0));
	game::CalculateGodNumber();
	LOSTurns = 1;

	_mkdir("Save");

	if(Name == "")
	{
		DOUBLEBUFFER->ClearToColor(0);
		SetPlayerName(iosystem::StringQuestion("What is your name?", vector2d(30, 50), WHITE, 3, 30));
	}
	else
		SetPlayerName(Name);

	if(Load())
	{
		Flag = true;
		GetCurrentArea()->SendNewDrawRequest();

		ADD_MESSAGE("Game loaded successfully.");
	}
	else
	{
		iosystem::TextScreen(	"Your legs are numbed after a long journey across a cold and gloomy forest.\n"
					"You should have never believed those that adviced you to seek adventure here,\n"
					"outside of all civilization. The next ship will arrive on the coast in eight\n"
					"months. Before that you must make yourself comfortable here.\n\n"
					"Finally your eyes catch your destination among the ocean of pines. A small\n"
					"settlement of a few houses build around a round, shining tower rises ahead\n"
					"of you. This is Attnam, the Holy City of Valpuri the Great Frog. And it is\n"
					"desperately seeking adventurers like you.");

		iosystem::TextScreen("Generating game...\n\nThis may take some time, please wait.", WHITE, false);

		CreateTeams();

		SetPlayer(new human);

		Player->SetTeam(GetTeam(0));
		GetTeam(0)->SetLeader(Player);

		Perttu = 0;

		InitDungeons();

		WorldMap = new worldmap(128, 128);
		WorldMap->Generate();

		InWilderness = true;

		UpdateCamera();

		game::SendLOSUpdateRequest();

		for(ushort c = 1; GetGod(c); ++c)
		{
			GetGod(c)->SetKnown(false);
			GetGod(c)->SetRelation(0);
			GetGod(c)->SetTimer(0);
		}

		GetGod(1)->SetKnown(true);

		Turns = 0;

		BaseScore = Player->Score();

		ADD_MESSAGE("Game generated successfully.");
	}
}

void game::DeInit()
{
	delete GetPlayerBackup();
	delete WorldMap;

	ushort c;

	for(c = 0; c < Dungeon.size(); ++c)
		delete Dungeon[c];

	for(c = 0; c < Team.size(); ++c)
		delete GetTeam(c);

	Team.clear();
}

void game::Run()
{
	while(GetRunning())
	{	
		if(!InWilderness)
			GetCurrentDungeon()->GetLevel(Current)->HandleCharacters();	// Temporary

		objectpool::Be();

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
					long xLevelSquare = abs(x - MaxDist), yLevelSquare = abs(y - MaxDist);

					LuxTable[c][x][y] = ushort(float(c) / (float(xLevelSquare * xLevelSquare + yLevelSquare * yLevelSquare) / 128 + 1));
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
}

bool game::LOSHandler(vector2d Pos, vector2d Origo)
{
	if(Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
		return false;

	GetCurrentArea()->GetSquare(Pos)->SetLastSeen(LOSTurns);

	if(Pos == Origo)
		return true;
	else
		return GetCurrentArea()->GetSquare(Pos)->GetOverTerrain()->GetIsWalkable();
}

bool game::DoLine(long X1, long Y1, long X2, long Y2, ulong MaxDistance, bool (*Proc)(vector2d, vector2d))
{
	long DX = X2 - X1, DY = Y2 - Y1, I1, I2, X, Y, DD;

	#define DO_LINE(PriSign, PriC, PriCond, SecSign, SecC, SecCond)			\
	{										\
		if(!D##PriC)								\
		{									\
			Proc(vector2d(X1, Y1), vector2d(X1, Y1));			\
			return true;							\
		}									\
											\
		I1 = D##SecC << 1;							\
		DD = I1 - (SecSign (PriSign D##PriC));					\
		I2 = DD - (SecSign (PriSign D##PriC));					\
											\
		X = X1;									\
		Y = Y1;									\
											\
		while(PriC PriCond PriC##2)						\
		{									\
			if((X - X1) * (X - X1) + (Y - Y1) * (Y - Y1) > MaxDistance ||	\
			   !Proc(vector2d(X, Y), vector2d(X1, Y1)))			\
				return false;						\
											\
			if(DD SecCond 0)						\
			{								\
				SecC##SecSign##SecSign;					\
				DD += I2;						\
			}								\
			else								\
				DD += I1;						\
											\
			PriC##PriSign##PriSign;						\
		}									\
	}

	if(DX >= 0)
		if(DY >= 0)
			if(DX >= DY)
				DO_LINE(+, X, <=, +, Y, >=)
			else
				DO_LINE(+, Y, <=, +, X, >=)
		else
			if (DX >= -DY)
				DO_LINE(+, X, <=, -, Y, <=)
			else
				DO_LINE(-, Y, >=, +, X, >=)
	else
		if (DY >= 0)
			if (-DX >= DY)
				DO_LINE(-, X, >=, +, Y, >=)
			else
				DO_LINE(+, Y, <=, -, X, <=)
		else
			if (-DX >= -DY)
				DO_LINE(-, X, >=, -, Y, <=)
			else
				DO_LINE(-, Y, >=, -, X, <=)

	return true;
}

void game::panel::Draw() const
{
	DOUBLEBUFFER->ClearToColor(0, 512, 800, 88, 0);

	character* Player = game::GetPlayer();

	FONT->Printf(DOUBLEBUFFER, 16, 524, WHITE, "%s %s", game::GetPlayerName().c_str(), Player->CNAME(DEFINITE));

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
	FONT->Printf(DOUBLEBUFFER, 200, 554, WHITE, "Damage: %d-%d", ushort(Player->GetAttackStrength() * Player->GetStrength() / 26667), ushort(Player->GetAttackStrength() * Player->GetStrength() / 16000 + 1));
	FONT->Printf(DOUBLEBUFFER, 200, 564, WHITE, "Money: %d", Player->GetMoney());

	FONT->Printf(DOUBLEBUFFER, 440, 534, WHITE, "Armor Value: %d", Player->CalculateArmorModifier());
	FONT->Printf(DOUBLEBUFFER, 440, 544, WHITE, "Dodge Value: %.0f", Player->GetDodgeValue());

	if(GetWizardMode())
		FONT->Printf(DOUBLEBUFFER, 440, 554, WHITE, "NP: %d", Player->GetNP());

	if(GetInWilderness())
		FONT->Printf(DOUBLEBUFFER, 620, 534, WHITE, "Worldmap");
	else
		FONT->Printf(DOUBLEBUFFER, 620, 534, WHITE, "%s", game::GetCurrentDungeon()->GetLevelDescription(GetCurrent()).c_str());

	FONT->Printf(DOUBLEBUFFER, 620, 544, WHITE, "Turns: %d", game::GetTurns());

	if(Player->GetNP() < CRITICALHUNGERLEVEL)
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
		if(!Player->GetPos().X)
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
	if(Player->GetPos().Y < 18)
		if(!Player->GetPos().Y)
			return;
		else
			Camera.Y = 0;
	else
		if(Camera.Y == Player->GetPos().Y - 18)
			return;
		else
			Camera.Y = Player->GetPos().Y - 18;

	GetCurrentArea()->SendNewDrawRequest();
}

const char* game::Insult()
{
switch(rand()%15)
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
	if(Pos.X >= Camera.X && Pos.Y >= Camera.Y && Pos.X < game::GetCamera().X + 50 && Pos.Y < game::GetCamera().Y + 30)
		return true;
	else
		return false;
}

void game::DrawEverythingNoBlit(bool EmptyMsg)
{
	if(LOSUpdateRequested)
	{
		game::GetCurrentArea()->UpdateLOS();
		LOSUpdateRequested = false;
	}

	game::GetCurrentArea()->Draw();
	if(OnScreen(GetPlayer()->GetPos()))
		igraph::DrawCursor((GetPlayer()->GetPos() - GetCamera() + vector2d(0,2)) << 4);
	game::Panel.Draw();

	if(EmptyMsg)
	{
		DRAW_MESSAGES();
		EMPTY_MESSAGES();
	}
}

bool game::Save(std::string SaveName)
{
	outputfile SaveFile(SaveName + ".sav");

	SaveFile << PlayerName;
	SaveFile << CurrentDungeon << Current << Camera << WizardMode << SeeWholeMapCheat;
	SaveFile << GoThroughWallsCheat << BaseScore << Turns << SoftGamma << InWilderness << NextObjectID;
	SaveFile << OutlineItems << OutlineCharacters << LOSTurns;

	time_t Time = time(0);
	srand(Time);
	SaveFile << Time;

	SaveFile << Dungeon << Team;

	if(InWilderness)
		SaveWorldMap();
	else
		GetCurrentDungeon()->SaveLevel(SaveName, Current, false);

	for(ushort c = 1; GetGod(c); ++c)
		SaveFile << GetGod(c);

	SaveFile << game::GetPlayer()->GetPos();

	SaveFile << PlayerBackup;
	globalmessagingsystem::GetMessageHistory()->Save(SaveFile);
	return true;
}

bool game::Load(std::string SaveName)
{
	inputfile SaveFile(SaveName + ".sav", false);

	if(!SaveFile.GetBuffer().is_open())
		return false;

	SaveFile >> PlayerName;
	SaveFile >> CurrentDungeon >> Current >> Camera >> WizardMode >> SeeWholeMapCheat;
	SaveFile >> GoThroughWallsCheat >> BaseScore >> Turns >> SoftGamma >> InWilderness >> NextObjectID;
	SaveFile >> OutlineItems >> OutlineCharacters >> LOSTurns;

	time_t Time;
	SaveFile >> Time;
	srand(Time);

	SaveFile >> Dungeon >> Team;

	if(InWilderness)
		LoadWorldMap();
	else
		GetCurrentDungeon()->LoadLevel(SaveName);

	for(ushort c = 1; GetGod(c); ++c)
		GetGod(c)->Load(SaveFile);

	vector2d Pos;

	SaveFile >> Pos;

	SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());

	SaveFile >> PlayerBackup;
	globalmessagingsystem::GetMessageHistory()->Load(SaveFile);
	return true;
}

std::string game::SaveName()
{
	std::string SaveName = std::string("Save/") + PlayerName;

	for(ushort c = 0; c < SaveName.length(); ++c)
		if(SaveName[c] == ' ')
			SaveName[c] = '_';

	if(SaveName.length() > 13)
		SaveName.resize(13);

	return SaveName;
}

bool game::EmitationHandler(vector2d Pos, vector2d Origo)
{
	if(Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
		return false;

	ushort Emit = GetLevel(Current)->GetLevelSquare(Origo)->GetEmitation();

	ushort MaxSize = game::GetLuxTableSize()[Emit] >> 1;

	if(long(Pos.X) - long(Origo.X) > MaxSize || long(Origo.X) - long(Pos.X) > MaxSize || long(Pos.Y) - long(Origo.Y) > MaxSize || long(Origo.Y) - long(Pos.Y) > MaxSize)
		Emit = 0;
	else
		Emit = game::GetLuxTable()[Emit][long(Pos.X) - long(Origo.X) + MaxSize][long(Pos.Y) - long(Origo.Y) + MaxSize];

	GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(Pos)->AlterLuminance(Origo, Emit);

	if(Pos == Origo)
		return true;
	else
		return GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(Pos)->GetOverLevelTerrain()->GetIsWalkable();
}

bool game::NoxifyHandler(vector2d Pos, vector2d Origo)
{
	if(Pos.X >= GetCurrentArea()->GetXSize() || Pos.Y >= GetCurrentArea()->GetYSize())
		return false;

	GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(Pos)->NoxifyEmitter(Origo);

	if(Pos == Origo)
		return true;
	else
		return GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(Pos)->GetOverLevelTerrain()->GetIsWalkable();
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
        if(Coord < 18)
                if(!Camera.Y)
			return;
		else
			Camera.Y = 0;
	else
                if(Camera.Y == Coord - 18)
			return;
		else
			Camera.Y = Coord - 18;

	GetCurrentArea()->SendNewDrawRequest();
}

int game::GetMoveCommandKey(vector2d A, vector2d B)
{
	for(uchar c = 0; c < 8; ++c)
		if((A + game::GetMoveVector(c)) == B)
			return game::MoveCommandKey[c];

	return 0xFF;
}

void game::ApplyDivineTick(ushort Turns)
{
	for(ushort c = 1; GetGod(c); ++c)
		GetGod(c)->ApplyDivineTick(Turns);
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, bool Good)
{
	for(ushort c = 1; GetGod(c); ++c)
		if(GetGod(c) != CompareTarget)
			GetGod(c)->AdjustRelation(CompareTarget, Good);
}

vector2d game::GetDirectionVectorForKey(ushort Key)
{
	for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
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
		return GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(Pos)->GetOverLevelTerrain()->GetIsWalkable();
}

long game::GodScore()
{
	long Score = -1000;

	for(ushort c = 1; GetGod(c); ++c)
		if(GetGod(c)->GetRelation() > Score)
			Score = GetGod(c)->GetRelation();

	return Score;
}

float game::Difficulty()
{
	float Base = game::GetPlayer()->Danger() / 25 * (GetCurrent() + 1);

	while(true)
	{
		float Dice = float(rand() % 5);

		if(!Dice)
		{
			Base /= 3;
			continue;
		}

		if(Dice == 4)
		{
			Base *= 3;
			continue;
		}

		Base *= (6 + Dice) / 8;

		return Base;
	}
}

void game::ShowLevelMessage()
{
	if(GetLevel(GetCurrent())->GetLevelMessage().length())
		ADD_MESSAGE(GetLevel(GetCurrent())->GetLevelMessage().c_str());

	GetLevel(GetCurrent())->SetLevelMessage("");
}

void game::TriggerQuestForMaakotkaShirt()
{
	GetDungeon(0)->PrepareLevel(6);
	GetDungeon(0)->GetLevel(6)->CreateStairs(false);
	GetDungeon(0)->GetLevel(6)->SetLevelMessage("You feel something has changed since you were last here...");
	GetDungeon(0)->SaveLevel(SaveName(), 6);
}

void game::CalculateGodNumber()
{
	for(ushort c = 1;; ++c)
		if(game::GetGod(c) == 0)
		{
			SetGodNumber(c - 1);
			break;
		}
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
		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; ++c)
			if(Key == game::GetMoveCommandKey(c))
				return c;

		if(DefaultAnswer < 8) return DefaultAnswer;
		if(!RequireAnswer) return 0xFF;
	}

}

void game::RemoveSaves()
{
	remove((SaveName() + ".sav").c_str());
	remove((AutoSaveFileName + ".sav").c_str());
	remove((SaveName() + ".wm").c_str());
	remove((AutoSaveFileName + ".wm").c_str());

	for(ushort i = 0; i < Dungeon.size(); ++i)
		for(ushort c = 0; c < GetDungeon(i)->GetLevels(); ++c)
		{
			remove((SaveName() + ".d" + i + "-" + c).c_str());
			remove((AutoSaveFileName + ".d" + i + "-" + c).c_str());
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

	for(uchar c = 0; c < Dungeon.size(); ++c)
	{
		Dungeon[c] = new dungeon(c);
		Dungeon[c]->SetIndex(c);
	}
}

void game::DoGoodDeed(short Amount)
{
	if(!Amount) return;
	
	for(uchar c = 1; c < game::GetGodNumber() + 1; ++c)
	{
		float Change = Amount - float(2 * Amount / 10) * GetGod(c)->Alignment();
		if(GetPlayer()->GetLevelSquareUnder()->GetDivineOwner() == c) 
			GetGod(c)->SetRelation( GetGod(c)->GetRelation()  + short(Change) * 2 );
		else
			GetGod(c)->SetRelation( GetGod(c)->GetRelation()  + short(Change));
	}
}

void game::DoEvilDeed(short Amount)
{
	if(!Amount) return;

	for(uchar c = 1; c < game::GetGodNumber() + 1; ++c)
	{
		float Change = - Amount + float(2 * Amount / 10) * GetGod(c)->Alignment();
		if(GetPlayer()->GetLevelSquareUnder()->GetDivineOwner() == c) 
			GetGod(c)->SetRelation( GetGod(c)->GetRelation()  + short(Change) * 2 );
		else
			GetGod(c)->SetRelation( GetGod(c)->GetRelation()  + short(Change));
	}
}

void game::DoNeutralDeed(short Amount)
{
	if(!Amount) return;

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

ulong game::CreateNewObjectID()
{
	if(NextObjectID == 0xFFFFFFFF)
		ABORT("Suddenly the Universe ends!");

	return NextObjectID++;
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

	for(c = 0; c < GameScript.GetTeam().size(); ++c)
		for(ushort i = 0; i < GameScript.GetTeam()[c].second->GetRelation().size(); ++i)
			GetTeam(GameScript.GetTeam()[c].second->GetRelation()[i].first)->SetRelation(GetTeam(GameScript.GetTeam()[c].first), GameScript.GetTeam()[c].second->GetRelation()[i].second);

	for(c = 0; c < Team.size(); ++c)
		if(c != 1)
			GetTeam(1)->SetRelation(GetTeam(c), HOSTILE);
}

bool game::IsValidPos(vector2d Pos)
{
	if(Pos.X < GetCurrentArea()->GetXSize() && Pos.Y < GetCurrentArea()->GetYSize())
		return true;
	else
		return false;
}

std::string game::StringQuestion(std::string Topic, vector2d Pos, ushort Color, ushort MinLetters, ushort MaxLetters)
{
	EMPTY_MESSAGES();
	game::DrawEverythingNoBlit();
	return iosystem::StringQuestion(Topic, Pos, Color, MinLetters, MaxLetters);
}

long game::NumberQuestion(std::string Topic, vector2d Pos, ushort Color)
{
	EMPTY_MESSAGES();
	game::DrawEverythingNoBlit();
	return iosystem::NumberQuestion(Topic, Pos, Color);
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

	if(Player->GetPos().Y < 18)
		Camera.Y = 0;
	else
		Camera.Y = Player->GetPos().Y - 18;

	GetCurrentArea()->SendNewDrawRequest();
}
