/* Compiled through levelset.cpp */

glterrainprototype::glterrainprototype(glterrainprototype* Base, glterrain* (*Cloner)(ushort, ushort), const char* ClassID) : Base(Base), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<glterrain>::Add(this); }
olterrainprototype::olterrainprototype(olterrainprototype* Base, olterrain* (*Cloner)(ushort, ushort), const char* ClassID) : Base(Base), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<olterrain>::Add(this); }
const glterraindatabase& glterrainprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }
const olterraindatabase& olterrainprototype::ChooseBaseForConfig(ushort) { return Config.begin()->second; }

square* lterrain::GetSquareUnderEntity(ushort) const { return LSquareUnder; }
level* lterrain::GetLevel() const { return LSquareUnder->GetLevel(); }
lsquare* lterrain::GetNearLSquare(vector2d Pos) const { return LSquareUnder->GetLevel()->GetLSquare(Pos); }
lsquare* lterrain::GetNearLSquare(ushort x, ushort y) const { return LSquareUnder->GetLevel()->GetLSquare(x, y); }
room* lterrain::GetRoom() const { return GetLSquareUnder()->GetRoom(); }

void glterrain::InstallDataBase(ushort Config) { databasecreator<glterrain>::InstallDataBase(this, Config); }
void olterrain::InstallDataBase(ushort Config) { databasecreator<olterrain>::InstallDataBase(this, Config); }
uchar glterrain::GetGraphicsContainerIndex() const { return GR_GLTERRAIN; }
uchar olterrain::GetGraphicsContainerIndex() const { return GR_OLTERRAIN; }

bool olterraindatabase::AllowRandomInstantiation() const { return !(Config & S_LOCK_ID); }

festring olterrain::GetText() const { return ""; }

void lterrain::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  object::Load(SaveFile);
}

void glterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  lterrain::Save(SaveFile);
  SaveFile << GetConfig();
}

void glterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  InstallDataBase(ReadType<ushort>(SaveFile));
}

void olterrain::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  lterrain::Save(SaveFile);
  SaveFile << GetConfig();
  SaveFile << HP;
}

void olterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  InstallDataBase(ReadType<ushort>(SaveFile));
  SaveFile >> HP;
}

vector2d lterrain::GetPos() const
{
  return LSquareUnder->GetPos();
}

bool lterrain::SitOn(character* Sitter)
{
  if(GetSitMessage().GetSize())
    {
      ADD_MESSAGE("%s", GetSitMessage().CStr());
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
      InstallDataBase(NewConfig);
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
  if(GetRestMessage().GetSize())
    ADD_MESSAGE("%s", GetRestMessage().CStr());
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
    HP = CalculateMaxHP();
}

ushort olterrain::GetStrengthValue() const
{
  return GetMainMaterial()->GetStrengthValue() / 20;
}

void olterrain::ReceiveDamage(character* Villain, ushort Damage, ushort)
{
  if(CanBeDestroyed() && Damage > GetStrengthValue())
    {
      EditHP(GetStrengthValue() - Damage);

      if(HP <= 0)
	{
	  room* Room = GetRoom();
	  Break();

	  if(Room)
	    Room->DestroyTerrain(Villain);
	}
    }
}

void olterrain::BeKicked(character* Kicker, ushort Damage, uchar)
{
  if(CanBeDestroyed() && Damage > GetMainMaterial()->GetStrengthValue() >> 1)
    {
      EditHP((GetMainMaterial()->GetStrengthValue() >> 1) - Damage);

      if(HP <= 0)
	{
	  room* Room = GetRoom();

	  if(CanBeSeenByPlayer())
	    ADD_MESSAGE("%s is shattered.", CHAR_NAME(DEFINITE));

	  Break();

	  if(Room)
	    Room->DestroyTerrain(Kicker);
	}
    }
  else if(Kicker->IsPlayer())
    ADD_MESSAGE("Your kick has no effect on %s.", CHAR_NAME(DEFINITE));
}

short olterrain::CalculateMaxHP()
{
  if(GetMainMaterial())
    {
      ulong SV = GetMainMaterial()->GetStrengthValue();
      return SV * SV * GetHPModifier() / 6000;
    }
  else
    return 0;
}

uchar glterrain::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

uchar olterrain::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

void olterraindatabase::InitDefaults(ushort NewConfig)
{
  IsAbstract = false;
  Config = NewConfig&~DEVOUT;

  /* TERRIBLE gum solution! */

  if(NewConfig & DEVOUT)
    PostFix << "of " << festring(protocontainer<god>::GetProto(NewConfig&0xFF)->GetClassID()).CapitalizeCopy();
}

void olterrain::SetConfig(ushort NewConfig, ushort SpecialFlags)
{
  InstallDataBase(NewConfig);
  CalculateAll();

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();
}

god* olterrain::GetMasterGod() const
{
  return game::GetGod(GetConfig());
}

bool olterrain::CanBeDestroyed() const
{
  return DataBase->CanBeDestroyed && ((GetPos().X != 0 && GetPos().Y != 0 && GetPos().X != GetLevel()->GetXSize() - 1 && GetPos().Y != GetLevel()->GetYSize() - 1) || GetLevel()->IsOnGround());
}

extern itemprototype key_ProtoType;

void olterrainprototype::CreateSpecialConfigurations()
{
  /* Gum solution */

  if(Config.begin()->second.CreateLockConfigurations)
    {
      const item::databasemap& KeyConfig = key_ProtoType.GetConfig();

      for(olterrain::databasemap::const_iterator i1 = Config.begin(); !(i1->first & LOCK_BITS); ++i1)
	if(!i1->second.IsAbstract)
	  {
	    ushort NewConfig = i1->first | BROKEN_LOCK;
	    olterraindatabase& TempDataBase = Config.insert(std::pair<ushort, olterraindatabase>(NewConfig, i1->second)).first->second;
	    TempDataBase.InitDefaults(NewConfig);
	    TempDataBase.PostFix << "with a broken lock";

	    for(item::databasemap::const_iterator i2 = ++KeyConfig.begin(); i2 != KeyConfig.end(); ++i2)
	      {
		ushort NewConfig = i1->first | i2->first;
		olterraindatabase& TempDataBase = Config.insert(std::pair<ushort, olterraindatabase>(NewConfig, i1->second)).first->second;
		TempDataBase.InitDefaults(NewConfig);
		TempDataBase.PostFix << "with " << i2->second.AdjectiveArticle << ' ' << i2->second.Adjective << " lock";
	      }
	  }
    }
}

bool olterrain::IsTransparent() const
{
  return IsAlwaysTransparent() || MainMaterial->IsTransparent();
}

void lterrain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate) const
{
  const ushort AF = GraphicData.AnimationFrames;
  const bitmap* P = GraphicData.Picture[!AllowAnimate || AF == 1 ? 0 : GET_TICK() % AF];
  P->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

void lterrain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate, bool AllowAlpha) const
{
  const ushort AF = GraphicData.AnimationFrames;
  const bitmap* P = GraphicData.Picture[!AllowAnimate || AF == 1 ? 0 : GET_TICK() % AF];

  if(AllowAlpha)
    P->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
  else
    P->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

void olterrain::ModifyAnimationFrames(ushort& AF) const
{
  if(UseBorderTiles())
    AF += AF << 3;
}

vector2d olterrain::GetBitmapPos(ushort Index) const
{
  if(UseBorderTiles())
    {
      Index = Index ? 8 - (Index + (Index << 3)) / GraphicData.AnimationFrames : 8;
      vector2d MV = game::GetMoveVector(Index);

      if(VisualEffects & MIRROR)
	MV.X = -MV.X;

      if(VisualEffects & FLIP)
	MV.Y = -MV.Y;

      if(VisualEffects & ROTATE)
	{
	  const short T = MV.Y;
	  MV.Y = -MV.X;
	  MV.X = T;
	}

      return DataBase->BitmapPos + (MV << 4);
    }
  else
    return DataBase->BitmapPos;
}

void olterrain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, ushort SquareIndex, bool AllowAnimate) const
{
  if(UseBorderTiles())
    {
      const ushort TrueAF = GraphicData.AnimationFrames / 9;
      ushort PictureIndex = SquareIndex * TrueAF;

      if(AllowAnimate && TrueAF != 1)
	PictureIndex += GET_TICK() % TrueAF;

      GraphicData.Picture[PictureIndex]->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
  else
    lterrain::Draw(Bitmap, Pos, Luminance, AllowAnimate);
}

void olterrain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, ushort SquareIndex, bool AllowAnimate, bool AllowAlpha) const
{
  if(UseBorderTiles())
    {
      const ushort TrueAF = GraphicData.AnimationFrames / 9;
      ushort PictureIndex = SquareIndex * TrueAF;

      if(AllowAnimate && TrueAF != 1)
	PictureIndex += GET_TICK() % TrueAF;

      if(AllowAlpha)
	GraphicData.Picture[PictureIndex]->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
      else
	GraphicData.Picture[PictureIndex]->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
  else
    lterrain::Draw(Bitmap, Pos, Luminance, AllowAnimate, AllowAlpha);
}

void lterrain::SignalRustLevelChange()
{
  UpdatePictures();
  GetLSquareUnder()->SendMemorizedUpdateRequest();
  GetLSquareUnder()->SendNewDrawRequest();
}

void olterrain::SignalRustLevelChange()
{
  lterrain::SignalRustLevelChange();
  HP = Min(HP, CalculateMaxHP());
}

void lterrain::TryToRust(ulong LiquidModifier)
{
  if(MainMaterial->TryToRust(LiquidModifier * 10))
    {
      if(CanBeSeenByPlayer())
	if(MainMaterial->GetRustLevel() == NOT_RUSTED)
	  ADD_MESSAGE("%s rusts.", CHAR_NAME(DEFINITE));
	else
	  ADD_MESSAGE("%s rusts more.", CHAR_NAME(DEFINITE));

      MainMaterial->SetRustLevel(MainMaterial->GetRustLevel() + 1);
    }
}

void olterrain::ReceiveAcid(material* Material, ulong Modifier)
{
  if(!GetMainMaterial()->IsImmuneToAcid())
    {
      ushort Damage = Modifier / 10000;

      if(Damage)
	{
	  Damage += RAND() % Damage;
	  ReceiveDamage(0, Damage, ACID);
	}
    }
}
