#include <cstdarg>

#include "object.h"
#include "error.h"
#include "game.h"
#include "godba.h"

object::object(bool AddToPool, bool HasBe) : unit(AddToPool, HasBe)
{
}

object::~object()
{
  igraph::RemoveUser(GraphicId);
}

void object::Save(outputfile& SaveFile) const
{
  type::Save(SaveFile);
  unit::Save(SaveFile);

  SaveFile << GraphicId;
}

void object::Load(inputfile& SaveFile)
{
  type::Load(SaveFile);
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
	  Material[c]->SetMotherEntity(this);

	  if(!Material[c]->GetVolume())
	    Material[c]->SetVolume(GetDefaultVolume(c));

	  /*if(c < 4)
	    GraphicId.Color[c] = Material[c]->GetColor();*/
	}

      /*if(c < 4)
	ColorChangeSpeciality(c, Material[c] ? false : true);*/
    }

  va_end(AP);

  UpdatePicture(false);

  /*GraphicId.BitmapPos = GetBitmapPos();
  GraphicId.FileIndex = GetGraphicsContainerIndex();
  Picture = igraph::AddUser(GraphicId).Bitmap;*/
}

void object::InitMaterials(material* FirstMaterial)
{
  Material.push_back(FirstMaterial);
  PreserveBit.push_back(false);

  if(Material[0])
    {
      Material[0]->SetMotherEntity(this);

      //GraphicId.Color[0] = Material[0]->GetColor();

      if(!Material[0]->GetVolume())
	Material[0]->SetVolume(GetDefaultVolume(0));
    }

  //ColorChangeSpeciality(0, Material[0] ? false : true);

  UpdatePicture(false);

  /*GraphicId.BitmapPos = GetBitmapPos();
  GraphicId.FileIndex = GetGraphicsContainerIndex();
  Picture = igraph::AddUser(GraphicId).Bitmap;*/
}

std::string object::NameArtifact(uchar Case, uchar DefaultMaterial) const
{
  std::string Temp;

  if(!GetMaterial(0)->IsType(DefaultMaterial))
    Temp = GetMaterial(0)->Name() + " " + NameSingular();
  else
    Temp = NameSingular();

  if(!(Case & PLURAL))
    if(!(Case & DEFINEBIT))
      return Temp;
    else
      return std::string("the ") + Temp;
  else
    {
      ABORT("The %s spawn error detected!", NameSingular().c_str());
      return "";
    }
}

std::string object::NameWithMaterial(uchar Case, uchar Index) const
{
  if(!(Case & PLURAL))
    if(!(Case & DEFINEBIT))
      return Material[Index]->Name() + " " + NameSingular();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + Material[Index]->Name() + " " + NameSingular();
      else
	return Material[Index]->Name(INDEFINITE) + " " + NameSingular();
  else
    if(!(Case & DEFINEBIT))
      return Material[Index]->Name() + " " + NamePlural();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + Material[Index]->Name() + " " + NamePlural();
      else
	return Material[Index]->Name() + " " + NamePlural();
}

std::string object::NameHandleDefaultMaterial(uchar Case, std::string Article, uchar DefaultMaterial) const
{
  if(!GetMaterial(0)->IsType(DefaultMaterial))
    return NameWithMaterial(Case);
  else
    return NameNormal(Case, Article);
}

std::string object::NameContainer(uchar Case) const
{
  if(!GetMaterial(1))
    {
      if(!(Case & PLURAL))
	if(!(Case & DEFINEBIT))
	  return std::string("empty ") + GetMaterial(0)->Name() + " " + NameSingular();
	else
	  if(!(Case & INDEFINEBIT))
	    return std::string("the empty ") + GetMaterial(0)->Name() + " " + NameSingular();
	  else
	    return std::string("an empty ") + GetMaterial(0)->Name() + " " + NameSingular();
      else
	if(!(Case & DEFINEBIT))
	  return std::string("empty ") + GetMaterial(0)->Name() + " " + NamePlural();
	else
	  if(!(Case & INDEFINEBIT))
	    return std::string("the empty ") + GetMaterial(0)->Name() + " " + NamePlural();
	  else
	    return std::string("empty ") + GetMaterial(0)->Name() + " " + NamePlural();
    }
  else
    {
      if(!(Case & PLURAL))
	if(!(Case & DEFINEBIT))
	  return GetMaterial(0)->Name() + " " + NameSingular() + " full of " + GetMaterial(1)->Name();
	else
	  if(!(Case & INDEFINEBIT))
	    return std::string("the ") + GetMaterial(0)->Name() + " " + NameSingular() + " full of " + GetMaterial(1)->Name();
	  else
	    return GetMaterial(0)->Name(INDEFINITE) + " " + NameSingular() + " full of " + GetMaterial(1)->Name();
      else
	if(!(Case & DEFINEBIT))
	  return GetMaterial(0)->Name() + " " + NamePlural() + " full of " + GetMaterial(1)->Name();
	else
	  if(!(Case & INDEFINEBIT))
	    return std::string("the ") + GetMaterial(0)->Name() + " " + NamePlural() + " full of " + GetMaterial(1)->Name();
	  else
	    return GetMaterial(0)->Name() + " " + NamePlural() + " full of " + GetMaterial(1)->Name();
    }
}

std::string object::NameThingsThatAreLikeLumps(uchar Case, std::string Article) const
{
  if(!(Case & PLURAL))
    if(!(Case & DEFINEBIT))
      return NameSingular() + " of " + GetMaterial(0)->Name();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + NameSingular() + " of " + GetMaterial(0)->Name();
      else
	return Article + " " + NameSingular() + " of " + GetMaterial(0)->Name();
  else
    if(!(Case & DEFINEBIT))
      return NamePlural() + " of " + GetMaterial(0)->Name();
    else
      if(!(Case & INDEFINEBIT))
	return std::string("the ") + NamePlural() + " of " + GetMaterial(0)->Name();
      else
	return NamePlural() + " of " + GetMaterial(0)->Name();
}

void object::SetMaterial(uchar Index, material* NewMaterial)
{
  if(Index >= Material.size())
    {
      Material.resize(Index + 1, 0);
      PreserveBit.resize(Index + 1, false);
    }

  /*if(Index < 4)
    if(( Material[Index] && NewMaterial && Material[Index]->GetColor() != NewMaterial->GetColor()) ||
       (!Material[Index] && NewMaterial) || (Material[Index] && !NewMaterial))
      {
	igraph::RemoveUser(GraphicId);
	GraphicId.Color[Index] = NewMaterial ? NewMaterial->GetColor() : 0;
	ColorChangeSpeciality(Index, NewMaterial ? false : true);
	Picture = igraph::AddUser(GraphicId).Bitmap;
      }*/

  if((Material[Index] = NewMaterial))
    {
      Material[Index]->SetMotherEntity(this);

      if(!Material[Index]->GetVolume())
	Material[Index]->SetVolume(GetDefaultVolume(Index));
    }

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
  Picture = igraph::AddUser(GraphicId).Bitmap;
}

std::string object::OwnerGodDescription(uchar OwnerGod) const
{
  return std::string(" of ") + game::GetGod(OwnerGod)->Name();
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
