#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

#include "independ.h"
#include "error.h"
#include "char.h"
#include "graphics.h"
#include "igraph.h"
#include "lsquare.h"
#include "stack.h"
#include "strover.h"
#include "terrain.h"
#include "whandler.h"
#include "independ.h"
#include "level.h"
#include "bitmap.h"
#include "item.h"

levelsquare::levelsquare(level* MotherLevel, vector Pos) : square(MotherLevel, Pos), MotherLevel(MotherLevel), Emitation(0), DivineOwner(0), /*FluidBuffer(new bitmap(16,16)), */Fluided(false)
{
	Stack = new stack(this);

	for(ushort c = 0; c < 4; c++)		//Is there a better way to do this? Only Stroustrup knows...
		SideStack[c] = new stack(this);
}

levelsquare::~levelsquare(void)
{
	delete Stack;

	for(ushort c = 0; c < 4; c++)
		delete SideStack[c];
}

void levelsquare::HandleCharacters(void)
{
	if(CCharacter() && CCharacter() != game::CPlayer())
		CCharacter()->Act();
}

void levelsquare::SignalEmitationIncrease(ushort EmitationUpdate)
{
	if(EmitationUpdate > Emitation)
		Emitate();
}

void levelsquare::SignalEmitationDecrease(ushort EmitationUpdate)
{
	if(EmitationUpdate == CEmitation() && EmitationUpdate != CalculateEmitation())
		ReEmitate();
}

ushort levelsquare::CalculateEmitation(void)
{
	ushort Emitation = CStack()->CEmitation();

	#define NE(D, S) game::CCurrentLevel()->CLevelSquare(Pos + D)->CSideStack(S)->CEmitation()

	if(CPos().X)
		if(NE(vector(-1, 0), 1) > Emitation)
			Emitation = NE(vector(-1, 0), 1);

	if(CPos().X < game::CCurrentLevel()->CXSize() - 1)
		if(NE(vector(1, 0), 3) > Emitation)
			Emitation = NE(vector(1, 0), 3);

	if(CPos().Y)
		if(NE(vector(0, -1), 2) > Emitation)
			Emitation = NE(vector(0, -1), 2);

	if(CPos().Y < game::CCurrentLevel()->CYSize() - 1)
		if(NE(vector(0, 1), 0) > Emitation)
			Emitation = NE(vector(0, 1), 0);

	if(CCharacter() && CCharacter()->CEmitation() > Emitation)
		Emitation = CCharacter()->CEmitation();

	return Emitation;
}

void levelsquare::DrawToTileBuffer(void)
{
	GroundTerrain->DrawToTileBuffer();

	if(Fluided)
		game::CCurrentLevel()->CFluidBuffer()->MaskedBlit(igraph::CTileBuffer(), Pos.X << 4, Pos.Y << 4, 0, 0, 16,16);

	OverTerrain->DrawToTileBuffer();
	CStack()->PositionedDrawToTileBuffer();

	#define NS(D, S) game::CCurrentLevel()->CLevelSquare(Pos + D)->CSideStack(S)

	if(CPos().X)
		NS(vector(-1, 0), 1)->PositionedDrawToTileBuffer(1);

	if(CPos().X < game::CCurrentLevel()->CXSize() - 1)
		NS(vector(1, 0), 3)->PositionedDrawToTileBuffer(3);

	if(CPos().Y)
		NS(vector(0, -1), 2)->PositionedDrawToTileBuffer(2);

	if(CPos().Y < game::CCurrentLevel()->CYSize() - 1)
		NS(vector(0, 1), 0)->PositionedDrawToTileBuffer(0);
}

void levelsquare::UpdateMemorizedAndDraw(void)
{
	SKnown(true);

	DrawToTileBuffer();

	ushort RealLuminance = CLuminance();

	igraph::CTileBuffer()->MaskedBlit(game::CCurrentLevel()->CMemorized(), 0, 0, Pos.X << 4, Pos.Y << 4, 16, 16, RealLuminance);
	igraph::CFOWGraphic()->MaskedBlit(game::CCurrentLevel()->CMemorized(), 0, 0, Pos.X << 4, Pos.Y << 4, 16, 16);

	if(CCharacter())
		CCharacter()->DrawToTileBuffer();

	if(game::CSeeWholeMapCheat())
		RealLuminance = 256;

	igraph::BlitTileBuffer(vector((CPos().X - game::CCamera().X) << 4, (CPos().Y - game::CCamera().Y + 2) << 4), RealLuminance);
}

void levelsquare::Emitate(void)
{
	SEmitation(CalculateEmitation());

	if(CEmitation() < 64)
		return;
	else
		int esko = 2;

	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::CCurrentLevel()->CXSize() - 1, game::CCurrentLevel()->CYSize() - 1,
	             game::CLuxTableSize()[Emitation] >> 1,
	             game::DoLine(CPos().X, CPos().Y, XPointer, Top,      game::EmitationHandler),
	             game::DoLine(CPos().X, CPos().Y, XPointer, Bottom,   game::EmitationHandler),
	             game::DoLine(CPos().X, CPos().Y, Left,     YPointer, game::EmitationHandler),
	             game::DoLine(CPos().X, CPos().Y, Rigth,    YPointer, game::EmitationHandler));
}

void levelsquare::ReEmitate(void)
{
	ushort OldEmitation = CEmitation();
	SEmitation(CalculateEmitation());

	if(OldEmitation < 64)
		return;

	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::CCurrentLevel()->CXSize() - 1, game::CCurrentLevel()->CYSize() - 1,
	             game::CLuxTableSize()[OldEmitation] >> 1,
	             game::DoLine(CPos().X, CPos().Y, XPointer, Top,      game::EmitationHandler),
	             game::DoLine(CPos().X, CPos().Y, XPointer, Bottom,   game::EmitationHandler),
	             game::DoLine(CPos().X, CPos().Y, Left,     YPointer, game::EmitationHandler),
	             game::DoLine(CPos().X, CPos().Y, Rigth,    YPointer, game::EmitationHandler));
}

void levelsquare::Noxify(void)
{
	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::CCurrentLevel()->CXSize() - 1, game::CCurrentLevel()->CYSize() - 1,
	             game::CLuxTableSize()[Emitation] >> 1,
	             game::DoLine(CPos().X, CPos().Y, XPointer, Top,      game::NoxifyHandler),
	             game::DoLine(CPos().X, CPos().Y, XPointer, Bottom,   game::NoxifyHandler),
	             game::DoLine(CPos().X, CPos().Y, Left,     YPointer, game::NoxifyHandler),
	             game::DoLine(CPos().X, CPos().Y, Rigth,    YPointer, game::NoxifyHandler));
}

void levelsquare::ForceEmitterNoxify(void)
{
	for(ushort c = 0; c < Emitter.Length(); c++)
		game::CCurrentLevel()->CLevelSquare(Emitter.Access(c).Pos)->Noxify();
}

void levelsquare::ForceEmitterEmitation(void)
{
	for(ushort c = 0; c < Emitter.Length(); c++)
		game::CCurrentLevel()->CLevelSquare(Emitter.Access(c).Pos)->Emitate();
}

void levelsquare::NoxifyEmitter(vector Dir)
{
	emitter DirEmitter(Dir, 0);

	ushort Index = Emitter.Search(DirEmitter);

	Emitter.Access(Index) = DirEmitter;
}

uchar levelsquare::CalculateBitMask(vector Dir)
{
	uchar BitMask = 0;

	#define IW(X, Y) game::CCurrentLevel()->CLevelSquare(Pos + vector(X, Y))->COverTerrain()->CIsWalkable()

	if(Dir.X < Pos.X)
	{
		if(Dir.Y < Pos.Y)
		{
			BitMask |= 1;
			if(IW(-1,  0)) BitMask |= 4;
			if(IW( 0, -1)) BitMask |= 2;
		}

		if(Dir.Y == Pos.Y)
			BitMask |= 5;

		if(Dir.Y > Pos.Y)
		{
			BitMask |= 4;
			if(IW(-1, 0)) BitMask |= 1;
			if(IW( 0, 1)) BitMask |= 8;
		}
	}

	if(Dir.X == Pos.X)
	{
		if(Dir.Y < Pos.Y)
			BitMask |= 3;

		if(Dir.Y == Pos.Y)
			BitMask |= 15;

		if(Dir.Y > Pos.Y)
			BitMask |= 12;
	}

	if(Dir.X > Pos.X)
	{
		if(Dir.Y < Pos.Y)
		{
			BitMask |= 2;
			if(IW(1,  0)) BitMask |= 8;
			if(IW(0, -1)) BitMask |= 1;
		}

		if(Dir.Y == Pos.Y)
			BitMask |= 10;

		if(Dir.Y > Pos.Y)
		{
			BitMask |= 8;
			if(IW(1, 0)) BitMask |= 2;
			if(IW(0, 1)) BitMask |= 4;
		}
	}

	return BitMask;
}

void levelsquare::AlterLuminance(vector Dir, ushort AiL)
{                                 // What does AL mean? Comments r0x0r

				  /* 
				   * Answer from the wise: Former AL was an acronym
				   * for "Altering Luminance". Current AiL, however,
				   * is completely meaningless (since AL is a reserved
				   * definition for "Alignment Lawful", it cannot be
				   * used here anymore).
				   */

	ushort Index;

	emitter DirEmitter(Dir, AiL);

	if((Index = Emitter.Search(DirEmitter)) == 0xFFFF)
	{
		if(AiL >= 64)
			Emitter << DirEmitter;
	}
	else
		if(AiL)
			Emitter.Access(Index) = DirEmitter;
		else
			Emitter.Remove(Index);
}

bool levelsquare::Open(character* Opener)
{
	return COverTerrain()->Open(Opener);
}

bool levelsquare::Close(character* Closer)
{
	if(!CStack()->CItems() && !Character)
		return COverTerrain()->Close(Closer);
	else
	{
		ADD_MESSAGE("There's something in the way...");

		return false;
	}
}

void levelsquare::Save(std::ofstream* SaveFile)
{
	square::Save(SaveFile);

	CStack()->Save(SaveFile);

	{
	for(ushort c = 0; c < 4; c++)
		SideStack[c]->Save(SaveFile);
	}

	ushort EmitterLength = Emitter.Length();

	SaveFile->write((char*)&EmitterLength, sizeof(EmitterLength));

	for(ushort c = 0; c < Emitter.Length(); c++)
	{
		SaveFile->write((char*)&Emitter.Access(c).Pos, sizeof(Emitter.Access(c).Pos));
		SaveFile->write((char*)&Emitter.Access(c).DilatedEmitation, sizeof(Emitter.Access(c).DilatedEmitation));
	}

	SaveFile->write((char*)&Fluided, sizeof(Fluided));

	if(Fluided)
		CMotherLevel()->CFluidBuffer()->Save(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);

	SaveFile->write((char*)&Emitation, sizeof(Emitation));

	SaveFile->write((char*)&Flag, sizeof(Flag));
	SaveFile->write((char*)&DivineOwner, sizeof(DivineOwner));
	*SaveFile += Engraved;
	*SaveFile += RememberedItems;
}

levelsquare::levelsquare(level* MotherLevel, std::ifstream* SaveFile, vector Pos) : square(MotherLevel, SaveFile, Pos), MotherLevel(MotherLevel)
{
	Stack = new stack(SaveFile);
	CStack()->SSquareUnder(this);

	{
	for(ushort c = 0; c < 4; c++)
		SideStack[c] = new stack(SaveFile);
	}

	ushort EmitterLength;

	SaveFile->read((char*)&EmitterLength, sizeof(EmitterLength));

	for(ushort c = 0; c < EmitterLength; c++)
	{
		vector EPos;
		ushort DilatedEmitation;
		SaveFile->read((char*)&EPos, sizeof(EPos));
		SaveFile->read((char*)&DilatedEmitation, sizeof(DilatedEmitation));
		emitter E(EPos, DilatedEmitation);
		Emitter.Add(E);
	}

	SaveFile->read((char*)&Fluided, sizeof(Fluided));

	if(Fluided)
		CMotherLevel()->CFluidBuffer()->Load(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);

	SaveFile->read((char*)&Emitation, sizeof(Emitation));
	SaveFile->read((char*)&Flag, sizeof(Flag));
	SaveFile->read((char*)&DivineOwner, sizeof(DivineOwner));
	*SaveFile -= Engraved;
	*SaveFile -= RememberedItems;
}

void levelsquare::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation) // ho ho ho /me is very funny. - Anonymous
{
	if(!Fluided)
	{
		Fluided = true;

		CMotherLevel()->CFluidBuffer()->ClearToColor(Pos.X << 4, Pos.Y << 4, 16, 16, 0xF81F);
	}

	for(ushort c = 0; c < Amount; c++)
	{
		vector Cords((Pos.X << 4) + 1 + rand() % 14, (Pos.Y << 4) + 1 + rand() % 14);
		game::CCurrentLevel()->CFluidBuffer()->PutPixel(Cords.X, Cords.Y, Color);
		for(ushort d = 0; d < 8; d++)
		{
			if((rand() % Lumpiness))
			{
				char Change[3];
				for(uchar x = 0; x < 3; x++)
				{
					Change[x] = rand() % Variation - rand() % Variation;
				}
				if(long(GET_RED(Color) + Change[0]) < 0) Change[0] = -GET_RED(Color);
				if(long(GET_GREEN(Color) + Change[1]) < 0) Change[1] = -GET_GREEN(Color);
				if(long(GET_BLUE(Color) + Change[2]) < 0) Change[2] = -GET_BLUE(Color);

				if(long(GET_RED(Color) + Change[0]) > 255) Change[0] = 255 - GET_RED(Color);
				if(long(GET_GREEN(Color) + Change[1]) > 255) Change[1] = 255 - GET_GREEN(Color);
				if(long(GET_BLUE(Color) + Change[2]) > 255) Change[2] = 255 - GET_BLUE(Color);

				game::CCurrentLevel()->CFluidBuffer()->PutPixel(Cords.X + game::CMoveVector()[d].X, Cords.Y + game::CMoveVector()[d].Y,
				RGB(GET_RED(Color) + Change[0],
				GET_GREEN(Color) + Change[1],
				GET_BLUE(Color) + Change[2]) );
			}
		}

	}
}

ushort levelsquare::CLuminance(void)
{
	ushort Luminance = 0;

	if(COverTerrain()->CIsWalkable())
	{
		for(ushort c = 0; c < Emitter.Length(); c++)
			if(Emitter.Access(c).DilatedEmitation > Luminance)
				Luminance = Emitter.Access(c).DilatedEmitation;
	}
	else
		for(ushort c = 0; c < Emitter.Length(); c++)
			if(CalculateBitMask(Emitter.Access(c).Pos) & CalculateBitMask(game::CPlayer()->CPos()))
				if(Emitter.Access(c).DilatedEmitation > Luminance)
					Luminance = Emitter.Access(c).DilatedEmitation;

	return Luminance < 511 ? Luminance : 511;
}

void levelsquare::AddCharacter(character* Guy)
{
	Character = Guy;
	Guy->SSquareUnder(this);
	SignalEmitationIncrease(Guy->CEmitation());
}	//lisätkää aborttiii!!!

void levelsquare::FastAddCharacter(character* Guy)
{
	SCharacter(Guy);
	Guy->SSquareUnder(this);
}	//lisätkää aborttiii!!!

void levelsquare::Clean(void)
{
	CStack()->Clean();

	for(uchar c = 0; c < 4; c++)
		CSideStack(c)->Clean();
}

void levelsquare::RemoveCharacter(void)
{
	if(Character)
	{
		character* Backup = Character;

		SCharacter(0);

		SignalEmitationDecrease(Backup->CEmitation());
	}
}

void levelsquare::UpdateItemMemory(void)
{
	SRememberedItems("");

	bool Anything = false;

	if(CStack()->CItems())
		Anything = true;

	if(CStack()->CItems() > 1)
		SRememberedItems("many items");
	else
		if(CStack()->CItems() == 1)
		{
			SRememberedItems(std::string(CStack()->CItem(0)->Name(INDEFINITE)));
		}

	for(uchar c = 0; c < 4; c++)
	{
		if(CSideStack(c)->CItems() == 1)
		{
			if(Anything)
				SRememberedItems(std::string(CRememberedItems() + " and " + CSideStack(c)->CItem(0)->Name(INDEFINITE)));
			else
				SRememberedItems(std::string(CSideStack(c)->CItem(0)->Name(INDEFINITE)));
		}
		else
		if(CSideStack(c)->CItems() > 1)
		{
			if(Anything)
				SRememberedItems(CRememberedItems());
			else
				SRememberedItems("some items");
		}
	}
}

bool levelsquare::CanBeSeen(void)
{
	float xDist = (float(CPos().X) - game::CPlayer()->CPos().X), yDist = (float(CPos().Y) - game::CPlayer()->CPos().Y);

	if(RetrieveFlag() && xDist * xDist + yDist * yDist <= game::CPlayer()->LOSRangeLevelSquare())
		return true;
	else
		return false;
}

void levelsquare::Kick(ushort Strength, uchar KickWay)
{
	CStack()->Kick(Strength, RetrieveFlag(), KickWay);
}

bool levelsquare::CanBeSeenFrom(vector FromPos)
{
	return game::DoLine(FromPos.X, FromPos.Y, CPos().X, CPos().Y, game::EyeHandler);
}

void levelsquare::SRememberedItems(std::string What)
{
	RememberedItems = What; 
}

bool levelsquare::Dig(character* DiggerCharacter, item* DiggerItem) // early prototype. Probably should include more checking with terrains etc
{
	char Result = CanBeDigged(DiggerCharacter, DiggerItem);
	if(Result != 2)
	{
		COverTerrain()->ShowDigMessage(DiggerCharacter, DiggerItem);
	}
	else 
		Result = false;

	if(Result == 1)
	{
		delete OverTerrain;
		SOverTerrain(new empty(NewMaterial(1, new air(3))));
		ForceEmitterEmitation();
		game::CCurrentLevel()->UpdateLOS();
	}
	for(uchar c = 0; c < 4; c++)
	{
		for(uchar x = 0; x < CSideStack(c)->CItems(); x++)
			CSideStack(c)->MoveItem(x, CStack());
	}

//	DiggerCharacter->SAP(DiggerCharacter->CAP() - 2000);
	return Result ? true : false;
}

char levelsquare::CanBeDigged(character* DiggerCharacter, item* DiggerItem)
{
	if(CPos().X == 0 || CPos().Y == 0 || CPos().X == game::CCurrentLevel()->CXSize() - 1 || CPos().Y == game::CCurrentLevel()->CYSize() - 1)
	{
		ADD_MESSAGE("Somehow you feel that by digging this square you would collapse the whole dungeon.");
		return 2;
	}
	return (COverTerrain()->CanBeDigged());
}
