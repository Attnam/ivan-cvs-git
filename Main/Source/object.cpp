#include <cstdarg>

#include "object.h"
#include "error.h"
#include "game.h"
#include "godba.h"
#include "save.h"
#include "proto.h"
#include "materba.h"
#include "femath.h"
#include "whandler.h"
#include "festring.h"
#include "colorbit.h"

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
  else if(OldMaterial && OldMaterial->HasBe() && (!NewMaterial || !NewMaterial->HasBe()))
    SetIsEnabled(CalculateHasBe());

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
  ulong FlySeed = 0;
  uchar FlyAmount = GetFlyAmount(); 
  bool Sparkling = false;

  if(!(SpecialFlags & ST_FLAME))
    {
      bool MColorSparkling[4] = { false, false, false, false };

      for(ushort c = 0; c < 4; ++c)
	if(IsSparkling(c))
	  Sparkling = MColorSparkling[c] = true;

      if(Sparkling)
	{
	  static ushort SeedModifier = 1;
	  long NewSeed = RAND(); 
	  vector2d BPos = GetBitmapPos(0);
	  femath::SetSeed(BPos.X + BPos.Y + GetMaterialColorA(0) + SeedModifier);

	  if(++SeedModifier > 0x10)
	    SeedModifier = 1;

	  SparklePos = igraph::GetRawGraphic(GraphicsContainerIndex)->RandomizeSparklePos(BPos, vector2d(16, 16), MColorSparkling);
	  SparkleTime = ((RAND() & 3) << 5) + (RAND() & 0xF);
	  femath::SetSeed(NewSeed);

	  if(AnimationFrames <= 128)
	    AnimationFrames = 128;
	}

      if(FlyAmount)
	{
	  static ushort SeedModifier = 1;
	  vector2d BPos = GetBitmapPos(0);
	  FlySeed = BPos.X + BPos.Y + GetMaterialColorA(0) + SeedModifier;

	  if(++SeedModifier > 0x10)
	    SeedModifier = 1;

	  if(AnimationFrames <= 32)
	    AnimationFrames = 32;
	}
    }
  else if(AnimationFrames <= 16)
    AnimationFrames = 16;

  ushort c;

  for(c = 0; c < GraphicId.size(); ++c)
    igraph::RemoveUser(GraphicId[c]);

  GraphicId.resize(AnimationFrames);
  Picture.resize(AnimationFrames);

  for(c = 0; c < GraphicId.size(); ++c)
    {
      GraphicId[c].Color[0] = GetMaterialColorA(c);
      GraphicId[c].Color[1] = GetMaterialColorB(c);
      GraphicId[c].Color[2] = GetMaterialColorC(c);
      GraphicId[c].Color[3] = GetMaterialColorD(c);
      ushort MaxAlpha = GetMaxAlpha(c);
      GraphicId[c].BaseAlpha = GetBaseAlpha(c);

      if(GraphicId[c].BaseAlpha > MaxAlpha)
	GraphicId[c].BaseAlpha = MaxAlpha;

      GraphicId[c].Alpha[0] = GetAlphaA(c);

      if(GraphicId[c].Alpha[0] > MaxAlpha)
	GraphicId[c].Alpha[0] = MaxAlpha;

      GraphicId[c].Alpha[1] = GetAlphaB(c);

      if(GraphicId[c].Alpha[1] > MaxAlpha)
	GraphicId[c].Alpha[1] = MaxAlpha;

      GraphicId[c].Alpha[2] = GetAlphaC(c);

      if(GraphicId[c].Alpha[2] > MaxAlpha)
	GraphicId[c].Alpha[2] = MaxAlpha;

      GraphicId[c].Alpha[3] = GetAlphaD(c);

      if(GraphicId[c].Alpha[3] > MaxAlpha)
	GraphicId[c].Alpha[3] = MaxAlpha;

      GraphicId[c].BitmapPos = GetBitmapPos(c);
      GraphicId[c].FileIndex = GraphicsContainerIndex;
      GraphicId[c].SpecialFlags = SpecialFlags;
      GraphicId[c].Frame = c;
      GraphicId[c].SparklePos = Sparkling && c >= SparkleTime && c < SparkleTime + 16 ? SparklePos : BITMAP_ERROR_VECTOR;
      GraphicId[c].SparkleTime = SparkleTime;
      GraphicId[c].OutlineColor = GetOutlineColor(c);
      GraphicId[c].FlySeed = FlySeed;
      GraphicId[c].FlyAmount = FlyAmount;
      Picture[c] = igraph::AddUser(GraphicId[c]).Bitmap;
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
    {
      uchar Flags = RAND() & 0x7;

      if(!(AcceptedFlags & MIRROR))
	Flags &= ~MIRROR;

      if(!(AcceptedFlags & FLIP))
	Flags &= ~FLIP;

      if(!(AcceptedFlags & ROTATE))
	Flags &= ~ROTATE;

      SetVisualEffects(Flags | GetForcedVisualEffects());
    }
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
