#include "object.h"
#include "error.h"
#include "proto.h"
#include "material.h"
#include "pool.h"

object::object(bool AddToPool) : InPool(AddToPool)
{
	if(AddToPool)
		SetPoolIterator(objectpool::Add(this));
}

object::~object(void)
{
	EraseMaterials();

	if(InPool)
		objectpool::Remove(GetPoolIterator());
}

void object::Save(std::ofstream& SaveFile) const
{
	typeable::Save(SaveFile);

	ushort Materials = Material.size();

	SaveFile.write((char*)&Materials, sizeof(Materials));

	for(ushort c = 0; c < Materials; c++)
		SaveFile << Material[c];

	SaveFile.write((char*)&Size, sizeof(Size));
}

void object::Load(std::ifstream& SaveFile)
{
	typeable::Load(SaveFile);

	ushort Materials;

	SaveFile.read((char*)&Materials, sizeof(Materials));

	Material.resize(Materials);

	for(ushort c = 0; c < Materials; c++)
		SaveFile >> Material[c];

	SaveFile.read((char*)&Size, sizeof(Size));
}

void object::InitMaterials(ushort Materials, ...)
{
	va_list AP;

	va_start(AP, Materials);

	for(ushort c = 0; c < Materials; c++)
		Material.push_back(va_arg(AP, material*));

	va_end(AP);
}

void object::InitMaterials(material* FirstMaterial)
{
	Material.push_back(FirstMaterial);
}

ushort object::GetEmitation(void) const
{
	ushort Emitation = 0;

	for(ushort c = 0; c < Material.size(); c++)
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

void object::EraseMaterials(void)
{
	for(ushort c = 0; c < Material.size(); c++)
		delete Material[c];

	Material.resize(0);
}

