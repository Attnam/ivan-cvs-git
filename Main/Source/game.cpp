#include <cmath>
#include <ctime>
#include <io.h>

#include "game.h"
#include "level.h"
#include "char.h"
#include "error.h"
#include "item.h"
#include "god.h"
#include "igraph.h"
#include "strover.h"
#include "list.h"
#include "whandler.h"
#include "lsquare.h"
#include "stack.h"
#include "lterrain.h"
#include "worldmap.h"
#include "message.h"
#include "pool.h"
#include "proto.h"

level** game::Level;
ushort game::Levels = 10, game::Current;
long game::BaseScore;
bool game::InWilderness = false;
worldmap* game::WorldMap;
area* game::AreaInLoad;
square* game::SquareInLoad;

bool game::Flag;

dynarray<character*> game::Hell;

std::string game::AutoSaveFileName = "Save/Autosave";
std::string game::Alignment[] = {"L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--"};
god* game::God[] = {0, new valpuri, new venius, new atavus, new dulcis, new inasnum, new seges, new consummo, new loricatus, new mellis, new calamus, new pestifer, new macellarius, new scabies, new infuscor, new cruentus, new erado, 0};

command* game::Command[] = {	0,
				new command(&character::Consume, "consume", 'e', true),
				new command(&character::ShowInventory, "show inventory", 'i', true),
				new command(&character::PickUp, "pick up", ',', false),
				new command(&character::Quit, "quit", 'q', true),
				new command(&character::Drop, "drop", 'd', false),
				new command(&character::Wield, "wield", 'w', true),
				new command(&character::GoUp, "go up", '<', false),
				new command(&character::GoDown, "go down", '>', true),
				new command(&character::Open, "open", 'o', false),
				new command(&character::Close, "close", 'c', false),
				new command(&character::WearArmor, "wear armor", 'W', true),
				new command(&character::Talk, "talk", 'C', false),
				new command(&character::NOP, "wait", '.', true),
				new command(&character::Save, "save", 's', true),
				new command(&character::Read, "read", 'r', false),
				new command(&character::Dip, "dip", 'D', true),
				new command(&character::WizardMode, "wizard mode", 'X', true),
				new command(&character::RaiseStats, "raise stats", 'R', true),
				new command(&character::LowerStats, "lower stats", 'T', true),
				new command(&character::SeeWholeMap, "see whole map", 'Y', true),
				new command(&character::IncreaseGamma, "increase hardware gamma", 'g', true),
				new command(&character::DecreaseGamma, "decrease hardware gamma", 'G', true),
				new command(&character::WalkThroughWalls, "toggle walk through walls cheat", 'U', true),
				new command(&character::ShowKeyLayout, "show key layout", '?', true),
				new command(&character::Look, "look", 'l', true),
				new command(&character::WhatToEngrave, "engrave", 'E', false),
				new command(&character::Pray, "pray", 'p', false),
				new command(&character::Kick, "kick", 'k', false),
				new command(&character::ScreenShot, "take screenshot", 'S', true),
				new command(&character::Offer, "offer", 'O', false),
				new command(&character::IncreaseSoftGamma, "increase software gamma", 'f', true),
				new command(&character::DecreaseSoftGamma, "decrease software gamma", 'F', true),
				new command(&character::DrawMessageHistory, "show message history", 'M', true),
				new command(&character::Throw, "throw", 't', false),
				new command(&character::Apply, "apply", 'a', false),
				new command(&character::GainAllItems, "give all items cheat", 'A', true),
				new command(&character::ForceVomit, "vomit", 'v', false),
				new command(&character::Zap, "zap", 'z', false),
				0};

int game::MoveCommandKey[DIRECTION_COMMAND_KEYS] = {0x14D, 0x148, 0x150, 0x14B, 0x147, 0x149, 0x151, 0x14F};
const vector game::MoveVector[DIRECTION_COMMAND_KEYS] = {vector(1, 0), vector(0, -1), vector(0, 1), vector(-1, 0), vector(-1, -1), vector(1, -1), vector(1, 1), vector(-1, 1)};

std::string game::LevelMsg[] = {"", "", "", "", "", "", "", "", "", ""};
game::panel game::Panel;

ushort*** game::LuxTable;
ushort* game::LuxTableSize;
bool game::Running;
character* game::Player;
vector game::Camera(0, 0);
bool game::WizardMode;
bool game::SeeWholeMapCheat;
uchar game::Gamma = 255;
bool game::GoThroughWallsCheat;
bool KeyIsOK(char);
std::string game::PlayerName;
uchar game::GodNumber;
ulong game::Turns;
float game::SoftGamma = 1;

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
	srand(time(0));
	game::CalculateGodNumber();

	LevelMsg[3] = "You hear a wailing scream in the distance. An Enner Beast must dwell in the level!";
	LevelMsg[6] = "You shudder as you sense a being of pure darkness nearby. Your goal is near.";
	LevelMsg[9] = "You are welcomed by an evil laughter: \"Welcome to my private lair, mortal! There's no escape now! Prepare to be pepsified!\" Suddenly the stairs behind you are gone.";

	if(Name == "")
		SetPlayerName(StringQuestionWithClear("What is your name?", 30));
	else
		SetPlayerName(Name);

	if(Load())
	{
		Flag = true;

		ADD_MESSAGE("Game loaded successfully.");
	}
	else
	{
		StoryScreen("On this evening Perttu seems very concerned about something.\nAfter the daily funerals he calls you to his office and explains:\n\"Elpuri, the Dark Frog, has infected the dungeon under the cathedral!\nIn the glory of Valpuri, I have decided to allow thee to slay him\nand bring me his head as proof. Return when thee hast succeeded.\"");

		StoryScreen("Generating game...\n\nThis may take some time, please wait.", false);

		SetPlayer(new human);

		BaseScore = Player->Score();

		GetPlayer()->SetRelations(2);

		WorldMap = new worldmap(400, 400);
		WorldMap->Generate();

		Level = new level*[Levels];

		{
		for(ushort c = 0; c < game::Levels; c++)
			Level[c] = new level(36, 36, c);
		}

		vector PerttuPos = vector(5 + rand() % (Level[0]->GetXSize() - 10), 5 + rand() % (Level[0]->GetYSize() - 10));

		{
		for(ushort c = 0; !Level[0]->MakeRoom(PerttuPos + vector(-2, -2), vector(5, 5), false); c++)
			PerttuPos = vector(5 + rand() % (Level[0]->GetXSize() - 10), 5 + rand() % (Level[0]->GetYSize() - 10));
		}

		Level[0]->GetLevelSquare(PerttuPos)->ChangeLevelTerrain(new parquet, new stairsup);
		Level[0]->SetUpStairs(PerttuPos);

		Level[0]->GetLevelSquare(PerttuPos)->FastAddCharacter(new perttu);
		//Level[0]->PutPlayerAround(PerttuPos);

		InWilderness = true;

		game::GetPlayer()->GetStack()->FastAddItem(new banana);
		WorldMap->GetSquare(vector(18, 18))->AddCharacter(game::GetPlayer());
		game::GetPlayer()->SetSquareUnder(WorldMap->GetSquare(vector(18, 18)));

		vector Pos = vector(6 + rand() % (Level[8]->GetXSize() - 12), 6 + rand() % (Level[8]->GetYSize() - 12));

		{
		for(ushort c = 0; !Level[8]->MakeRoom(Pos + vector(-3, -3), vector(7, 7), false); c++)
			Pos = vector(6 + rand() % (Level[8]->GetXSize() - 12), 6 + rand() % (Level[8]->GetYSize() - 12));
		}

		Level[9]->MakeRoom(Pos + vector(-3, -3), vector(7, 7), false, 16);

		Level[8]->GetLevelSquare(Pos)->ChangeLevelTerrain(new parquet, new stairsdown(new pepsi(1)));
		Level[8]->SetDownStairs(Pos);
		Level[8]->GetLevelSquare(Pos + vector(0, -2))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
		Level[8]->GetLevelSquare(Pos + vector(0,  2))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
		Level[8]->GetLevelSquare(Pos + vector(-2, 0))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
		Level[8]->GetLevelSquare(Pos + vector( 2, 0))->ChangeLevelTerrain(new parquet, new wall(new pepsi(1)));
		Level[9]->GetLevelSquare(Pos + vector(0,  2))->ChangeLevelTerrain(new parquet, new stairsup(new pepsi(1)));
		Level[9]->SetUpStairs(Pos + vector(0,  2));
		altar* Altar = new altar(new pepsi(1));
		Altar->SetOwnerGod(16);
		Level[9]->GetLevelSquare(Pos + vector(0, -2))->ChangeLevelTerrain(new parquet, Altar); //GGG

		DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, Level[9]->GetXSize() - 1, Level[9]->GetYSize() - 1, Level[9]->GetLevelSquare(vector(DoX, DoY))->FastAddCharacter(new swatcommando);)
		Level[9]->GetLevelSquare(Pos + vector(0, -2))->FastAddCharacter(new oree);

		Level[9]->GetLevelSquare(Pos + vector(0,  2))->FastAddCharacter(new golem(new pepsi(100000)));
		Level[9]->GetLevelSquare(Pos + vector(-2, 0))->FastAddCharacter(new golem(new pepsi(100000)));
		Level[9]->GetLevelSquare(Pos + vector( 2, 0))->FastAddCharacter(new golem(new pepsi(100000)));

		{
		for(ushort c = 0; c < game::GetLevels() - 1; c++)
		{
			Level[c]->Generate();

			if(c != 6 && c != 8)
				Level[c]->CreateDownStairs();
		}
		}

		Level[0]->GetLevelSquare(PerttuPos)->GetStack()->Clean();

		Pos = Level[3]->RandomSquare(true);
		Level[3]->GetLevelSquare(Pos)->FastAddCharacter(new ennerbeast);

		Pos = Level[6]->RandomSquare(true);
		Level[6]->GetLevelSquare(Pos)->FastAddCharacter(new elpuri);

		{
		for(ushort c = 0; c < Levels; c++)
			SaveLevel(SaveName(), c);
		}

		/*Current = 0;

		Level[0]->Luxify();*/

		UpDateCameraX();
		UpDateCameraY();

		game::GetCurrentArea()->UpdateLOS();

		{
		for(ushort c = 1; GetGod(c); c++)
		{
			GetGod(c)->SetRelation(0);
			GetGod(c)->SetTimer(0);
		}
		}

		Turns = 0;

		ADD_MESSAGE("Game generated successfully.");
	}
}

void game::DeInit(void)
{
	for(ushort c = 0; c < game::Levels; c++)
		delete Level[c];

	delete [] Level;

	delete WorldMap;

	BurnHellsContents();
}

void game::Run(void)
{
	while(GetRunning())
	{
		game::GetPlayer()->Act();
 
		/*if(!GetRunning())
			break;

		if(!InWilderness)
			Level[Current]->HandleCharacters();
		else
			game::GetPlayer()->SetHasActed(false);*/

		if(!InWilderness)
			Level[Current]->HandleCharacters();	// Temporary

		objectpool::Be();

		if(!GetRunning())
			break;

		BurnHellsContents();
	}
}

void game::InitLuxTable(void)
{
	static bool AlreadyInitialized = false;

	if(!AlreadyInitialized)
	{
		AlreadyInitialized = true;

		LuxTable = new ushort**[0x200];

		LuxTableSize = new ushort[0x200];

		for(ushort c = 0; c < 0x200; c++)
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

/*
c / (x^2 / 64 + 1) >= 64
c >= x^2 + 64
c - 64 >= x^2
x <= sqrt(c - 64)
*/

void game::DeInitLuxTable(void)
{
	if(LuxTable)
	{
		for(ushort c = 0; c < 0x200; c++)
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

void game::Quit(void)
{
	Running = false;
}

bool game::FlagHandler(ushort CX, ushort CY, ushort OX, ushort OY) // CurrentX = CX, CurrentY = CY
{                                                   // OrigoX = OX, OrigoY = OY
	if(CX >= GetCurrentArea()->GetXSize() || CY >= GetCurrentArea()->GetYSize())
		return false;

	//Level[Current]->GetLevelSquare(vector(CX, CY))->SetFlag();
	GetCurrentArea()->GetSquare(vector(CX, CY))->SetFlag();
	//GGG Level[Current]->GetLevelSquare(vector(CX, CY))->UpdateItemMemory();

	if(CX == OX && CY == OY)
		return true;
	else
		if(!InWilderness)
			return Level[Current]->GetLevelSquare(vector(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
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

void game::panel::Draw(void) const
{
	character* Player = game::GetPlayer();

	FONTW->PrintfToDB(16, 524, "Name: %s", game::GetPlayerName().c_str());

	FONTW->PrintfToDB(16, 534, "Strength: %d", Player->GetStrength());
	FONTW->PrintfToDB(16, 544, "Endurance: %d", Player->GetEndurance());
	FONTW->PrintfToDB(16, 554, "Agility: %d", Player->GetAgility());
	FONTW->PrintfToDB(16, 564, "Perception: %d", Player->GetPerception());
	FONTW->PrintfToDB(16, 574, "Size: %d", Player->GetSize());
	(Player->GetHP() < (Player->GetEndurance() << 1) / 3 ? FONTR : FONTW)->PrintfToDB(16, 584, "HP: %d/%d", Player->GetHP(), (Player->GetEndurance() << 1));

	FONTW->PrintfToDB(160, 534, "Exp: %d", Player->GetStrengthExperience());
	FONTW->PrintfToDB(160, 544, "Exp: %d", Player->GetEnduranceExperience());
	FONTW->PrintfToDB(160, 554, "Exp: %d", Player->GetAgilityExperience());
	FONTW->PrintfToDB(160, 564, "Exp: %d", Player->GetPerceptionExperience());

	if(Player->GetWielded())
		FONTW->PrintfToDB(160, 574, "Wielded: %s", Player->GetWielded()->CNAME(INDEFINITE));
        if(Player->GetTorsoArmor())
		FONTW->PrintfToDB(160, 584, "Worn: %s", Player->GetTorsoArmor()->CNAME(INDEFINITE));

	FONTW->PrintfToDB(320, 534, "Speed: %d", Player->GetWielded() ? ushort(sqrt((ulong(Player->GetAgility() << 2) + Player->GetStrength()) * 20000 / Player->GetWielded()->GetWeight())) : ulong(Player->GetAgility() << 2) + Player->GetStrength());
	FONTW->PrintfToDB(320, 544, "Armor Value: %d", Player->CalculateArmorModifier());
	FONTW->PrintfToDB(320, 554, "Weaponstrength: %.0f", Player->GetAttackStrength());
	FONTW->PrintfToDB(320, 564, "Min dam & Max dam: %d, %d", ushort(Player->GetAttackStrength() * Player->GetStrength() / 26667), ushort(Player->GetAttackStrength() * Player->GetStrength() / 16000 + 1));
	FONTW->PrintfToDB(600, 534, "You are %s", Player->CNAME(INDEFINITE));
	FONTW->PrintfToDB(600, 544, "Dungeon level: %d", game::GetCurrent() + 1);
	FONTW->PrintfToDB(600, 554, "NP: %d", Player->GetNP());
	FONTW->PrintfToDB(600, 564, "Turns: %d", game::GetTurns());
	if(Player->GetNP() < CRITICALHUNGERLEVEL) FONTR->PrintfToDB(600, 574, "Fainting");
	else if(Player->GetNP() < HUNGERLEVEL) FONTB->PrintfToDB(600, 574, "Hungry");
	switch(Player->GetBurdenState())
	{
		case OVERLOADED:
			FONTR->PrintfToDB(600, 584, "Overload!");
		break;
		case STRESSED:
			FONTB->PrintfToDB(600, 584, "Stressed");
		break;
		case BURDENED:
			FONTB->PrintfToDB(600, 584, "Burdened!");
                case UNBURDENED:
		break;
	}
}

void game::UpDateCameraX(void)
{
	if(Player->GetPos().X < 25)
		Camera.X = 0;
	else
		Camera.X = Player->GetPos().X - 25;
}

void game::UpDateCameraY(void)
{
	if(Player->GetPos().Y < 18)
		Camera.Y = 0;
	else
		Camera.Y = Player->GetPos().Y - 18;
}

const char* game::Insult(void)
{
switch(rand()%11)
	{
	case 0  : return "you moron";
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
        default : return "hugger-mugger";
	}
}

unsigned int game::CountChars(char cSF,std::string sSH) // (MENU)
{
	unsigned int iReturnCounter = 0;
	for(unsigned int i = 0; i < sSH.length(); i++)
		if(sSH[i] == cSF)
			++iReturnCounter;
	return iReturnCounter;
}

int game::Menu(std::string sMS) // (MENU)
{
	if(CountChars('\r',sMS) < 1)
		return (-1);
	bool bReady = false;
	unsigned int iSelected = 0;
	while(!bReady)
	{
		std::string sCopyOfMS = sMS;
		graphics::ClearDBToColor(0);
		for(unsigned int i = 0; i < CountChars('\r',sMS); i++)
		{
			std::string HYVINEPAGURUPRINTF = sCopyOfMS.substr(0,sCopyOfMS.find_first_of('\r'));
			sCopyOfMS.erase(0,sCopyOfMS.find_first_of('\r')+1);
			(i == iSelected ? FONTB : FONTW)->PrintfToDB(400 - ((HYVINEPAGURUPRINTF.length() + 4) << 2),200+(i*50),"%d. %s",i + 1,HYVINEPAGURUPRINTF.c_str());
		}

		graphics::BlitDBToScreen();
		int k;
		switch (k = GETKEY())
		{
		// I don't know if you have included keys.h, so...
			case 0x148:
				if (iSelected > 0)
					--iSelected;
				else
					iSelected = (CountChars('\r',sMS)-1);
				break;
			case 0x150:
				if (iSelected < (CountChars('\r',sMS)-1))
					++iSelected;
				else
					iSelected = 0;
				break;
			case 0x00D:
				bReady = true;
				break;
			default:
				if(k > 0x30 && k < int(0x31 + CountChars('\r',sMS)))
					return signed(k - 0x31);

				break;
		}
	}
	return signed(iSelected);
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

void game::DrawEverythingNoBlit(bool EmptyMsg)
{
	graphics::ClearDBToColor(0);
	game::GetCurrentArea()->Draw();
	game::Panel.Draw();
	DRAW_MESSAGES();
	if(EmptyMsg) EMPTY_MESSAGES();
}

void game::StoryScreen(const char* Text, bool GKey)
{
	char Line[200];
        ushort LastBeginningOfLine = 0;
        ushort c;
        ushort Lines = 0;
	ushort LineNumber = 1;
	
	graphics::ClearDBToColor(0);
	{
        for(ushort cc = 0; cc < 200; cc++)
		Line[cc] = 0;
	}

	for(ushort cc = 0; cc < strlen(Text); cc++)
		if(Text[cc] == '\n') LineNumber++;

	for(c = 0; Text[c] != 0; c++)
	{
		if(Text[c] == '\n')
		{
		FONTW->PrintfToDB(400 - strlen(Line) * 4, 275 - (LineNumber - Lines) * 15, "%s", Line);
                        LastBeginningOfLine = c + 1;
                        Lines++;
                        for(ushort cc = 0; cc < 200; cc++)
                        	Line[cc] = 0;
                }
		else
			Line[c - LastBeginningOfLine] = Text[c];
	}

	FONTW->PrintfToDB(400 - strlen(Line) * 4, 275 - (LineNumber - Lines) * 15, "%s", Line);

	graphics::BlitDBToScreen();

	if(GKey)
		GETKEY();
}

bool game::Save(std::string SaveName)
{
	std::ofstream SaveFile((SaveName + ".sav").c_str(), std::ios::out | std::ios::binary);

	if(!SaveFile.is_open())
		return false;

	SaveFile << PlayerName;
	SaveFile.write((char*)&Levels, sizeof(Levels));
	SaveFile.write((char*)&Current, sizeof(Current));
	SaveFile.write((char*)&Camera, sizeof(Camera));
	SaveFile.write((char*)&WizardMode, sizeof(WizardMode));
	SaveFile.write((char*)&SeeWholeMapCheat, sizeof(SeeWholeMapCheat));
	SaveFile.write((char*)&Gamma, sizeof(Gamma));
	SaveFile.write((char*)&GoThroughWallsCheat, sizeof(GoThroughWallsCheat));
	SaveFile.write((char*)&BaseScore, sizeof(BaseScore));
	SaveFile.write((char*)&Turns, sizeof(Turns));
	SaveFile.write((char*)&SoftGamma, sizeof(SoftGamma));
	SaveFile.write((char*)&InWilderness, sizeof(InWilderness));

	time_t Time = time(0);
	srand(Time);
	SaveFile.write((char*)&Time, sizeof(Time));

	if(InWilderness)
		WorldMap->Save(SaveFile);
	else
		SaveLevel(SaveName, Current, false);

	{
	for(ushort c = 1; GetGod(c); c++)
		GetGod(c)->Save(SaveFile);
	}

	vector Pos = game::GetPlayer()->GetPos();

	SaveFile.write((char*)&Pos, sizeof(Pos));

	for(ushort c = 0; c < GetLevels(); c++)
		SaveFile << LevelMsg[c];

	return true;
}

bool game::Load(std::string SaveName)
{
	std::ifstream SaveFile((SaveName + ".sav").c_str(), std::ios::in | std::ios::binary);

	if(!SaveFile.is_open())
		return false;

	SaveFile >> PlayerName;
	SaveFile.read((char*)&Levels, sizeof(Levels));
	SaveFile.read((char*)&Current, sizeof(Current));
	SaveFile.read((char*)&Camera, sizeof(Camera));
	SaveFile.read((char*)&WizardMode, sizeof(WizardMode));
	SaveFile.read((char*)&SeeWholeMapCheat, sizeof(SeeWholeMapCheat));
	SaveFile.read((char*)&Gamma, sizeof(Gamma));
	SaveFile.read((char*)&GoThroughWallsCheat, sizeof(GoThroughWallsCheat));
	SaveFile.read((char*)&BaseScore, sizeof(BaseScore));
	SaveFile.read((char*)&Turns, sizeof(Turns));
	SaveFile.read((char*)&SoftGamma, sizeof(SoftGamma));
	SaveFile.read((char*)&InWilderness, sizeof(InWilderness));

	time_t Time;
	SaveFile.read((char*)&Time, sizeof(Time));
	srand(Time);

	Level = new level*[Levels];

	{
	for(ushort c = 0; c < Levels; c++)
		Level[c] = 0;
	}

	if(InWilderness)
	{
		WorldMap = new worldmap;
		WorldMap->Load(SaveFile);
	}
	else
		LoadLevel(SaveName);

	{
	for(ushort c = 1; GetGod(c); c++)
		GetGod(c)->Load(SaveFile);
	}

	vector Pos;

	SaveFile.read((char*)&Pos, sizeof(Pos));

	SetPlayer(GetCurrentArea()->GetSquare(Pos)->GetCharacter());

	for(ushort c = 0; c < GetLevels(); c++)
		SaveFile >> LevelMsg[c];

	return true;
}

std::string game::SaveName(void)
{
	std::string SaveName = std::string("Save/") + PlayerName;

	for(ushort c = 0; c < SaveName.length(); c++)
		if(SaveName[c] == ' ')
			SaveName[c] = '_';

	if(SaveName.length() > 13)
		SaveName.resize(13);

	return SaveName;
}

bool game::EmitationHandler(ushort CX, ushort CY, ushort OX, ushort OY)
{
	ushort Emit = Level[Current]->GetLevelSquare(vector(OX, OY))->GetEmitation();

	ushort MaxSize = (game::GetLuxTableSize()[Emit] >> 1);

	if(long(CX) - long(OX) > MaxSize || long(OX) - long(CX) > MaxSize || long(CY) - long(OY) > MaxSize || long(OY) - long(CY) > MaxSize)
		Emit = 0;
	else
		Emit = game::GetLuxTable()[Emit][long(CX) - long(OX) + (game::GetLuxTableSize()[Emit] >> 1)][long(CY) - long(OY) + (game::GetLuxTableSize()[Emit] >> 1)];

	Level[Current]->GetLevelSquare(vector(CX, CY))->AlterLuminance(vector(OX, OY), Emit);

	if(CX == OX && CY == OY)
		return true;
	else
		return Level[Current]->GetLevelSquare(vector(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
}

bool game::NoxifyHandler(ushort CX, ushort CY, ushort OX, ushort OY)
{
	Level[Current]->GetLevelSquare(vector(CX, CY))->NoxifyEmitter(vector(OX, OY));

	if(CX == OX && CY == OY)
		return true;
	else
		return Level[Current]->GetLevelSquare(vector(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
}

void game::UpdateCameraXWithPos(ushort Coord)
{
        if(Coord < 25)
		Camera.X = 0;
	else
                Camera.X = Coord - 25;
}

void game::UpdateCameraYWithPos(ushort Coord)
{
        if(Coord < 25)
                Camera.Y = 0;
	else
                Camera.Y = Coord - 25;
}

std::string game::StringQuestion(const char* String, ushort MaxLetters)
{
	char LastKey = 0;
	std::string Input,Buffer;

	while(LastKey != 13)
	{
		LastKey = 0;
		Buffer = Input + "_";
		game::DrawEverythingNoBlit();
		FONTW->PrintfToDB(50, 50, "%s", Buffer.c_str());
		FONTW->PrintfToDB(40, 35, "%s", String);
		graphics::BlitDBToScreen();

		while(!(game::KeyIsOK(LastKey) || LastKey == 8 || LastKey == 13) )
		{
			LastKey = GETKEY();
		}

		if(LastKey == 8)
		{
			if(Input.length()) Input.resize(Input.length() - 1);
		}
		else if(LastKey == 13)
		{
			break;
		}
		else
		{
			if(Input.length() < MaxLetters)
				Input += LastKey;
		}


	}
	return Input;
}

bool game::KeyIsOK(char Key)
{
        return Key >= 0x20;
}

int game::GetMoveCommandKey(vector A, vector B)
{
	for(uchar c = 0; c < 8; c++)
	{
		if((A + game::GetMoveVector(c)) == B)
			return game::MoveCommandKey[c];
	}
	return 0xFF;
}

void game::ApplyDivineTick(ushort Turns)
{
	for(ushort c = 1; GetGod(c); c++)
		GetGod(c)->ApplyDivineTick(Turns);
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, bool Good)
{
	for(ushort c = 1; GetGod(c); c++)
		if(GetGod(c) != CompareTarget)
			GetGod(c)->AdjustRelation(CompareTarget, Good);
}

void game::BurnHellsContents(void)
{
	while(Hell.Length())
		delete Hell.Remove(0);
}

void game::SendToHell(character* PassedAway)
{
	for(ushort c = 0; c < Hell.Length(); c++)
		if(Hell.Access(c) == PassedAway)
			return;

	Hell.Add(PassedAway);
}

vector game::GetDirectionVectorForKey(ushort Key)
{
	for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
		if(Key == game::GetMoveCommandKey(c))
			return game::GetMoveVector(c);

	return vector(0,0);
}

vector game::AskForDirectionVector(std::string String)
{
	if(String != "")
	{
		FONTW->PrintfToDB(40, 35, "%s", String.c_str());
		graphics::BlitDBToScreen();
	}
	return GetDirectionVectorForKey(GETKEY());
}

std::string game::StringQuestionWithClear(std::string String, ushort MaxLetters)
{
	char LastKey = 0;
	std::string Input,Buffer;

	while(true)
	{
		LastKey = 0;
		Buffer = Input + "_";
		graphics::ClearDBToColor(0);
		FONTW->PrintfToDB(50, 50, "%s", Buffer.c_str());
		FONTW->PrintfToDB(40, 35, "%s", String.c_str());
		graphics::BlitDBToScreen();

		while(!(game::KeyIsOK(LastKey) || LastKey == 8 || LastKey == 13) )
		{
			LastKey = GETKEY();
		}

		if(LastKey == 8)
		{
			if(Input.length()) Input.resize(Input.length() - 1);
		}
		else if(LastKey == 13)
		{
			if(Input.length()) break;
		}
		else
		{
			if(Input.length() < MaxLetters)
				Input += LastKey;
		}


	}
	return Input;
}

bool game::EyeHandler(ushort CX, ushort CY, ushort OX, ushort OY)  // CurrentX = CX, CurrentY = CY
{                                                   // OrigoX = OX, OrigoY = OY
	if(CX == OX && CY == OY)
		return true;
	else
		return Level[Current]->GetLevelSquare(vector(CX, CY))->GetOverLevelTerrain()->GetIsWalkable();
}

long game::GodScore(void)
{
	long Score = -1000;

	for(ushort c = 1; GetGod(c); c++)
	{
		if(GetGod(c)->GetRelation() > Score)
			Score = GetGod(c)->GetRelation();
	}

	return Score;
}

float game::Difficulty(void)
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

void game::ShowLevelMessage(void)
{
	if(LevelMsg[GetCurrent()].length())
		ADD_MESSAGE(LevelMsg[GetCurrent()].c_str());

	LevelMsg[GetCurrent()] = "";
}

void game::TriggerQuestForMaakotkaShirt(void)
{
	ADD_MESSAGE("The dungeon underneath vibrates violently.");

	LoadLevel(SaveName(), 6);
	LoadLevel(SaveName(), 7);

	vector Pos = Level[6]->CreateDownStairs();

	Level[7]->PutStairs(Pos);
	Level[7]->AttachPos(Pos);

	LevelMsg[6] = "You feel something has changed since you were last here...";

	SaveLevel(SaveName(), 6);
	SaveLevel(SaveName(), 7);
}

void game::CalculateGodNumber(void)
{
	for(ushort c = 1;; c++)
	{
		if(game::GetGod(c) == 0)
		{
			SetGodNumber(c - 1);
			break;
		}
	}
}

void game::WhatToLoadMenu(void) // for some _very_ strange reason "LoadMenu" occasionaly generates an error!
{
	struct _finddata_t Found;
	long hFile;
	int Check = 0;
	list Buffer("Chooseth a file and be sorry");
	std::string Name;
	hFile = _findfirst("Save/*.sav", &Found);
	if(hFile == -1L)
	{
		graphics::ClearDBToColor(0);
		FONTW->PrintfToDB(260, 200, "You don't have any previous saves.");
		graphics::BlitDBToScreen();
		GETKEY();
		return;
	}
	while(!Check)
	{
		Buffer.AddString(Found.name);
		Check = _findnext(hFile, &Found);
	}

	Check = 0xFFFF;
	while(Check > 0xFFFD)
	{
		graphics::ClearDBToColor(0);
		Check = Buffer.Draw();
	}
	if(Check == 0xFFFD)
		return;
	Name = Buffer.GetString(Check);
	Name.resize(Name.size() - 4);
	game::Init(Name);
	game::Run();
	game::DeInit();
}

uchar game::DirectionQuestion(std::string Topic, uchar DefaultAnswer, bool RequireAnswer)
{
	ADD_MESSAGE(Topic.c_str());

	DRAW_MESSAGES();

	EMPTY_MESSAGES();

	graphics::BlitDBToScreen();

	while(true)
	{
		int Key = GETKEY();

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::GetMoveCommandKey(c))
				return c;

		if(DefaultAnswer < 8) return DefaultAnswer;
		if(!RequireAnswer) return 0xFF;
	}

}

void game::SaveLevel(std::string SaveName, ushort Index, bool DeleteAfterwards)
{
	std::ofstream SaveFile((SaveName + ".l" + Index).c_str(), std::ios::out | std::ios::binary);

	if(!SaveFile.is_open())
		ABORT("Level lost!");

	Level[Index]->Save(SaveFile);

	if(DeleteAfterwards)
	{
		delete Level[Index];
		Level[Index] = 0;
	}
}

void game::LoadLevel(std::string SaveName, ushort Index)
{
	std::ifstream SaveFile((SaveName + ".l" + Index).c_str(), std::ios::in | std::ios::binary);

	if(!SaveFile.is_open())
		ABORT("Level gone!");

	Level[Index] = new level;
	Level[Index]->Load(SaveFile);
}

void game::RemoveSaves(void)
{
	remove((SaveName() + ".sav").c_str());
	remove((AutoSaveFileName + ".sav").c_str());

	for(ushort c = 0; c < Levels; c++)
	{
		remove((SaveName() + ".l" + c).c_str());
		remove((AutoSaveFileName + ".l" + c).c_str());
	}
}

void game::SetPlayer(character* NP)
{
	Player = NP;

	if(Player)
		Player->SetIsPlayer(true);
}

