#include <cstdarg>

#include "object.h"
#include "error.h"
#include "game.h"
#include "godba.h"

object::~object()
{
  igraph::RemoveUser(GraphicId);
}

void object::Save(outputfile& SaveFile) const
{
  SaveFile << Type();
  unit::Save(SaveFile);
  SaveFile << GraphicId;
}

void object::Load(inputfile& SaveFile)
{
  unit::Load(SaveFile);
  SaveFile >> GraphicId;
  Picture = igraph::AddUser(GraphicId).Bitmap;
}

void object::InitMaterials(ushort Materials, ...)
{
  va_list AP;
  va_start(AP, Materials);

  for(ushort c = 0; c < Materials; ++c)
    {
      Material.push_back(va_arg(AP, material*));
      PreserveBit.push_back(false);

      if(Material[c])
	{
	  if(Material[c]->HasBe())
	    SetHasBe(true);

	  Material[c]->SetMotherEntity(this);

	  if(!Material[c]->GetVolume())
	    Material[c]->SetVolume(GetDefaultVolume(c));
	}
    }

  va_end(AP);
  UpdatePicture(false);
}

void object::InitMaterials(material* FirstMaterial)
{
  Material.push_back(FirstMaterial);
  PreserveBit.push_back(false);

  if(Material[0])
    {
      if(Material[0]->HasBe())
	SetHasBe(true);

      Material[0]->SetMotherEntity(this);

      if(!Material[0]->GetVolume())
	Material[0]->SetVolume(GetDefaultVolume(0));
    }

  UpdatePicture(false);
}

void object::SetMaterial(uchar Index, material* NewMaterial)
{
  if(Index >= Material.size())
    {
      Material.resize(Index + 1, 0);
      PreserveBit.resize(Index + 1, false);
    }

  if((!Material[Index] || !Material[Index]->HasBe()) && NewMaterial && NewMaterial->HasBe())
    SetHasBe(true);

  if(Material[Index] && Material[Index]->HasBe() && (!NewMaterial || !NewMaterial->HasBe()))
    {
      bool HasBe = false;

      for(ushort c = 0; c < Material.size(); ++c)
	if(c != Index && Material[c] && Material[c]->HasBe())
	  {
	    HasBe = true;
	    break;
	  }

      SetHasBe(HasBe);
    }

  if(NewMaterial)
    {
      NewMaterial->SetMotherEntity(this);

      if(!NewMaterial->GetVolume())
	NewMaterial->SetVolume(GetDefaultVolume(Index));
    }

  Material[Index] = NewMaterial;
  UpdatePicture();
}

void object::ChangeMaterial(uchar Index, material* NewMaterial)
{
  if(Index >= Material.size())
    SetMaterial(Index, NewMaterial);
  else
    {
      material* OldMaterial = Material[Index];
      SetMaterial(Index, NewMaterial);
      delete OldMaterial;
    }
}

void object::UpdatePicture(bool RemoveOld)
{
  if(RemoveOld)
    igraph::RemoveUser(GraphicId);

  GraphicId.Color[0] = GetMaterialColor0();
  GraphicId.Color[1] = GetMaterialColor1();
  GraphicId.Color[2] = GetMaterialColor2();
  GraphicId.Color[3] = GetMaterialColor3();
  GraphicId.BitmapPos = GetBitmapPos();
  GraphicId.FileIndex = GetGraphicsContainerIndex();
  GraphicId.SpecialType = GetSpecialType();
  Picture = igraph::AddUser(GraphicId).Bitmap;
}

ushort object::GetMaterialColor0() const
{
  if(GetMaterial(0))
    return GetMaterial(0)->GetColor();
  else
    return 0;
}

ushort object::GetMaterialColor1() const
{
  if(GetMaterial(1))
    return GetMaterial(1)->GetColor();
  else
    return GetMaterialColor0();
}

ushort object::GetMaterialColor2() const
{
  if(GetMaterial(2))
    return GetMaterial(2)->GetColor();
  else
    return GetMaterialColor0();
}

ushort object::GetMaterialColor3() const
{
  if(GetMaterial(3))
    return GetMaterial(3)->GetColor();
  else
    return GetMaterialColor0();
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
  ulong TotalWeight = 0;

  for(ushort c = 0; c < GetMaterials(); ++c)
    if(GetMaterial(c))
      TotalWeight += GetMaterial(c)->GetWeight();

  return TotalWeight;
}
