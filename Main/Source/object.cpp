#include <cstdarg>

#include "object.h"
#include "error.h"
#include "game.h"
#include "godba.h"
#include "save.h"
#include "proto.h"

object::~object()
{
  for(ushort c = 0; c < AnimationFrames(); ++c)
    igraph::RemoveUser(GraphicId[c]);
}

void object::Save(outputfile& SaveFile) const
{
  SaveFile << Type();
  entity::Save(SaveFile);
  SaveFile << GraphicId << MainMaterial;
}

void object::Load(inputfile& SaveFile)
{
  entity::Load(SaveFile);
  SaveFile >> GraphicId >> MainMaterial;

  Picture.resize(GraphicId.size());

  for(ushort c = 0; c < AnimationFrames(); ++c)
    Picture[c] = igraph::AddUser(GraphicId[c]).Bitmap;
}

void object::InitMaterials(material* FirstMaterial)
{
  InitMaterial(MainMaterial, FirstMaterial, GetDefaultMainVolume());
  UpdatePictures(false);
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, ulong FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, ulong SecondDefaultVolume)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);
  UpdatePictures(false);
}

void object::ObjectInitMaterials(material*& FirstMaterial, material* FirstNewMaterial, ulong FirstDefaultVolume, material*& SecondMaterial, material* SecondNewMaterial, ulong SecondDefaultVolume, material*& ThirdMaterial, material* ThirdNewMaterial, ulong ThirdDefaultVolume)
{
  InitMaterial(FirstMaterial, FirstNewMaterial, FirstDefaultVolume);
  InitMaterial(SecondMaterial, SecondNewMaterial, SecondDefaultVolume);
  InitMaterial(ThirdMaterial, ThirdNewMaterial, ThirdDefaultVolume);
  UpdatePictures(false);
}

void object::InitMaterial(material*& Material, material* NewMaterial, ulong DefaultVolume)
{
  Material = NewMaterial;

  if(Material)
    {
      if(Material->HasBe())
	SetHasBe(true);

      Material->SetMotherEntity(this);

      if(!Material->GetVolume())
	Material->SetVolume(DefaultVolume);
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
      NewMaterial->SetMotherEntity(this);

      if(!NewMaterial->GetVolume())
	if(OldMaterial)
	  NewMaterial->SetVolume(OldMaterial->GetVolume());
	else
	  if(DefaultVolume)
	    NewMaterial->SetVolume(DefaultVolume);
	  else
	    ABORT("Singularity spawn detected!");
    }

  UpdatePictures();
  return OldMaterial;
}

void object::UpdatePictures(bool RemoveOld)
{
  if(RemoveOld)
    for(ushort c = 0; c < AnimationFrames(); ++c)
      igraph::RemoveUser(GraphicId[c]);
  else
    {
      GraphicId.resize(AnimationFrames());
      Picture.resize(AnimationFrames());
    }

  for(ushort c = 0; c < AnimationFrames(); ++c)
    {
      GraphicId[c].Color[0] = GetMaterialColor0(c);
      GraphicId[c].Color[1] = GetMaterialColor1(c);
      GraphicId[c].Color[2] = GetMaterialColor2(c);
      GraphicId[c].Color[3] = GetMaterialColor3(c);
      GraphicId[c].BitmapPos = GetBitmapPos(c);
      GraphicId[c].FileIndex = GetGraphicsContainerIndex(c);
      GraphicId[c].SpecialType = GetSpecialType(c);
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

std::string object::MaterialDescription(bool Articled) const
{
  return GetMainMaterial()->Name(Articled);
}

std::string object::ContainerPostFix() const
{
  return "full of " + GetContainedMaterial()->Name();
}

std::string object::LumpyPostFix() const
{
  return "of " + GetMainMaterial()->Name();
}

ulong object::GetWeight() const
{
  ulong Weight = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      Weight += GetMaterial(c)->GetWeight();

  return Weight;
}

ulong object::GetVolume() const
{
  ulong Volume = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      Volume += GetMaterial(c)->GetVolume();

  return Volume;
}

ushort object::GetEmitation() const
{
  ushort Emitation = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c) && GetMaterial(c)->GetEmitation() > Emitation)
      Emitation = GetMaterial(c)->GetEmitation();

  return Emitation;
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
