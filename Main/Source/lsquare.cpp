#include <ctime>
#include <cmath>

#include "lsquare.h"
#include "stack.h"
#include "lterrade.h"
#include "level.h"
#include "itemba.h"
#include "message.h"
#include "stdover.h"
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

lsquare::lsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), GLTerrain(0), OLTerrain(0), Emitation(0), DivineMaster(0), Room(0), TemporaryEmitation(0), Fluid(0), Luminance(0)
{
  Stack = new stack(this);
  SideStack[DOWN] = new stack(this, 0, DOWN);
  SideStack[LEFT] = new stack(this, 0, LEFT);
  SideStack[UP] = new stack(this, 0, UP);
  SideStack[RIGHT] = new stack(this, 0, RIGHT);
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
  if(EmitationUpdate == Emitation && EmitationUpdate != CalculateEmitation())
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
  GLTerrain->DrawToTileBuffer(Animate);

  if(Fluid)
    Fluid->DrawToTileBuffer(Animate);
	
  OLTerrain->DrawToTileBuffer(Animate);
  return true;
}

bool lsquare::DrawStacks(bool Animate) const
{
  bool Items = false;

  if(OLTerrain->IsWalkable())
    {
      if(Stack->DrawToTileBuffer(Animate))
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
  if(Character)
    {
      Character->DrawToTileBuffer(Animate);
      return true;
    }
  else
    return false;
}

void lsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      //ushort Luminance = GetLuminance();

      if(Luminance >= LIGHT_BORDER)
	{
	  DrawTerrain(false);
	  DrawStacks(false);

	  igraph::GetTileBuffer()->Blit(Memorized);

	  if(Stack->GetItems() > 1 && OLTerrain->IsWalkable())
	    igraph::GetSymbolGraphic()->MaskedBlit(Memorized, 0, 16, 0, 0, 16, 16);
	  
	  igraph::GetFOWGraphic()->MaskedBlit(Memorized);
	}
      else
	{
	  Memorized->Fill(0);
	  igraph::GetFOWGraphic()->MaskedBlit(Memorized);
	}

      MemorizedUpdateRequested = false;
    }
}

void lsquare::Draw()
{
  if(NewDrawRequested || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);
      //ushort Luminance = GetLuminance();

      if(Luminance >= LIGHT_BORDER || game::GetSeeWholeMapCheat())
	{
	  ushort ContrastLuminance = (configuration::GetContrast() << 8) / 100;
	  ushort RealLuminance = game::GetSeeWholeMapCheat() ? ContrastLuminance : Luminance * configuration::GetContrast() / 100;

	  if(!configuration::GetOutlineItems())
	    {
	      DrawTerrain(true);
	      DrawStacks(true);

	      if(!configuration::GetOutlineCharacters())
		if(Stack->GetItems() <= 1)
		  {
		    DrawCharacters(true);
		    igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		    DrawCharacterSymbols();
		  }
		else
		  {
		    igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);

		    if(OLTerrain->IsWalkable())
		      igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos, 16, 16, ContrastLuminance);

		    if(Character)
		      {
			igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
			DrawCharacters(true);
			igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
			DrawCharacterSymbols();
		      }
		  }
	      else
		{
		  igraph::GetTileBuffer()->Blit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);

		  if(Stack->GetItems() > 1 && OLTerrain->IsWalkable()) 
		    igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 16, BitPos, 16, 16, ContrastLuminance);

		  if(Character)
		    {
		      igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
		      DrawCharacters(true);
		      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		      igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
		      igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
		      DrawCharacterSymbols();
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

		  if(Stack->GetItems() > 1 && OLTerrain->IsWalkable())
		    igraph::GetSymbolGraphic()->MaskedBlit(igraph::GetOutlineBuffer(), 0, 16, 0, 0, 16, 16);

		  igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);

		  if(Character)
		    igraph::GetTileBuffer()->Fill(TRANSPARENTCOL);
		}

	      if(!configuration::GetOutlineCharacters())
		{
		  if(DrawCharacters(true))
		    {
		      igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		      DrawCharacterSymbols();
		    }
		}
	      else
		if(DrawCharacters(true))
		  {
		    igraph::GetTileBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, RealLuminance);
		    igraph::GetTileBuffer()->CreateOutlineBitmap(igraph::GetOutlineBuffer(), configuration::GetCharacterOutlineColor());
		    igraph::GetOutlineBuffer()->MaskedBlit(DOUBLEBUFFER, 0, 0, BitPos, 16, 16, ContrastLuminance);
		    DrawCharacterSymbols();
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
  game::SetCurrentEmitterEmitation(GetEmitation());
  game::SetCurrentEmitterPos(GetPos());

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(Pos.X) - XPointer, long(Pos.Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(Pos.X, Pos.Y, XPointer, YPointer, game::EmitationHandler);
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
  game::SetCurrentEmitterEmitation(GetEmitation());
  game::SetCurrentEmitterPos(GetPos());

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(Pos.X) - XPointer, long(Pos.Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(Pos.X, Pos.Y, XPointer, YPointer, game::EmitationHandler);
  });
}

void lsquare::Noxify()
{
  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(Emitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterPos(GetPos());

  DO_FILLED_RECTANGLE(Pos.X, Pos.Y, 0, 0, game::GetCurrentLevel()->GetXSize() - 1, game::GetCurrentLevel()->GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(Pos.X) - XPointer, long(Pos.Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(Pos.X, Pos.Y, XPointer, YPointer, game::NoxifyHandler);
  });
}

void lsquare::ForceEmitterNoxify()
{
  for(ushort c = 0; c < Emitter.size(); ++c)
    game::GetCurrentLevel()->GetLSquare(Emitter[c].Pos)->Noxify();
}

void lsquare::ForceEmitterEmitation()
{
  for(ushort c = 0; c < Emitter.size(); ++c)
    game::GetCurrentLevel()->GetLSquare(Emitter[c].Pos)->Emitate();
}

void lsquare::NoxifyEmitter(vector2d Dir)
{
  /*
   * DO NOT remove the iterator, or ForceEmitterEmitation() will not work properly!
   * (had to add add this comment when I came here for nth time and thought not to do it is really stupid...)
   */

  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(!Luminance || Luminance != i->DilatedEmitation || !OLTerrain->IsWalkable())
	  i->DilatedEmitation = 0;
	else
	  {
	    ushort OldLuminance = Luminance;
	    i->DilatedEmitation = 0;
	    CalculateLuminance();

	    if(OldLuminance == Luminance)
	      return;

	    NewDrawRequested = true;

	    if(!Luminance)
	      {
		MemorizedUpdateRequested = true;
		DescriptionChanged = true;

		if(GetLastSeen() == game::GetLOSTurns())
		  {
		    UpdateMemorized();
		    UpdateMemorizedDescription();
		  }
	      }
	  }

	return;
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

void lsquare::AlterLuminance(vector2d Dir, ushort NewLuminance)
{
  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(i->DilatedEmitation == NewLuminance)
	  return;

	if(NewLuminance >= LIGHT_BORDER)
	  {
	    if(NewLuminance != Luminance && OLTerrain->IsWalkable())
	      {
		ushort OldLuminance = Luminance;
		i->DilatedEmitation = NewLuminance;
  		CalculateLuminance();

		if(OldLuminance == Luminance)
		  return;

		NewDrawRequested = true;

		if(!OldLuminance)
		  {
		    MemorizedUpdateRequested = true;
		    DescriptionChanged = true;

		    if(GetLastSeen() == game::GetLOSTurns())
		      {
			UpdateMemorized();
			UpdateMemorizedDescription();
		      }
		  }
	      }
	    else
	      i->DilatedEmitation = NewLuminance;
	  }
	else
	  {
	    if(!Luminance || Luminance != i->DilatedEmitation || !OLTerrain->IsWalkable())
	      Emitter.erase(i);
	    else
	      {
		ushort OldLuminance = Luminance;
		Emitter.erase(i);
		CalculateLuminance();

		if(OldLuminance == Luminance)
		  return;

		NewDrawRequested = true;

		if(!Luminance)
		  {
		    MemorizedUpdateRequested = true;
		    DescriptionChanged = true;

		    if(GetLastSeen() == game::GetLOSTurns())
		      {
			UpdateMemorized();
			UpdateMemorizedDescription();
		      }
		  }
	      }
	  }

	return;
      }

  /* Emitter was not found, add it. */

  if(NewLuminance >= LIGHT_BORDER)
    {
      if(NewLuminance > Luminance && OLTerrain->IsWalkable())
	{
	  ushort OldLuminance = Luminance;
	  Emitter.push_back(emitter(Dir, NewLuminance));
  	  CalculateLuminance();

	  if(OldLuminance == Luminance)
	    return;

	  NewDrawRequested = true;

	  if(!Luminance)
	    {
	      DescriptionChanged = true;
	      MemorizedUpdateRequested = true;

	      if(GetLastSeen() == game::GetLOSTurns())
		{
		  UpdateMemorized();
		  UpdateMemorizedDescription();
		}
	    }
	}
      else
	Emitter.push_back(emitter(Dir, NewLuminance));
    }
}

bool lsquare::Open(character* Opener)
{
  return GetStack()->Open(Opener) || GetOLTerrain()->Open(Opener); 
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

  for(ushort c = 0; c < 4; ++c)
    SideStack[c]->Save(SaveFile);

  SaveFile << Emitter << Fluid << Emitation << DivineMaster << Engraved << Room << Luminance;
}

void lsquare::Load(inputfile& SaveFile)
{
  GetStack()->Load(SaveFile); // This must be before square::Load!
  square::Load(SaveFile);
  SaveFile >> GLTerrain >> OLTerrain;

  for(ushort c = 0; c < 4; ++c)
    SideStack[c]->Load(SaveFile);

  SaveFile >> Emitter >> Fluid >> Emitation >> DivineMaster >> Engraved >> Room >> Luminance;
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

void lsquare::CalculateLuminance()
{
  Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

  if(OLTerrain->IsWalkable())
    {
      for(ushort c = 0; c < Emitter.size(); ++c)
	if(Emitter[c].DilatedEmitation > Luminance)
	  Luminance = Emitter[c].DilatedEmitation;
    }
  else
    for(ushort c = 0; c < Emitter.size(); ++c)
      if(CalculateBitMask(Emitter[c].Pos) & CalculateBitMask(game::GetPlayer()->GetPos()))
	if(Emitter[c].DilatedEmitation > Luminance)
	  Luminance = Emitter[c].DilatedEmitation;

  if(Luminance > 511)
    Luminance = 511;
  //return Luminance > 511 ? 511 : Luminance;
}

ushort lsquare::GetRawLuminance() const
{
  ushort Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

  for(ushort c = 0; c < Emitter.size(); ++c)
    if(Emitter[c].DilatedEmitation > Luminance)
      Luminance = Emitter[c].DilatedEmitation;

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

	    if(!GetOLTerrain()->IsEmpty())
	      {
		SetMemorizedDescription(GetOLTerrain()->GetName(INDEFINITE));
		Anything = true;
	      }

	    if(GetStack()->GetItems())
	      {
		if(Anything)
		  if(GetStack()->GetItems() == 1)
		    SetMemorizedDescription(GetMemorizedDescription() + " and " + std::string(GetStack()->GetBottomItem()->GetName(INDEFINITE)));
		  else
		    SetMemorizedDescription(GetMemorizedDescription() + " and " + "many items");
		else
		  if(GetStack()->GetItems() == 1)
		    SetMemorizedDescription(std::string(GetStack()->GetBottomItem()->GetName(INDEFINITE)));
		  else
		    SetMemorizedDescription("many items");

		Anything = true;

		SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGLTerrain()->GetName(INDEFINITE));
	      }
	    else
	      if(Anything)
		SetMemorizedDescription(GetMemorizedDescription() + " on " + GetGLTerrain()->GetName(INDEFINITE));
	      else
		SetMemorizedDescription(GetGLTerrain()->GetName(INDEFINITE));
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
	      SetMemorizedDescription("many items on " + GetOLTerrain()->GetName(INDEFINITE));
	    else if(HasItems == 1)
	      for(ushort c = 0; c < 4; ++c)
		{
		  if(GetSideStack(c)->GetItems())
		    {
		      SetMemorizedDescription(GetSideStack(c)->GetBottomItem()->GetName(INDEFINITE) + " on " + GetOLTerrain()->GetName(INDEFINITE));
		      break;
		    }
		}
	    else
	      SetMemorizedDescription(GetOLTerrain()->GetName(INDEFINITE));
	  }
      else
	SetMemorizedDescription("darkness");

      DescriptionChanged = false;
    }
}

void lsquare::BeKicked(character* Kicker, float KickStrength, float KickToHitValue, short Success, bool Critical)
{
  if(Room)
    GetLevelUnder()->GetRoom(Room)->KickSquare(Kicker, this);

  GetStack()->BeKicked(Kicker, KickStrength);

  if(GetCharacter())
    GetCharacter()->BeKicked(Kicker, KickStrength, KickToHitValue, Success, Critical);

  GetOLTerrain()->BeKicked(Kicker, KickStrength);
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

bool lsquare::CanBeSeenByPlayer(bool IgnoreDarkness) const
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
	return GetAreaUnder()->GetSquare(FromPos)->CanBeSeenByPlayer(true);
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
  bool WasWalkable = GetOLTerrain()->IsWalkable();

  if(WasWalkable && !NewTerrain->IsWalkable())
    ForceEmitterNoxify();

  ushort OldEmit = GetOLTerrain()->GetEmitation();
  ChangeOLTerrain(NewTerrain);
  SignalEmitationIncrease(NewTerrain->GetEmitation());
  SignalEmitationDecrease(OldEmit);

  for(ushort c = 0; c < 4; ++c)
    while(GetSideStack(c)->GetItems())
      GetSideStack(c)->MoveItem(GetSideStack(c)->GetBottomSlot(), GetStack())->SignalSquarePositionChange(false);

  if(WasWalkable != GetOLTerrain()->IsWalkable())
    {
      ForceEmitterEmitation();
      CalculateLuminance();

      if(GetLastSeen() == game::GetLOSTurns())
	{
	  game::SendLOSUpdateRequest();
	  //GetAreaUnder()->UpdateLOS();
	}
    }
}

void lsquare::PolymorphEverything(character* Zapper)
{
  character* Character = GetCharacter();

  if(Character)
    {
      if(Character != Zapper && Character->GetTeam() != Zapper->GetTeam())
	Zapper->Hostility(Character);

      Character->PolymorphRandomly(5000 + RAND() % 5000);
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

  if(CanBeSeenByPlayer())
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

void lsquare::HasBeenHitBy(item* Hitter, float Speed, uchar FlyingDirection)
{
  GetOLTerrain()->HasBeenHitBy(Hitter, Speed, FlyingDirection);
}

void lsquare::TeleportEverything(character* Teleporter)
{
  if(GetCharacter())
    {
      Teleporter->Hostility(GetCharacter());
      GetCharacter()->TeleportRandomly();
    }

  if(Room)
    GetLevelUnder()->GetRoom(Room)->TeleportSquare(Teleporter, this);

  GetStack()->TeleportRandomly();

  //Teleporter->EditPerceptionExperience(50);
  //Teleporter->EditNP(-50);
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

void lsquare::DrawCharacterSymbols()
{
  if(Character && Character->GetTeam() == game::GetPlayer()->GetTeam() && !Character->IsPlayer())
    igraph::GetSymbolGraphic()->MaskedBlit(DOUBLEBUFFER, 32, 16, game::CalculateScreenCoordinates(Pos), 16, 16, ushort((configuration::GetContrast() << 8) / 100));
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

bool lsquare::TryKey(item* Key, character* Applier)
{
  if(GetOLTerrain()->TryKey(Key,Applier) || GetStack()->TryKey(Key, Applier))
    {
      return true;
    }
  else
    {
      if(Applier->IsPlayer()) 
	ADD_MESSAGE("There's no keyhole here!");

      return false;
    }
}

outputfile& operator<<(outputfile& SaveFile, const emitter& EmitterElement)
{
  SaveFile << EmitterElement.Pos << EmitterElement.DilatedEmitation;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, emitter& EmitterElement)
{
  SaveFile >> EmitterElement.Pos >> EmitterElement.DilatedEmitation;
  return SaveFile;
}

void lsquare::SetLastSeen(ulong What)
{
  if(LastSeen == What)
    return;

  if(!LastSeen)
    Memorized = new bitmap(16, 16);

  if(LastSeen < What - 1)
    NewDrawRequested = true;

  if(!OLTerrain->IsWalkable())
    {
      ushort OldLuminance = Luminance;
      CalculateLuminance();

      if(OldLuminance != Luminance)
	{
	  NewDrawRequested = true;

	  if((Luminance >= LIGHT_BORDER && OldLuminance < LIGHT_BORDER) || (OldLuminance >= LIGHT_BORDER && Luminance < LIGHT_BORDER))
	    {
	      MemorizedUpdateRequested = true;
	      DescriptionChanged = true;
	    }
	}
    }

  UpdateMemorized();
  UpdateMemorizedDescription();
  LastSeen = What;
}
