/* Compiled through levelset.cpp */

glterrainprototype::glterrainprototype(glterrainprototype* Base, glterrain* (*Cloner)(int, int), const char* ClassID) : Base(Base), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<glterrain>::Add(this); }
olterrainprototype::olterrainprototype(olterrainprototype* Base, olterrain* (*Cloner)(int, int), const char* ClassID) : Base(Base), Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<olterrain>::Add(this); }

square* lterrain::GetSquareUnderEntity(int) const { return LSquareUnder; }
level* lterrain::GetLevel() const { return LSquareUnder->GetLevel(); }
lsquare* lterrain::GetNearLSquare(vector2d Pos) const { return LSquareUnder->GetLevel()->GetLSquare(Pos); }
lsquare* lterrain::GetNearLSquare(int x, int y) const { return LSquareUnder->GetLevel()->GetLSquare(x, y); }
room* lterrain::GetRoom() const { return GetLSquareUnder()->GetRoom(); }
void lterrain::SetMainMaterial(material* NewMaterial, int SpecialFlags) { SetMaterial(MainMaterial, NewMaterial, 0, SpecialFlags); }
void lterrain::ChangeMainMaterial(material* NewMaterial, int SpecialFlags) { ChangeMaterial(MainMaterial, NewMaterial, 0, SpecialFlags); }
void lterrain::InitMaterials(const materialscript* M, const materialscript*, bool CUP) { InitMaterials(M->Instantiate(), CUP); }

void glterrain::InstallDataBase(int NewConfig) { databasecreator<glterrain>::InstallDataBase(this, NewConfig); }
void olterrain::InstallDataBase(int NewConfig) { databasecreator<olterrain>::InstallDataBase(this, NewConfig); }
int glterrain::GetGraphicsContainerIndex() const { return GR_GLTERRAIN; }
int olterrain::GetGraphicsContainerIndex() const { return GR_OLTERRAIN; }

bool olterraindatabase::AllowRandomInstantiation() const { return !(Config & S_LOCK_ID); }

festring olterrain::GetText() const { return ""; }

void lterrain::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  object::Load(SaveFile);
}

void glterrain::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  lterrain::Save(SaveFile);
  SaveFile << (ushort)GetConfig();
}

void glterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  databasecreator<glterrain>::InstallDataBase(this, ReadType<ushort>(SaveFile));
}

void olterrain::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
  lterrain::Save(SaveFile);
  SaveFile << (ushort)GetConfig();
  SaveFile << HP;
}

void olterrain::Load(inputfile& SaveFile)
{
  lterrain::Load(SaveFile);
  databasecreator<olterrain>::InstallDataBase(this, ReadType<ushort>(SaveFile));
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
  lsquare* Square = GetLSquareUnder();
  const fearray<contentscript<item> >& ItemArray = GetLeftOverItems();

  for(uint c1 = 0; c1 < ItemArray.Size; ++c1)
    if(ItemArray[c1].IsValid())
      {
	const interval* TimesPtr = ItemArray[c1].GetTimes();
	int Times = TimesPtr ? TimesPtr->Randomize() : 1;

	for(int c2 = 0; c2 < Times; ++c2)
	  {
	    item* Item = ItemArray[c1].InstantiateBasedOnMaterial(GetMainMaterial()->GetDigProductMaterial());

	    if(Item)
	      {
		/*const materialscript* MScript = ItemArray[c1].GetMainMaterial();

		if(MScript && MScript->GetConfig() == MAIN_MATERIAL)
		  {
		    long Volume = MScript->GetVolume() ? MScript->GetVolume()->Randomize() : 0;
		    Item->ChangeMainMaterial(GetMainMaterial()->CloneDigMaterial(Volume));
		  }*/

		Square->AddItem(Item);
		Item->SpecialGenerationHandler();
	      }
	  }
      }

  Square->ChangeOLTerrainAndUpdateLights(0);
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

void lterrain::Initialize(int NewConfig, int SpecialFlags)
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

void lterrain::SignalEmitationIncrease(color24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0)
    {
      game::CombineLights(Emitation, EmitationUpdate);

      if(LSquareUnder)
	LSquareUnder->SignalEmitationIncrease(EmitationUpdate);
    }
}

void lterrain::SignalEmitationDecrease(color24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation)
    {
      color24 Backup = Emitation;
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

int olterrain::GetStrengthValue() const
{
  return GetMainMaterial()->GetStrengthValue() / 20;
}

void olterrain::ReceiveDamage(character* Villain, int Damage, int)
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

void olterrain::BeKicked(character* Kicker, int Damage, int)
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

int olterrain::CalculateMaxHP()
{
  if(GetMainMaterial())
    {
      long SV = GetMainMaterial()->GetStrengthValue();
      return SV * SV * GetHPModifier() / 6000;
    }
  else
    return 0;
}

int glterrain::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

int olterrain::GetAttachedGod() const
{
  return DataBase->AttachedGod ? DataBase->AttachedGod : MainMaterial->GetAttachedGod();
}

void olterrain::SetConfig(int NewConfig, int SpecialFlags)
{
  databasecreator<olterrain>::InstallDataBase(this, NewConfig);

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();
}

god* olterrain::GetMasterGod() const
{
  return game::GetGod(GetConfig());
}

bool olterrain::CanBeDestroyed() const
{
  return DataBase->CanBeDestroyed && ((GetPos().X && GetPos().Y && GetPos().X != GetLevel()->GetXSize() - 1 && GetPos().Y != GetLevel()->GetYSize() - 1) || GetLevel()->IsOnGround());
}

extern itemprototype key_ProtoType;

int olterrainprototype::CreateSpecialConfigurations(olterraindatabase** TempConfig, int Configs)
{
  if(TempConfig[0]->CreateDivineConfigurations)
    Configs = databasecreator<olterrain>::CreateDivineConfigurations(this, TempConfig, Configs);

  /* Gum solution */

  if(TempConfig[0]->CreateLockConfigurations)
    {
      const item::database*const* KeyConfigData = key_ProtoType.GetConfigData();
      int KeyConfigSize = key_ProtoType.GetConfigSize();
      int OldConfigs = Configs;

      for(int c1 = 0; c1 < OldConfigs; ++c1)
	if(!TempConfig[c1]->IsAbstract)
	  {
	    int BaseConfig = TempConfig[c1]->Config;
	    int NewConfig = BaseConfig | BROKEN_LOCK;
	    olterraindatabase* ConfigDataBase = new olterraindatabase(*TempConfig[c1]);//Config.insert(std::pair<int, itemdatabase>(NewConfig, i1->second)).first->second;
	    ConfigDataBase->InitDefaults(this, NewConfig);
	    ConfigDataBase->PostFix << "with a broken lock";
	    TempConfig[Configs++] = ConfigDataBase;

	    for(int c2 = 0; c2 < KeyConfigSize; ++c2)
	      {
		NewConfig = BaseConfig | KeyConfigData[c2]->Config;
		ConfigDataBase = new olterraindatabase(*TempConfig[c1]);// = Config.insert(std::pair<int, itemdatabase>(NewConfig, i1->second)).first->second;
		ConfigDataBase->InitDefaults(this, NewConfig);
		ConfigDataBase->PostFix << "with " << KeyConfigData[c2]->AdjectiveArticle << ' ' << KeyConfigData[c2]->Adjective << " lock";
		TempConfig[Configs++] = ConfigDataBase;
	      }
	  }
    }

  /*if(TempConfig[0]->CreateLockConfigurations)
    {
      const item::databasemap& KeyConfig = key_ProtoType.GetConfig();

      for(olterrain::databasemap::const_iterator i1 = Config.begin(); !(i1->first & LOCK_BITS); ++i1)
	if(!i1->second.IsAbstract)
	  {
	    int NewConfig = i1->first | BROKEN_LOCK;
	    olterraindatabase& TempDataBase = Config.insert(std::pair<int, olterraindatabase>(NewConfig, i1->second)).first->second;
	    TempDataBase.InitDefaults(this, NewConfig);
	    TempDataBase.PostFix << "with a broken lock";

	    for(item::databasemap::const_iterator i2 = ++KeyConfig.begin(); i2 != KeyConfig.end(); ++i2)
	      {
		int NewConfig = i1->first | i2->first;
		olterraindatabase& TempDataBase = Config.insert(std::pair<int, olterraindatabase>(NewConfig, i1->second)).first->second;
		TempDataBase.InitDefaults(this, NewConfig);
		TempDataBase.PostFix << "with " << i2->second.AdjectiveArticle << ' ' << i2->second.Adjective << " lock";
	      }
	  }
    }*/

  if(TempConfig[0]->CreateWindowConfigurations)
    {
      int OldConfigs = Configs;

      for(int c1 = 0; c1 < OldConfigs; ++c1)
	if(!TempConfig[c1]->IsAbstract)
	  {
	    int NewConfig = TempConfig[c1]->Config | WINDOW;
	    olterraindatabase* ConfigDataBase = new olterraindatabase(*TempConfig[c1]);// = Config.insert(std::pair<int, olterraindatabase>(NewConfig, i->second)).first->second;
	    ConfigDataBase->InitDefaults(this, NewConfig);
	    ConfigDataBase->PostFix << "with a window";
	    ConfigDataBase->IsAlwaysTransparent = true;
	    ConfigDataBase->BitmapPos = ConfigDataBase->WindowBitmapPos;
	    TempConfig[Configs++] = ConfigDataBase;
	  }
    }

  return Configs;
}

bool olterrain::IsTransparent() const
{
  return IsAlwaysTransparent() || MainMaterial->IsTransparent();
}

void glterrain::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool AllowAnimate) const
{
  const int AF = GraphicData.AnimationFrames;
  const bitmap* P = GraphicData.Picture[!AllowAnimate || AF == 1 ? 0 : GET_TICK() % AF];
  P->LuminanceBlit(Bitmap, 0, 0, Pos.X, Pos.Y, 16, 16, Luminance);
}

void olterrain::ModifyAnimationFrames(int& AF) const
{
  if(UseBorderTiles())
    AF += AF << 3;
}

vector2d olterrain::GetBitmapPos(int I) const
{
  if(UseBorderTiles())
    {
      int Index = I ? 8 - (I + (I << 3)) / GraphicData.AnimationFrames : 8;
      vector2d MV = game::GetMoveVector(Index);

      if(VisualEffects & MIRROR)
	MV.X = -MV.X;

      if(VisualEffects & FLIP)
	MV.Y = -MV.Y;

      if(VisualEffects & ROTATE)
	{
	  const int T = MV.Y;
	  MV.Y = -MV.X;
	  MV.X = T;
	}

      return DataBase->BitmapPos + (MV << 4);
    }
  else
    return DataBase->BitmapPos;
}

void olterrain::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance, int SquareIndex, bool AllowAnimate) const
{
  if(UseBorderTiles())
    {
      const int TrueAF = GraphicData.AnimationFrames / 9;
      int PictureIndex = SquareIndex * TrueAF;

      if(AllowAnimate && TrueAF != 1)
	PictureIndex += GET_TICK() % TrueAF;

      GraphicData.Picture[PictureIndex]->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
  else
    {
      const int AF = GraphicData.AnimationFrames;
      const bitmap* P = GraphicData.Picture[!AllowAnimate || AF == 1 ? 0 : GET_TICK() % AF];
      P->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
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

void lterrain::TryToRust(long LiquidModifier)
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

void olterrain::ReceiveAcid(material*, long Modifier)
{
  if(!GetMainMaterial()->IsImmuneToAcid())
    {
      int Damage = Modifier / 10000;

      if(Damage)
	{
	  Damage += RAND() % Damage;
	  ReceiveDamage(0, Damage, ACID);
	}
    }
}

void lterrain::InitMaterials(material* FirstMaterial, bool CallUpdatePictures)
{
  InitMaterial(MainMaterial, FirstMaterial, 0);
  SignalVolumeAndWeightChange();

  if(CallUpdatePictures)
    UpdatePictures();
}

void lterrain::GenerateMaterials()
{
  int Chosen = RandomizeMaterialConfiguration();
  const fearray<long>& MMC = GetMainMaterialConfig();
  InitMaterial(MainMaterial,
	       MAKE_MATERIAL(MMC.Data[MMC.Size == 1 ? 0 : Chosen]),
	       0);
}

void glterraindatabase::InitDefaults(const glterrainprototype* NewProtoType, int NewConfig)
{
  IsAbstract = false;
  ProtoType = NewProtoType;
  Config = NewConfig;
}

void olterraindatabase::InitDefaults(const olterrainprototype* NewProtoType, int NewConfig)
{
  IsAbstract = false;
  ProtoType = NewProtoType;
  Config = NewConfig;
}
