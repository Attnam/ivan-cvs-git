/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through wmapset.cpp */


wsquare::wsquare(worldmap* WorldMapUnder, vector2d Pos) : square(WorldMapUnder, Pos), GWTerrain(0), OWTerrain(0) { }

wsquare::~wsquare()
{
  delete GWTerrain;
  delete OWTerrain;
}

void wsquare::Save(outputfile& SaveFile) const
{
  square::Save(SaveFile);
  SaveFile << GWTerrain << OWTerrain;
  SaveFile << bool(!!LastSeen);
}

void wsquare::Load(inputfile& SaveFile)
{
  square::Load(SaveFile);
  SaveFile >> GWTerrain >> OWTerrain;
  LastSeen = ReadType<bool>(SaveFile) ? 1 : 0;
  CalculateLuminance();
}

void wsquare::Draw()
{
  if(Flags & NEW_DRAW_REQUEST || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);
      long RealLuminance = ivanconfig::ApplyContrastTo(Luminance);
      GWTerrain->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);

      if(OWTerrain)
	OWTerrain->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);

      if(Character && Character->CanBeSeenByPlayer())
	Character->Draw(DOUBLE_BUFFER, BitPos, ivanconfig::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);

      Flags &= ~STRONG_NEW_DRAW_REQUEST;
    }
}

void wsquare::ChangeWTerrain(gwterrain* NewGround, owterrain* NewOver)
{
  ChangeGWTerrain(NewGround);
  ChangeOWTerrain(NewOver);
}

void wsquare::ChangeGWTerrain(gwterrain* NewGround)
{
  if(GWTerrain->IsAnimated())
    DecAnimatedEntities();

  delete GWTerrain;
  SetGWTerrain(NewGround);
  Flags |= DESCRIPTION_CHANGE|NEW_DRAW_REQUEST;
}

void wsquare::ChangeOWTerrain(owterrain* NewOver)
{
  if(OWTerrain && OWTerrain->IsAnimated())
    DecAnimatedEntities();

  delete OWTerrain;
  SetOWTerrain(NewOver);
  Flags |= DESCRIPTION_CHANGE|NEW_DRAW_REQUEST;
}

void wsquare::SetWTerrain(gwterrain* NewGround, owterrain* NewOver)
{
  SetGWTerrain(NewGround);
  SetOWTerrain(NewOver);
}

void wsquare::SetGWTerrain(gwterrain* What)
{
  GWTerrain = What;

  if(What)
    {
      What->SetWSquareUnder(this);

      if(What->IsAnimated())
	IncAnimatedEntities();
    }
}

void wsquare::SetOWTerrain(owterrain* What)
{
  OWTerrain = What;

  if(What)
    {
      What->SetWSquareUnder(this);

      if(What->IsAnimated())
	IncAnimatedEntities();
    }
}

void wsquare::UpdateMemorizedDescription(bool Cheat)
{
  if(Flags & DESCRIPTION_CHANGE || Cheat)
    {
      MemorizedDescription.Empty();

      if(OWTerrain)
	{
	  OWTerrain->AddName(MemorizedDescription, INDEFINITE);
	  MemorizedDescription << " surrounded by ";
	  GWTerrain->AddName(MemorizedDescription, UNARTICLED);
	}
      else
	GWTerrain->AddName(MemorizedDescription, UNARTICLED);

      if(Cheat)
	{
	  festring Continent;

	  if(GetWorldMap()->GetContinentUnder(Pos))
	    Continent << ", continent " << GetWorldMap()->GetContinentUnder(Pos)->GetName();

	  MemorizedDescription << " (pos " << Pos.X << ':' << Pos.Y << Continent << ", height " << GetWorldMap()->GetAltitude(Pos) << " m)";
	}

      Flags &= ~DESCRIPTION_CHANGE;
    }
}

gterrain* wsquare::GetGTerrain() const
{
  return GWTerrain;
}

oterrain* wsquare::GetOTerrain() const
{
  return OWTerrain;
}

bool wsquare::SignalSeen()
{
  UpdateMemorizedDescription();
  LastSeen = 1;
  return true;
}

void wsquare::CalculateLuminance()
{
  int Element = Min((128 - int(37.5 * log(1.0 + fabs(GetWorldMap()->GetAltitude(Pos)) / 500.0))), 255);
  Luminance = MakeRGB24(Element, Element, Element);
}

int wsquare::GetWalkability() const 
{ 
  return OWTerrain ? OWTerrain->GetWalkability() & GWTerrain->GetWalkability() : GWTerrain->GetWalkability(); 
}

bool wsquare::CanBeSeenByPlayer(bool) const
{
  return !!LastSeen;
}

bool wsquare::CanBeSeenFrom(vector2d FromPos, long MaxDistance, bool) const
{
  return (Pos - FromPos).GetLengthSquare() <= MaxDistance;
}
