#include "object.h"
#include "materia.h"
#include "festring.h"
#include "whandler.h"
#include "colorbit.h"
#include "proto.h"
#include "game.h"
#include "bitmap.h"

object::object() : entity(0), MainMaterial(0), AnimationFrames(1) { }
void object::SetMainMaterial(material* NewMaterial, ushort SpecialFlags) { SetMaterial(MainMaterial, NewMaterial, GetDefaultMainVolume(), SpecialFlags); }
void object::ChangeMainMaterial(material* NewMaterial, ushort SpecialFlags) { ChangeMaterial(MainMaterial, NewMaterial, GetDefaultMainVolume(), SpecialFlags); }
void object::SetConsumeMaterial(material* NewMaterial, ushort SpecialFlags) { SetMainMaterial(NewMaterial, SpecialFlags); }
void object::ChangeConsumeMaterial(material* NewMaterial, ushort SpecialFlags) { ChangeMainMaterial(NewMaterial, SpecialFlags); }
uchar object::GetSpecialFlags() const { return ST_NORMAL; }
ushort object::GetOutlineColor(ushort) const { return TRANSPARENT_COLOR; }

object::object(const object& Object) : entity(Object), id(Object), Config(Object.Config), VisualEffects(Object.VisualEffects)
{
  CopyMaterial(Object.MainMaterial, MainMaterial);
}

object::~object()
{
  for(ushort c = 0; c < GraphicId.size(); ++c)
    igraph::RemoveUser(GraphicId[c]);

  delete MainMaterial;
}

void object::CopyMaterial(material* const& Source, material*& Dest)
{
  if(Source)
    {
      Dest = Source->Duplicate();
      Dest->SetMotherEntity(this);
    }
  else
    Dest = 0;
}

void object::Save(outputfile& SaveFile) const
{
  SaveFile << GraphicId << Config << VisualEffects;
  SaveFile << MainMaterial;
}

void object::Load(inputfile& SaveFile)
{
  SaveFile >> GraphicId >> Config >> VisualEffects;
  LoadMaterial(SaveFile, MainMaterial);
  AnimationFrames = GraphicId.size();
  Picture.resize(AnimationFrames);

  for(ushort c = 0; c < AnimationFrames; ++c)
    Picture[c] = igraph::AddUser(GraphicId[c]).Bitmap;
}

void object::InitMaterials(material* FirstMaterial, bool CallUpdatePictures)
{
  InitMaterial(MainMaterial, FirstMaterial, GetDefaultMainVolume());
  SignalVolumeAndWeightChange();

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, ulong FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, ulong SecondDefaultVolume, bool CallUpdatePictures)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);
  SignalVolumeAndWeightChange();

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, ulong FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, ulong SecondDefaultVolume, material*& ThirdMaterial, material* ThirdNewMaterial, ulong ThirdDefaultVolume, bool CallUpdatePictures)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);
  InitMaterial(ThirdMaterial, ThirdNewMaterial, ThirdDefaultVolume);
  SignalVolumeAndWeightChange();

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::InitMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume)
{
  Material = NewMaterial;

  if(Material)
    {
      if(Material->HasBe())
	Enable();

      if(!Material->GetVolume())
	Material->SetVolume(DefaultVolume);

      Material->SetMotherEntity(this);
      SignalEmitationIncrease(Material->GetEmitation());
    }
}

void object::ChangeMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume, ushort SpecialFlags)
{
  delete SetMaterial(Material, NewMaterial, DefaultVolume, SpecialFlags);
}

material* object::SetMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume, ushort SpecialFlags)
{
  material* OldMaterial = Material;
  Material = NewMaterial;

  if((!OldMaterial || !OldMaterial->HasBe()) && NewMaterial && NewMaterial->HasBe())
    Enable();
  else if(OldMaterial && OldMaterial->HasBe() && (!NewMaterial || !NewMaterial->HasBe()) && !CalculateHasBe())
    Disable();

  if(NewMaterial)
    {
      if(!NewMaterial->GetVolume())
	{
	  if(OldMaterial)
	    NewMaterial->SetVolume(OldMaterial->GetVolume());
	  else if(DefaultVolume)
	    NewMaterial->SetVolume(DefaultVolume);
	  else
	    ABORT("Singularity spawn detected!");
	}

      NewMaterial->SetMotherEntity(this);

      if(!(SpecialFlags & NO_SIGNALS))
	SignalEmitationIncrease(NewMaterial->GetEmitation());
    }

  if(!(SpecialFlags & NO_SIGNALS))
    {
      if(OldMaterial)
	SignalEmitationDecrease(OldMaterial->GetEmitation());
  
      SignalVolumeAndWeightChange();
    }

  if(!(SpecialFlags & NO_PIC_UPDATE))
    UpdatePictures();

  return OldMaterial;
}

void object::UpdatePictures()
{
  AnimationFrames = GetClassAnimationFrames();
  vector2d SparklePos;
  uchar GraphicsContainerIndex = GetGraphicsContainerIndex();
  uchar SpecialFlags = (VisualEffects & 0x7)|GetSpecialFlags();
  uchar SparkleTime = 0;
  ushort Seed = 0;
  uchar FlyAmount = GetFlyAmount(); 
  bool Sparkling = false;
  bool FrameNeeded = HasSpecialAnimation();

  if(!(SpecialFlags & (ST_FLAME|ST_LIGHTNING)))
    {
      bool MColorSparkling[4] = { false, false, false, false };

      for(ushort c = 0; c < 4; ++c)
	if(IsSparkling(c))
	  Sparkling = MColorSparkling[c] = true;

      if(Sparkling)
	{
	  static ushort SeedModifier = 1;
	  ulonglong OldSeed = femath::GetSeed(); 
	  vector2d BPos = GetBitmapPos(0);
	  femath::SetSeed(BPos.X + BPos.Y + GetMaterialColorA(0) + SeedModifier);

	  if(++SeedModifier > 0x10)
	    SeedModifier = 1;

	  SparklePos = igraph::GetRawGraphic(GraphicsContainerIndex)->RandomizeSparklePos(BPos, vector2d(16, 16), MColorSparkling);
	  SparkleTime = ((RAND() & 3) << 5) + (RAND() & 0xF);
	  femath::SetSeed(OldSeed);

	  if(AnimationFrames <= 128)
	    AnimationFrames = 128;
	}

      if(FlyAmount)
	{
	  static ushort SeedModifier = 1;
	  vector2d BPos = GetBitmapPos(0);
	  Seed = BPos.X + BPos.Y + GetMaterialColorA(0) + SeedModifier;

	  if(++SeedModifier > 0x10)
	    SeedModifier = 1;

	  if(AnimationFrames <= 32)
	    AnimationFrames = 32;

	  FrameNeeded = true;
	}
    }
  else if(SpecialFlags & ST_FLAME && AnimationFrames <= 16)
    {
      AnimationFrames = 16;
      FrameNeeded = true;
    }
  else if(SpecialFlags & ST_LIGHTNING)
    {
      static ushort SeedModifier = 1;
      vector2d BPos = GetBitmapPos(0);
      Seed = BPos.X + BPos.Y + GetMaterialColorA(0) + SeedModifier + 0x42;

      if(++SeedModifier > 0x10)
	SeedModifier = 1;

      if(AnimationFrames <= 128)
	AnimationFrames = 128;
    }

  ushort c;

  for(c = 0; c < GraphicId.size(); ++c)
    igraph::RemoveUser(GraphicId[c]);

  GraphicId.resize(AnimationFrames);
  Picture.resize(AnimationFrames);

  for(c = 0; c < AnimationFrames; ++c)
    {
      graphicid& GI = GraphicId[c];
      GI.Color[0] = GetMaterialColorA(c);
      GI.Color[1] = GetMaterialColorB(c);
      GI.Color[2] = GetMaterialColorC(c);
      GI.Color[3] = GetMaterialColorD(c);
      ushort MaxAlpha = GetMaxAlpha(c);
      GI.BaseAlpha = GetBaseAlpha(c);

      if(GI.BaseAlpha > MaxAlpha)
	GI.BaseAlpha = MaxAlpha;

      GI.Alpha[0] = GetAlphaA(c);

      if(GI.Alpha[0] > MaxAlpha)
	GI.Alpha[0] = MaxAlpha;

      GI.Alpha[1] = GetAlphaB(c);

      if(GI.Alpha[1] > MaxAlpha)
	GI.Alpha[1] = MaxAlpha;

      GI.Alpha[2] = GetAlphaC(c);

      if(GI.Alpha[2] > MaxAlpha)
	GI.Alpha[2] = MaxAlpha;

      GI.Alpha[3] = GetAlphaD(c);

      if(GI.Alpha[3] > MaxAlpha)
	GI.Alpha[3] = MaxAlpha;

      GI.BitmapPos = GetBitmapPos(c);
      GI.FileIndex = GraphicsContainerIndex;
      GI.SpecialFlags = SpecialFlags;

      bool SparkleInfoNeeded = Sparkling && c >= SparkleTime && c < SparkleTime + 16;

      if(SparkleInfoNeeded)
	{
	  GI.SparklePos = SparklePos;
	  GI.SparkleFrame = c - SparkleTime;
	}
      else
	{
	  GI.SparklePos = ERROR_VECTOR;
	  GI.SparkleFrame = 0;
	}

      GI.Frame = !c || FrameNeeded || (SpecialFlags & ST_LIGHTNING && !((c + 1) & 7)) ? c : 0;
      GI.OutlineColor = GetOutlineColor(c);
      GI.Seed = Seed;
      GI.FlyAmount = FlyAmount;
      Picture[c] = igraph::AddUser(GI).Bitmap;
    }
}

ushort object::GetMaterialColorA(ushort) const
{
  if(GetMainMaterial())
    return GetMainMaterial()->GetColor();
  else
    return 0;
}

bool object::AddMaterialDescription(std::string& String, bool Articled) const
{
  GetMainMaterial()->AddName(String, Articled);
  String << " ";
  return true;
}

void object::AddContainerPostFix(std::string& String) const
{
  if(GetContainedMaterial())
    GetContainedMaterial()->AddName(String << " full of ");
}

void object::AddLumpyPostFix(std::string& String) const
{
  if(GetMainMaterial())
    GetMainMaterial()->AddName(String << " of ");
}

void object::SetSecondaryMaterial(material*, ushort)
{
  ABORT("Illegal object::SetSecondaryMaterial call!");
}

void object::ChangeSecondaryMaterial(material*, ushort)
{
  ABORT("Illegal object::ChangeSecondaryMaterial call!");
}

void object::SetContainedMaterial(material*, ushort)
{
  ABORT("Illegal object::SetContainedMaterial call!");
}

void object::ChangeContainedMaterial(material*, ushort)
{
  ABORT("Illegal object::ChangeContainedMaterial call!");
}

uchar object::GetAlphaA(ushort) const
{
  if(GetMainMaterial())
    return GetMainMaterial()->GetAlpha();
  else
    return 255;
}

void object::RandomizeVisualEffects()
{
  uchar AcceptedFlags = GetOKVisualEffects();

  if(AcceptedFlags)
    SetVisualEffects(RAND() & 0x7 & AcceptedFlags | GetForcedVisualEffects());
  else
    SetVisualEffects(GetForcedVisualEffects());
}

void object::LoadMaterial(inputfile& SaveFile, material*& Material)
{
  SaveFile >> Material;

  if(Material)
    {
      if(Material->HasBe())
	Enable();

      Material->SetMotherEntity(this);
      game::CombineLights(Emitation, Material->GetEmitation());
    }
}

void object::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  if(AllowAlpha)
    Picture[!AllowAnimate || AnimationFrames == 1 ? 0 : globalwindowhandler::GetTick() % AnimationFrames]->PowerBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
  else
    Picture[!AllowAnimate || AnimationFrames == 1 ? 0 : globalwindowhandler::GetTick() % AnimationFrames]->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

god* object::GetMasterGod() const
{
  return game::GetGod(GetConfig());
}

ushort object::RandomizeMaterialConfiguration()
{
  return GetMaterialConfigChances().size() > 1 ? femath::WeightedRand(GetMaterialConfigChances()) : 0;
}

void object::GenerateMaterials()
{
  InitChosenMaterial(MainMaterial, GetMainMaterialConfig(), GetDefaultMainVolume(), RandomizeMaterialConfiguration());
}

void object::InitChosenMaterial(material*& Material, const std::vector<long>& MaterialConfig, ulong DefaultVolume, ushort Chosen)
{
  if(MaterialConfig.size() == 1)
    InitMaterial(Material, MAKE_MATERIAL(MaterialConfig[0]), DefaultVolume);
  else if(MaterialConfig.size() == GetMaterialConfigChances().size())
    InitMaterial(Material, MAKE_MATERIAL(MaterialConfig[Chosen]), DefaultVolume);
  else
    ABORT("MaterialConfig array of illegal size detected!");
}

void object::SetConfig(ushort NewConfig)
{
  Config = NewConfig;
  InstallDataBase();
  CalculateAll();
  UpdatePictures();
}

bool object::AddEmptyAdjective(std::string& String, bool Articled) const
{
  if(GetContainedMaterial())
    return false;
  else
    {
      String << (Articled ? "an empty " : "empty ");
      return true;
    }
}

void object::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      game::CombineLights(Emitation, GetMaterial(c)->GetEmitation());
}

bool object::CalculateHasBe() const
{
  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && GetMaterial(c)->HasBe())
      return true;

  return false;
}

bool object::IsSparkling(ushort ColorIndex) const
{
  return !ColorIndex && MainMaterial->IsSparkling();
}
