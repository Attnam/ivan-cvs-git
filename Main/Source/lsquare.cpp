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
#include "team.h"

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

bool levelsquare::DrawTerrain() const
{
	GetGroundLevelTerrain()->DrawToTileBuffer();

	if(Fluided)
		GetFluidBuffer()->AlphaBlit(igraph::GetTileBuffer(), 0, 0);
	
	GetOverLevelTerrain()->DrawToTileBuffer();

	return true;
}

bool levelsquare::DrawStacks() const
{
	bool Items = false;

	if(GetStack()->PositionedDrawToTileBuffer())
		Items = true;

	#define NS(D, S) game::GetCurrentLevel()->GetLevelSquare(Pos + D)->GetSideStack(S)

	if(GetPos().X)
		if(NS(vector2d(-1, 0), 1)->PositionedDrawToTileBuffer(1))
			Items = true;

	if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
		if(NS(vector2d(1, 0), 3)->PositionedDrawToTileBuffer(3))
			Items = true;

	if(GetPos().Y)
		if(NS(vector2d(0, -1), 2)->PositionedDrawToTileBuffer(2))
			Items = true;

	if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
		if(NS(vector2d(0, 1), 0)->PositionedDrawToTileBuffer(0))
			Items = true;

	return Items;
}

bool levelsquare::DrawCharacters() const
{
	if(GetCharacter())
	{
		GetCharacter()->DrawToTileBuffer();
		return true;
	}
	else
		return false;
}

void levelsquare::UpdateMemorizedAndDraw()
{
	if(NewDrawRequested || Fluided)
	{
		vector2d BitPos = vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4);

		ushort Luminance = GetLuminance();

		if(Luminance >= LIGHT_BORDER)
		{
			ushort GammaLuminance = ushort(256 * game::GetSoftGamma());
			ushort RealLuminance = game::GetSeeWholeMapCheat() ? GammaLuminance : ushort(Luminance * game::GetSoftGamma());

			if(!game::GetOutlineItems())
			{
				DrawTerrain();
				DrawStacks();

				igraph::GetTileBuffer()->Blit(GetMemorized(), 0, 0, 0, 0, 16, 16, Luminance);

				if(GetStack()->GetItems() > 1)
					igraph::GetSymbolGraphic()->MaskedBlit(GetMemorized(), 0, 16, 0, 0, 16, 16);

				igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);

				if(!game::GetOutlineCharacters())
					if(GetStack()->GetItems() <= 1)
					{
						DrawCharacters();
						igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
					}
					else
					{
						igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
						igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);

						if(GetCharacter())
						{
							igraph::GetTileBuffer()->ClearToColor(0xF81F);
							DrawCharacters();
							igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
						}
					}
				else
				{
					igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);

					if(GetStack()->GetItems() > 1)
						igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);

					if(GetCharacter())
					{
						igraph::GetTileBuffer()->ClearToColor(0xF81F);

						DrawCharacters();
						igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);

						igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), CHARACTER_OUTLINE_COLOR);
						igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
					}
				}
			}
			else
			{
				DrawTerrain();

				igraph::GetTileBuffer()->Blit(GetMemorized(), 0, 0, 0, 0, 16, 16, Luminance);
				igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
				igraph::GetTileBuffer()->ClearToColor(0xF81F);

				if(DrawStacks())
				{
					igraph::GetTileBuffer()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16, Luminance);

					if(GetStack()->GetItems() > 1)
						igraph::GetSymbolGraphic()->MaskedBlit(GetMemorized(), 0, 16, 0, 0, 16, 16);

					igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);

					igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), ITEM_OUTLINE_COLOR);

					if(GetStack()->GetItems() > 1)
						igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetOutlineBuffer(), 0, 16, 0, 0, 16, 16);

					igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);

					if(GetCharacter())
						igraph::GetTileBuffer()->ClearToColor(0xF81F);
				}

				igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);

				if(!game::GetOutlineCharacters())
				{
					if(DrawCharacters())
						igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
				}
				else
					if(DrawCharacters())
					{
						igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
						igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), CHARACTER_OUTLINE_COLOR);
						igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
					}
			}
		}
		else
		{
			Memorized->ClearToColor(0);

			if(game::GetSeeWholeMapCheat())
				DrawCheat();
			else
				Memorized->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16);
			
			igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);
		}

		NewDrawRequested = false;
	}
}

void levelsquare::Emitate()
{
	SetEmitation(CalculateEmitation());

	if(GetEmitation() < LIGHT_BORDER)
		return;

	ushort Radius = game::GetLuxTableSize()[Emitation] >> 1;
	ulong RadiusSquare = Radius * Radius;

	DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
			    game::DoLine(GetPos().X, GetPos().Y, XPointer,	YPointer,      RadiusSquare,	game::EmitationHandler);)
}

void levelsquare::ReEmitate()
{
	ushort OldEmitation = GetEmitation();
	SetEmitation(CalculateEmitation());

	if(OldEmitation < LIGHT_BORDER)
		return;

	ushort Radius = game::GetLuxTableSize()[OldEmitation] >> 1;
	ulong RadiusSquare = Radius * Radius;

	DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
			    game::DoLine(GetPos().X, GetPos().Y, XPointer,	YPointer,      RadiusSquare,	game::EmitationHandler);)
}

void levelsquare::Noxify()
{
	ushort Radius = game::GetLuxTableSize()[Emitation] >> 1;
	ulong RadiusSquare = Radius * Radius;

	DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
			    game::DoLine(GetPos().X, GetPos().Y, XPointer,	YPointer,      RadiusSquare,	game::NoxifyHandler);)
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

	NewDrawRequested = true;
	DescriptionChanged = true;
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
		if(AiL >= LIGHT_BORDER)
		{
			if(GetRawLuminance() < LIGHT_BORDER)
				DescriptionChanged = true;

			Emitter << DirEmitter;
		}
	}
	else
		if(AiL)
		{
			if(Emitter.Access(Index).DilatedEmitation == AiL)
				return;

			if(GetRawLuminance() < LIGHT_BORDER)
			{
				Emitter.Access(Index) = DirEmitter;

				if(AiL >= LIGHT_BORDER)
					DescriptionChanged = true;
			}
			else
			{
				Emitter.Access(Index) = DirEmitter;

				if(AiL < LIGHT_BORDER && GetRawLuminance() < LIGHT_BORDER)
					DescriptionChanged = true;
			}
		}
		else
		{
			if(GetRawLuminance() >= LIGHT_BORDER)
			{
				Emitter.Remove(Index);

				if(GetRawLuminance() < LIGHT_BORDER)
					DescriptionChanged = true;
			}
			else
				Emitter.Remove(Index);
		}

	NewDrawRequested = true;

	if(GetLastSeen() == game::GetLOSTurns())
		game::SendLOSUpdateRequest();
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
	GetStack()->Save(SaveFile); // This must be before square::Save!

	square::Save(SaveFile);

	SaveFile << GroundLevelTerrain << OverLevelTerrain;

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
	game::SetSquareInLoad(this);

	GetStack()->Load(SaveFile); // This must be before square::Load!

	square::Load(SaveFile);

	SaveFile >> GroundLevelTerrain >> OverLevelTerrain;

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
		FluidBuffer->ClearToColor(0xF81F);
		Fluided = true;
		GetFluidBuffer()->CreateAlphaMap(0);
	}

	for(ushort c = 0; c < Amount; ++c)
	{
		vector2d Cords(1 + rand() % 14, 1 + rand() % 14);
		GetFluidBuffer()->PutPixel(Cords.X, Cords.Y, Color);
		GetFluidBuffer()->SetAlpha(Cords.X, Cords.Y, 200 + rand() % 56);

		for(ushort d = 0; d < 8; ++d)
			if(rand() % Lumpiness)
			{
				char Change[3];

				for(uchar x = 0; x < 3; ++x)
					Change[x] = rand() % Variation - rand() % Variation;

				if(short(GET_RED(Color) + Change[0]) < 0) Change[0] = -GET_RED(Color);
				if(short(GET_GREEN(Color) + Change[1]) < 0) Change[1] = -GET_GREEN(Color);
				if(short(GET_BLUE(Color) + Change[2]) < 0) Change[2] = -GET_BLUE(Color);

				if(short(GET_RED(Color) + Change[0]) > 255) Change[0] = 255 - GET_RED(Color);
				if(short(GET_GREEN(Color) + Change[1]) > 255) Change[1] = 255 - GET_GREEN(Color);
				if(short(GET_BLUE(Color) + Change[2]) > 255) Change[2] = 255 - GET_BLUE(Color);

				GetFluidBuffer()->PutPixel(Cords.X + game::GetMoveVector(d).X, Cords.Y + game::GetMoveVector(d).Y,
				MAKE_RGB(GET_RED(Color) + Change[0], GET_GREEN(Color) + Change[1], GET_BLUE(Color) + Change[2]));

				GetFluidBuffer()->SetAlpha(Cords.X + game::GetMoveVector(d).X, Cords.Y + game::GetMoveVector(d).Y, 200 + rand() % 56);
			}
	}
}

ushort levelsquare::GetLuminance() const
{
	ushort Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

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

	return Luminance > 511 ? 511 : Luminance;
}

ushort levelsquare::GetRawLuminance() const
{
	ushort Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

	for(ushort c = 0; c < Emitter.Length(); ++c)
		if(Emitter.Access(c).DilatedEmitation > Luminance)
			Luminance = Emitter.Access(c).DilatedEmitation;

	return Luminance > 511 ? 511 : Luminance;
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

void levelsquare::UpdateMemorizedDescription(bool Cheat)
{
	if(DescriptionChanged || Cheat)
	{
		if(GetLuminance() >= LIGHT_BORDER || Cheat)
		{
			bool Anything = false;

			if(GetOverLevelTerrain()->Name(UNARTICLED) != "air atmosphere")
			{
				SetMemorizedDescription(GetOverLevelTerrain()->Name(INDEFINITE));
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

				SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGroundLevelTerrain()->Name(INDEFINITE));
			}
			else
				if(Anything)
					SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGroundLevelTerrain()->Name(INDEFINITE));
				else
					SetMemorizedDescription(GetGroundLevelTerrain()->Name(INDEFINITE));

			for(uchar c = 0; c < 4; ++c)
			{
				if(GetSideStack(c)->GetItems() == 1)
				{
					if(!Anything)
						SetMemorizedDescription(GetGroundLevelTerrain()->Name(INDEFINITE));

					SetMemorizedDescription(GetMemorizedDescription() + " and " + GetSideStack(c)->GetItem(0)->Name(INDEFINITE) + " on the wall");
				}
				else
				if(GetSideStack(c)->GetItems() > 1)
				{
					if(!Anything)
						SetMemorizedDescription(GetGroundLevelTerrain()->Name(INDEFINITE));

					SetMemorizedDescription(GetMemorizedDescription() + " and many items on the wall");
				}
			}

			if(!Anything)
				SetMemorizedDescription(GetGroundLevelTerrain()->Name(INDEFINITE));
		}
		else
			SetMemorizedDescription("darkness");

		DescriptionChanged = false;
	}
}

bool levelsquare::Kick(ushort Strength, uchar KickWay, character* Kicker)
{
	if(GetCharacter() && Kicker->GetTeam()->GetRelation(GetCharacter()->GetTeam()) != HOSTILE)
		if(Kicker->GetIsPlayer() && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
			return false;
		else
			Kicker->GetTeam()->Hostility(GetCharacter()->GetTeam());

	GetStack()->Kick(Strength, GetLastSeen() == game::GetLOSTurns(), KickWay);

	if(GetCharacter())
		GetCharacter()->BeKicked(Strength, GetLastSeen() == game::GetLOSTurns(), KickWay, Kicker);

	GetOverLevelTerrain()->Kick(Strength, GetLastSeen() == game::GetLOSTurns(), KickWay);
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
		ushort Emit = GetOverLevelTerrain()->GetEmitation();
		ChangeOverLevelTerrain(new empty);
		SignalEmitationDecrease(Emit);
		ForceEmitterEmitation();

		game::SendLOSUpdateRequest();
	}
	for(uchar c = 0; c < 4; ++c)
	{
		for(uchar x = 0; x < GetSideStack(c)->GetItems(); ++x)
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
	if(Fluided && !(rand() % 10) && !GetFluidBuffer()->FadeAlpha(1))
	{
		Fluided = false;
		delete FluidBuffer;
		FluidBuffer = 0;
	}
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
	DescriptionChanged = true;
}

void levelsquare::ChangeOverLevelTerrain(overlevelterrain* NewOver)
{
	delete GetOverLevelTerrain();
	SetOverLevelTerrain(NewOver);
	NewDrawRequested = true;
	DescriptionChanged = true;
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
	if(NewDrawRequested || Fluided)
	{
		vector2d BitPos = vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4);

		ushort GammaLuminance = ushort(256 * game::GetSoftGamma());

		if(!game::GetOutlineItems())
		{
			DrawTerrain();
			DrawStacks();

			if(GetStack()->GetItems() > 1)
				igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 16, 0, 0, 16, 16);

			if(!game::GetOutlineCharacters())
			{
				DrawCharacters();
				igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
			}
			else
			{
				igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);

				if(GetCharacter())
				{
					igraph::GetTileBuffer()->ClearToColor(0xF81F);
					DrawCharacters();
					igraph::GetTileBuffer()->Outline(CHARACTER_OUTLINE_COLOR);
					igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
				}
			}
		}
		else
		{
			DrawTerrain();

			igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
			igraph::GetTileBuffer()->ClearToColor(0xF81F);

			if(DrawStacks())
			{
				igraph::GetTileBuffer()->Outline(ITEM_OUTLINE_COLOR);

				if(GetStack()->GetItems() > 1)
					igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetTileBuffer(), 0, 16, 0, 0, 16, 16);

				igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
				igraph::GetTileBuffer()->ClearToColor(0xF81F);
			}

			if(!game::GetOutlineCharacters())
			{
				if(DrawCharacters())
					igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
			}
			else
				if(DrawCharacters())
				{
					igraph::GetTileBuffer()->Outline(CHARACTER_OUTLINE_COLOR);
					igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, GammaLuminance);
				}
		}

		NewDrawRequested = false;
	}
}

void levelsquare::ApplyScript(squarescript* SquareScript)
{
	Clean();

	if(SquareScript->GetCharacter(false))
	{
		character* Char = SquareScript->GetCharacter()->Instantiate();

		if(!Char->GetTeam())
			Char->SetTeam(game::GetTeam(*GetLevelUnder()->GetLevelScript()->GetTeamDefault()));

		FastAddCharacter(Char);
	}

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

bool levelsquare::CanBeSeen() const
{
	if(GetLuminance() >= LIGHT_BORDER && GetLastSeen() == game::GetLOSTurns())
		return true;
	else
		return false;
}

bool levelsquare::CanBeSeenIgnoreDarkness() const
{
	if(GetLastSeen() == game::GetLOSTurns())
		return true;
	else
		return false;
}

bool levelsquare::CanBeSeenFrom(vector2d FromPos, ulong MaxDistance) const
{
	if(GetLuminance() < LIGHT_BORDER)
		return false;
	else
		return game::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, MaxDistance, game::EyeHandler);
}

void levelsquare::MoveCharacter(levelsquare* To)
{
	if(Character)
	{
		if(To->Character)
			ABORT("Overgrowing of square population detected!");

		character* Movee = Character;
		ushort Emit = Movee->GetEmitation();
		SetCharacter(0);
		To->Character = Movee;
		Movee->SetSquareUnder(To);
		To->SignalEmitationIncrease(Emit);
		SignalEmitationDecrease(Emit);
		NewDrawRequested = true;
		To->NewDrawRequested = true;
	}
}

void levelsquare::StepOn(character* Stepper)
{
	GetGroundLevelTerrain()->StepOn(Stepper);
	GetOverLevelTerrain()->StepOn(Stepper);
	GetStack()->CheckForStepOnEffect(Stepper);
}
