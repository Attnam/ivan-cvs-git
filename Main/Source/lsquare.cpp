#include "charba.h"
#include "igraph.h"
#include "lsquare.h"
#include "stack.h"
#include "lterrade.h"
#include "level.h"
#include "itemba.h"
#include "proto.h"
#include "message.h"
#include "strover.h"
#include "save.h"
#include "graphics.h"
#include "script.h"

levelsquare::levelsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), OverLevelTerrain(0), GroundLevelTerrain(0), Emitation(0), DivineOwner(0), Fluided(false), FluidBuffer(0)
{
	Stack = new stack(this);

	for(ushort c = 0; c < 4; ++c)	//Is there a better way to do this? Only Stroustrup knows...
		SideStack[c] = new stack(this);
}

levelsquare::~levelsquare()
{
	delete GetGroundLevelTerrain();
	delete GetOverLevelTerrain();

	delete Stack;

	for(ushort c = 0; c < 4; ++c)
		delete SideStack[c];

	delete FluidBuffer;
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

ushort levelsquare::CalculateEmitation() const
{
	ushort Emitation = GetStack()->GetEmitation();

	#define NE(D, S) game::GetCurrentLevel()->GetLevelSquare(Pos + D)->GetSideStack(S)->GetEmitation()

	if(GetPos().X)
		if(NE(vector2d(-1, 0), 1) > Emitation)
			Emitation = NE(vector2d(-1, 0), 1);

	if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
		if(NE(vector2d(1, 0), 3) > Emitation)
			Emitation = NE(vector2d(1, 0), 3);

	if(GetPos().Y)
		if(NE(vector2d(0, -1), 2) > Emitation)
			Emitation = NE(vector2d(0, -1), 2);

	if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
		if(NE(vector2d(0, 1), 0) > Emitation)
			Emitation = NE(vector2d(0, 1), 0);

	if(GetCharacter() && GetCharacter()->GetEmitation() > Emitation)
		Emitation = GetCharacter()->GetEmitation();

	if(GetGroundLevelTerrain() && GetGroundLevelTerrain()->GetEmitation() > Emitation)
		Emitation = GetGroundLevelTerrain()->GetEmitation();

	if(GetOverLevelTerrain() && GetOverLevelTerrain()->GetEmitation() > Emitation)
		Emitation = GetOverLevelTerrain()->GetEmitation();

	return Emitation;
}

void levelsquare::DrawToTileBuffer() const
{
	GetGroundLevelTerrain()->DrawToTileBuffer();

	if(Fluided)
		GetFluidBuffer()->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);

	GetOverLevelTerrain()->DrawToTileBuffer();
	GetStack()->PositionedDrawToTileBuffer();

	#define NS(D, S) game::GetCurrentLevel()->GetLevelSquare(Pos + D)->GetSideStack(S)

	if(GetPos().X)
		NS(vector2d(-1, 0), 1)->PositionedDrawToTileBuffer(1);

	if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
		NS(vector2d(1, 0), 3)->PositionedDrawToTileBuffer(3);

	if(GetPos().Y)
		NS(vector2d(0, -1), 2)->PositionedDrawToTileBuffer(2);

	if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
		NS(vector2d(0, 1), 0)->PositionedDrawToTileBuffer(0);
}

void levelsquare::UpdateMemorizedAndDraw()
{
	if(NewDrawRequested)
	{
		if(!GetKnown())
		{
			Memorized = new bitmap(16, 16);

			SetKnown(true);
		}

		DrawToTileBuffer();

		ushort RealLuminance = GetLuminance();

		igraph::GetTileBuffer()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16, RealLuminance);
		igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);

		if(GetCharacter())
			GetCharacter()->DrawToTileBuffer();

		if(game::GetSeeWholeMapCheat())
			RealLuminance = 256;

		igraph::BlitTileBuffer(vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4), RealLuminance);

		NewDrawRequested = false;
	}
}

void levelsquare::Emitate()
{
	SetEmitation(CalculateEmitation());

	if(GetEmitation() < 64)
		return;

	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,
	             game::GetLuxTableSize()[Emitation] >> 1,
	             {game::DoLine(GetPos().X, GetPos().Y, XPointer, Top,      game::EmitationHandler);
	              game::DoLine(GetPos().X, GetPos().Y, XPointer, Bottom,   game::EmitationHandler);},
	             {game::DoLine(GetPos().X, GetPos().Y, Left,     YPointer, game::EmitationHandler);
	              game::DoLine(GetPos().X, GetPos().Y, Rigth,    YPointer, game::EmitationHandler);})
}

void levelsquare::ReEmitate()
{
	ushort OldEmitation = GetEmitation();
	SetEmitation(CalculateEmitation());

	if(OldEmitation < 64)
		return;

	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,
	             game::GetLuxTableSize()[OldEmitation] >> 1,
	             {game::DoLine(GetPos().X, GetPos().Y, XPointer, Top,      game::EmitationHandler);
	              game::DoLine(GetPos().X, GetPos().Y, XPointer, Bottom,   game::EmitationHandler);},
	             {game::DoLine(GetPos().X, GetPos().Y, Left,     YPointer, game::EmitationHandler);
	              game::DoLine(GetPos().X, GetPos().Y, Rigth,    YPointer, game::EmitationHandler);})
}

void levelsquare::Noxify()
{
	DO_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1,
	             game::GetLuxTableSize()[Emitation] >> 1,
	             {game::DoLine(GetPos().X, GetPos().Y, XPointer, Top,      game::NoxifyHandler);
	              game::DoLine(GetPos().X, GetPos().Y, XPointer, Bottom,   game::NoxifyHandler);},
	             {game::DoLine(GetPos().X, GetPos().Y, Left,     YPointer, game::NoxifyHandler);
	              game::DoLine(GetPos().X, GetPos().Y, Rigth,    YPointer, game::NoxifyHandler);})
}

void levelsquare::ForceEmitterNoxify()
{
	for(ushort c = 0; c < Emitter.Length(); ++c)
		game::GetCurrentLevel()->GetLevelSquare(Emitter.Access(c).Pos)->Noxify();
}

void levelsquare::ForceEmitterEmitation()
{
	for(ushort c = 0; c < Emitter.Length(); ++c)
		game::GetCurrentLevel()->GetLevelSquare(Emitter.Access(c).Pos)->Emitate();
}

void levelsquare::NoxifyEmitter(vector2d Dir)
{
	emitter DirEmitter(Dir, 0);

	ushort Index = Emitter.Search(DirEmitter);

	if(Index != 0xFFFF)
		Emitter.Access(Index) = DirEmitter;
}

uchar levelsquare::CalculateBitMask(vector2d Dir) const
{
	uchar BitMask = 0;

	#define IW(X, Y) game::GetCurrentLevel()->GetLevelSquare(Pos + vector2d(X, Y))->GetOverLevelTerrain()->GetIsWalkable()

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

void levelsquare::AlterLuminance(vector2d Dir, ushort AiL)
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

	NewDrawRequested = true;
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

void levelsquare::Save(outputfile& SaveFile) const
{
	square::Save(SaveFile);

	SaveFile << GroundLevelTerrain << OverLevelTerrain;

	GetStack()->Save(SaveFile);

	{
	for(ushort c = 0; c < 4; ++c)
		SideStack[c]->Save(SaveFile);
	}

	ushort EmitterLength = Emitter.Length();

	SaveFile << EmitterLength;

	for(ushort c = 0; c < Emitter.Length(); ++c)
		SaveFile << Emitter.Access(c).Pos << Emitter.Access(c).DilatedEmitation;

	SaveFile << Fluided;

	if(Fluided)
		GetFluidBuffer()->Save(SaveFile);

	SaveFile << Emitation << DivineOwner;
	SaveFile << Engraved;
}

void levelsquare::Load(inputfile& SaveFile)
{
	square::Load(SaveFile);

	SaveFile >> GroundLevelTerrain >> OverLevelTerrain;

	Stack->Load(SaveFile);

	{
	for(ushort c = 0; c < 4; ++c)
		SideStack[c]->Load(SaveFile);
	}

	ushort EmitterLength;

	SaveFile >> EmitterLength;

	for(ushort c = 0; c < EmitterLength; ++c)
	{
		vector2d EPos;
		ushort DilatedEmitation;
		SaveFile >> EPos >> DilatedEmitation;
		emitter E(EPos, DilatedEmitation);
		Emitter.Add(E);
	}

	SaveFile >> Fluided;

	if(Fluided)
	{
		FluidBuffer = new bitmap(16, 16);
		GetFluidBuffer()->Load(SaveFile);
	}

	SaveFile >> Emitation >> DivineOwner;
	SaveFile >> Engraved;
}

void levelsquare::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation) // ho ho ho /me is very funny. - Anonymous
{
	NewDrawRequested = true;

	if(!Fluided)
	{
		FluidBuffer = new bitmap(16, 16);
		GetFluidBuffer()->ClearToColor(0xF81F);
		Fluided = true;
		TimeFromSpill = 0;
	}

	for(ushort c = 0; c < Amount; ++c)
	{
		vector2d Cords(1 + rand() % 14, 1 + rand() % 14);
		GetFluidBuffer()->PutPixel(Cords.X, Cords.Y, Color);
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

				GetFluidBuffer()->PutPixel(Cords.X + game::GetMoveVector(d).X, Cords.Y + game::GetMoveVector(d).Y,
				MAKE_RGB(GET_RED(Color) + Change[0],
				GET_GREEN(Color) + Change[1],
				GET_BLUE(Color) + Change[2]) );
			}
		}

	}
}

ushort levelsquare::GetLuminance() const
{
	ushort Luminance = 0;

	if(GetOverLevelTerrain()->GetIsWalkable())
	{
		for(ushort c = 0; c < Emitter.Length(); ++c)
			if(Emitter.Access(c).DilatedEmitation > Luminance)
				Luminance = Emitter.Access(c).DilatedEmitation;
	}
	else
		for(ushort c = 0; c < Emitter.Length(); ++c)
			if(CalculateBitMask(Emitter.Access(c).Pos) & CalculateBitMask(game::GetPlayer()->GetPos()))
				if(Emitter.Access(c).DilatedEmitation > Luminance)
					Luminance = Emitter.Access(c).DilatedEmitation;

	return Luminance < 511 ? Luminance : 511;
}

void levelsquare::AddCharacter(character* Guy)
{
	if(Character)
		ABORT("Overgrowing of square population detected!");

	Character = Guy;
	Guy->SetSquareUnder(this);
	SignalEmitationIncrease(Guy->GetEmitation());
	NewDrawRequested = true;
}

void levelsquare::FastAddCharacter(character* Guy)
{
	if(Character)
		ABORT("Overgrowing of square population detected!");

	SetCharacter(Guy);
	Guy->SetSquareUnder(this);
}

void levelsquare::Clean()
{
	GetStack()->Clean();

	for(uchar c = 0; c < 4; ++c)
		GetSideStack(c)->Clean();
}

void levelsquare::RemoveCharacter()
{
	if(Character)
	{
		character* Backup = Character;
		SetCharacter(0);
		SignalEmitationDecrease(Backup->GetEmitation());
		NewDrawRequested = true;
	}
}

void levelsquare::UpdateMemorizedDescription()
{
	bool Anything = false;
	if(GetLuminance() > 63 || game::GetSeeWholeMapCheat())
	{
		if(GetOverTerrain()->Name(UNARTICLED) != "air atmosphere")
		{
			SetMemorizedDescription(GetOverTerrain()->Name(INDEFINITE));
			Anything = true;
		}

		if(GetStack()->GetItems())
		{
			if(Anything)
				if(GetStack()->GetItems() == 1)
					SetMemorizedDescription(GetMemorizedDescription() + " and " + std::string(GetStack()->GetItem(0)->Name(INDEFINITE)));
				else
					SetMemorizedDescription(GetMemorizedDescription() + " and " + "many items");
			else
				if(GetStack()->GetItems() == 1)
					SetMemorizedDescription(std::string(GetStack()->GetItem(0)->Name(INDEFINITE)));
				else
					SetMemorizedDescription("many items");

			Anything = true;

			SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGroundTerrain()->Name(INDEFINITE));
		}
		else
			if(Anything)
				SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGroundTerrain()->Name(INDEFINITE));
			else
				SetMemorizedDescription(GetGroundTerrain()->Name(INDEFINITE));

		for(uchar c = 0; c < 4; ++c)
		{
			if(GetSideStack(c)->GetItems() == 1)
			{
				if(!Anything)
					SetMemorizedDescription(GetGroundTerrain()->Name(INDEFINITE));

				SetMemorizedDescription(GetMemorizedDescription() + " and " + GetSideStack(c)->GetItem(0)->Name(INDEFINITE) + " on the wall");
			}
			else
			if(GetSideStack(c)->GetItems() > 1)
			{
				if(!Anything)
					SetMemorizedDescription(GetGroundTerrain()->Name(INDEFINITE));

				SetMemorizedDescription(GetMemorizedDescription() + " and many items on the wall");
			}
		}
	}
	else
	{
		SetMemorizedDescription("darkness");
		Anything = true;
	}
	if(!Anything)
		SetMemorizedDescription(GetGroundTerrain()->Name(INDEFINITE));
}

bool levelsquare::Kick(ushort Strength, uchar KickWay, character* Kicker)
{
	if(GetCharacter() && GetCharacter()->GetRelations() != 0)
	{
	if(Kicker->GetIsPlayer())
		if(!game::BoolQuestion("This might cause a hostile reaction. Are you sure? [Y/N]"))
			return false;
		else
			GetCharacter()->SetRelations(HOSTILE);
	}



	GetStack()->Kick(Strength, RetrieveFlag(), KickWay);
	if(GetCharacter())
		GetCharacter()->BeKicked(Strength, RetrieveFlag(), KickWay, Kicker);
	GetOverLevelTerrain()->Kick(Strength, RetrieveFlag(), KickWay);
	return true;
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
	for(uchar c = 0; c < 4; ++c)
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

void levelsquare::HandleFluids()
{
	if(TimeFromSpill > 255)
	{
		Fluided = false;
		delete FluidBuffer;
		FluidBuffer = 0;
	}
	else
		TimeFromSpill++;
}

void levelsquare::ChangeLevelTerrain(groundlevelterrain* NewGround, overlevelterrain* NewOver)
{
	ChangeGroundLevelTerrain(NewGround);
	ChangeOverLevelTerrain(NewOver);
}

void levelsquare::ChangeGroundLevelTerrain(groundlevelterrain* NewGround)
{
	delete GetGroundLevelTerrain();
	SetGroundLevelTerrain(NewGround);
	NewDrawRequested = true;
}

void levelsquare::ChangeOverLevelTerrain(overlevelterrain* NewOver)
{
	delete GetOverLevelTerrain();
	SetOverLevelTerrain(NewOver);
	NewDrawRequested = true;
}

void levelsquare::SetGroundLevelTerrain(groundlevelterrain* What)
{
	GroundLevelTerrain = What;

	if(What)
		What->SetSquareUnder(this);
}

void levelsquare::SetOverLevelTerrain(overlevelterrain* What)
{
	OverLevelTerrain = What;

	if(What)
		What->SetSquareUnder(this);
}

groundterrain* levelsquare::GetGroundTerrain() const
{
	return GroundLevelTerrain;
}

overterrain* levelsquare::GetOverTerrain() const
{
	return OverLevelTerrain;
}

void levelsquare::DrawCheat()
{
	if(NewDrawRequested)
	{
		DrawToTileBuffer();

		if(GetCharacter())
			GetCharacter()->DrawToTileBuffer();

		igraph::BlitTileBuffer(vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4));

		NewDrawRequested = false;
	}
}

void levelsquare::ApplyScript(squarescript* SquareScript)
{
	if(SquareScript->GetCharacter(false))
		FastAddCharacter(SquareScript->GetCharacter()->Instantiate());

	if(SquareScript->GetItem(false))
		GetStack()->FastAddItem(SquareScript->GetItem()->Instantiate());

	if(SquareScript->GetGroundTerrain(false))
		ChangeGroundLevelTerrain(SquareScript->GetGroundTerrain()->Instantiate());

	if(SquareScript->GetOverTerrain(false))
		ChangeOverLevelTerrain(SquareScript->GetOverTerrain()->Instantiate());

	if(SquareScript->GetIsUpStairs(false) && *SquareScript->GetIsUpStairs())
		GetLevelUnder()->SetUpStairs(Pos);

	if(SquareScript->GetIsDownStairs(false) && *SquareScript->GetIsDownStairs())
		GetLevelUnder()->SetDownStairs(Pos);

	if(SquareScript->GetIsWorldMapEntry(false) && *SquareScript->GetIsWorldMapEntry())
		GetLevelUnder()->SetWorldMapEntry(Pos);
}
