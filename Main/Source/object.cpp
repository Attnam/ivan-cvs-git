#include <cstdarg>

#include "object.h"
#include "error.h"
#include "game.h"
#include "godba.h"
#include "save.h"
#include "proto.h"
#include "materba.h"
#include "femath.h"

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
  entity::Load(SaveFile);
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

void object::UpdatePictures()
{
  if(GraphicId.size())
    for(ushort c = 0; c < GraphicId.size(); ++c)
      igraph::RemoveUser(GraphicId[c]);

  GraphicId.resize(AnimationFrames);
  Picture.resize(AnimationFrames);

  for(ushort c = 0; c < GraphicId.size(); ++c)
    {
      GraphicId[c].Color[0] = GetMaterialColor0(c);
      GraphicId[c].Color[1] = GetMaterialColor1(c);
      GraphicId[c].Color[2] = GetMaterialColor2(c);
      GraphicId[c].Color[3] = GetMaterialColor3(c);
      GraphicId[c].BaseAlpha = GetBaseAlpha(c);
      GraphicId[c].Alpha[0] = GetAlpha0(c);
      GraphicId[c].Alpha[1] = GetAlpha1(c);
      GraphicId[c].Alpha[2] = GetAlpha2(c);
      GraphicId[c].Alpha[3] = GetAlpha3(c);
      GraphicId[c].BitmapPos = GetBitmapPos(c);
      GraphicId[c].FileIndex = GetGraphicsContainerIndex(c);
      GraphicId[c].SpecialFlags = (GetVisualEffects() & 0x7)|GetSpecialFlags(c);
      Picture[c] = igraph::AddUser(GraphicId[c]).Bitmap;
    }
}

ushort object::GetMaterialColor0(ushort) const
{
  if(GetMainMaterial())
    return GetMainMaterial()->GetColor();
  else
    return 0;
}

std::string object::GetMaterialDescription(bool Articled) const
{
  return GetMainMaterial()->GetName(Articled);
}

std::string object::ContainerPostFix() const
{
  return GetContainedMaterial() ? "full of " + GetContainedMaterial()->GetName() : "";
}

std::string object::LumpyPostFix() const
{
  return GetMainMaterial() ? "of " + GetMainMaterial()->GetName() : "";
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

uchar object::GetAlpha0(ushort) const
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
