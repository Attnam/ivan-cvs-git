#define __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__ // for prototyping system

#include "game.h"

prototypecontainer<character> game::CharacterPrototype;
prototypecontainer<item> game::ItemPrototype;

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <io.h>

#include "level.h"
#include "char.h"
#include "error.h"
#include "graphics.h"
#include "bitmap.h"
#include "item.h"
#include "god.h"
#include "igraph.h"
#include "strover.h"
#include "list.h"
#include "whandler.h"
#include "command.h"
#include "lsquare.h"
#include "stack.h"
#include "terrain.h"

level** game::Level;
ushort game::Levels = 10, game::Current;
long game::BaseScore;

bool game::Flag;

dynarray<character*> game::Hell;

std::string game::AutoSaveFileName = "Save/Autosave";
std::string game::Alignment[] = {"L++", "L+", "L", "L-", "N+", "N=", "N-", "C+", "C", "C-", "C--"};
god* game::God[] = {0, new valpuri, new venius, new atavus, new dulcis, new inasnum, new seges, new consummo, new loricatus, new mellis, new calamus, new pestifer, new macellarius, new scabies, new infuscor, new cruentus, new erado, 0};

command* game::Command[] = {	0,
				new command(&character::Consume, "consume", 'e'),
				new command(&character::ShowInventory, "show inventory", 'i'),
				new command(&character::PickUp, "pick up", ','),
				new command(&character::Quit, "quit", 'q'),
				new command(&character::Drop, "drop", 'd'),
				new command(&character::Wield, "wield", 'w'),
				new command(&character::GoUp, "go up", '<'),
				new command(&character::GoDown, "go down", '>'),
				new command(&character::Open, "open", 'o'),
				new command(&character::Close, "close", 'c'),
				new command(&character::WearArmor, "wear armor", 'W'),
				new command(&character::Talk, "talk", 'C'),
				new command(&character::NOP, "wait", '.'),
				new command(&character::Save, "save", 's'),
				new command(&character::Read, "read", 'r'),
				new command(&character::Dip, "dip", 'D'),
				new command(&character::WizardMode, "wizard mode", 'X'),
				new command(&character::RaiseStats, "raise stats", 'R'),
				new command(&character::LowerStats, "lower stats", 'T'),
				new command(&character::SeeWholeMap, "see whole map", 'Y'),
				new command(&character::IncreaseGamma, "increase hardware gamma", 'g'),
				new command(&character::DecreaseGamma, "decrease hardware gamma", 'G'),
				new command(&character::WalkThroughWalls, "toggle walk through walls cheat", 'U'),
				new command(&character::ShowKeyLayout, "show key layout", '?'),
				new command(&character::Look, "look", 'l'),
				new command(&character::WhatToEngrave, "engrave", 'E'),
				new command(&character::Pray, "pray", 'p'),
				new command(&character::Kick, "kick", 'k'),
				new command(&character::ScreenShot, "take screenshot", 'S'),
				new command(&character::Offer, "offer", 'O'),
				new command(&character::IncreaseSoftGamma, "increase software gamma", 'f'),
				new command(&character::DecreaseSoftGamma, "decrease software gamma", 'F'),
				new command(&character::DrawMessageHistory, "show message history", 'M'),
				new command(&character::Throw, "throw", 't'),
				new command(&character::Apply, "apply", 'a'),
				new command(&character::GainAllItems, "give all items cheat", 'A'),
				new command(&character::ForceVomit, "vomit", 'v'),
				0};

int game::MoveCommandKey[DIRECTION_COMMAND_KEYS] = {0x14D, 0x148, 0x150, 0x14B, 0x147, 0x149, 0x151, 0x14F};
const vector game::MoveVector[DIRECTION_COMMAND_KEYS] = {vector(1, 0), vector(0, -1), vector(0, 1), vector(-1, 0), vector(-1, -1), vector(1, -1), vector(1, 1), vector(-1, 1)};

std::string game::LevelMsg[] = {"", "", "", "", "", "", "", "", "", ""};
game::globalmessagingsystem game::GlobalMessagingSystem;
game::panel game::Panel;

ushort*** game::LuxTable;
ushort* game::LuxTableSize;
bool game::Running;
character* game::Player;
vector game::Camera(0, 0);
bool game::WizardMode;
bool game::SeeWholeMapCheat;
uchar game::Gamma = 255;
clock_t game::HandleCharTim;
clock_t game::HandlePlayTim;
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
	EMPTY_MESSAGES;
	game::GlobalMessagingSystem.Format();
	ADD_MESSAGE("Initialization of game number %d started...", ++Counter);

	fprintf(Debug, "Game: %d.", Counter);

	fclose(Debug);

	Running = true;
	WizardMode = false;
	SeeWholeMapCheat = false;
	GoThroughWallsCheat = false;
	srand(time(0));
	game::CalculateGodNumber();

	LevelMsg[3] = "You hear a wailing scream in the distance. An Enner Beast must dwell in the level!";
	LevelMsg[6] = "You shudder as you sense a being of pure darkness nearby. Your goal is near.";
	LevelMsg[9] = "You are welcomed by an evil laughter: \"Welcome to my private lair, mortal! There's no escape now! Prepare to be pepsified!\" Suddenly the stairs behind you are gone.";

	if(Name == "")
		SPlayerName(StringQuestionWithClear("What is your name?", 30));
	else
		SPlayerName(Name);

	if(Load())
	{
		Flag = true;

		ADD_MESSAGE("Game loaded successfully.");
	}
	else
	{
		StoryScreen("On this evening Perttu seems very concerned about something.\nAfter the daily funerals he calls you to his office and explains:\n\"Elpuri, the Dark Frog, has infected the dungeon under the cathedral!\nIn the glory of Valpuri, I have decided to allow thee to slay him\nand bring me his head as proof. Return when thee hast succeeded.\"");

		StoryScreen("Generating game...\n\nThis may take some time, please wait.", false);

		SPlayer(new human);

		BaseScore = Player->Score();

		CPlayer()->SRelations(2);

		Level = new level*[Levels];

		{
		for(ushort c = 0; c < game::Levels; c++)
			Level[c] = new level(36, 36, c);
		}

		vector Pos = vector(5 + rand() % (Level[0]->CXSize() - 10), 5 + rand() % (Level[0]->CYSize() - 10));

		{
		for(ushort c = 0; !Level[0]->MakeRoom(Pos + vector(-2, -2), vector(5, 5), false); c++)
			Pos = vector(5 + rand() % (Level[0]->CXSize() - 10), 5 + rand() % (Level[0]->CYSize() - 10));
		}

		Level[0]->CLevelSquare(Pos)->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new stairsup(NewMaterial(1, new stone(3))));

		Level[0]->CLevelSquare(Pos)->FastAddCharacter(new perttu);
		Level[0]->CLevelSquare(Pos)->CStack()->Clean();
		Level[0]->PutPlayerAround(Pos);

		Pos = vector(6 + rand() % (Level[8]->CXSize() - 12), 6 + rand() % (Level[8]->CYSize() - 12));

		{
		for(ushort c = 0; !Level[8]->MakeRoom(Pos + vector(-3, -3), vector(7, 7), false); c++)
			Pos = vector(6 + rand() % (Level[8]->CXSize() - 12), 6 + rand() % (Level[8]->CYSize() - 12));
		}

		Level[9]->MakeRoom(Pos + vector(-3, -3), vector(7, 7), false, 16);

		Level[8]->CLevelSquare(Pos)->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new stairsdown(NewMaterial(1, new stone(3))));
		Level[8]->CLevelSquare(Pos + vector(0, -2))->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new wall(NewMaterial(1, new pepsi(3))));
		Level[8]->CLevelSquare(Pos + vector(0,  2))->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new wall(NewMaterial(1, new pepsi(3))));
		Level[8]->CLevelSquare(Pos + vector(-2, 0))->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new wall(NewMaterial(1, new pepsi(3))));
		Level[8]->CLevelSquare(Pos + vector( 2, 0))->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new wall(NewMaterial(1, new pepsi(3))));
		Level[9]->CLevelSquare(Pos + vector(0,  2))->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new stairsup(NewMaterial(1, new stone(3))));
		Level[9]->CLevelSquare(Pos + vector(0, -2))->ChangeTerrain(new parquet(NewMaterial(1, new wood(3))), new altar(NewMaterial(1, new pepsi(3)), 16));

		DO_FOR_SQUARES_AROUND(Pos.X, Pos.Y, Level[9]->CXSize() - 1, Level[9]->CYSize() - 1, Level[9]->CLevelSquare(vector(DoX, DoY))->FastAddCharacter(new swatcommando);)
		Level[9]->CLevelSquare(Pos + vector(0, -2))->FastAddCharacter(new oree);
		Level[9]->CLevelSquare(Pos + vector(0,  2))->FastAddCharacter(new golem(NewMaterial(1, new pepsi(100000)), vector(256, 0), 250, 5, 35, 20, 12, 0));
		Level[9]->CLevelSquare(Pos + vector(-2, 0))->FastAddCharacter(new golem(NewMaterial(1, new pepsi(100000)), vector(256, 0), 250, 5, 35, 20, 12, 0));
		Level[9]->CLevelSquare(Pos + vector( 2, 0))->FastAddCharacter(new golem(NewMaterial(1, new pepsi(100000)), vector(256, 0), 250, 5, 35, 20, 12, 0));

		{
		for(ushort c = 0; c < game::CLevels() - 1; c++)
		{
			Level[c]->Generate();

			if(c != 6 && c != 8)
				Level[c]->CreateDownStairs();
		}
		}

		Pos = Level[3]->RandomSquare(true);
		Level[3]->CLevelSquare(Pos)->FastAddCharacter(new ennerbeast);

		Pos = Level[6]->RandomSquare(true);
		Level[6]->CLevelSquare(Pos)->FastAddCharacter(new elpuri);

		{
		for(ushort c = 1; c < Levels; c++)
			SaveLevel(SaveName(), c);
		}

		Current = 0;

		Level[0]->Luxify();

		UpDateCameraX();
		UpDateCameraY();

		game::CCurrentLevel()->UpdateLOS();

		{
		for(ushort c = 1; CGod(c); c++)
		{
			CGod(c)->SRelation(0);
			CGod(c)->STimer(0);
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

	BurnHellsContents();
}

void game::Run(void)
{
	HandleCharTim = HandlePlayTim = 0;

	while(CRunning())
	{
		game::CPlayer()->Act();

		if(!CRunning())
			break;

		clock_t Tim = clock();

		Level[Current]->HandleCharacters();

		BurnHellsContents();

		HandleCharTim += clock() - Tim;
	}
}

void game::globalmessagingsystem::AddMessage(const char* Format, ...)
{
	char Message[256];
	char Buffer[256];

	va_list AP;
	va_start(AP, Format);
	vsprintf(Message, Format, AP);
	va_end(AP);

	if(Message[0] > 0x60 && Message[0] < 0x7B)
		Message[0] &= ~0x20;              // Very guru and odd. Capitalizes the first letter!

	if(MessageBuffer)
	{
		ushort NewLength = BufferLength + strlen(Message) + 1;

		char* TempBuffer = new char[NewLength];

		{
		for(ushort c = 0; c < BufferLength; c++)
			TempBuffer[c] = MessageBuffer[c];
		}

		TempBuffer[BufferLength] = ' ';

		for(ushort c = BufferLength + 1, i = 0; c < NewLength; c++, i++)
			TempBuffer[c] = Message[i];

		delete [] MessageBuffer;

		MessageBuffer = TempBuffer;

		BufferLength = NewLength;
	}
	else
	{
		BufferLength = strlen(Message);

		MessageBuffer = new char[BufferLength];

		for(ushort c = 0; c < BufferLength; c++)
			MessageBuffer[c] = Message[c];
	}
	sprintf(Buffer, "%d - %s", int(game::CTurns()), Message);
	game::GlobalMessagingSystem.MessageHistory.AddString(Buffer);
	while(game::GlobalMessagingSystem.MessageHistory.Length() > 200)
		game::GlobalMessagingSystem.MessageHistory.CString()->Remove(0);
}

void game::globalmessagingsystem::Draw(void)
{
	graphics::ClearDBToColor(0, 0, 800, 32);
	//GGG DOUBLEBUFFER->ClearToColor(0, 0, 800, 16);

	ulong Length = BufferLength, Pointer = 0;

	char Buffer[99];

	Buffer[98] = 0;

	if(MessageBuffer)
	while(Length)
	{
		for(ushort y = 0; y < 2; y++)
		{
			if(Length <= 98)
			{
				for(ulong c = 0; c < Length; c++)
					Buffer[c] = MessageBuffer[c + Pointer];

				Buffer[Length] = 0;

				FONTW->PrintfToDB(7, 7 + y * 10, "%s", Buffer);

				Length = 0;

				break;
			}
			else
			{
				ulong i = 97;

				for(; i; i--)
					if(MessageBuffer[Pointer + i] == ' ')
					{
						for(ulong c = 0; c < i; c++)
							Buffer[c] = MessageBuffer[c + Pointer];

						Buffer[i] = 0;

						Pointer += i + 1;

						Length -= i + 1;

						break;
					}

				if(!i)
				{
					for(ulong c = 0; c < 98; c++)
						Buffer[c] = MessageBuffer[c + Pointer];

					Pointer += 98;

					Length -= 98;
				}

				FONTW->PrintfToDB(7, 7 + y * 10, "%s", Buffer);
			}
		}
		if(Length)
		{
			graphics::BlitDBToScreen();

			//GGG DOUBLEBUFFER->ClearToColor(0, 0, 800, 16);

			globalwindowhandler::GetKey();

			graphics::ClearDBToColor(0, 0, 800, 32);
		}
	}
}

void game::globalmessagingsystem::Empty(void)
{
	delete [] MessageBuffer;

	MessageBuffer = 0;

	BufferLength = 0;
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
	Level[Current]->CLevelSquare(vector(CX, CY))->SetFlag();
	Level[Current]->CLevelSquare(vector(CX, CY))->UpdateItemMemory();

	if(CX == OX && CY == OY)
		return true;
	else
		return Level[Current]->CLevelSquare(vector(CX, CY))->COverTerrain()->CIsWalkable();
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

void game::panel::Draw(void)
{
	character* Player = game::CPlayer();

	FONTW->PrintfToDB(16, 524, "Name: %s", game::CPlayerName().c_str());

	FONTW->PrintfToDB(16, 534, "Strength: %d", Player->CStrength());
	FONTW->PrintfToDB(16, 544, "Endurance: %d", Player->CEndurance());
	FONTW->PrintfToDB(16, 554, "Agility: %d", Player->CAgility());
	FONTW->PrintfToDB(16, 564, "Perception: %d", Player->CPerception());
	FONTW->PrintfToDB(16, 574, "Size: %d", Player->CSize());
	(Player->CHP() < (Player->CEndurance() << 1) / 3 ? FONTR : FONTW)->PrintfToDB(16, 584, "HP: %d/%d", Player->CHP(), (Player->CEndurance() << 1));

	FONTW->PrintfToDB(160, 534, "Exp: %d", Player->CStrengthExperience());
	FONTW->PrintfToDB(160, 544, "Exp: %d", Player->CEnduranceExperience());
	FONTW->PrintfToDB(160, 554, "Exp: %d", Player->CAgilityExperience());
	FONTW->PrintfToDB(160, 564, "Exp: %d", Player->CPerceptionExperience());

	if(Player->CWielded())
		FONTW->PrintfToDB(160, 574, "Wielded: %s", Player->CWielded()->CNAME(INDEFINITE));
        if(Player->CTorsoArmor())
		FONTW->PrintfToDB(160, 584, "Worn: %s", Player->CTorsoArmor()->CNAME(INDEFINITE));

	FONTW->PrintfToDB(320, 534, "Speed: %d", Player->CWielded() ? ushort(sqrt((ulong(Player->CAgility() << 2) + Player->CStrength()) * 20000 / Player->CWielded()->CWeight())) : ulong(Player->CAgility() << 2) + Player->CStrength());
	FONTW->PrintfToDB(320, 544, "Armor Value: %d", Player->CalculateArmorModifier());
	FONTW->PrintfToDB(320, 554, "Weaponstrength: %.0f", Player->CWeaponStrength());
	FONTW->PrintfToDB(320, 564, "Min dam & Max dam: %d, %d", ushort(Player->CWeaponStrength() * Player->CStrength() / 26667), ushort(Player->CWeaponStrength() * Player->CStrength() / 16000 + 1));
	FONTW->PrintfToDB(600, 534, "You are %s", Player->CNAME(INDEFINITE));
	FONTW->PrintfToDB(600, 544, "Dungeon level: %d", game::CCurrent() + 1);
	FONTW->PrintfToDB(600, 554, "NP: %d", Player->CNP());
	FONTW->PrintfToDB(600, 564, "Turns: %d", game::CTurns());
	if(Player->CNP() < CRITICALHUNGERLEVEL) FONTR->PrintfToDB(600, 574, "Fainting");
	else if(Player->CNP() < HUNGERLEVEL) FONTB->PrintfToDB(600, 574, "Hungry");
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
	if(Player->CPos().X < 25)
		Camera.X = 0;
	else
		Camera.X = Player->CPos().X - 25;
}

void game::UpDateCameraY(void)
{
	if(Player->CPos().Y < 18)
		Camera.Y = 0;
	else
		Camera.Y = Player->CPos().Y - 18;
}

character* game::BalancedCreateMonster(void)
{
	ushort Types = 0;

	{
	for(ushort c = 1; CharacterPrototype[c]; c++)
		Types++;
	}

	for(ushort c = 0; ; c++)
	{
		ushort Chosen = 1 + rand() % Types;

		if(!CharacterPrototype[Chosen]->Possibility())
			continue;

		character* Monster = CharacterPrototype[Chosen]->Clone();

		if(c == 99 || (Monster->Danger() < Difficulty() * 1.5f && Monster->Danger() > Difficulty() * 0.5f))
			return Monster;
		else
			delete Monster;
	}
}

item* game::BalancedCreateItem(void)
{
	ushort SumOfPossibilities = 0, Counter = 0, RandomOne;

	{
	for(ushort c = 1; ItemPrototype[c]; c++)
	{
		SumOfPossibilities += ItemPrototype[c]->Possibility();
	}
	}
		
	RandomOne = 1+ rand() % (SumOfPossibilities);
	
	for(ushort c = 1; ItemPrototype[c]; c++)
	{
		Counter += ItemPrototype[c]->Possibility();
		if(Counter >= RandomOne)
		{
			return CreateItem(c);
		}
	}
	ABORT("Balanced Create Item kaatuuu");
	return 0;
}

character* game::CreateMonster(ushort Index)
{
	return CharacterPrototype[Index]->Clone();
}

item* game::CreateItem(ushort Index)
{
	return ItemPrototype[Index]->Clone();
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

const unsigned int game::CountChars(const char cSF,std::string sSH) // (MENU)
{
	unsigned int iReturnCounter = 0;
	for(unsigned int i = 0; i < sSH.length(); i++)
		if(sSH[i] == cSF)
			++iReturnCounter;
	return iReturnCounter;
}

const int game::Menu(std::string sMS) // (MENU)
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
		switch (k = globalwindowhandler::GetKey())
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


bool game::BoolQuestion(std::string String, char DefaultAnswer, int OtherKeyForTrue, int (*Key)(void))
{
	int ch;
	ADD_MESSAGE(String.c_str());
	DRAW_MESSAGES;
	graphics::BlitDBToScreen();
	//GGG DOUBLEBUFFER->Blit(SCREEN,0,0,0,0,800,16);
	for(;;)
	{
	ch = Key();
	if (ch == 0x00d || ch == 'y' || ch == 'Y' || ch == OtherKeyForTrue) { EMPTY_MESSAGES; graphics::BlitDBToScreen(); return true;  }
	if (ch == 'n' || ch == 'N') { EMPTY_MESSAGES; graphics::BlitDBToScreen(); return false; }
	if(DefaultAnswer == 2) continue;
	EMPTY_MESSAGES;
	graphics::BlitDBToScreen();
	//GGG DOUBLEBUFFER->Blit(SCREEN,0,0,0,0,800,16);
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

void game::DrawEverythingWithDebug(bool EmptyMsg)
{
	clock_t Tim = clock(), Temp[5];

	graphics::ClearDBToColor(0);
	Tim += Temp[0] = clock() - Tim;
	game::CCurrentLevel()->Draw();
	Tim += Temp[1] = clock() - Tim;
	game::Panel.Draw();
	Tim += Temp[2] = clock() - Tim;
	DRAW_MESSAGES;
	Tim += Temp[3] = clock() - Tim;
	if(EmptyMsg) EMPTY_MESSAGES;
	Temp[4] = clock() - Tim;
	//FONTW->PrintfToDB(16, 574, "Player: %lld, Monst: %lld, Clear: %lld.", HandlePlayTim, HandleCharTim, Temp[0]);
	//FONTW->PrintfToDB(16, 584, "Draw: %lld, Panel: %lld, DrawMess: %lld, EmptyMess: %lld.", Temp[1], Temp[2], Temp[3], Temp[4]);
	HandleCharTim = 0;
	HandlePlayTim = 0;
	graphics::BlitDBToScreen();
}

void game::DrawEverything(bool EmptyMsg)
{
	DrawEverythingNoBlit(EmptyMsg);
	graphics::BlitDBToScreen();
}

void game::DrawEverythingNoBlit(bool EmptyMsg)
{
	graphics::ClearDBToColor(0);
	game::CCurrentLevel()->Draw();
	game::Panel.Draw();
	DRAW_MESSAGES;
	if(EmptyMsg) EMPTY_MESSAGES;
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
	//FONTW->PrintfToDB(100, 550 , "Press any key to continue");

	if(GKey)
		globalwindowhandler::GetKey();
}

bool game::Save(std::string SaveName)
{
	std::ofstream* SaveFile = new std::ofstream((SaveName + ".sav").c_str(), std::ios::out | std::ios::binary);

	*SaveFile += PlayerName;

	if(!SaveFile->is_open())
		return false;

	SaveFile->write((char*)&Levels, sizeof(Levels));
	SaveFile->write((char*)&Current, sizeof(Current));
	SaveFile->write((char*)&Camera, sizeof(Camera));
	SaveFile->write((char*)&WizardMode, sizeof(WizardMode));
	SaveFile->write((char*)&SeeWholeMapCheat, sizeof(SeeWholeMapCheat));
	SaveFile->write((char*)&Gamma, sizeof(Gamma));
	SaveFile->write((char*)&GoThroughWallsCheat, sizeof(GoThroughWallsCheat));
	SaveFile->write((char*)&BaseScore, sizeof(BaseScore));
	SaveFile->write((char*)&Turns, sizeof(Turns));
	SaveFile->write((char*)&SoftGamma, sizeof(SoftGamma));

	time_t Time = time(0);
	srand(Time);
	SaveFile->write((char*)&Time, sizeof(Time));

	SaveLevel(SaveName, Current, false);

	{
	for(ushort c = 1; CGod(c); c++)
		CGod(c)->Save(SaveFile);
	}

	vector Pos = game::CPlayer()->CPos();

	SaveFile->write((char*)&Pos, sizeof(Pos));

	for(ushort c = 0; c < CLevels(); c++)
		*SaveFile += LevelMsg[c];

	SaveFile->close();

	return true;
}

bool game::Load(std::string SaveName)
{
	std::ifstream* SaveFile = new std::ifstream((SaveName + ".sav").c_str(), std::ios::in | std::ios::binary);

	if(!SaveFile->is_open())
		return false;

	*SaveFile -= PlayerName;

	SaveFile->read((char*)&Levels, sizeof(Levels));
	SaveFile->read((char*)&Current, sizeof(Current));
	SaveFile->read((char*)&Camera, sizeof(Camera));
	SaveFile->read((char*)&WizardMode, sizeof(WizardMode));
	SaveFile->read((char*)&SeeWholeMapCheat, sizeof(SeeWholeMapCheat));
	SaveFile->read((char*)&Gamma, sizeof(Gamma));
	SaveFile->read((char*)&GoThroughWallsCheat, sizeof(GoThroughWallsCheat));
	SaveFile->read((char*)&BaseScore, sizeof(BaseScore));
	SaveFile->read((char*)&Turns, sizeof(Turns));
	SaveFile->read((char*)&SoftGamma, sizeof(SoftGamma));

	time_t Time;
	SaveFile->read((char*)&Time, sizeof(Time));
	srand(Time);

	Level = new level*[Levels];

	{
	for(ushort c = 0; c < Levels; c++)
		Level[c] = 0;
	}

	LoadLevel(SaveName);

	{
	for(ushort c = 1; CGod(c); c++)
		CGod(c)->Load(SaveFile);
	}

	vector Pos;

	SaveFile->read((char*)&Pos, sizeof(Pos));

	SPlayer(Level[Current]->CLevelSquare(Pos)->CCharacter());

	for(ushort c = 0; c < CLevels(); c++)
		*SaveFile -= LevelMsg[c];

	SaveFile->close();

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

material* game::CreateRandomSolidMaterial(ulong Volume)
{
	switch ( rand() % NUMB_OF_SOLID_MATERIALS )
	{
	case 0:
		return new iron(Volume);
	break;
	case 1:
		return new valpurium(Volume);
	break;
	case 2:
		return new stone(Volume);
	break;
	case 3:
		return new wood(Volume);
	break;
	case 4:
		return new glass(Volume);
	break;
	case 5:
		return new parchment(Volume);
	break;
	case 6:
                return new cloth(Volume);
	break;
	case 7:
		return new bone(Volume);
	break;
	case 8:
		return new mithril(Volume);
	break;
	default:
		ABORT("Thisss Golemmm refusesss to have this materialll");
		return 0;
	break;
	}
}

material* game::CreateMaterial(ushort Index, ulong Volume)
{
	switch(Index)
	{
	case IIRON:
                return new iron(Volume);
	case IVALPURIUM:
		return new valpurium(Volume);
	case ISTONE:
		return new stone(Volume);
	case IBANANAFLESH:
		return new bananaflesh(Volume);
	case IGRAVEL:
		return new gravel(Volume);
	case IMORAINE:
		return new moraine(Volume);
	case ISCHOOLFOOD:
		return new schoolfood(Volume);
	case IAIR:
		return new air(Volume);
	case IWOOD:
		return new wood(Volume);
	case IFLESH:
		return new flesh(Volume);
	case IBONE:
		return new bone(Volume);
	case IDARKFROGFLESH:
		return new darkfrogflesh(Volume);
	case IELPURIFLESH:
		return new elpuriflesh(Volume);
	case IGLASS:
		return new glass(Volume);
	case IOMLEURINE:
		return new omleurine(Volume);
	case IBANANAPEAL:
		return new bananapeal(Volume);
	case IPARCHMENT:
		return new parchment(Volume);
	case ICLOTH:
		return new cloth(Volume);
	case IHUMANFLESH:
		return new humanflesh(Volume);
	case ISLIME:
		return new slime(Volume);
	case IBROWNSLIME:
		return new brownslime(Volume);
	case IGOBLINOIDFLESH:
		return new goblinoidflesh(Volume);
	case IPORK:
		return new pork(Volume);
	case IBEEF:
		return new beef(Volume);
	case IPEPSI:
		return new pepsi(Volume);
	case IENNERBEASTFLESH:
		return new ennerbeastflesh(Volume);
	case IWOLFFLESH:
		return new wolfflesh(Volume);
	case IDOGFLESH:
		return new dogflesh(Volume);
	case IMITHRIL:
		return new mithril(Volume);
	default:
		ABORT("Loosing lifesupport in create material...");
	break;
	}
	return 0;
}

groundterrain* game::LoadGroundTerrain(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	switch(Type)
	{
	case IPARQUET:
		return new parquet(SaveFile);
	case IFLOORY:
		return new floory(SaveFile);
	default:
		ABORT("The time-space-vacuum of this savefile has been corrupted. Aborting Universe.");
		return 0;
	}
}

overterrain* game::LoadOverTerrain(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	switch(Type)
	{
	case IEARTH:
		return new earth(SaveFile);
	case IWALL:
		return new wall(SaveFile);
	case IEMPTY:
		return new empty(SaveFile);
	case IDOOR:
		return new door(SaveFile);
	case ISTAIRSUP:
		return new stairsup(SaveFile);
	case ISTAIRSDOWN:
		return new stairsdown(SaveFile);
	case IALTAR:
		return new altar(SaveFile);
	default:
		ABORT("The time-space-vacuum of this savefile has been corrupted. Aborting Universe.");
		return 0;
	}
}

material* game::LoadMaterial(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	switch(Type)
	{
	case 0:
		return 0;
	case IIRON:
                return new iron(SaveFile);
	case IVALPURIUM:
		return new valpurium(SaveFile);
	case ISTONE:
		return new stone(SaveFile);
	case IBANANAFLESH:
		return new bananaflesh(SaveFile);
	case IGRAVEL:
		return new gravel(SaveFile);
	case IMORAINE:
		return new moraine(SaveFile);
	case ISCHOOLFOOD:
		return new schoolfood(SaveFile);
	case IAIR:
		return new air(SaveFile);
	case IWOOD:
		return new wood(SaveFile);
	case IFLESH:
		return new flesh(SaveFile);
	case IBONE:
		return new bone(SaveFile);
	case IDARKFROGFLESH:
		return new darkfrogflesh(SaveFile);
	case IELPURIFLESH:
		return new elpuriflesh(SaveFile);
	case IGLASS:
		return new glass(SaveFile);
	case IOMLEURINE:
		return new omleurine(SaveFile);
	case IBANANAPEAL:
		return new bananapeal(SaveFile);
	case IPARCHMENT:
		return new parchment(SaveFile);
	case ICLOTH:
		return new cloth(SaveFile);
	case IHUMANFLESH:
		return new humanflesh(SaveFile);
	case ISLIME:
		return new slime(SaveFile);
	case IBROWNSLIME:
		return new brownslime(SaveFile);
	case IGOBLINOIDFLESH:
		return new goblinoidflesh(SaveFile);
	case IPORK:
		return new pork(SaveFile);
	case IBEEF:
		return new beef(SaveFile);
	case IPEPSI:
		return new pepsi(SaveFile);
	case IENNERBEASTFLESH:
		return new ennerbeastflesh(SaveFile);
	case IWOLFFLESH:
		return new wolfflesh(SaveFile);
	case IDOGFLESH:
		return new dogflesh(SaveFile);
	case IMITHRIL:
		return new mithril(SaveFile);
	default:
		ABORT("Unidentified evil substance detected while loading. Most likely Windows-garbage is again infecting innocent savefiles; destroy and reinstall your OS and try again.");
		return 0;
	}
}

item* game::LoadItem(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	return Type ? ItemPrototype[Type]->Load(SaveFile) : 0;
}

character* game::LoadCharacter(std::ifstream* SaveFile)
{
	ushort Type;

	SaveFile->read((char*)&Type, sizeof(Type));

	if(Type)
		int esko = 2;

	return Type ? CharacterPrototype[Type]->Load(SaveFile) : 0;
}

bool game::EmitationHandler(ushort CX, ushort CY, ushort OX, ushort OY)
{
	ushort Emit = Level[Current]->CLevelSquare(vector(OX, OY))->CEmitation();

	ushort MaxSize = (game::CLuxTableSize()[Emit] >> 1);

	if(long(CX) - long(OX) > MaxSize || long(OX) - long(CX) > MaxSize || long(CY) - long(OY) > MaxSize || long(OY) - long(CY) > MaxSize)
		Emit = 0;
	else
		Emit = game::CLuxTable()[Emit][long(CX) - long(OX) + (game::CLuxTableSize()[Emit] >> 1)][long(CY) - long(OY) + (game::CLuxTableSize()[Emit] >> 1)];

	Level[Current]->CLevelSquare(vector(CX, CY))->AlterLuminance(vector(OX, OY), Emit);

	if(CX == OX && CY == OY)
		return true;
	else
		return Level[Current]->CLevelSquare(vector(CX, CY))->COverTerrain()->CIsWalkable();
}

bool game::NoxifyHandler(ushort CX, ushort CY, ushort OX, ushort OY)
{
	Level[Current]->CLevelSquare(vector(CX, CY))->NoxifyEmitter(vector(OX, OY));

	if(CX == OX && CY == OY)
		return true;
	else
		return Level[Current]->CLevelSquare(vector(CX, CY))->COverTerrain()->CIsWalkable();
}

int game::GetKey(void)
{
	clock_t Tim = clock();

	int Key = globalwindowhandler::GetKey();

	HandlePlayTim -= clock() - Tim;

	return Key;
}

int game::MonsGetKey(void)
{
	clock_t Tim = clock();

	int Key = globalwindowhandler::GetKey();

	HandleCharTim -= clock() - Tim;

	return Key;
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
			LastKey = globalwindowhandler::GetKey();
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
		if((A + game::CMoveVector()[c]) == B)
			return game::MoveCommandKey[c];
	}
	return 0xFF;
}

void game::ApplyDivineTick(ushort Turns)
{
	for(ushort c = 1; CGod(c); c++)
		CGod(c)->ApplyDivineTick(Turns);
}

void game::ApplyDivineAlignmentBonuses(god* CompareTarget, bool Good)
{
	for(ushort c = 1; CGod(c); c++)
		if(CGod(c) != CompareTarget)
			CGod(c)->AdjustRelation(CompareTarget, Good);
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
		if(Key == game::CMoveCommandKey()[c])
			return game::CMoveVector()[c];

	return vector(0,0);
}

vector game::AskForDirectionVector(std::string String)
{
	if(String != "")
	{
		FONTW->PrintfToDB(40, 35, "%s", String.c_str());
		graphics::BlitDBToScreen();
	}
	return GetDirectionVectorForKey(globalwindowhandler::GetKey());
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
			LastKey = globalwindowhandler::GetKey();
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
		return Level[Current]->CLevelSquare(vector(CX, CY))->COverTerrain()->CIsWalkable();
}

long game::GodScore(void)
{
	long Score = -1000;

	for(ushort c = 1; CGod(c); c++)
	{
		if(CGod(c)->CRelation() > Score)
			Score = CGod(c)->CRelation();
	}

	return Score;
}

float game::Difficulty(void)
{
	float Base = game::CPlayer()->CDifficulty() * (Current + 1);

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
	if(LevelMsg[CCurrent()].length())
		ADD_MESSAGE(LevelMsg[CCurrent()].c_str());

	LevelMsg[CCurrent()] = "";
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
		if(game::CGod(c) == 0)
		{
			SGodNumber(c - 1);
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
		globalwindowhandler::GetKey();
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

void game::globalmessagingsystem::DrawMessageHistory(void)
{
	MessageHistory.Draw(false);
}

void game::globalmessagingsystem::Format(void)
{
	MessageHistory.Empty();
}

uchar game::DirectionQuestion(std::string Topic, uchar DefaultAnswer, bool RequireAnswer)
{
	ADD_MESSAGE(Topic.c_str());

	DRAW_MESSAGES;

	EMPTY_MESSAGES;

	graphics::BlitDBToScreen();

	while(true)
	{
		int Key = game::GetKey();

		for(uchar c = 0; c < DIRECTION_COMMAND_KEYS; c++)
			if(Key == game::CMoveCommandKey()[c])
				return c;

		if(DefaultAnswer < 8) return DefaultAnswer;
		if(!RequireAnswer) return 0xFF;
	}

}

void game::SaveLevel(std::string SaveName, ushort Index, bool DeleteAfterwards)
{
	std::ofstream* SaveFile = new std::ofstream((SaveName + ".l" + Index).c_str(), std::ios::out | std::ios::binary);

	if(!SaveFile->is_open())
		ABORT("Level lost!");

	Level[Index]->Save(SaveFile);

	SaveFile->close();

	delete SaveFile;

	if(DeleteAfterwards)
	{
		delete Level[Index];
		Level[Index] = 0;
	}
}

void game::LoadLevel(std::string SaveName, ushort Index)
{
	std::ifstream* SaveFile = new std::ifstream((SaveName + ".l" + Index).c_str(), std::ios::in | std::ios::binary);

	if(!SaveFile->is_open())
		ABORT("Level gone!");

	Level[Index] = new level(SaveFile, Index);

	SaveFile->close();

	delete SaveFile;
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

item* game::CreateItem(std::string What)
{
	for(ushort x = 1; true; x++)
	{
		if(ItemPrototype[x])
		{
			item* Item = ItemPrototype[x]->Clone();
			if(Item->CanBeWished())
				if(Item->CNameSingular() == What)
				{
					if(item* Wished = Item->CreateWishedItem())
					{
						delete Item;
						return Wished;
					}
					else
						return Item;
				}
			delete Item;
		}
		else
		{
			//ADD_MESSAGE("There is no such item!");
			return 0;
		}
	}
	return 0;
}
