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
}

void wsquare::Load(inputfile& SaveFile)
{
  square::Load(SaveFile);
  SaveFile >> GWTerrain >> OWTerrain;
  CalculateLuminance();
}

void wsquare::Draw()
{
  if(NewDrawRequested || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);
      ulong RealLuminance = ivanconfig::ApplyContrastTo(Luminance);
      GWTerrain->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);

      if(OWTerrain)
	OWTerrain->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);

      if(Character && Character->CanBeSeenByPlayer())
	Character->Draw(DOUBLE_BUFFER, BitPos, ivanconfig::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);

      NewDrawRequested = false;
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
  DescriptionChanged = NewDrawRequested = true;
}

void wsquare::ChangeOWTerrain(owterrain* NewOver)
{
  if(OWTerrain && OWTerrain->IsAnimated())
    DecAnimatedEntities();

  delete OWTerrain;
  SetOWTerrain(NewOver);
  DescriptionChanged = NewDrawRequested = true;
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
  if(DescriptionChanged || Cheat)
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

      DescriptionChanged = false;
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

void wsquare::SetLastSeen(ulong What)
{
  UpdateMemorizedDescription();
  LastSeen = What;
}

void wsquare::CalculateLuminance()
{
  uchar Element = Min((128 - ushort(37.5f * log(1.0f + fabs(GetWorldMap()->GetAltitude(Pos)) / 500.0f))), 255);
  Luminance = MakeRGB24(Element, Element, Element);
}

uchar wsquare::GetWalkability() const 
{ 
  return OWTerrain ? OWTerrain->GetWalkability() & GWTerrain->GetWalkability() : GWTerrain->GetWalkability(); 
}
