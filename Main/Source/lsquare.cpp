#include <ctime>
#include <cmath>

#include "lsquare.h"
#include "stack.h"
#include "lterrade.h"
#include "level.h"
#include "itemba.h"
#include "message.h"
#include "festring.h"
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

lsquare::lsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), GLTerrain(0), OLTerrain(0), Emitation(0), DivineMaster(0), Room(0), TemporaryEmitation(0), Fluid(0), Memorized(0), MemorizedUpdateRequested(true)
{
  Stack = new stack(this, 0, CENTER, false);
  SideStack[DOWN] = new stack(this, 0, DOWN, false);
  SideStack[LEFT] = new stack(this, 0, LEFT, false);
  SideStack[UP] = new stack(this, 0, UP, false);
  SideStack[RIGHT] = new stack(this, 0, RIGHT, false);
}

lsquare::~lsquare()
{
  delete GLTerrain;
  delete OLTerrain;
  delete Stack;

  for(ushort c = 0; c < 4; ++c)
    delete SideStack[c];

  delete Fluid;
  delete Memorized;
}

void lsquare::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0 && !game::IsGenerating())
    {
      CalculateEmitation();
      Emitate();
    }
}

void lsquare::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation && !game::IsGenerating())
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	ReEmitate(EmitationUpdate);
    }
}

void lsquare::CalculateEmitation()
{
  Emitation = Stack->GetEmitation();

  for(ushort c = 0; c < 4; ++c)
    {
      stack* Stack = GetSideStackOfAdjacentSquare(c);

      if(Stack)
	game::CombineLights(Emitation, Stack->GetEmitation());
    }

  if(Character)
    game::CombineLights(Emitation, Character->GetEmitation());

  if(GLTerrain)
    game::CombineLights(Emitation, GLTerrain->GetEmitation());

  if(OLTerrain)
    game::CombineLights(Emitation, OLTerrain->GetEmitation());

  game::CombineLights(Emitation, TemporaryEmitation);
}

void lsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      if(!IsDark())
	{
	  DrawStaticContents(Memorized, vector2d(0, 0), MakeRGB24(128, 128, 128), false);
	  igraph::GetFOWGraphic()->MaskedBlit(Memorized, 0, 0, 0, 0, 16, 16, uchar(0), 0);
	}
      else
	igraph::GetFOWGraphic()->Blit(Memorized);

      MemorizedUpdateRequested = false;
    }
}

void lsquare::DrawStaticContents(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool RealDraw) const
{
  GLTerrain->Draw(Bitmap, Pos, Luminance, false, RealDraw);

  if(Fluid)
    Fluid->Draw(Bitmap, Pos, Luminance, true, RealDraw);
	
  OLTerrain->Draw(Bitmap, Pos, Luminance, true, RealDraw);

  if(OLTerrain->IsWalkable())
    {
      Stack->Draw(game::GetPlayer(), Bitmap, Pos, Luminance, true, RealDraw, RealDraw);

      for(ushort c = 0; c < 4; ++c)
	{
	  stack* Stack = GetSideStackOfAdjacentSquare(c);

	  if(Stack)
	    Stack->Draw(game::GetPlayer(), Bitmap, Pos, Luminance, true, RealDraw, RealDraw);
	}
    }
}

void lsquare::Draw()
{
  if(NewDrawRequested || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(!IsDark() || game::GetSeeWholeMapCheat())
	{
	  ulong RealLuminance = game::GetSeeWholeMapCheat() ? configuration::GetContrastLuminance() : configuration::ApplyContrastTo(Luminance);
	  DrawStaticContents(DOUBLE_BUFFER, BitPos, RealLuminance, true);

	  if(Character && (Character->CanBeSeenByPlayer() || game::GetSeeWholeMapCheat()))
	    Character->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true, true);
	}
      else
	{
	  DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

	  if(Character && Character->CanBeSeenByPlayer())
	    Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), false, true);
	}

      NewDrawRequested = false;
    }
}

void lsquare::Emitate()
{
  if(game::IsDark(GetEmitation()))
    return;

  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(GetEmitation());

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterEmitation(GetEmitation());
  game::SetCurrentEmitterPos(GetPos());

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevelUnder()->GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, game::EmitationHandler);
}

void lsquare::ReEmitate(ulong OldEmitation)
{
  if(game::IsDark(OldEmitation))
    return;

  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(OldEmitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterEmitation(GetEmitation());
  game::SetCurrentEmitterPos(GetPos());

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevelUnder()->GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, game::EmitationHandler);
}

void lsquare::Noxify()
{
  ushort Radius = GetLevelUnder()->CalculateMinimumEmitationRadius(GetEmitation());

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterPos(GetPos());

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevelUnder()->GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, game::NoxifyHandler);
}

void lsquare::ForceEmitterNoxify()
{
  for(ushort c = 0; c < Emitter.size(); ++c)
    GetNearLSquare(Emitter[c].Pos)->Noxify();
}

void lsquare::ForceEmitterEmitation()
{
  for(ushort c = 0; c < Emitter.size(); ++c)
    GetNearLSquare(Emitter[c].Pos)->Emitate();
}

void lsquare::NoxifyEmitter(vector2d Dir)
{
  /*
   * DO NOT remove the iterator, or ForceEmitterEmitation() will not work properly!
   * (had to add add this comment when I came here for the nth time and thought not to do it is really stupid...)
   */

  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(!Luminance || game::CompareLights(i->DilatedEmitation, Luminance) < 0)
	  i->DilatedEmitation = 0;
	else if(!OLTerrain->IsWalkable())
	  {
	    i->DilatedEmitation = 0;
	    NewDrawRequested = true;

	    if(LastSeen == game::GetLOSTurns())
	      game::SendLOSUpdateRequest();
	  }
	else
	  {
	    ulong OldLuminance = Luminance;
	    i->DilatedEmitation = 0;
	    CalculateLuminance();

	    if(OldLuminance == Luminance)
	      return;

	    NewDrawRequested = true;

	    if(!Luminance)
	      {
		MemorizedUpdateRequested = true;
		DescriptionChanged = true;

		if(LastSeen == game::GetLOSTurns())
		  game::SendLOSUpdateRequest();
	      }
	  }

	return;
      }
}

uchar lsquare::CalculateBitMask(vector2d Dir) const
{
  uchar BitMask = 0;

  #define IW(X, Y) GetNearLSquare(Pos + vector2d(X, Y))->GetOLTerrain()->IsWalkable()

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

void lsquare::AlterLuminance(vector2d Dir, ulong NewLuminance)
{
  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(i->DilatedEmitation == NewLuminance)
	  return;

	if(!game::IsDark(NewLuminance))
	  {
	    if(NewLuminance != Luminance)
	      {
		if(!OLTerrain->IsWalkable())
		  {
		    i->DilatedEmitation = NewLuminance;
		    NewDrawRequested = true;

		    if(LastSeen == game::GetLOSTurns())
		      game::SendLOSUpdateRequest();

		    return;
		  }

		ulong OldLuminance = Luminance;
		i->DilatedEmitation = NewLuminance;
  		CalculateLuminance();

		if(OldLuminance == Luminance)
		  return;

		NewDrawRequested = true;

		if(!OldLuminance)
		  {
		    MemorizedUpdateRequested = true;
		    DescriptionChanged = true;

		    if(LastSeen == game::GetLOSTurns())
		      game::SendLOSUpdateRequest();
		  }
	      }
	    else
	      i->DilatedEmitation = NewLuminance;
	  }
	else
	  {
	    if(!Luminance || game::CompareLights(i->DilatedEmitation, Luminance) < 0)
	      Emitter.erase(i);
	    else if(!OLTerrain->IsWalkable())
	      {
		Emitter.erase(i);
		NewDrawRequested = true;

		if(LastSeen == game::GetLOSTurns())
		  game::SendLOSUpdateRequest();
	      }
	    else
	      {
		ulong OldLuminance = Luminance;
		Emitter.erase(i);
		CalculateLuminance();

		if(OldLuminance == Luminance)
		  return;

		NewDrawRequested = true;

		if(!Luminance)
		  {
		    MemorizedUpdateRequested = true;
		    DescriptionChanged = true;

		    if(LastSeen == game::GetLOSTurns())
		      game::SendLOSUpdateRequest();
		  }
	      }
	  }

	return;
      }

  /* Emitter was not found, add it. */

  if(!game::IsDark(NewLuminance))
    {
      if(game::CompareLights(NewLuminance, Luminance) > 0)
	{
	  if(!OLTerrain->IsWalkable())
	    {
	      Emitter.push_back(emitter(Dir, NewLuminance));
	      NewDrawRequested = true;

	      if(LastSeen == game::GetLOSTurns())
		game::SendLOSUpdateRequest();

	      return;
	    }

	  ulong OldLuminance = Luminance;
	  Emitter.push_back(emitter(Dir, NewLuminance));
  	  CalculateLuminance();

	  if(OldLuminance == Luminance)
	    return;

	  NewDrawRequested = true;

	  if(!OldLuminance)
	    {
	      DescriptionChanged = true;
	      MemorizedUpdateRequested = true;

	      if(LastSeen == game::GetLOSTurns())
		game::SendLOSUpdateRequest();
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
  Stack->Save(SaveFile); // This must be before square::Save! (Note: This comment is years old. It's probably obsolete)
  square::Save(SaveFile);
  SaveFile << GLTerrain << OLTerrain;

  for(ushort c = 0; c < 4; ++c)
    SideStack[c]->Save(SaveFile);

  SaveFile << Emitter << Fluid << Emitation << DivineMaster << Engraved << Room << Luminance;

  if(LastSeen)
    Memorized->Save(SaveFile);
}

void lsquare::Load(inputfile& SaveFile)
{
  Stack->Load(SaveFile); // This must be before square::Load! (Note: This comment is years old. It's probably obsolete)
  Stack->SetMotherSquare(this);
  square::Load(SaveFile);
  SaveFile >> GLTerrain >> OLTerrain;

  for(ushort c = 0; c < 4; ++c)
    {
      SideStack[c]->Load(SaveFile);
      SideStack[c]->SetMotherSquare(this);
    }

  SaveFile >> Emitter >> Fluid >> Emitation >> DivineMaster >> Engraved >> Room >> Luminance;

  if(LastSeen)
    {
      Memorized = new bitmap(16, 16);
      Memorized->Load(SaveFile);
    }
}

void lsquare::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation) // ho ho ho /me is very funny. - Anonymous
{
  if(!Amount)
    return;
	
  if(!GetFluid())
    SetFluid(new fluid(this));

  GetFluid()->SpillFluid(Amount, Color, Lumpiness, Variation);
  SendMemorizedUpdateRequest();
}

void lsquare::CalculateLuminance()
{
  Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

  if(OLTerrain->IsWalkable())
    {
      for(ushort c = 0; c < Emitter.size(); ++c)
	game::CombineLights(Luminance, Emitter[c].DilatedEmitation);
    }
  else
    for(ushort c = 0; c < Emitter.size(); ++c)
      if(CalculateBitMask(Emitter[c].Pos) & CalculateBitMask(game::GetPlayer()->GetPos()))
	game::CombineLights(Luminance, Emitter[c].DilatedEmitation);
}

ulong lsquare::GetRawLuminance() const
{
  ulong Luminance = *GetLevelUnder()->GetLevelScript()->GetAmbientLight();

  for(ushort c = 0; c < Emitter.size(); ++c)
    game::CombineLights(Luminance, Emitter[c].DilatedEmitation);

  return Luminance;
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
      if(!IsDark() || Cheat)
	{
	  MemorizedDescription.resize(0);

	  if(GetOTerrain()->IsWalkable())
	    {
	      bool Anything = false;

	      if(OLTerrain->GetNameSingular().length())
		{
		  OLTerrain->AddName(MemorizedDescription, INDEFINITE);
		  Anything = true;
		}

	      ushort VisibleItems = GetStack()->GetItems(game::GetPlayer(), Cheat);

	      if(VisibleItems)
		{
		  if(Anything)
		    MemorizedDescription << " and ";

		  if(VisibleItems == 1)
		    GetStack()->GetBottomItem(game::GetPlayer(), Cheat)->AddName(MemorizedDescription, INDEFINITE);
		  else
		    MemorizedDescription = "many items";

		  MemorizedDescription << " on ";
		  Anything = true;
		}
	      else if(Anything)
		MemorizedDescription << " on ";

	      GLTerrain->AddName(MemorizedDescription, INDEFINITE);
	      ushort Items = 0;

	      for(ushort c = 0; c < 4; ++c)
		{
		  stack* Stack = GetSideStackOfAdjacentSquare(c);

		  if(Stack)
		    Items += Stack->GetItems(game::GetPlayer(), Cheat);
		}

	      if(Items > 1)
		MemorizedDescription << " and many items on the wall";
	      else if(Items == 1)
		{
		  MemorizedDescription << " and ";

		  for(ushort c = 0; c < 4; ++c)
		    {
		      stack* Stack = GetSideStackOfAdjacentSquare(c);

		      if(Stack && Stack->GetItems(game::GetPlayer(), Cheat))
			Stack->GetBottomItem(game::GetPlayer(), Cheat)->AddName(MemorizedDescription, INDEFINITE);
		    }

		  MemorizedDescription << " on the wall";
		}
	    }
	  else
	    OLTerrain->AddName(MemorizedDescription, INDEFINITE);

	  if(Cheat)
	    MemorizedDescription << " (pos " << Pos.X << ":" << Pos.Y << ")";
	}
      else
	MemorizedDescription = "darkness";

      DescriptionChanged = false;
    }
}

void lsquare::BeKicked(character* Kicker, item* Boot, float KickDamage, float KickToHitValue, short Success, bool Critical)
{
  if(Room)
    GetLevelUnder()->GetRoom(Room)->KickSquare(Kicker, this);

  GetStack()->BeKicked(Kicker, ushort(KickDamage));

  if(GetCharacter())
    GetCharacter()->BeKicked(Kicker, Boot, KickDamage, KickToHitValue, Success, Critical);

  GetOLTerrain()->BeKicked(Kicker, ushort(KickDamage));
}

bool lsquare::CanBeDug() const
{
  if((GetPos().X == 0 || GetPos().Y == 0 || GetPos().X == GetLevelUnder()->GetXSize() - 1 || GetPos().Y == GetLevelUnder()->GetYSize() - 1) && !*GetLevelUnder()->GetLevelScript()->GetOnGround())
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
  NewGround->SetLSquareUnder(this);
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
  NewOver->SetLSquareUnder(this);
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
  NewGround->SetLSquareUnder(this);

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::SetOLTerrain(olterrain* NewOver)
{
  OLTerrain = NewOver;
  NewOver->SetLSquareUnder(this);

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
  if(SquareScript->GetAttachRequired(false) && *SquareScript->GetAttachRequired())
    GetLevelUnder()->AttachPos(GetPos());

  if(SquareScript->GetEntryIndex(false))
    GetLevelUnder()->SetEntryPos(*SquareScript->GetEntryIndex(), GetPos());

  if(SquareScript->GetCharacter(false))
    {
      character* Char = SquareScript->GetCharacter()->Instantiate();

      if(!Char->GetTeam())
	Char->SetTeam(game::GetTeam(*GetLevelUnder()->GetLevelScript()->GetTeamDefault()));

      AddCharacter(Char);

      if(Room)
	Room->HandleInstantiatedCharacter(Char);
    }

  if(SquareScript->GetItems(false))
    for(ushort c = 0; c < SquareScript->GetItems()->size(); ++c)
      {
	stack* Stack;
	item* Item = (*SquareScript->GetItems())[c].Instantiate();

	if(Item)
	  {
	    uchar* SideStackIndex = (*SquareScript->GetItems())[c].GetSideStackIndex(false);

	    if(!SideStackIndex)
	      Stack = GetStack();
	    else
	      {
		Item->SignalSquarePositionChange(*SideStackIndex);
		Stack = GetSideStack(*SideStackIndex);
	      }

	    Stack->AddItem(Item);
	  }
      }

  if(SquareScript->GetGTerrain(false))
    ChangeGLTerrain(SquareScript->GetGTerrain()->Instantiate());

  if(SquareScript->GetOTerrain(false))
    {
      olterrain* Terrain = SquareScript->GetOTerrain()->Instantiate();
      ChangeOLTerrain(Terrain);

      if(Room)
	Room->HandleInstantiatedOLTerrain(Terrain);
    }
}

bool lsquare::CanBeSeenByPlayer(bool IgnoreDarkness) const
{
  return (IgnoreDarkness || !IsDark()) && LastSeen == game::GetLOSTurns();
}

bool lsquare::CanBeSeenFrom(vector2d FromPos, ulong MaxDistance, bool IgnoreDarkness) const
{
  ulong Distance = (GetPos() - FromPos).Length();

  if(Distance > MaxDistance || (!IgnoreDarkness && IsDark()))
    return false;
  else
    {
      character* Char = GetCharacter();

      if(Char && Char->IsPlayer() && Distance < Char->LOSRangeSquare())
	return GetNearSquare(FromPos)->CanBeSeenByPlayer(true);
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
      ulong Emit = Movee->GetEmitation();
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

      To->StepOn(Movee, this);
    }
}

void lsquare::StepOn(character* Stepper, square* ComingFrom)
{
  if(Room && ((lsquare*)ComingFrom)->GetRoom() != Room)
    GetLevelUnder()->GetRoom(Room)->Enter(Stepper);

  GetGLTerrain()->StepOn(Stepper);
  GetOLTerrain()->StepOn(Stepper);

  if(!Stepper->CanFly())
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
	ulong EmitOne = MoveeOne->GetEmitation();
	ulong EmitTwo = MoveeTwo->GetEmitation();
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

	With->StepOn(MoveeOne, this);
	StepOn(MoveeTwo, With);
      }
  else
    if(With->Character)
      With->MoveCharacter(this);
}

void lsquare::ReceiveVomit(character* Who, ushort Amount)
{
  SpillFluid(Amount, MakeRGB16(10, 230, 10), 5, 60);
  GetOLTerrain()->ReceiveVomit(Who);
}

room* lsquare::GetRoomClass() const
{
  if(Room)
    return GetLevelUnder()->GetRoom(Room);
  else
    return 0;
}

void lsquare::SetTemporaryEmitation(ulong What)
{
  ulong Old = TemporaryEmitation;
  TemporaryEmitation = 0;
  SignalEmitationDecrease(Old);
  TemporaryEmitation = What;
  SignalEmitationIncrease(What);
}

void lsquare::ChangeOLTerrainAndUpdateLights(olterrain* NewTerrain)
{
  bool WasWalkable = GetOLTerrain()->IsWalkable();

  if(WasWalkable && !NewTerrain->IsWalkable())
    ForceEmitterNoxify();

  ulong OldEmit = GetOLTerrain()->GetEmitation();
  ChangeOLTerrain(NewTerrain);
  SignalEmitationIncrease(NewTerrain->GetEmitation());
  SignalEmitationDecrease(OldEmit);

  for(ushort c = 0; c < 4; ++c)
    {
      for(stackiterator i = GetSideStack(c)->GetBottom(); i.HasItem(); ++i)
	i->SignalSquarePositionChange(CENTER);

      GetSideStack(c)->MoveItemsTo(GetStack());
    }

  if(WasWalkable != GetOLTerrain()->IsWalkable())
    {
      ForceEmitterEmitation();
      CalculateLuminance();

      if(LastSeen == game::GetLOSTurns())
	game::SendLOSUpdateRequest();
    }
}

void lsquare::PolymorphEverything(character* Zapper)
{
  GetStack()->Polymorph();
  GetOLTerrain()->Polymorph(Zapper);
  character* Character = GetCharacter();

  if(Character)
    {
      if(Character != Zapper && Character->GetTeam() != Zapper->GetTeam())
	Zapper->Hostility(Character);

      Character->PolymorphRandomly(5000 + RAND() % 5000);
    }
}

void lsquare::DrawParticles(ushort Color, uchar)
{
  if(GetPos().X < game::GetCamera().X || GetPos().Y < game::GetCamera().Y || GetPos().X >= game::GetCamera().X + game::GetScreenSize().X || GetPos().Y >= game::GetCamera().Y + game::GetScreenSize().Y)
    return;

  clock_t StartTime = clock();
  vector2d BitPos = game::CalculateScreenCoordinates(GetPos());
  game::DrawEverythingNoBlit();

  for(ushort c = 0; c < 10; ++c)
    DOUBLE_BUFFER->PutPixel(BitPos + vector2d(1 + RAND() % 14, 1 + RAND() % 14), Color);

  graphics::BlitDBToScreen();
  NewDrawRequested = true; // Clean the pixels from the screen afterwards

  if(CanBeSeenByPlayer())
    while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
}

void lsquare::StrikeEverything(character* Zapper, const std::string& DeathMsg, ushort Damage, uchar Direction)
{
  GetStack()->ReceiveDamage(Zapper, Damage, ENERGY);

  switch(Direction)
    {
    case 0:
      GetSideStack(DOWN)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(RIGHT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 1:
      GetSideStack(DOWN)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 2:
      GetSideStack(DOWN)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(LEFT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 3:
      GetSideStack(RIGHT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 4:
      GetSideStack(LEFT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 5:
      GetSideStack(UP)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(RIGHT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 6:
      GetSideStack(UP)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    case 7:
      GetSideStack(UP)->ReceiveDamage(Zapper, Damage, ENERGY);
      GetSideStack(LEFT)->ReceiveDamage(Zapper, Damage, ENERGY);
      break;
    }

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("You are hit by a burst of energy!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is hit by a burst of energy!", Char->CHAR_NAME(DEFINITE));

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
    return true;
  else
    {
      if(Dipper->IsPlayer())
	ADD_MESSAGE("You cannot dip %s on that square!", Thingy->CHAR_NAME(DEFINITE));

      return false;
    }
}

bool lsquare::LockEverything(character*)
{
  if(!GetOLTerrain()->IsLocked())
    {
      GetOLTerrain()->Lock();
      return true;
    }
  else
    return false;
}

bool lsquare::RaiseTheDead(character* Summoner)
{
  if(GetCharacter())
    return GetCharacter()->RaiseTheDead(Summoner);
  else if(GetStack()->RaiseTheDead(Summoner))
    return true;
  else
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

void lsquare::SetLastSeen(ulong What)
{
  if(LastSeen == What)
    return;

  if(LastSeen < What - 1)
    NewDrawRequested = true;

  if(!OLTerrain->IsWalkable())
    {
      ulong OldLuminance = Luminance;
      CalculateLuminance();

      if(OldLuminance != Luminance)
	{
	  NewDrawRequested = true;

	  if(IsDark() != game::IsDark(OldLuminance))
	    {
	      MemorizedUpdateRequested = true;
	      DescriptionChanged = true;
	    }
	}
    }

  if(IsDark())
    {
      short XDist = Pos.X - game::GetPlayer()->GetPos().X;

      if(XDist < -1 || XDist > 1)
	return;

      short YDist = Pos.Y - game::GetPlayer()->GetPos().Y;

      if(YDist < -1 || YDist > 1)
	return;
    }

  if(!LastSeen)
    Memorized = new bitmap(16, 16);

  LastSeen = What;
  UpdateMemorized();
  UpdateMemorizedDescription();
}

void lsquare::DrawMemorized()
{
  if(NewDrawRequested || LastSeen == game::GetLOSTurns() - 1)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(LastSeen)
	Memorized->Blit(DOUBLE_BUFFER, 0, 0, BitPos, 16, 16, configuration::GetContrastLuminance());
      else
	DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

      if(Character && Character->CanBeSeenByPlayer())
	Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), LastSeen != 0, true);

      NewDrawRequested = false;
    }
}

bool lsquare::IsDangerousForAIToStepOn(const character* Who) const
{
  return !Who->CanFly() && GetStack()->IsDangerousForAIToStepOn(Who); 
} 

/* returns true if clones something */
bool lsquare::CloneEverything(character* Cloner)
{
  bool ClonedSomething = false;
  character* Character = GetCharacter();

  if(Character)
    ClonedSomething = Character->CloneToNearestSquare(Cloner) != 0;

  if(GetStack()->Clone(5))
    ClonedSomething = true;

  return ClonedSomething;
}

stack* lsquare::GetSideStackOfAdjacentSquare(ushort Index) const
{
  switch(Index)
    {
    case LEFT: return Pos.X ? GetNearLSquare(Pos + vector2d(-1, 0))->GetSideStack(RIGHT) : 0;
    case RIGHT: return Pos.X < GetLevelUnder()->GetXSize() - 1 ? GetNearLSquare(Pos + vector2d(1, 0))->GetSideStack(LEFT) : 0;
    case UP: return Pos.Y ? GetNearLSquare(Pos + vector2d(0, -1))->GetSideStack(DOWN) : 0;
    case DOWN: return Pos.Y < GetLevelUnder()->GetYSize() - 1 ? GetNearLSquare(Pos + vector2d(0, 1))->GetSideStack(UP) : 0;
    default: return 0;
    }
}

void lsquare::SendMemorizedUpdateRequest()
{
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;

  if(CanBeSeenByPlayer())
    {
      UpdateMemorized();
      UpdateMemorizedDescription();
    }
}

void lsquare::KickAnyoneStandingHereAway()
{
  if(Character)
    {
      vector2d Pos = GetLevelUnder()->GetNearestFreeSquare(Character, GetPos());

      if(Pos == DIR_ERROR_VECTOR)
	Pos = GetLevelUnder()->GetRandomSquare(Character);

      GetLevelUnder()->AddCharacter(Pos, Character);
      RemoveCharacter();
    }
}

outputfile& operator<<(outputfile& SaveFile, const emitter& Emitter)
{
  SaveFile << Emitter.Pos << Emitter.DilatedEmitation;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, emitter& Emitter)
{
  SaveFile >> Emitter.Pos >> Emitter.DilatedEmitation;
  return SaveFile;
}

void lsquare::SpillFluid(material* Liquid, ushort HitPercent, character* Spiller)
{
  SpillFluid(1, Liquid->GetColor(), 5, 60);
  character* CharHere = GetCharacter();
  if(CharHere && (RAND() % 100) > 99 - short(HitPercent))
    {
      if(Spiller)
	Spiller->Hostility(CharHere);
      CharHere->ReceiveFluidSpill(Liquid, HitPercent);
    }
  GetStack()->ReceiveFluidSpill(Liquid);

  delete Liquid;
}

bool lsquare::IsDark() const
{
  return !Luminance || (GetRed24(Luminance) < LIGHT_BORDER && GetGreen24(Luminance) < LIGHT_BORDER && GetBlue24(Luminance) < LIGHT_BORDER);
}
