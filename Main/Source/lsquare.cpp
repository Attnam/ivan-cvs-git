#include <ctime>
#include <cmath>

#include "lsquare.h"
#include "stack.h"
#include "lterrade.h"
#include "level.h"
#include "itemba.h"
#include "message.h"
#include "strover.h"
#include "script.h"
#include "charba.h"
#include "team.h"
#include "config.h"
#include "femath.h"
#include "fluid.h"

lsquare::lsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), GLTerrain(0), OLTerrain(0), Emitation(0), DivineOwner(0), Room(0), TemporaryEmitation(0), Fluid(0)
{
  Stack = new stack(this);

  for(ushort c = 0; c < 4; ++c) //Is there a better way to do this? Only Stroustrup knows...
    SideStack[c] = new stack(this, c);
}

lsquare::~lsquare()
{
  delete GetGLTerrain();
  delete GetOLTerrain();

  delete Stack;

  for(ushort c = 0; c < 4; ++c)
    delete SideStack[c];

  delete Fluid;
}

void lsquare::SignalEmitationIncrease(ushort EmitationUpdate)
{
  if(EmitationUpdate > Emitation)
    Emitate();
}

void lsquare::SignalEmitationDecrease(ushort EmitationUpdate)
{
  if(EmitationUpdate == GetEmitation() && EmitationUpdate != CalculateEmitation())
    ReEmitate();
}

ushort lsquare::CalculateEmitation() const
{
  ushort Emitation = GetStack()->GetEmitation();

#define NE(D, S) GetLevelUnder()->GetLSquare(Pos + D)->GetSideStack(S)->GetEmitation()

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

  if(GetGLTerrain() && GetGLTerrain()->GetEmitation() > Emitation)
    Emitation = GetGLTerrain()->GetEmitation();

  if(GetOLTerrain() && GetOLTerrain()->GetEmitation() > Emitation)
    Emitation = GetOLTerrain()->GetEmitation();

  if(TemporaryEmitation > Emitation)
    Emitation = TemporaryEmitation;

  return Emitation;
}

bool lsquare::DrawTerrain() const
{
  GetGLTerrain()->DrawToTileBuffer();

  if(GetFluid())
    GetFluid()->DrawToTileBuffer();
	
  GetOLTerrain()->DrawToTileBuffer();

  return true;
}

bool lsquare::DrawStacks() const
{
  bool Items = false;

  if(GetOTerrain()->GetIsWalkable())
    {
      if(GetStack()->DrawToTileBuffer())
	Items = true;

#define NS(D, S) game::GetCurrentLevel()->GetLSquare(Pos + D)->GetSideStack(S)

      if(GetPos().X)
	if(NS(vector2d(-1, 0), 1)->DrawToTileBuffer())
	  Items = true;

      if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
	if(NS(vector2d(1, 0), 3)->DrawToTileBuffer())
	  Items = true;

      if(GetPos().Y)
	if(NS(vector2d(0, -1), 2)->DrawToTileBuffer())
	  Items = true;

      if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
	if(NS(vector2d(0, 1), 0)->DrawToTileBuffer())
	  Items = true;
    }

  return Items;
}

bool lsquare::DrawCharacters() const
{
  if(GetCharacter())
    {
      GetCharacter()->DrawToTileBuffer();
      return true;
    }
  else
    return false;
}

void lsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      ushort Luminance = GetLuminance();

      if(Luminance >= LIGHT_BORDER)
	{
	  DrawTerrain();
	  DrawStacks();

	  igraph::GetTileBuffer()->Blit(GetMemorized(), 0, 0, 0, 0, 16, 16);

	  if(GetStack()->GetItems() > 1 && GetOTerrain()->GetIsWalkable())
	    igraph::GetSymbolGraphic()->MaskedBlit(GetMemorized(), 0, 16, 0, 0, 16, 16);

	  igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);
	}
      else
	{
	  Memorized->Fill(0);
	  igraph::GetFOWGraphic()->MaskedBlit(GetMemorized(), 0, 0, 0, 0, 16, 16);
	}

      MemorizedUpdateRequested = false;
    }
}

void lsquare::Draw()
{
  if(NewDrawRequested)
    {
      vector2d BitPos = vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4);
      ushort Luminance = GetLuminance();

      if(Luminance >= LIGHT_BORDER || game::GetSeeWholeMapCheat())
	{
	  ushort ContrastLuminance = ushort(256.0f * configuration::GetContrast() / 100);
	  ushort RealLuminance = game::GetSeeWholeMapCheat() ? ContrastLuminance : ushort(float(Luminance) * configuration::GetContrast() / 100);

	  if(!configuration::GetOutlineItems())
	    {
	      DrawTerrain();
	      DrawStacks();

	      if(!configuration::GetOutlineCharacters())
		if(GetStack()->GetItems() <= 1)
		  {
		    DrawCharacters();
		    igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
		  }
		else
		  {
		    igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);

		    if(GetOTerrain()->GetIsWalkable())
		      igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);

		    if(GetCharacter())
		      {
			igraph::GetTileBuffer()->Fill(0xF81F);
			DrawCharacters();
			igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
		      }
		  }
	      else
		{
		  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);

		  if(GetStack()->GetItems() > 1 && GetOTerrain()->GetIsWalkable()) 
		    igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);

		  if(GetCharacter())
		    {
		      igraph::GetTileBuffer()->Fill(0xF81F);
		      DrawCharacters();
		      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
		      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
		      igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);
		    }
		}
	    }
	  else
	    {
	      DrawTerrain();

	      igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
	      igraph::GetTileBuffer()->Fill(0xF81F);

	      if(DrawStacks())
		{
		  igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
		  igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetItemOutlineColor());

		  if(GetStack()->GetItems() > 1 && GetOTerrain()->GetIsWalkable())
		    igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetOutlineBuffer(), 0, 16, 0, 0, 16, 16);

		  igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);

		  if(GetCharacter())
		    igraph::GetTileBuffer()->Fill(0xF81F);
		}

	      if(!configuration::GetOutlineCharacters())
		{
		  if(DrawCharacters())
		    igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
		}
	      else
		if(DrawCharacters())
		  {
		    igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, RealLuminance);
		    igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
		    igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ContrastLuminance);
		  }
	    }
	}
      else
	DOUBLEBUFFER->Fill(BitPos.X, BitPos.Y, 16, 16, 0);

      NewDrawRequested = false;
    }
}

void lsquare::Emitate()
{
  SetEmitation(CalculateEmitation());

  if(GetEmitation() < LIGHT_BORDER)
    return;

  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(Emitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(GetPos().X) - XPointer, long(GetPos().Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(GetPos().X, GetPos().Y, XPointer, YPointer, RadiusSquare, game::EmitationHandler);
  });
}

void lsquare::ReEmitate()
{
  ushort OldEmitation = GetEmitation();
  SetEmitation(CalculateEmitation());

  if(OldEmitation < LIGHT_BORDER)
    return;

  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(OldEmitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(GetPos().X) - XPointer, long(GetPos().Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(GetPos().X, GetPos().Y, XPointer, YPointer, RadiusSquare, game::EmitationHandler);
  });
}

void lsquare::Noxify()
{
  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(Emitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(GetPos().X) - XPointer, long(GetPos().Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(GetPos().X, GetPos().Y, XPointer, YPointer, RadiusSquare, game::NoxifyHandler);
  });
}

void lsquare::ForceEmitterNoxify()
{
  for(ushort c = 0; c < Emitter.Length(); ++c)
    game::GetCurrentLevel()->GetLSquare(Emitter.Access(c).Pos)->Noxify();
}

void lsquare::ForceEmitterEmitation()
{
  for(ushort c = 0; c < Emitter.Length(); ++c)
    game::GetCurrentLevel()->GetLSquare(Emitter.Access(c).Pos)->Emitate();
}

void lsquare::NoxifyEmitter(vector2d Dir)
{
  emitter DirEmitter(Dir, 0);

  ushort Index = Emitter.Search(DirEmitter);

  if(Index != 0xFFFF)
    {
      Emitter.Access(Index) = DirEmitter;

      NewDrawRequested = true;
      MemorizedUpdateRequested = true;
      DescriptionChanged = true;
    }
}

uchar lsquare::CalculateBitMask(vector2d Dir) const
{
  uchar BitMask = 0;

#define IW(X, Y) game::GetCurrentLevel()->GetLSquare(Pos + vector2d(X, Y))->GetOLTerrain()->GetIsWalkable()

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

void lsquare::AlterLuminance(vector2d Dir, ushort AiL)
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
	  if(!GetRawLuminance())
	    {
	      DescriptionChanged = true;
	      MemorizedUpdateRequested = true;
	    }

	  Emitter << DirEmitter;
	}
    }
  else
    if(AiL >= LIGHT_BORDER)
      {
	if(Emitter.Access(Index).DilatedEmitation == AiL)
	  return;

	if(!GetRawLuminance())
	  {
	    DescriptionChanged = true;
	    MemorizedUpdateRequested = true;
	  }

	Emitter.Access(Index) = DirEmitter;
      }
    else
      {
	ushort RawLum = GetRawLuminance();

	if(RawLum && RawLum == Emitter.Access(Index).DilatedEmitation)
	  {
	    Emitter.Remove(Index);

	    if(!GetRawLuminance())
	      {
		DescriptionChanged = true;
		MemorizedUpdateRequested = true;
	      }
	  }
	else
	  Emitter.Remove(Index);
      }

  NewDrawRequested = true;

  if(GetLastSeen() == game::GetLOSTurns())
    game::SendLOSUpdateRequest();
}

bool lsquare::Open(character* Opener)
{
  return GetOLTerrain()->Open(Opener);
}

bool lsquare::Close(character* Closer)
{
  if(!GetStack()->GetItems() && !Character)
    return GetOLTerrain()->Close(Closer);
  else
    {
      ADD_MESSAGE("There's something in the way...");
      return false;
    }
}

void lsquare::Save(outputfile& SaveFile) const
{
  GetStack()->Save(SaveFile); // This must be before square::Save!

  square::Save(SaveFile);

  SaveFile << GLTerrain << OLTerrain;

  {
    for(ushort c = 0; c < 4; ++c)
      SideStack[c]->Save(SaveFile);
  }

  ushort EmitterLength = Emitter.Length();

  SaveFile << EmitterLength;

  for(ushort c = 0; c < Emitter.Length(); ++c)
    SaveFile << Emitter.Access(c).Pos << Emitter.Access(c).DilatedEmitation;

  SaveFile << Fluid;
  SaveFile << Emitation << DivineOwner;
  SaveFile << Engraved << Room;
}

void lsquare::Load(inputfile& SaveFile)
{
  game::SetSquareInLoad(this);

  GetStack()->Load(SaveFile); // This must be before square::Load!

  square::Load(SaveFile);

  SaveFile >> GLTerrain >> OLTerrain;

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

  SaveFile >> Fluid;
  SaveFile >> Emitation >> DivineOwner;
  SaveFile >> Engraved >> Room;
}

void lsquare::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation) // ho ho ho /me is very funny. - Anonymous
{
  if(!Amount)
    return;

  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
	
  if(!GetFluid())
    {
      SetFluid(new fluid);
      GetFluid()->SetSquareUnder(this);
    }

  GetFluid()->SpillFluid(Amount, Color, Lumpiness, Variation);
}

ushort lsquare::GetLuminance() const
{
  ushort Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

  if(GetOLTerrain()->GetIsWalkable())
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

ushort lsquare::GetRawLuminance() const
{
  ushort Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

  for(ushort c = 0; c < Emitter.Length(); ++c)
    if(Emitter.Access(c).DilatedEmitation > Luminance)
      Luminance = Emitter.Access(c).DilatedEmitation;

  return Luminance > 511 ? 511 : Luminance;
}

void lsquare::AddCharacter(character* Guy)
{
  if(Character)
    ABORT("Overgrowing of square population detected!");

  Character = Guy;
  Guy->SetSquareUnder(this);
  SignalEmitationIncrease(Guy->GetEmitation());
  NewDrawRequested = true;
}

void lsquare::FastAddCharacter(character* Guy)
{
  if(Character)
    ABORT("Overgrowing of square population detected!");

  SetCharacter(Guy);
  Guy->SetSquareUnder(this);
}

void lsquare::Clean()
{
  GetStack()->Clean();

  for(ushort c = 0; c < 4; ++c)
    GetSideStack(c)->Clean();
}

void lsquare::RemoveCharacter()
{
  if(Character)
    {
      character* Backup = Character;
      SetCharacter(0);
      SignalEmitationDecrease(Backup->GetEmitation());
      NewDrawRequested = true;
    }
}

void lsquare::UpdateMemorizedDescription(bool Cheat)
{
  if(DescriptionChanged || Cheat)
    {
      if(GetLuminance() >= LIGHT_BORDER || Cheat)
	if(GetOTerrain()->GetIsWalkable())
	  {
	    bool Anything = false;

	    if(GetOLTerrain()->GetNameSingular() != "")
	      {
		SetMemorizedDescription(GetOLTerrain()->Name(INDEFINITE));
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

		SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGLTerrain()->Name(INDEFINITE));
	      }
	    else
	      if(Anything)
		SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGLTerrain()->Name(INDEFINITE));
	      else
		SetMemorizedDescription(GetGLTerrain()->Name(INDEFINITE));
	  }
	else
	  {
	    uchar HasItems = 0;
	    bool HasManyItems = false;

	    for(ushort c = 0; c < 4; ++c)
	      if(GetSideStack(c)->GetItems())
		{
		  if(++HasItems > 1)
		    break;

		  if(GetSideStack(c)->GetItems() > 1)
		    {
		      HasManyItems = true;
		      break;
		    }
		}

	    if(HasItems > 1 || HasManyItems)
	      SetMemorizedDescription("many items on " + GetOLTerrain()->Name(INDEFINITE));
	    else if(HasItems == 1)
	      for(ushort c = 0; c < 4; ++c)
		{
		  if(GetSideStack(c)->GetItems())
		    {
		      SetMemorizedDescription(GetSideStack(c)->GetItem(0)->Name(INDEFINITE) + " on " + GetOLTerrain()->Name(INDEFINITE));
		      break;
		    }
		}
	    else
	      SetMemorizedDescription(GetOLTerrain()->Name(INDEFINITE));
	  }
      else
	SetMemorizedDescription("darkness");

      DescriptionChanged = false;
    }
}

bool lsquare::Kick(ushort Strength, uchar KickWay, character* Kicker)
{
  if(GetCharacter() && Kicker->GetTeam()->GetRelation(GetCharacter()->GetTeam()) != HOSTILE)
    if(Kicker->GetIsPlayer() && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
      return false;
    else
      Kicker->Hostility(GetCharacter());

  if(Room)
    GetLevelUnder()->GetRoom(Room)->KickSquare(Kicker, this);

  GetStack()->Kick(Strength, GetLastSeen() == game::GetLOSTurns(), KickWay);

  if(GetCharacter())
    GetCharacter()->BeKicked(Strength, GetLastSeen() == game::GetLOSTurns(), KickWay, Kicker);

  GetOLTerrain()->Kick(Strength, GetLastSeen() == game::GetLOSTurns(), KickWay);
  Kicker->EditStrengthExperience(25);
  Kicker->EditAgilityExperience(50);
  Kicker->EditNP(-50);
  return true;
}

bool lsquare::Dig(character*, item*) // early prototype. Probably should include more checking with lterrains etc
{
  ADD_MESSAGE(GetOLTerrain()->DigMessage().c_str());
  ChangeOLTerrainAndUpdateLights(new empty);
  return true;
}

bool lsquare::CanBeDigged(character*, item*) const
{
  if((GetPos().X == 0 || GetPos().Y == 0 || GetPos().X == game::GetCurrentLevel()->GetXSize() - 1 || GetPos().Y == game::GetCurrentLevel()->GetYSize() - 1) && !*GetLevelUnder()->GetLevelScript()->GetOnGround())
    {
      ADD_MESSAGE("Somehow you feel that by digging this square you would collapse the whole dungeon.");
      return false;
    }

  return GetOLTerrain()->CanBeDigged();
}

void lsquare::ChangeLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  ChangeGLTerrain(NewGround);
  ChangeOLTerrain(NewOver);
}

void lsquare::ChangeGLTerrain(glterrain* NewGround)
{
  delete GetGLTerrain();
  GLTerrain = NewGround;

  if(NewGround)
    NewGround->SetSquareUnder(this);

  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;
}

void lsquare::ChangeOLTerrain(olterrain* NewOver)
{
  delete GetOLTerrain();
  OLTerrain = NewOver;

  if(NewOver)
    NewOver->SetSquareUnder(this);

  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;
}

gterrain* lsquare::GetGTerrain() const
{
  return GLTerrain;
}

oterrain* lsquare::GetOTerrain() const
{
  return OLTerrain;
}

void lsquare::ApplyScript(squarescript* SquareScript, room* Room)
{
  if(SquareScript->GetCharacter(false))
    {
      character* Char = SquareScript->GetCharacter()->Instantiate();

      if(!Char->GetTeam())
	Char->SetTeam(game::GetTeam(*GetLevelUnder()->GetLevelScript()->GetTeamDefault()));

      FastAddCharacter(Char);

      if(Room)
	Room->HandleInstantiatedCharacter(Char);
    }

  if(SquareScript->GetItem(false))
    GetStack()->FastAddItem(SquareScript->GetItem()->Instantiate());

  if(SquareScript->GetGTerrain(false))
    ChangeGLTerrain(SquareScript->GetGTerrain()->Instantiate());

  if(SquareScript->GetOTerrain(false))
    {
      Clean();

      olterrain* Terrain = SquareScript->GetOTerrain()->Instantiate();

      ChangeOLTerrain(Terrain);

      if(Room)
	Room->HandleInstantiatedOLTerrain(Terrain);
    }

  if(SquareScript->GetIsUpStairs(false) && *SquareScript->GetIsUpStairs())
    GetLevelUnder()->SetUpStairs(Pos);

  if(SquareScript->GetIsDownStairs(false) && *SquareScript->GetIsDownStairs())
    GetLevelUnder()->SetDownStairs(Pos);

  if(SquareScript->GetIsWorldMapEntry(false) && *SquareScript->GetIsWorldMapEntry())
    GetLevelUnder()->SetWorldMapEntry(Pos);
}

bool lsquare::CanBeSeen(bool IgnoreDarkness) const
{
  if((IgnoreDarkness || GetLuminance() >= LIGHT_BORDER) && GetLastSeen() == game::GetLOSTurns())
    return true;
  else
    return false;
}

bool lsquare::CanBeSeenFrom(vector2d FromPos, ulong MaxDistance, bool IgnoreDarkness) const
{
  if(!IgnoreDarkness && GetLuminance() < LIGHT_BORDER)
    return false;
  else
    return femath::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, MaxDistance, game::EyeHandler);
}

void lsquare::MoveCharacter(lsquare* To)
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
      if(!Movee->CanFly()) To->StepOn(Movee, this);
    }
}

void lsquare::StepOn(character* Stepper, square* ComingFrom)
{
  if(Room && ((lsquare*)ComingFrom)->GetRoom() != Room)
    GetLevelUnder()->GetRoom(Room)->Enter(Stepper);

  GetGLTerrain()->StepOn(Stepper);
  GetOLTerrain()->StepOn(Stepper);
  GetStack()->CheckForStepOnEffect(Stepper);
}

void lsquare::SwapCharacter(lsquare* With)
{
  if(Character)
    if(!With->Character)
      MoveCharacter(With);
    else
      {
	character* MoveeOne = Character, * MoveeTwo = With->Character;;
	ushort EmitOne = MoveeOne->GetEmitation(), EmitTwo = MoveeTwo->GetEmitation();
	SetCharacter(MoveeTwo);
	With->SetCharacter(MoveeOne);
	MoveeTwo->SetSquareUnder(this);
	MoveeOne->SetSquareUnder(With);
	SignalEmitationIncrease(EmitTwo);
	With->SignalEmitationIncrease(EmitOne);
	SignalEmitationDecrease(EmitOne);
	With->SignalEmitationDecrease(EmitTwo);
	NewDrawRequested = true;
	With->NewDrawRequested = true;
	With->StepOn(MoveeOne, this);
	StepOn(MoveeTwo, With);
      }
  else
    if(With->Character)
      With->MoveCharacter(this);
}

void lsquare::ReceiveVomit(character* Who)
{
  SpillFluid(1, MAKE_RGB(10,230,10),5,60);
  GetOLTerrain()->ReceiveVomit(Who);
}

room* lsquare::GetRoomClass() const
{
  if(Room)
    return GetLevelUnder()->GetRoom(Room);
  else
    return 0;
}

void lsquare::SetTemporaryEmitation(ushort What)
{
  ushort Old = TemporaryEmitation;
  TemporaryEmitation = What;

  if(What > Emitation)
    SignalEmitationIncrease(What);
  else if(What < Emitation)
    SignalEmitationDecrease(Old);
}

void lsquare::ChangeOLTerrainAndUpdateLights(olterrain* NewTerrain)
{
  ushort Emit = GetOLTerrain()->GetEmitation();
  ChangeOLTerrain(NewTerrain);
  SignalEmitationDecrease(Emit);
  ForceEmitterEmitation();
  game::SendLOSUpdateRequest();

  for(ushort c = 0; c < 4; ++c)
    for(ushort x = 0; x < GetSideStack(c)->GetItems(); ++x)
      GetSideStack(c)->MoveItem(x, GetStack())->SignalSquarePositionChange(false);
}

void lsquare::PolymorphEverything(character* Zapper)
{
  character* Character;

  if((Character = GetCharacter()))
    {
      if(Character != Zapper && Character->GetTeam() != Zapper->GetTeam())
	Zapper->Hostility(Character);

      Character->Polymorph(protosystem::CreateMonster(false), 5000 + RAND() % 5000);
    }

  GetStack()->Polymorph();
  GetOLTerrain()->Polymorph(Zapper);
}

void lsquare::DrawParticles(ushort Color, uchar)
{
  if(GetPos().X < game::GetCamera().X || GetPos().Y < game::GetCamera().Y || GetPos().X >= game::GetCamera().X + 50 || GetPos().Y >= game::GetCamera().Y + 30)
    return;

  clock_t StartTime = clock();

  vector2d BitPos = vector2d((GetPos().X - game::GetCamera().X) << 4, (GetPos().Y - game::GetCamera().Y + 2) << 4);

  game::DrawEverythingNoBlit(false);

  for(ushort c = 0; c < 10; c++)
    DOUBLEBUFFER->PutPixel(1 + BitPos.X + RAND() % 14, 1 + BitPos.Y + RAND() % 14, Color);

  graphics::BlitDBToScreen();

  NewDrawRequested = true; // Clean the pixels from the screen afterwards

  if(CanBeSeen())
    while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
}

void lsquare::StrikeEverything(character* Zapper, std::string DeathMsg, uchar Direction)
{
  GetStack()->StruckByWandOfStriking(Zapper, DeathMsg);

  switch(Direction)
    {
    case 0:
      GetSideStack(UP)->StruckByWandOfStriking(Zapper, DeathMsg);
      GetSideStack(LEFT)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 1:
      GetSideStack(UP)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 2:
      GetSideStack(UP)->StruckByWandOfStriking(Zapper, DeathMsg);
      GetSideStack(RIGHT)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 3:
      GetSideStack(LEFT)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 4:
      GetSideStack(RIGHT)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 5:
      GetSideStack(DOWN)->StruckByWandOfStriking(Zapper, DeathMsg);
      GetSideStack(LEFT)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 6:
      GetSideStack(DOWN)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    case 7:
      GetSideStack(DOWN)->StruckByWandOfStriking(Zapper, DeathMsg);
      GetSideStack(RIGHT)->StruckByWandOfStriking(Zapper, DeathMsg);
      break;
    }

  character* Char;

  if((Char = GetCharacter()))
    {
      Char->StruckByWandOfStriking(Zapper, DeathMsg);
      Zapper->Hostility(Char);
    }

  GetOLTerrain()->ReceiveStrike();	
}

void lsquare::RemoveFluid()
{
  if(GetFluid())
    {
      fluid* Backup = GetFluid();
      SetFluid(0);
      SignalEmitationDecrease(Backup->GetEmitation());
      SendNewDrawRequest();
      SendMemorizedUpdateRequest();
    }
}

void lsquare::HasBeenHitBy(item* Hitter, float Speed, uchar FlyingDirection, bool Visible)
{
  GetOLTerrain()->HasBeenHitBy(Hitter, Speed, FlyingDirection, Visible);
}

bool lsquare::TeleportEverything(character* Teleporter)
{
  bool HasHitSomething = false; 
  if(GetCharacter())
    {
      Teleporter->Hostility(GetCharacter());
      GetCharacter()->Teleport();
      HasHitSomething = true;
    }
  if(Room)
    GetLevelUnder()->GetRoom(Room)->TeleportSquare(Teleporter, this);

  if(GetStack()->Teleport())
    HasHitSomething = true;

  Teleporter->EditPerceptionExperience(50);
  Teleporter->EditNP(-50);
  return true;  
}
