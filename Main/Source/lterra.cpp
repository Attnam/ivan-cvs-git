/* Compiled through levelset.cpp */

glterrainprototype::glterrainprototype(glterrainprototype* Base, glterrain* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<glterrain>::Add(this); }
olterrainprototype::olterrainprototype(olterrainprototype* Base, olterrain* (*Cloner)(ushort, ushort), const std::string& ClassId) : Base(Base), Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<olterrain>::Add(this); }
const glterraindatabase& glterrainprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }
const olterraindatabase& olterrainprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }

square* lterrain::GetSquareUnderEntity() const { return LSquareUnder; }
level* lterrain::GetLevel() const { return LSquareUnder->GetLevel(); }
lsquare* lterrain::GetNearLSquare(vector2d Pos) const { return LSquareUnder->GetLevel()->GetLSquare(Pos); }
lsquare* lterrain::GetNearLSquare(ushort x, ushort y) const { return LSquareUnder->GetLevel()->GetLSquare(x, y); }
room* lterrain::GetRoom() const { return GetLSquareUnder()->GetRoom(); }

void glterrain::InstallDataBase() { ::database<glterrain>::InstallDataBase(this); }
void olterrain::InstallDataBase() { ::database<olterrain>::InstallDataBase(this); }
uchar glterrain::GetGraphicsContainerIndex() const { return GR_GLTERRAIN; }
uchar olterrain::GetGraphicsContainerIndex() const { return GR_OLTERRAIN; }

std::string olterrain::GetText() const { return ""; }

void lterrain::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  object::Load(SaveFile);
}

void glterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  lterrain::Save(SaveFile);
}

void glterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  InstallDataBase();
}

void olterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  lterrain::Save(SaveFile);
  SaveFile << HP;
}

void olterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  InstallDataBase();
  SaveFile >> HP;
}

vector2d lterrain::GetPos() const
{
  return LSquareUnder->GetPos();
}

bool lterrain::SitOn(character* Sitter)
{
  if(GetSitMessage().length())
    {
      ADD_MESSAGE("%s", GetSitMessage().c_str());
      Sitter->EditAP(-1000);
      return true;
    }
  else
    return false;
}

void olterrain::Break()
{
  GetLSquareUnder()->ChangeOLTerrainAndUpdateLights(0);
}

glterrain* glterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  glterrain* Terrain = Cloner(0, LOAD);
  Terrain->Load(SaveFile);
  Terrain->CalculateAll();
  return Terrain;
}

olterrain* olterrainprototype::CloneAndLoad(inputfile& SaveFile) const
{
  olterrain* Terrain = Cloner(0, LOAD);
  Terrain->Load(SaveFile);
  Terrain->CalculateAll();
  return Terrain;
}

void lterrain::Initialize(ushort NewConfig, ushort SpecialFlags)
{
  if(!(SpecialFlags & LOAD))
    {
      Config = NewConfig;
      InstallDataBase();
      RandomizeVisualEffects();

      if(!(SpecialFlags & NO_MATERIALS))
	GenerateMaterials();
    }

  VirtualConstructor(SpecialFlags & LOAD);

  if(!(SpecialFlags & LOAD))
    {
      if(!(SpecialFlags & NO_MATERIALS))
	{
	  CalculateAll();

	  if(!(SpecialFlags & NO_PIC_UPDATE))
	    UpdatePictures();
	}
    }
}

bool lterrain::CanBeSeenByPlayer() const
{
  return LSquareUnder->CanBeSeenByPlayer();
}

bool lterrain::CanBeSeenBy(character* Who) const
{
  if(Who->IsPlayer())
    return CanBeSeenByPlayer();
  else
    return LSquareUnder->CanBeSeenFrom(Who->GetPos(), Who->GetLOSRangeSquare());
}

void olterrain::ShowRestMessage(character*) const
{
  if(GetRestMessage().length())
    ADD_MESSAGE("%s", GetRestMessage().c_str());
}

void lterrain::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(LSquareUnder)
	LSquareUnder->SignalEmitationIncrease(EmitationUpdate);
    }
}

void lterrain::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation && LSquareUnder)
	LSquareUnder->SignalEmitationDecrease(EmitationUpdate);
    }
}

bool olterrain::Enter(bool DirectionUp) const
{
  if(DirectionUp)
    ADD_MESSAGE("You can't go up.");
  else
    ADD_MESSAGE("You can't go down.");

  return false;
}

void olterrain::VirtualConstructor(bool Load)
{
  if(!Load)
    CalculateHP();
}

ushort olterrain::GetStrengthValue() const
{
  return GetMainMaterial()->GetStrengthValue() / 20;
}

void olterrain::ReceiveDamage(character* Villain, ushort What, uchar)
{
  if(CanBeDestroyed() && What > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - What);

      if(HP <= 0)
	{
	  room* Room = GetLSquareUnder()->GetRoom();

	  if(Room)
	    Room->DestroyTerrain(Villain, this);

	  Break();
	}
    }
}

void olterrain::CalculateHP()
{
  if(GetMainMaterial())
    {
      ulong SV = GetMainMaterial()->GetStrengthValue();
      HP = SV * SV * GetHPModifier() / 5000;
    }
}

uchar glterrain::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

uchar olterrain::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

void terraindatabase::InitDefaults(ushort Config)
{
  IsAbstract = false;

  /* TERRIBLE gum solution! */

  if(Config & DEVOUT)
    PostFix.append("of " + festring::CapitalizeCopy(protocontainer<god>::GetProto(Config&0xFF)->GetClassId()));
}
