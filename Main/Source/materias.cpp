/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

/* Compiled through materset.cpp */

void organic::ResetSpoiling() { SpoilCounter = SpoilLevel = 0; }

const char* liquid::GetConsumeVerb() const { return "drinking"; }

bool powder::IsExplosive() const { return !Wetness && material::IsExplosive(); }

bool ironalloy::IsSparkling() const { return material::IsSparkling() && GetRustLevel() == NOT_RUSTED; }

void organic::Be()
{
  if(SpoilCheckCounter++ >= 50)
    {
      if(MotherEntity->AllowSpoil())
	{
	  if((SpoilCounter += 50) < GetSpoilModifier())
	    {
	      if(SpoilCounter << 1 >= GetSpoilModifier())
		{
		  int NewSpoilLevel = ((SpoilCounter << 4) / GetSpoilModifier()) - 7;

		  if(NewSpoilLevel != SpoilLevel)
		    {
		      SpoilLevel = NewSpoilLevel;
		      MotherEntity->SignalSpoilLevelChange(this);
		    }
		}
	    }
	  else
	    {
	      SpoilLevel = 8;
	      MotherEntity->SignalSpoil(this);
	    }
	}

      SpoilCheckCounter = 0;
    }
}

void organic::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << SpoilCounter << SpoilCheckCounter << SpoilLevel;
}

void organic::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> SpoilCounter >> SpoilCheckCounter >> SpoilLevel;
}

void organic::VirtualConstructor(bool Load)
{
  if(!Load)
    {
      SpoilLevel = SpoilCheckCounter = 0;
      SpoilCounter = (RAND() % GetSpoilModifier()) >> 5;
    }
}

void flesh::VirtualConstructor(bool Load)
{
  organic::VirtualConstructor(Load);

  if(!Load)
    {
      SkinColorSparkling = InfectedByLeprosy = false;
      SkinColor = GetColor();
    }
}

void flesh::Save(outputfile& SaveFile) const
{
  organic::Save(SaveFile);
  SaveFile << SkinColor << SkinColorSparkling << InfectedByLeprosy;
}

void flesh::Load(inputfile& SaveFile)
{
  organic::Load(SaveFile);
  SaveFile >> SkinColor >> SkinColorSparkling >> InfectedByLeprosy;
}

void powder::Be()
{
  if(Wetness > 0)
    --Wetness;
}

void powder::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << Wetness;
}

void powder::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> Wetness;
}

material* organic::EatEffect(character* Eater, long Amount)
{
  Amount = Volume > Amount ? Amount : Volume;
  GetMotherEntity()->SpecialEatEffect(Eater, Amount);
  Effect(Eater, Amount);
  Eater->ReceiveNutrition(GetNutritionValue() * Amount * 20 / (1000 * (GetSpoilLevel() + 1)));

  if(IsInfectedByLeprosy() && Amount && !RAND_N(25000 / Amount))
    Eater->GainIntrinsic(LEPROSY);

  if(GetSpoilLevel() > 0)
    {
      Eater->BeginTemporaryState(CONFUSED, int(Amount * GetSpoilLevel() * sqrt(GetNutritionValue()) / 1000));

      if(CanHaveParasite() && !(RAND() % (500 / GetSpoilLevel())))
	Eater->GainIntrinsic(PARASITIZED);
    }

  if(GetSpoilLevel() > 4)
    Eater->BeginTemporaryState(POISONED, int(Amount * (GetSpoilLevel() - 4) * sqrt(GetNutritionValue()) / 1000));

  if(Volume != Amount)
    {
      EditVolume(-Amount);
      return 0;
    }
  else
    return MotherEntity->RemoveMaterial(this);
}

void organic::AddConsumeEndMessage(character* Eater) const
{
  if(Eater->IsPlayer())
    {
      if(GetSpoilLevel() > 0 && GetSpoilLevel() <= 4)
	ADD_MESSAGE("Ugh. This stuff was slightly spoiled.");
      else if(GetSpoilLevel() > 4)
	ADD_MESSAGE("Ugh. This stuff was terribly spoiled!");
    }

  material::AddConsumeEndMessage(Eater);
}

void organic::SetSpoilCounter(int What)
{
  SpoilCounter = What;

  if(SpoilCounter < GetSpoilModifier())
    {
      if(SpoilCounter << 1 >= GetSpoilModifier())
	{
	  int NewSpoilLevel = ((SpoilCounter << 4) / GetSpoilModifier()) - 7;

	  if(NewSpoilLevel != SpoilLevel)
	    {
	      SpoilLevel = NewSpoilLevel;
	      MotherEntity->SignalSpoilLevelChange(this);
	    }
	}
    }
  else
    MotherEntity->SignalSpoil(this);
}

void ironalloy::SetRustLevel(int What)
{
  if(GetRustLevel() != What)
    {
      if(!RustData)
	RustData = RAND() & 0xFC | What;
      else if(!What)
	RustData = 0;
      else
	RustData = RustData & 0xFC | What;

      if(MotherEntity)
	MotherEntity->SignalRustLevelChange();
    }
}

int ironalloy::GetStrengthValue() const
{
  int Base = material::GetStrengthValue();

  switch(GetRustLevel())
    {
    case NOT_RUSTED: return Base;
    case SLIGHTLY_RUSTED: return ((Base << 3) + Base) / 10;
    case RUSTED: return ((Base << 1) + Base) >> 2;
    case VERY_RUSTED: return Base >> 1;
    }

  return 0; /* not possible */
}

/*void ironalloy::AddName(festring& Name, bool Articled, bool Adjective) const
{
  if(Articled)
    if(GetRustLevel() == NOT_RUSTED)
      Name << GetArticle() << ' ';
    else
      Name << "a ";

  switch(GetRustLevel())
    {
    case SLIGHTLY_RUSTED:
      Name << "sligthly rusted ";
      break;
    case RUSTED:
      Name << "rusted ";
      break;
    case VERY_RUSTED:
      Name << "very rusted ";
      break;
    }

  Name << (Adjective ? GetAdjectiveStem() : GetNameStem());
}*/

bool ironalloy::AddRustLevelDescription(festring& Name, bool Articled) const
{
  if(GetRustLevel() == NOT_RUSTED)
    return false;

  if(Articled)
    Name << "a ";

  switch(GetRustLevel())
    {
    case SLIGHTLY_RUSTED:
      Name << "sligthly rusted ";
      break;
    case RUSTED:
      Name << "rusted ";
      break;
    case VERY_RUSTED:
      Name << "very rusted ";
      break;
    }

  return true;
}

void ironalloy::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << RustData;
}

void ironalloy::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> RustData;
}

void liquid::TouchEffect(item* Item, const festring& LocationName)
{
  if(GetRustModifier())
    Item->TryToRust(GetRustModifier() * GetVolume());

  if(GetAcidicity())
    Item->ReceiveAcid(this, LocationName, Volume * GetAcidicity());
}

void liquid::TouchEffect(lterrain* Terrain)
{
  if(GetRustModifier())
    Terrain->TryToRust(GetRustModifier() * GetVolume());

  if(GetAcidicity())
    Terrain->ReceiveAcid(this, Volume * GetAcidicity());
}

void liquid::TouchEffect(character* Char, int BodyPartIndex)
{
  if(GetRustModifier())
    Char->GetBodyPart(BodyPartIndex)->TryToRust(GetRustModifier() * GetVolume());

  if(GetAcidicity())
    Char->GetBodyPart(BodyPartIndex)->ReceiveAcid(this, CONST_S(""), Volume * GetAcidicity() >> 1);
}

/* Doesn't do the actual rusting, just returns whether it should happen */

bool ironalloy::TryToRust(long Modifier)
{
  if(GetRustLevel() != VERY_RUSTED)
    {
      long Chance = long(5000000. * sqrt(GetVolume()) / (Modifier * GetRustModifier()));

      if(Chance <= 1 || !(RAND() % Chance))
	return true;
    }

  return false;
}

