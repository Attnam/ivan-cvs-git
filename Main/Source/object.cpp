#include "object.h"
#include "error.h"
#include "proto.h"
#include "materba.h"
#include "pool.h"
#include "save.h"
#include "game.h"
#include "lsquare.h"
#include "god.h"

object::object(bool AddToPool) : InPool(AddToPool), Exists(true), Picture(0), SquareUnder(0)
{
	*(ulong*)(&GraphicId.Color[0]) = *(ulong*)(&GraphicId.Color[2]) = 0;

	if(AddToPool)
	{
		SetPoolIterator(objectpool::Add(this));
		ID = game::CreateNewObjectID();
	}
}

object::~object()
{
	EraseMaterials();

	if(InPool)
		objectpool::Remove(GetPoolIterator());
}

void object::Save(outputfile& SaveFile) const
{
	typeable::Save(SaveFile);

	SaveFile << Material << Size << GraphicId << ID;
}

void object::Load(inputfile& SaveFile)
{
	typeable::Load(SaveFile);

	SquareUnder = game::GetSquareInLoad();

	game::PopObjectID(ID);

	SaveFile >> Material >> Size >> GraphicId >> ID;

	for(ushort c = 0; c < Material.size(); ++c)
		if(Material[c])
			Material[c]->SetMotherObject(this);

	PreserveBit.resize(Material.size(), false);

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
			Material[c]->SetMotherObject(this);

			if(!Material[c]->GetVolume())
				Material[c]->SetVolume(GetDefaultVolume(c));

			if(c < 4)
				GraphicId.Color[c] = Material[c]->GetColor();
		}

		if(c < 4)
			ColorChangeSpeciality(c, Material[c] ? false : true);
	}

	va_end(AP);

	GraphicId.BitmapPos = GetBitmapPos();
	GraphicId.FileIndex = GetGraphicsContainerIndex();
	Picture = igraph::AddUser(GraphicId).Bitmap;
}

void object::InitMaterials(material* FirstMaterial)
{
	Material.push_back(FirstMaterial);
	PreserveBit.push_back(false);

	if(Material[0])
	{
		Material[0]->SetMotherObject(this);

		GraphicId.Color[0] = Material[0]->GetColor();

		if(!Material[0]->GetVolume())
			Material[0]->SetVolume(GetDefaultVolume(0));
	}

	ColorChangeSpeciality(0, Material[0] ? false : true);

	GraphicId.BitmapPos = GetBitmapPos();
	GraphicId.FileIndex = GetGraphicsContainerIndex();
	Picture = igraph::AddUser(GraphicId).Bitmap;
}

ushort object::GetEmitation() const
{
	ushort Emitation = 0;

	for(ushort c = 0; c < Material.size(); ++c)
		if(Material[c])
			if(Material[c]->GetEmitation() > Emitation)
				Emitation = Material[c]->GetEmitation();

	return Emitation;
}

std::string object::NameNormal(uchar Case, std::string Article) const
{
	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return NameSingular();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + NameSingular();
			else
				return Article + " " + NameSingular();
	else
		if(!(Case & DEFINEBIT))
			return NamePlural();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + NamePlural();
			else
				return NamePlural();
}

std::string object::NameProperNoun(uchar Case) const
{
	if(!(Case & PLURAL))
		return NameSingular();
	else
		return NamePlural();
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
		return "there can be only one";
	}
}

std::string object::NameWithMaterial(uchar Case) const
{
	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return Material[0]->Name() + " " + NameSingular();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + Material[0]->Name() + " " + NameSingular();
			else
				return Material[0]->Name(INDEFINITE) + " " + NameSingular();
	else
		if(!(Case & DEFINEBIT))
			return Material[0]->Name() + " " + NamePlural();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + Material[0]->Name() + " " + NamePlural();
			else
				return Material[0]->Name() + " " + NamePlural();
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

std::string object::NameSized(uchar Case, std::string Article, ushort LillaBorder, ushort StoraBorder) const
{
	std::string Temp;

	if(GetSize() >= StoraBorder) Temp = "big ";
	if(GetSize() >= LillaBorder && Size < StoraBorder) Temp = "standard sized ";
	if(GetSize() < LillaBorder) Temp = "small ";            // SWEDISH SUX0R BAD!

	if(!(Case & PLURAL))
		if(!(Case & DEFINEBIT))
			return Temp + NameSingular();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + Temp + NameSingular();
			else
				return Article + " " + Temp + NameSingular();
	else
		if(!(Case & DEFINEBIT))
			return Temp + NamePlural();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + Temp + NamePlural();
			else
				return Temp + NamePlural();
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

void object::EraseMaterials()
{
	igraph::RemoveUser(GraphicId);

	for(ushort c = 0; c < Material.size(); ++c)
		if(!PreserveBit[c])
			delete Material[c];

	Material.resize(0);
}

void object::SetMaterial(uchar Index, material* NewMaterial)
{
	if(Index >= Material.size())
	{
		Material.resize(Index + 1, 0);
		PreserveBit.resize(Index + 1, false);
	}

	if(Index < 4)
		if(( Material[Index] && NewMaterial && Material[Index]->GetColor() != NewMaterial->GetColor()) ||
		   (!Material[Index] && NewMaterial) || (Material[Index] && !NewMaterial))
		{
			igraph::RemoveUser(GraphicId);
			GraphicId.Color[Index] = NewMaterial ? NewMaterial->GetColor() : 0;
			ColorChangeSpeciality(Index, NewMaterial ? false : true);
			Picture = igraph::AddUser(GraphicId).Bitmap;
		}

	if((Material[Index] = NewMaterial))
	{
		Material[Index]->SetMotherObject(this);

		if(!Material[Index]->GetVolume())
			Material[Index]->SetVolume(GetDefaultVolume(Index));
	}
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

void object::UpdatePicture()
{
	igraph::RemoveUser(GraphicId);
	GraphicId.BitmapPos = GetBitmapPos();
	GraphicId.FileIndex = GetGraphicsContainerIndex();
	Picture = igraph::AddUser(GraphicId).Bitmap;
}

void object::PreserveMaterial(ushort Index)
{
	PreserveBit[Index] = true;
}

levelsquare* object::GetLevelSquareUnder() const
{
	return (levelsquare*)SquareUnder;
}

void object::SetLevelSquareUnder(levelsquare* What)
{
	SquareUnder = What;
}

material* object::GetMaterial(ushort Index) const
{
	if(Index < Material.size())
		return Material[Index];
	else
		return 0;
}

std::string object::OwnerGodDescription(uchar OwnerGod) const
{
	return std::string(" of ") + game::GetGod(OwnerGod)->Name();
}
