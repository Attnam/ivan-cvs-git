#include <cmath>
#include <ctime>

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

ushort game::Current;
long game::BaseScore;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;
std::vector<dungeon*> game::Dungeon;
character* game::PlayerBackup;
ushort game::PolymorphCounter = 0xFFFF;
uchar game::CurrentDungeon;

gamescript game::GameScript;

bool game::Flag;

std::string game::AutoSaveFileName = "Save/Autosave";
std::string game::Alignment[] = {"L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--"};
god* game::God[] = {0, new valpuri, new venius, new atavus, new dulcis, new inasnum, new seges, new consummo, new loricatus, new mellis, new calamus, new pestifer, new macellarius, new scabies, new infuscor, new cruentus, new erado, 0};

command* game::Command[] = {	0,
				new command(&character::Apply, "apply", 'a', false),
				new command(&character::Close, "close", 'c', false),
				new command(&character::Consume, "consume", 'e', true),
				new command(&character::DecreaseGamma, "decrease hardware gamma", 'G', true),
				new command(&character::DecreaseSoftGamma, "decrease software gamma", 'F', true),
				new command(&character::Dip, "dip", 'D', true),
				new command(&character::Drop, "drop", 'd', false),
				new command(&character::WhatToEngrave, "engrave", 'E', false),
				new command(&character::GainAllItems, "give all items cheat", 'A', true),
				new command(&character::GoDown, "go down", '>', true),
				new command(&character::GoUp, "go up", '<', true),
				new command(&character::IncreaseGamma, "increase hardware gamma", 'g', true),
				new command(&character::IncreaseSoftGamma, "increase software gamma", 'f', true),
				new command(&character::Kick, "kick", 'k', false),
				new command(&character::Look, "look", 'l', true),
				new command(&character::LowerStats, "lower stats", 'T', true),
				new command(&character::Offer, "offer", 'O', false),
				new command(&character::Open, "open", 'o', false),
				new command(&character::PickUp, "pick up", ',', false),
				new command(&character::Pray, "pray", 'p', false),
				new command(&character::Quit, "quit", 'q', true),
				new command(&character::RaiseStats, "raise stats", 'R', true),
				new command(&character::Read, "read", 'r', false),
				new command(&character::Save, "save", 's', true),
				new command(&character::SeeWholeMap, "see whole map", 'Y', true),
				new command(&character::ShowInventory, "show inventory", 'i', true),
				new command(&character::ShowKeyLayout, "show key layout", '?', true),
				new command(&character::DrawMessageHistory, "show message history", 'M', true),
				new command(&character::Talk, "talk", 'C', false),
				new command(&character::ScreenShot, "take screenshot", 'S', true),
				new command(&character::Throw, "throw", 't', false),
				new command(&character::WalkThroughWalls, "toggle walk through walls cheat", 'U', true),
				new command(&character::ForceVomit, "vomit", 'v', false),
				new command(&character::NOP, "wait", '.', true),
				new command(&character::WearArmor, "wear armor", 'W', true),
				new command(&character::Wield, "wield", 'w', true),
				new command(&character::WizardMode, "wizard mode", 'X', true),
				new command(&character::Zap, "zap", 'z', false),
				0};

int game::MoveCommandKey[DIRECTION_COMMAND_KEYS] = {0x147, 0x148, 0x149, 0x14B, 0x14D, 0x14F, 0x150, 0x151};
const vector2d game::MoveVector[DIRECTION_COMMAND_KEYS] = {vector2d(-1, -1), vector2d(0, -1), vector2d(1, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(-1, 1), vector2d(0, 1), vector2d(1, 1)};

game::panel game::Panel;

ushort*** game::LuxTable;
ushort* game::LuxTableSize;
bool game::Running;
character* game::Player;
vector2d game::Camera(0, 0);
bool game::WizardMode;
bool game::SeeWholeMapCheat;
uchar game::Gamma = 255;
bool game::GoThroughWallsCheat;
bool KeyIsOK(char);
std::string game::PlayerName;
uchar game::GodNumber;
ulong game::Turns;
float game::SoftGamma = 1;

void game::InitScript()
{
	srand(time(0));
	inputfile ScriptFile("Script/Dungeon.dat");
	GameScript.ReadFrom(ScriptFile);
}

void game::Init(std::string Name)
{
	static ushort Counter = 0;

	FILE* Debug = fopen("Debug.txt", "w");
	Turns = 0;
	EMPTY_MESSAGES();
	globalmessagingsystem::Format();
	ADD_MESSAGE("Initialization of game number %d started...", ++Counter);

	fprintf(Debug, "Game: %d.", Counter);

	fclose(Debug);

	Running = true;
	WizardMode = false;
	SeeWholeMapCheat = false;
	GoThroughWallsCheat = false;
	InWilderness = false;
	PlayerBackup = 0;
	PolymorphCounter = 0xFFFF;
	srand(time(0));
	game::CalculateGodNumber();

	if(Name == "")
	{
		DOUBLEBUFFER->ClearToColor(0);
		SetPlayerName(iosystem::StringQuestion(FONTW, "What is your name?", vector2d(30, 50), 3, 30));
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
		iosystem::TextScreen(FONTW, "On the evening Perttu seems very concerned about something.\nAfter the daily funerals he calls you to his throne room and explains:\n\"Elpuri, the Dark Frog, has infected the dungeon under the cathedral!\nIn the glory of Valpuri, I have decided to allow thee to slay him\nand bring me his head as proof. Return when thou hast succeeded.\"");

		iosystem::TextScreen(FONTW, "Generating game...\n\nThis may take some time, please wait.", false);

		SetPlayer(new human);

		BaseScore = Player->Score();

		GetPlayer()->SetRelations(2);

		game::GetPlayer()->GetStack()->FastAddItem(new lamp);

		InitDungeons();

		WorldMap = new worldmap(128, 128);
		WorldMap->Generate();

		InWilderness = true;

		UpDateCameraX();
		UpDateCameraY();

		GetCurrentArea()->UpdateLOS();
		GetCurrentArea()->SendNewDrawRequest();

		{
		for(ushort c = 1; GetGod(c); ++c)
		{
			GetGod(c)->SetRelation(0);
			GetGod(c)->SetTimer(0);
		}
		}

		Turns = 0;

		ADD_MESSAGE("Game generated successfully.");
	}
}

void game::DeInit()
{
	delete GetPlayerBackup();
	delete WorldMap;

	for(uchar c = 0; c < Dungeon.size(); ++c)
		delete Dungeon[c];
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
			ushort MaxDist = c >= 64 ? ushort(sqrt(c - 64)) : 0, MaxSize = (MaxDist << 1) + 1;

			LuxTableSize[c] = MaxSize;

			LuxTable[c] = new ushort*[MaxSize];

			for(long x = 0; x < MaxSize; x++)
			{
				LuxTable[c][x] = new ushort[MaxSize];

				for(long y = 0; y < MaxSize; y++)
				{
					long xLevelSquare = abs(x - MaxDist), yLevelSquare = abs(y - MaxDist);

					LuxTable[c][x][y] = ushort(float(c) / (float(xLevelSquare * xLevelSquare + yLevelSquare * yLevelSquare) / 64 + 1));
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
			for(ushort x = 0; x < LuxTableSize[c]; x++)
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

bool game::FlagHandler(ushort CX, ushort CY, ushort OX, ushort OY) // CurrentX = CX, CurrentY = CY
{                                                   // OrigoX = OX, OrigoY = OY
	if(CX >= GetCurrentArea()->GetXSize() || CY >= GetCurrentArea()->GetYSize())
		return false;

	GetCurrentArea()->GetSquare(vector2d(CX, CY))->SetFlag();
	GetCurrentArea()->GetSquare(vector2d(CX, CY))->UpdateMemorizedDescription();
	GetCurrentArea()->GetSquare(vector2d(CX, CY))->SendNewDrawRequest();

	if(CX == OX && CY == OY)
		return true;
	else
		if(!InWilderness)
			return GetLevel(Current)->GetLevelSquare(vector2d(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
		else
			return true;
}

bool game::DoLine(int X1, int Y1, int X2, int Y2, bool (*Proc)(ushort, ushort, ushort, ushort))
{
	int DX = X2 - X1, DY = Y2 - Y1, I1, I2, X, Y, DD;

	#define DO_LINE(PriSign, PriC, PriCond, SecSign, SecC, SecCond)		\
	{									\
		if(!D##PriC)							\
		{								\
			Proc(X1, Y1, X1, Y1);					\
			return true;						\
		}								\
										\
		I1 = D##SecC << 1;						\
		DD = I1 - (SecSign (PriSign D##PriC));				\
		I2 = DD - (SecSign (PriSign D##PriC));				\
										\
		X = X1;								\
		Y = Y1;								\
										\
		while(PriC PriCond PriC##2)					\
		{								\
			if(!Proc(X, Y, X1, Y1))					\
				return false;					\
										\
			if(DD SecCond 0)					\
			{							\
				SecC##SecSign##SecSign;				\
				DD += I2;					\
			}							\
			else							\
				DD += I1;					\
										\
			PriC##PriSign##PriSign;					\
		}								\
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

	FONTW->Printf(DOUBLEBUFFER, 16, 524, "%s %s", game::GetPlayerName().c_str(), Player->CNAME(DEFINITE));

	FONTW->Printf(DOUBLEBUFFER, 16, 534, "Strength: %d", Player->GetStrength());
	FONTW->Printf(DOUBLEBUFFER, 16, 544, "Endurance: %d", Player->GetEndurance());
	FONTW->Printf(DOUBLEBUFFER, 16, 554, "Agility: %d", Player->GetAgility());
	FONTW->Printf(DOUBLEBUFFER, 16, 564, "Perception: %d", Player->GetPerception());
	FONTW->Printf(DOUBLEBUFFER, 16, 574, "Size: %d", Player->GetSize());
	(Player->GetHP() < (Player->GetEndurance() << 1) / 3 ? FONTR : FONTW)->Printf(DOUBLEBUFFER, 16, 584, "HP: %d/%d", Player->GetHP(), (Player->GetEndurance() << 1));

	FONTW->Printf(DOUBLEBUFFER, 160, 534, "Exp: %d", Player->GetStrengthExperience());
	FONTW->Printf(DOUBLEBUFFER, 160, 544, "Exp: %d", Player->GetEnduranceExperience());
	FONTW->Printf(DOUBLEBUFFER, 160, 554, "Exp: %d", Player->GetAgilityExperience());
	FONTW->Printf(DOUBLEBUFFER, 160, 564, "Exp: %d", Player->GetPerceptionExperience());

	if(Player->GetWielded())
		FONTW->Printf(DOUBLEBUFFER, 160, 574, "Wielded: %s", Player->GetWielded()->CNAME(INDEFINITE));
        if(Player->GetTorsoArmor())
		FONTW->Printf(DOUBLEBUFFER, 160, 584, "Worn: %s", Player->GetTorsoArmor()->CNAME(INDEFINITE));

	FONTW->Printf(DOUBLEBUFFER, 320, 534, "Speed: %d", Player->GetWielded() ? ushort(sqrt((ulong(Player->GetAgility() << 2) + Player->GetStrength()) * 20000 / Player->GetWielded()->GetWeight())) : ulong(Player->GetAgility() << 2) + Player->GetStrength());
	FONTW->Printf(DOUBLEBUFFER, 320, 544, "Armor Value: %d", Player->CalculateArmorModifier());
	FONTW->Printf(DOUBLEBUFFER, 320, 554, "Weaponstrength: %.0f", Player->GetAttackStrength());
	FONTW->Printf(DOUBLEBUFFER, 320, 564, "Min dam & Max dam: %d, %d", ushort(Player->GetAttackStrength() * Player->GetStrength() / 26667), ushort(Player->GetAttackStrength() * Player->GetStrength() / 16000 + 1));
	FONTW->Printf(DOUBLEBUFFER, 600, 544, "Dungeon level: %d", game::GetCurrent() + 1);
	FONTW->Printf(DOUBLEBUFFER, 600, 554, "NP: %d", Player->GetNP());
	FONTW->Printf(DOUBLEBUFFER, 600, 564, "Turns: %d", game::GetTurns());

	if(Player->GetNP() < CRITICALHUNGERLEVEL)
		FONTR->Printf(DOUBLEBUFFER, 600, 574, "Fainting");
	else
		if(Player->GetNP() < HUNGERLEVEL)
			FONTB->Printf(DOUBLEBUFFER, 600, 574, "Hungry");

	switch(Player->GetBurdenState())
	{
		case OVERLOADED:
			FONTR->Printf(DOUBLEBUFFER, 600, 584, "Overload!");
		break;
		case STRESSED:
			FONTB->Printf(DOUBLEBUFFER, 600, 584, "Stressed");
		break;
		case BURDENED:
			FONTB->Printf(DOUBLEBUFFER, 600, 584, "Burdened!");
                case UNBURDENED:
		break;
	}
}

void game::UpDateCameraX()
{
	if(Player->GetPos().X < 25)
		Camera.X = 0;
	else
		Camera.X = Player->GetPos().X - 25;

	GetCurrentArea()->SendNewDrawRequest();
}

void game::UpDateCameraY()
{
	if(Player->GetPos().Y < 18)
		Camera.Y = 0;
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
	game::GetCurrentArea()->Draw();
	if(OnScreen(GetPlayer()->GetPos()))
		igraph::DrawCursor((GetPlayer()->GetPos() - GetCamera() + vector2d(0,2)) << 4);
	game::Panel.Draw();
	DRAW_MESSAGES();
	if(EmptyMsg) EMPTY_MESSAGES();
}

bool game::Save(std::string SaveName)
{
	outputfile SaveFile(SaveName + ".sav");

	SaveFile << PlayerName;
	SaveFile << CurrentDungeon << Current << Camera << WizardMode << SeeWholeMapCheat << Gamma;
	SaveFile << GoThroughWallsCheat << BaseScore << Turns << SoftGamma << InWilderness << PolymorphCounter;

	time_t Time = time(0);
	srand(Time);
	SaveFile << Time;

	SaveFile << Dungeon;

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
	SaveFile >> CurrentDungeon >> Current >> Camera >> WizardMode >> SeeWholeMapCheat >> Gamma;
	SaveFile >> GoThroughWallsCheat >> BaseScore >> Turns >> SoftGamma >> InWilderness >> PolymorphCounter;

	time_t Time;
	SaveFile >> Time;
	srand(Time);

	SaveFile >> Dungeon;

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

bool game::EmitationHandler(ushort CX, ushort CY, ushort OX, ushort OY)
{
	ushort Emit = GetLevel(Current)->GetLevelSquare(vector2d(OX, OY))->GetEmitation();

	ushort MaxSize = (game::GetLuxTableSize()[Emit] >> 1);

	if(long(CX) - long(OX) > MaxSize || long(OX) - long(CX) > MaxSize || long(CY) - long(OY) > MaxSize || long(OY) - long(CY) > MaxSize)
		Emit = 0;
	else
		Emit = game::GetLuxTable()[Emit][long(CX) - long(OX) + (game::GetLuxTableSize()[Emit] >> 1)][long(CY) - long(OY) + (game::GetLuxTableSize()[Emit] >> 1)];

	GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(vector2d(CX, CY))->AlterLuminance(vector2d(OX, OY), Emit);

	if(CX == OX && CY == OY)
		return true;
	else
		return GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(vector2d(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
}

bool game::NoxifyHandler(ushort CX, ushort CY, ushort OX, ushort OY)
{
	GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(vector2d(CX, CY))->NoxifyEmitter(vector2d(OX, OY));

	if(CX == OX && CY == OY)
		return true;
	else
		return GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(vector2d(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
}

void game::UpdateCameraXWithPos(ushort Coord)
{
        if(Coord < 25)
		Camera.X = 0;
	else
                Camera.X = Coord - 25;

	GetCurrentArea()->SendNewDrawRequest();
}

void game::UpdateCameraYWithPos(ushort Coord)
{
        if(Coord < 25)
                Camera.Y = 0;
	else
                Camera.Y = Coord - 25;

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
		FONTW->Printf(DOUBLEBUFFER, 40, 35, "%s", String.c_str());
		graphics::BlitDBToScreen();
	}
	return GetDirectionVectorForKey(GETKEY());
}

bool game::EyeHandler(ushort CX, ushort CY, ushort OX, ushort OY)  // CurrentX = CX, CurrentY = CY
{                                                   // OrigoX = OX, OrigoY = OY
	if(CX == OX && CY == OY)
		return true;
	else
		return GetCurrentDungeon()->GetLevel(Current)->GetLevelSquare(vector2d(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
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
	float Base = game::GetPlayer()->GetDifficulty() * (Current + 1);

	while(true)
	{
		float Dice = float(rand() % 5);

		if(!Dice)
		{
			Base /= 5;
			continue;
		}

		if(Dice == 4)
		{
			Base *= 5;
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
	ADD_MESSAGE("The dungeon underneath vibrates violently.");
	GetCurrentDungeon()->LoadLevel(SaveName(), 6);
	GetLevel(6)->CreateStairs(false);
	GetCurrentDungeon()->GetLevel(6)->SetLevelMessage("You feel something has changed since you were last here...");
	GetCurrentDungeon()->SaveLevel(SaveName(), 6);
}

void game::CalculateGodNumber()
{
	for(ushort c = 1;; ++c)
	{
		if(game::GetGod(c) == 0)
		{
			SetGodNumber(c - 1);
			break;
		}
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
		GetGod(c)->SetRelation( GetGod(c)->GetRelation()  + short(Change) );
	}
}

void game::DoEvilDeed(short Amount)
{
	if(!Amount) return;

	for(uchar c = 1; c < game::GetGodNumber() + 1; ++c)
	{
		float Change = - Amount + float(2 * Amount / 10) * GetGod(c)->Alignment();
		GetGod(c)->SetRelation( GetGod(c)->GetRelation()  + short(Change) );
	}
}

void game::DoNeutralDeed(short Amount)
{
	if(!Amount) return;

	ADD_MESSAGE("If you are a coder, you could help us make game::DoNeutralDeed");
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
