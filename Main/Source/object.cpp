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
#include "stdover.h"

object::~object()
{
  for(ushort c = 0; c < GraphicId.size(); ++c)
    igraph::RemoveUser(GraphicId[c]);
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
  Picture.resize(GraphicId.size());

  for(ushort c = 0; c < GraphicId.size(); ++c)
    Picture[c] = igraph::AddUser(GraphicId[c]).Bitmap;
}

void object::InitMaterials(material* FirstMaterial, bool CallUpdatePictures)
{
  InitMaterial(MainMaterial, FirstMaterial, GetDefaultMainVolume());

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, ulong FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, ulong SecondDefaultVolume, bool CallUpdatePictures)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, ulong FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, ulong SecondDefaultVolume, material*& ThirdMaterial, material* ThirdNewMaterial, ulong ThirdDefaultVolume, bool CallUpdatePictures)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);
  InitMaterial(ThirdMaterial, ThirdNewMaterial, ThirdDefaultVolume);

  if(CallUpdatePictures)
    UpdatePictures();
}

void object::InitMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume)
{
  Material = NewMaterial;

  if(Material)
    {
      if(Material->HasBe())
	SetHasBe(true);

      if(!Material->GetVolume())
	Material->SetVolume(DefaultVolume);

      Material->SetMotherEntity(this);
      EditVolume(NewMaterial->GetVolume());
      EditWeight(NewMaterial->GetWeight());

      if(NewMaterial->GetEmitation() > Emitation)
	Emitation = NewMaterial->GetEmitation();
    }
}

void object::ChangeMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume)
{
  delete SetMaterial(Material, NewMaterial, DefaultVolume);
}

material* object::SetMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume)
{
  material* OldMaterial = Material;
  Material = NewMaterial;

  if((!OldMaterial || !OldMaterial->HasBe()) && NewMaterial && NewMaterial->HasBe())
    SetHasBe(true);

  if(OldMaterial && OldMaterial->HasBe() && (!NewMaterial || !NewMaterial->HasBe()))
    SetHasBe(CalculateHasBe());

  if(NewMaterial)
    {
      if(!NewMaterial->GetVolume())
	if(OldMaterial)
	  NewMaterial->SetVolume(OldMaterial->GetVolume());
	else
	  if(DefaultVolume)
	    NewMaterial->SetVolume(DefaultVolume);
	  else
	    ABORT("Singularity spawn detected!");

      NewMaterial->SetMotherEntity(this);
      EditVolume(NewMaterial->GetVolume());
      EditWeight(NewMaterial->GetWeight());

      if(NewMaterial->GetEmitation() > Emitation)
	Emitation = NewMaterial->GetEmitation();
    }

  if(OldMaterial)
    {
      EditVolume(-OldMaterial->GetVolume());
      EditWeight(-OldMaterial->GetWeight());

      if(OldMaterial->GetEmitation() == Emitation)
	CalculateEmitation();
    }

  UpdatePictures();
  return OldMaterial;
}

#include "lterrade.h"

void object::UpdatePictures()
{
  if(GraphicId.size())
    for(ushort c = 0; c < GraphicId.size(); ++c)
      igraph::RemoveUser(GraphicId[c]);

  GraphicId.resize(AnimationFrames);
  Picture.resize(AnimationFrames);

  for(ushort c = 0; c < GraphicId.size(); ++c)
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
      GraphicId[c].FileIndex = GetGraphicsContainerIndex(c);
      GraphicId[c].SpecialFlags = (GetVisualEffects() & 0x7)|GetSpecialFlags(c);
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

void object::CalculateEmitation()
{
  Emitation = GetBaseEmitation();

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && GetMaterial(c)->GetEmitation() > Emitation)
      Emitation = GetMaterial(c)->GetEmitation();
}

bool object::CalculateHasBe() const
{
  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && GetMaterial(c)->HasBe())
      return true;

  return false;
}

void object::SetSecondaryMaterial(material*)
{
  ABORT("Illegal object::SetSecondaryMaterial call!");
}

void object::ChangeSecondaryMaterial(material*)
{
  ABORT("Illegal object::ChangeSecondaryMaterial call!");
}

void object::SetContainedMaterial(material*)
{
  ABORT("Illegal object::SetContainedMaterial call!");
}

void object::ChangeContainedMaterial(material*)
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
  uchar Flags = 0, AcceptedFlags = GetOKVisualEffects();
  ushort c;

  for(c = 0; c < 8; ++c)
    if((AcceptedFlags & (1 << c)) && (RAND() % 2))
      Flags |= 1 << c;

  SetVisualEffects(Flags | GetForcedVisualEffects());
}

void object::LoadMaterial(inputfile& SaveFile, material*& Material)
{
  SaveFile >> Material;

  if(Material)
    {
      Material->SetMotherEntity(this);
      Volume += Material->GetVolume();
      Weight += Material->GetWeight();

      if(Material->GetEmitation() > Emitation)
	Emitation = Material->GetEmitation();
    }
}

void object::Draw(bitmap* Bitmap, vector2d Pos, ushort Luminance, bool AllowAlpha, bool AllowAnimate) const
{
  if(AllowAlpha)
    {
      if(!AllowAnimate || AnimationFrames == 1)
	Picture[0]->PowerBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
      else
	Picture[globalwindowhandler::GetTick() % AnimationFrames]->PowerBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
  else
    {
      if(!AllowAnimate || AnimationFrames == 1)
	Picture[0]->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
      else
	Picture[globalwindowhandler::GetTick() % AnimationFrames]->MaskedBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
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
