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
#include "error.h"
#include "game.h"
#include "proto.h"
#include "save.h"

lsquare::lsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), GLTerrain(0), OLTerrain(0), Emitation(0), DivineMaster(0), Room(0), TemporaryEmitation(0), Fluid(0)
{
  Stack = new stack(this);
  SideStack[DOWN] = new stack(this, DOWN);
  SideStack[LEFT] = new stack(this, LEFT);
  SideStack[UP] = new stack(this, UP);
  SideStack[RIGHT] = new stack(this, RIGHT);
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

bool lsquare::DrawTerrain(bool Animate) const
{
  GetGLTerrain()->DrawToTileBuffer(Animate);

  if(GetFluid())
    GetFluid()->DrawToTileBuffer(Animate);
	
  GetOLTerrain()->DrawToTileBuffer(Animate);

  return true;
}

bool lsquare::DrawStacks(bool Animate) const
{
  bool Items = false;

  if(GetOTerrain()->IsWalkable())
    {
      if(GetStack()->DrawToTileBuffer(Animate))
	Items = true;

#define NS(D, S) game::GetCurrentLevel()->GetLSquare(Pos + D)->GetSideStack(S)

      if(GetPos().X)
	if(NS(vector2d(-1, 0), 1)->DrawToTileBuffer(Animate))
	  Items = true;

      if(GetPos().X < game::GetCurrentLevel()->GetXSize() - 1)
	if(NS(vector2d(1, 0), 3)->DrawToTileBuffer(Animate))
	  Items = true;

      if(GetPos().Y)
	if(NS(vector2d(0, -1), 2)->DrawToTileBuffer(Animate))
	  Items = true;

      if(GetPos().Y < game::GetCurrentLevel()->GetYSize() - 1)
	if(NS(vector2d(0, 1), 0)->DrawToTileBuffer(Animate))
	  Items = true;
    }

  return Items;
}

bool lsquare::DrawCharacters(bool Animate) const
{
  if(GetCharacter())
    {
      GetCharacter()->DrawToTileBuffer(Animate);
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
	  DrawTerrain(false);
	  DrawStacks(false);

	  igraph::GetTileBuffer()->Blit(GetMemorized());

	  if(GetStack()->GetItems() > 1 && GetOTerrain()->IsWalkable())
	    igraph::GetSymbolGraphic()->MaskedBlit(GetMemorized(), 0, 16, 0, 0, 16, 16);
	  
	  igraph::GetFOWGraphic()->MaskedBlit(GetMemorized());
	}
      else
	{
	  Memorized->Fill(0);
	  igraph::GetFOWGraphic()->MaskedBlit(GetMemorized());
	}

      MemorizedUpdateRequested = false;
    }
}

void lsquare::Draw()
{
  if(NewDrawRequested || GetAnimatedEntities())
    {
      vector2d BitPos = game::CalculateScreenCoordinates(GetPos());
      ushort Luminance = GetLuminance();

      if(Luminance >= LIGHT_BORDER || game::GetSeeWholeMapCheat())
	{
	  ushort ContrastLuminance = ushort(256.0f * configuration::GetContrast() / 100);
	  ushort RealLuminance = game::GetSeeWholeMapCheat() ? ContrastLuminance : ushort(float(Luminance) * configuration::GetContrast() / 100);

	  if(!configuration::GetOutlineItems())
	    {
	      DrawTerrain(true);
	      DrawStacks(true);

	      if(!configuration::GetOutlineCharacters())
		if(GetStack()->GetItems() <= 1)
		  {
		    DrawCharacters(true);
		    igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		    DrawCharacterSymbols(BitPos, ContrastLuminance);
		  }
		else
		  {
		    igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);

		    if(GetOTerrain()->IsWalkable())
		      igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos, 16, 16, ContrastLuminance);

		    if(GetCharacter())
		      {
			igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
			DrawCharacters(true);
			igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
			DrawCharacterSymbols(BitPos, ContrastLuminance);
		      }
		  }
	      else
		{
		  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);

		  if(GetStack()->GetItems() > 1 && GetOTerrain()->IsWalkable()) 
		    igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos, 16, 16, ContrastLuminance);

		  if(GetCharacter())
		    {
		      igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
		      DrawCharacters(true);
		      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
		      igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
		      DrawCharacterSymbols(BitPos, ContrastLuminance);
		    }
		}
	    }
	  else
	    {
	      DrawTerrain(true);

	      igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
	      igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);

	      if(DrawStacks(true))
		{
		  igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		  igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetItemOutlineColor());

		  if(GetStack()->GetItems() > 1 && GetOTerrain()->IsWalkable())
		    igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetOutlineBuffer(), 0, 16, 0, 0, 16, 16);

		  igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);

		  if(GetCharacter())
		    igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
		}

	      if(!configuration::GetOutlineCharacters())
		{
		  if(DrawCharacters(true))
		    {
		      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		      DrawCharacterSymbols(BitPos, ContrastLuminance);
		    }
		}
	      else
		if(DrawCharacters(true))
		  {
		    igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		    igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
		    igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
		    DrawCharacterSymbols(BitPos, ContrastLuminance);
		  }
	    }
	}
      else
	DOUBLEBUFFER->Fill(BitPos, 16, 16, 0);

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
      femath::DoLine(GetPos().X, GetPos().Y, XPointer, YPointer, game::EmitationHandler);
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
      femath::DoLine(GetPos().X, GetPos().Y, XPointer, YPointer, game::EmitationHandler);
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
      femath::DoLine(GetPos().X, GetPos().Y, XPointer, YPointer, game::NoxifyHandler);
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

#define IW(X, Y) game::GetCurrentLevel()->GetLSquare(Pos + vector2d(X, Y))->GetOLTerrain()->IsWalkable()

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

  /*
   * This seems to be a completely useless piece of conditional code.
   * It is, however, extremely important, although I don't remember why.
   */

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
  SaveFile << Emitation << DivineMaster;
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
  SaveFile >> Emitation >> DivineMaster;
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

  if(GetOLTerrain()->IsWalkable())
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
    ABORT("Overgrowth of square population detected!");

  Character = Guy;
  Guy->SetSquareUnder(this);
  SignalEmitationIncrease(Guy->GetEmitation());
  NewDrawRequested = true;

  if(Guy->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::FastAddCharacter(character* Guy)
{
  if(Character)
    ABORT("Overgrowth of square population detected!");

  SetCharacter(Guy);
  Guy->SetSquareUnder(this);

  if(Guy->IsAnimated())
    IncAnimatedEntities();
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
      if(Character->IsAnimated())
	DecAnimatedEntities();

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
	if(GetOTerrain()->IsWalkable())
	  {
	    bool Anything = false;

	    if(GetOLTerrain()->NameSingular() != "")
	      {
		SetMemorizedDescription(GetOLTerrain()->Name(INDEFINITE));
		Anything = true;
	      }

	    if(GetStack()->GetItems())
	      {
		if(Anything)
		  if(GetStack()->GetItems() == 1)
		    SetMemorizedDescription(GetMemorizedDescription() + " and " + std::string(GetStack()->GetBottomItem()->Name(INDEFINITE)));
		  else
		    SetMemorizedDescription(GetMemorizedDescription() + " and " + "many items");
		else
		  if(GetStack()->GetItems() == 1)
		    SetMemorizedDescription(std::string(GetStack()->GetBottomItem()->Name(INDEFINITE)));
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
		      SetMemorizedDescription(GetSideStack(c)->GetBottomItem()->Name(INDEFINITE) + " on " + GetOLTerrain()->Name(INDEFINITE));
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

bool lsquare::Kick(character* Kicker, ushort Strength, uchar KickWay)
{
  if(GetCharacter() && Kicker->GetTeam()->GetRelation(GetCharacter()->GetTeam()) != HOSTILE)
    if(Kicker->IsPlayer() && !game::BoolQuestion("This might cause a hostile reaction. Are you sure? [y/N]"))
      return false;
    else
      Kicker->Hostility(GetCharacter());

  if(Room)
    GetLevelUnder()->GetRoom(Room)->KickSquare(Kicker, this);

  GetStack()->Kick(Kicker, Strength, KickWay);

  if(GetCharacter())
    GetCharacter()->BeKicked(Kicker, Strength, KickWay);

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

bool lsquare::CanBeDug() const
{
  if((GetPos().X == 0 || GetPos().Y == 0 || GetPos().X == game::GetCurrentLevel()->GetXSize() - 1 || GetPos().Y == game::GetCurrentLevel()->GetYSize() - 1) && !*GetLevelUnder()->GetLevelScript()->GetOnGround())
    {
      ADD_MESSAGE("Somehow you feel that by digging this square you would collapse the whole dungeon.");
      return false;
    }
  else
    return true;
}

void lsquare::ChangeLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  ChangeGLTerrain(NewGround);
  ChangeOLTerrain(NewOver);
}

void lsquare::ChangeGLTerrain(glterrain* NewGround)
{
  if(GetGLTerrain()->IsAnimated())
    DecAnimatedEntities();

  delete GetGLTerrain();
  GLTerrain = NewGround;
  NewGround->SetSquareUnder(this);
  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::ChangeOLTerrain(olterrain* NewOver)
{
  if(GetOLTerrain()->IsAnimated())
    DecAnimatedEntities();

  delete GetOLTerrain();
  OLTerrain = NewOver;
  NewOver->SetSquareUnder(this);
  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;

  if(NewOver->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::SetLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  SetGLTerrain(NewGround);
  SetOLTerrain(NewOver);
}

void lsquare::SetGLTerrain(glterrain* NewGround)
{
  GLTerrain = NewGround;
  NewGround->SetSquareUnder(this);

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::SetOLTerrain(olterrain* NewOver)
{
  OLTerrain = NewOver;
  NewOver->SetSquareUnder(this);

  if(NewOver->IsAnimated())
    IncAnimatedEntities();
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
  ulong Distance = (GetPos() - FromPos).Length();

  if(Distance > MaxDistance || (!IgnoreDarkness && GetLuminance() < LIGHT_BORDER))
    return false;
  else
    {
      character* Char = GetCharacter();

      if(Char && Char->IsPlayer() && Distance < Char->LOSRangeSquare())
	return GetAreaUnder()->GetSquare(FromPos)->CanBeSeen(IgnoreDarkness);
      else
	return femath::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, game::EyeHandler);
    }
}

void lsquare::MoveCharacter(lsquare* To)
{
  if(Character)
    {
      if(To->Character)
	ABORT("Overgrowth of square population detected!");

      character* Movee = Character;
      ushort Emit = Movee->GetEmitation();
      SetCharacter(0);
      To->Character = Movee;
      Movee->SetSquareUnder(To);
      To->SignalEmitationIncrease(Emit);
      SignalEmitationDecrease(Emit);
      NewDrawRequested = true;
      To->NewDrawRequested = true;

      if(Movee->IsAnimated())
	{
	  DecAnimatedEntities();
	  To->IncAnimatedEntities();
	}

      if(!Movee->CanFly())
	To->StepOn(Movee, this);
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

      if(MoveeOne->IsAnimated())
	{
	  DecAnimatedEntities();
	  With->IncAnimatedEntities();
	}

      if(MoveeTwo->IsAnimated())
	{
	  IncAnimatedEntities();
	  With->DecAnimatedEntities();
	}

	if(!MoveeOne->CanFly())
	  With->StepOn(MoveeOne, this);

	if(!MoveeTwo->CanFly())
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
  game::GetCurrentArea()->UpdateLOS();

  for(ushort c = 0; c < 4; ++c)
    for(stackiterator i = GetSideStack(c)->GetBottomSlot(); i != GetSideStack(c)->GetSlotAboveTop(); ++i)
      GetSideStack(c)->MoveItem(i, GetStack())->SignalSquarePositionChange(false);
}

void lsquare::PolymorphEverything(character* Zapper)
{
  character* Character = GetCharacter();

  if(Character)
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
  if(GetPos().X < game::GetCamera().X || GetPos().Y < game::GetCamera().Y || GetPos().X >= game::GetCamera().X + game::GetScreenSize().X || GetPos().Y >= game::GetCamera().Y + game::GetScreenSize().Y)
    return;

  clock_t StartTime = clock();

  vector2d BitPos = game::CalculateScreenCoordinates(GetPos());

  game::DrawEverythingNoBlit();

  for(ushort c = 0; c < 10; ++c)
    DOUBLEBUFFER->PutPixel(BitPos + vector2d(1 + RAND() % 14, 1 + RAND() % 14), Color);

  graphics::BlitDBToScreen();

  NewDrawRequested = true; // Clean the pixels from the screen afterwards

  if(CanBeSeen())
    while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
}

void lsquare::StrikeEverything(character* Zapper, const std::string& DeathMsg, short Damage, uchar Direction)
{
  GetStack()->ReceiveDamage(Zapper, Damage, ENERGY);

  switch(Direction)
    {
    case 0:
      GetSideStack(UP)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(LEFT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 1:
      GetSideStack(UP)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 2:
      GetSideStack(UP)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(RIGHT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 3:
      GetSideStack(LEFT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 4:
      GetSideStack(RIGHT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 5:
      GetSideStack(DOWN)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(LEFT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 6:
      GetSideStack(DOWN)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 7:
      GetSideStack(DOWN)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(RIGHT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    }

  character* Char = GetCharacter();

  if(Char)
    {
      Char->ReceiveDamage(Zapper, Damage, ENERGY, ALL);
      Zapper->Hostility(Char);
      Char->CheckDeath("killed by a burst of energy");
    }

  GetOLTerrain()->ReceiveDamage(Zapper, Damage, ENERGY);	
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

void lsquare::TeleportEverything(character* Teleporter)
{
  if(GetCharacter())
    {
      Teleporter->Hostility(GetCharacter());
      GetCharacter()->Teleport();
    }

  if(Room)
    GetLevelUnder()->GetRoom(Room)->TeleportSquare(Teleporter, this);

  GetStack()->Teleport();

  Teleporter->EditPerceptionExperience(50);
  Teleporter->EditNP(-50);
}

bool lsquare::ReceiveApply(item* Thingy, character* Applier)
{
  if(GetGLTerrain()->ReceiveApply(Thingy, Applier) || GetOLTerrain()->ReceiveApply(Thingy,Applier))
    {
      return true;
    }
  else
    {
      if(Applier->IsPlayer()) 
	ADD_MESSAGE("You cannot apply that on this!");
      return false;
    }
}

bool lsquare::DipInto(item* Thingy, character* Dipper)
{
  if(GetGLTerrain()->IsDipDestination() || GetOLTerrain()->IsDipDestination())
    {
      if(GetRoom() && GetLevelUnder()->GetRoom(GetRoom())->HasDipHandler())
	{
	  if(!GetLevelUnder()->GetRoom(GetRoom())->Dip(Dipper))
	    return false;
	}
    }
  
  if(GetGLTerrain()->DipInto(Thingy, Dipper) || GetOLTerrain()->DipInto(Thingy, Dipper))
    {
      return true;
    }
  else
    {
      if(Dipper->IsPlayer())
	ADD_MESSAGE("You cannot dip %s on that square!", Thingy->CHARNAME(DEFINITE));

      return false;
    }
}

void lsquare::DrawCharacterSymbols(vector2d BitPos, ushort ContrastLuminance)
{
  if(GetCharacter() && !GetCharacter()->IsPlayer())
    {
      if(GetCharacter()->GetTeam() == game::GetPlayer()->GetTeam())
	igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 32, 16, BitPos, 16, 16, ContrastLuminance);
    }
}

bool lsquare::LockEverything(character*)
{
  if(GetOLTerrain()->IsLocked())
    {
      GetOLTerrain()->Lock();
      return true;
    }
  else
    return false;
}

bool lsquare::RaiseTheDead(character* Summoner)
{
  if(GetStack()->RaiseTheDead(Summoner))
      return true;

  if(GetCharacter())
    {
      return GetCharacter()->RaiseTheDead(Summoner);
    }

  return false;
}

