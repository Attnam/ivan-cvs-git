#include <cstdarg>

#include "object.h"
#include "error.h"
#include "game.h"
#include "godba.h"
#include "save.h"
#include "proto.h"
#include "materba.h"

object::~object()
{
  for(ushort c = 0; c < GraphicId.size(); ++c)
    igraph::RemoveUser(GraphicId[c]);
}

void object::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  entity::Save(SaveFile);
  SaveFile << GraphicId << MainMaterial << Config;
}

void object::Load(inputfile& SaveFile)
{
  entity::Load(SaveFile);
  SaveFile >> GraphicId >> MainMaterial >> Config;

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

void object::UpdatePictures()
{
  if(GraphicId.size())
    for(ushort c = 0; c < GraphicId.size(); ++c)
      igraph::RemoveUser(GraphicId[c]);

  GraphicId.resize(GetAnimationFrames());
  Picture.resize(GetAnimationFrames());

  for(ushort c = 0; c < GraphicId.size(); ++c)
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
  ushort Emitation = GetBaseEmitation();

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
