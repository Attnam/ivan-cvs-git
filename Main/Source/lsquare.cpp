#include "char.h"
#include "igraph.h"
#include "lsquare.h"
#include "stack.h"
#include "strover.h"
#include "lterrain.h"
#include "level.h"
#include "item.h"
#include "proto.h"
#include "message.h"

levelsquare::levelsquare(level* MotherLevel, vector Pos) : square(MotherLevel, Pos), Emitation(0), DivineOwner(0), Fluided(false)
{
	Stack = new stack(this);
	GetStack()->SetSquareUnder(this);

	for(ushort c = 0; c < 4; c++)	//Is there a better way to do this? Only Stroustrup knows...
	{
		SideStack[c] = new stack(this);
		SideStack[c]->SetSquareUnder(this);
	}
}

levelsquare::~levelsquare(void)
{
	delete GetGroundLevelTerrain();
	delete GetOverLevelTerrain();

	delete Stack;

	for(ushort c = 0; c < 4; c++)
		delete SideStack[c];
}

void levelsquare::HandleCharacters(void)
{
	if(CCharacter() && CCharacter() != game::GetPlayer())
		CCharacter()->Act();
}

void levelsquare::SignalEmitationIncrease(ushort EmitationUpdate)
{
	if(EmitationUpdate > Emitation)
		Emitate();
}

void levelsquare::SignalEmitationDecrease(ushort EmitationUpdate)
{
	if(EmitationUpdate == GetEmitation() && EmitationUpdate != CalculateEmitation())
		ReEmitate();
}

ushort levelsquare::CalculateEmitation(void) const
{
	ushort Emitation = GetStack()->GetEmitation();

	#define NE(D, S) game::GetCurrentLevel()->GetLevelSquare(Pos + D)->GetSideStack(S)->GetEmitation()

	if(GetPos().X)
		if(NE(vector(-1, 0), 1) > Emitation)
			Emitation = NE(vector(-1, 0), 1);

	if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
		if(NE(vector(1, 0), 3) > Emitation)
			Emitation = NE(vector(1, 0), 3);

	if(GetPos().Y)
		if(NE(vector(0, -1), 2) > Emitation)
			Emitation = NE(vector(0, -1), 2);

	if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
		if(NE(vector(0, 1), 0) > Emitation)
			Emitation = NE(vector(0, 1), 0);

	if(CCharacter() && CCharacter()->GetEmitation() > Emitation)
		Emitation = CCharacter()->GetEmitation();

	return Emitation;
}

void levelsquare::DrawToTileBuffer(void) const
{
	GetGroundLevelTerrain()->DrawToTileBuffer();

	if(Fluided)
		game::GetCurrentLevel()->GetFluidBuffer()->MaskedBlit(igraph::GetTileBuffer(), Pos.X << 4, Pos.Y << 4, 0, 0, 16,16, ushort(256 - TimeFromSpill));

	GetOverLevelTerrain()->DrawToTileBuffer();
	GetStack()->PositionedDrawToTileBuffer();

	#define NS(D, S) game::GetCurrentLevel()->GetLevelSquare(Pos + D)->GetSideStack(S)

	if(GetPos().X)
		NS(vector(-1, 0), 1)->PositionedDrawToTileBuffer(1);

	if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
		NS(vector(1, 0), 3)->PositionedDrawToTileBuffer(3);

	if(GetPos().Y)
		NS(vector(0, -1), 2)->PositionedDrawToTileBuffer(2);

	if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
		NS(vector(0, 1), 0)->PositionedDrawToTileBuffer(0);
}

void levelsquare::UpdateMemorizedAndDraw(void)
{
	SetKnown(true);

	DrawToTileBuffer();

	ushort RealLuminance = GetLuminance();

	igraph::GetTileBuffer()->MaskedBlit(game::GetCurrentLevel()->GetMemorized(), 0, 0, Pos.X << 4, Pos.Y << 4, 16, 16, RealLuminance);
	igraph::GetFOWGraphic()->MaskedBlit(game::GetCurrentLevel()->GetMemorized(), 0, 0, Pos.X << 4, Pos.Y << 4, 16, 16);

	if(CCharacter())
		CCharacter()->DrawToTileBuffer();

	if(game::GetSeeWholeMapCheat())
		RealLuminance = 256;

	igraph::BlitTileBuffer(vector((GetPos().X - game::CCamera().X) << 4, (GetPos().Y - game::CCamera().Y + 2) << 4), RealLuminance);
}

void levelsquare::Emitate(void)
{
	SetEmitation(CalculateEmitation());

	if(GetEmitation() < 64)
		return;
	else
		int esko = 2;

	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,
	             game::GetLuxTableSize()[Emitation] >> 1,
	             game::DoLine(GetPos().X, GetPos().Y, XPointer, Top,      game::EmitationHandler),
	             game::DoLine(GetPos().X, GetPos().Y, XPointer, Bottom,   game::EmitationHandler),
	             game::DoLine(GetPos().X, GetPos().Y, Left,     YPointer, game::EmitationHandler),
	             game::DoLine(GetPos().X, GetPos().Y, Rigth,    YPointer, game::EmitationHandler));
}

void levelsquare::ReEmitate(void)
{
	ushort OldEmitation = GetEmitation();
	SetEmitation(CalculateEmitation());

	if(OldEmitation < 64)
		return;

	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,
	             game::GetLuxTableSize()[OldEmitation] >> 1,
	             game::DoLine(GetPos().X, GetPos().Y, XPointer, Top,      game::EmitationHandler),
	             game::DoLine(GetPos().X, GetPos().Y, XPointer, Bottom,   game::EmitationHandler),
	             game::DoLine(GetPos().X, GetPos().Y, Left,     YPointer, game::EmitationHandler),
	             game::DoLine(GetPos().X, GetPos().Y, Rigth,    YPointer, game::EmitationHandler));
}

void levelsquare::Noxify(void)
{
	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,
	             game::GetLuxTableSize()[Emitation] >> 1,
	             game::DoLine(GetPos().X, GetPos().Y, XPointer, Top,      game::NoxifyHandler),
	             game::DoLine(GetPos().X, GetPos().Y, XPointer, Bottom,   game::NoxifyHandler),
	             game::DoLine(GetPos().X, GetPos().Y, Left,     YPointer, game::NoxifyHandler),
	             game::DoLine(GetPos().X, GetPos().Y, Rigth,    YPointer, game::NoxifyHandler));
}

void levelsquare::ForceEmitterNoxify(void)
{
	for(ushort c = 0; c < Emitter.Length(); c++)
		game::GetCurrentLevel()->GetLevelSquare(Emitter.Access(c).Pos)->Noxify();
}

void levelsquare::ForceEmitterEmitation(void)
{
	for(ushort c = 0; c < Emitter.Length(); c++)
		game::GetCurrentLevel()->GetLevelSquare(Emitter.Access(c).Pos)->Emitate();
}

void levelsquare::NoxifyEmitter(vector Dir)
{
	emitter DirEmitter(Dir, 0);

	ushort Index = Emitter.Search(DirEmitter);

	Emitter.Access(Index) = DirEmitter;
}

uchar levelsquare::CalculateBitMask(vector Dir) const
{
	uchar BitMask = 0;

	#define IW(X, Y) game::GetCurrentLevel()->GetLevelSquare(Pos + vector(X, Y))->GetOverLevelTerrain()->GetIsWalkable()

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
	return GetOverLevelTerrain()->Open(Opener);
}

bool levelsquare::Close(character* Closer)
{
	if(!GetStack()->GetItems() && !Character)
		return GetOverLevelTerrain()->Close(Closer);
	else
	{
		ADD_MESSAGE("There's something in the way...");

		return false;
	}
}

void levelsquare::Save(std::ofstream* SaveFile) const
{
	square::Save(SaveFile);

	GetGroundLevelTerrain()->Save(SaveFile);
	GetOverLevelTerrain()->Save(SaveFile);

	GetStack()->Save(SaveFile);

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
		GetMotherLevel()->GetFluidBuffer()->Save(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);

	SaveFile->write((char*)&Emitation, sizeof(Emitation));
	SaveFile->write((char*)&DivineOwner, sizeof(DivineOwner));
	*SaveFile += Engraved;
	*SaveFile += RememberedItems;
}

void levelsquare::Load(std::ifstream* SaveFile)
{
	square::Load(SaveFile);

	SetGroundLevelTerrain(prototypesystem::LoadGroundLevelTerrain(SaveFile));
	SetOverLevelTerrain(prototypesystem::LoadOverLevelTerrain(SaveFile));

	Stack->Load(SaveFile);

	{
	for(ushort c = 0; c < 4; c++)
		SideStack[c]->Load(SaveFile);
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
		GetMotherLevel()->GetFluidBuffer()->Load(SaveFile, Pos.X << 4, Pos.Y << 4, 16, 16);

	SaveFile->read((char*)&Emitation, sizeof(Emitation));
	SaveFile->read((char*)&DivineOwner, sizeof(DivineOwner));
	*SaveFile -= Engraved;
	*SaveFile -= RememberedItems;
}

void levelsquare::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation) // ho ho ho /me is very funny. - Anonymous
{
	if(!Fluided)
	{
		Fluided = true;
		TimeFromSpill = 0;
		GetMotherLevel()->GetFluidBuffer()->ClearToColor(Pos.X << 4, Pos.Y << 4, 16, 16, 0xF81F);
	}

	for(ushort c = 0; c < Amount; c++)
	{
		vector Cords((Pos.X << 4) + 1 + rand() % 14, (Pos.Y << 4) + 1 + rand() % 14);
		game::GetCurrentLevel()->GetFluidBuffer()->PutPixel(Cords.X, Cords.Y, Color);
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

				game::GetCurrentLevel()->GetFluidBuffer()->PutPixel(Cords.X + game::CMoveVector()[d].X, Cords.Y + game::CMoveVector()[d].Y,
				MAKE_RGB(GET_RED(Color) + Change[0],
				GET_GREEN(Color) + Change[1],
				GET_BLUE(Color) + Change[2]) );
			}
		}

	}
}

ushort levelsquare::GetLuminance(void) const
{
	ushort Luminance = 0;

	if(GetOverLevelTerrain()->GetIsWalkable())
	{
		for(ushort c = 0; c < Emitter.Length(); c++)
			if(Emitter.Access(c).DilatedEmitation > Luminance)
				Luminance = Emitter.Access(c).DilatedEmitation;
	}
	else
		for(ushort c = 0; c < Emitter.Length(); c++)
			if(CalculateBitMask(Emitter.Access(c).Pos) & CalculateBitMask(game::GetPlayer()->GetPos()))
				if(Emitter.Access(c).DilatedEmitation > Luminance)
					Luminance = Emitter.Access(c).DilatedEmitation;

	return Luminance < 511 ? Luminance : 511;
}

void levelsquare::AddCharacter(character* Guy)
{
	Character = Guy;
	Guy->SetSquareUnder(this);
	SignalEmitationIncrease(Guy->GetEmitation());
}	//lisätkää aborttiii!!!

void levelsquare::FastAddCharacter(character* Guy)
{
	SetCharacter(Guy);
	Guy->SetSquareUnder(this);
}	//lisätkää aborttiii!!!

void levelsquare::Clean(void)
{
	GetStack()->Clean();

	for(uchar c = 0; c < 4; c++)
		GetSideStack(c)->Clean();
}

void levelsquare::RemoveCharacter(void)
{
	if(Character)
	{
		character* Backup = Character;

		SetCharacter(0);

		SignalEmitationDecrease(Backup->GetEmitation());
	}
}

void levelsquare::UpdateItemMemory(void)
{
	SetRememberedItems("");

	bool Anything = false;

	if(GetStack()->GetItems())
		Anything = true;

	if(GetStack()->GetItems() > 1)
		SetRememberedItems("many items");
	else
		if(GetStack()->GetItems() == 1)
		{
			SetRememberedItems(std::string(GetStack()->GetItem(0)->Name(INDEFINITE)));
		}

	for(uchar c = 0; c < 4; c++)
	{
		if(GetSideStack(c)->GetItems() == 1)
		{
			if(Anything)
				SetRememberedItems(std::string(GetRememberedItems() + " and " + GetSideStack(c)->GetItem(0)->Name(INDEFINITE)));
			else
				SetRememberedItems(std::string(GetSideStack(c)->GetItem(0)->Name(INDEFINITE)));
		}
		else
		if(GetSideStack(c)->GetItems() > 1)
		{
			if(Anything)
				SetRememberedItems(GetRememberedItems());
			else
				SetRememberedItems("some items");
		}
	}
}

bool levelsquare::CanBeSeen(void) const
{
	float xDist = (float(GetPos().X) - game::GetPlayer()->GetPos().X), yDist = (float(GetPos().Y) - game::GetPlayer()->GetPos().Y);

	if(RetrieveFlag() && xDist * xDist + yDist * yDist <= game::GetPlayer()->LOSRangeLevelSquare())
		return true;
	else
		return false;
}

void levelsquare::Kick(ushort Strength, uchar KickWay)
{
	GetStack()->Kick(Strength, RetrieveFlag(), KickWay);
}

bool levelsquare::CanBeSeenFrom(vector FromPos) const
{
	return game::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, game::EyeHandler);
}

void levelsquare::SetRememberedItems(std::string What)
{
	RememberedItems = What; 
}

bool levelsquare::Dig(character* DiggerCharacter, item* DiggerItem) // early prototype. Probably should include more checking with levelterrains etc
{
	char Result = CanBeDigged(DiggerCharacter, DiggerItem);
	if(Result != 2)
		if(DiggerCharacter->GetIsPlayer())
			ADD_MESSAGE(GetOverLevelTerrain()->DigMessage().c_str());
	else 
		Result = false;

	if(Result == 1)
	{
		delete GetOverLevelTerrain();
		SetOverLevelTerrain(new empty);
		ForceEmitterEmitation();
		game::GetCurrentLevel()->UpdateLOS();
	}
	for(uchar c = 0; c < 4; c++)
	{
		for(uchar x = 0; x < GetSideStack(c)->GetItems(); x++)
			GetSideStack(c)->MoveItem(x, GetStack());
	}

	return Result ? true : false;
}

char levelsquare::CanBeDigged(character* DiggerCharacter, item* DiggerItem) const
{
	if(GetPos().X == 0 || GetPos().Y == 0 || GetPos().X == game::GetCurrentLevel()->GetXSize() - 1 || GetPos().Y == game::GetCurrentLevel()->GetYSize() - 1)
	{
		ADD_MESSAGE("Somehow you feel that by digging this square you would collapse the whole dungeon.");
		return 2;
	}
	return GetOverLevelTerrain()->CanBeDigged();
}

void levelsquare::HandleFluids(void)
{
	if(TimeFromSpill > 255)
		Fluided = false;
	else
		TimeFromSpill++;
	if(Fluided)
		UpdateMemorizedAndDraw();
}

void levelsquare::ChangeLevelTerrain(groundlevelterrain* NewGround, overlevelterrain* NewOver)
{
	delete GetGroundLevelTerrain();
	SetGroundLevelTerrain(NewGround);
	delete GetOverLevelTerrain();
	SetOverLevelTerrain(NewOver);
}

void levelsquare::SetGroundLevelTerrain(groundlevelterrain* What)
{
	GroundTerrain = What;

	if(What)
		What->SetLevelSquareUnder(this);
}

void levelsquare::SetOverLevelTerrain(overlevelterrain* What)
{
	OverTerrain = What;

	if(What)
		What->SetLevelSquareUnder(this);
}

groundlevelterrain* levelsquare::GetGroundLevelTerrain(void) const
{
	return (groundlevelterrain*)GroundTerrain;
}

overlevelterrain* levelsquare::GetOverLevelTerrain(void) const
{
	return (overlevelterrain*)OverTerrain;
}
