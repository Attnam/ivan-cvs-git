/* Compiled through areaset.cpp */

square::square(area* AreaUnder, vector2d Pos) : AreaUnder(AreaUnder), Character(0), Pos(Pos), NewDrawRequested(true), LastSeen(0), AnimatedEntities(0), DescriptionChanged(true) { }

square::~square()
{
  delete Character;
}

void square::Save(outputfile& SaveFile) const
{
  SaveFile << Character << LastSeen << AnimatedEntities << MemorizedDescription;
}

void square::Load(inputfile& SaveFile)
{
  SaveFile >> Character >> LastSeen >> AnimatedEntities >> MemorizedDescription;
}

void square::AddCharacter(character* Guy)
{
  Character = Guy;
  Guy->SetSquareUnder(this);
  NewDrawRequested = true;
  IncAnimatedEntities();
}

void square::RemoveCharacter()
{
  if(Character)
    DecAnimatedEntities();

  SetCharacter(0);
  NewDrawRequested = true;
}

bool square::CanBeSeenByPlayer(bool) const
{
  return GetLastSeen() == game::GetLOSTurns();
}

bool square::CanBeSeenFrom(vector2d FromPos, ulong MaxDistance, bool) const
{
  ulong Distance = (GetPos() - FromPos).GetLengthSquare();

  if(Distance > MaxDistance)
    return false;
  else
    {
      character* Char = GetCharacter();

      if(Char && Char->IsPlayer() && Distance < Char->GetLOSRangeSquare())
	return GetNearSquare(FromPos)->CanBeSeenByPlayer(true);
      else
	return femath::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, game::EyeHandler);
    }
}

const char* square::SurviveMessage(character* Char) const
{
  if(GetOTerrain() && !Char->CanMoveOn(GetOTerrain()))
    return GetOTerrain()->SurviveMessage();
  else
    return GetGTerrain()->SurviveMessage();
}

const char* square::MonsterSurviveMessage(character* Char) const
{
  if(GetOTerrain() && !Char->CanMoveOn(GetOTerrain()))
    return GetOTerrain()->MonsterSurviveMessage();
  else
    return GetGTerrain()->MonsterSurviveMessage();
}

const char* square::DeathMessage(character* Char) const
{
  if(GetOTerrain() && !Char->CanMoveOn(GetOTerrain()))
    return GetOTerrain()->DeathMessage();
  else
    return GetGTerrain()->DeathMessage();
}

const char* square::MonsterDeathVerb(character* Char) const
{
  if(GetOTerrain() && !Char->CanMoveOn(GetOTerrain()))
    return GetOTerrain()->MonsterDeathVerb();
  else
    return GetGTerrain()->MonsterDeathVerb();
}

const char* square::ScoreEntry(character* Char) const
{
  if(GetOTerrain() && !Char->CanMoveOn(GetOTerrain()))
    return GetOTerrain()->ScoreEntry();
  else
    return GetGTerrain()->ScoreEntry();
}

bool square::IsFatalToStay() const
{
  return GetGTerrain()->IsFatalToStay() || (GetOTerrain() && GetOTerrain()->IsFatalToStay());
}

uchar square::GetEntryDifficulty() const
{
  return GetGTerrain()->GetEntryDifficulty();
}

uchar square::GetRestModifier() const
{
  return GetOTerrain() ? GetOTerrain()->GetRestModifier() : 1;
}

bool square::CanBeSeenBy(const character* Who, bool IgnoreDarkness) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer(IgnoreDarkness);
  else
    return CanBeSeenFrom(Who->GetPos(), Who->GetLOSRangeSquare(), IgnoreDarkness);
}
