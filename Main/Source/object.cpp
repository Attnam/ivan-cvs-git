#include "object.h"
#include "error.h"
#include "proto.h"

object::~object(void)
{
	EraseMaterials();
}

void object::Save(std::ofstream* SaveFile) const
{
	ushort TypeVar = Type();

	SaveFile->write((char*)&TypeVar, sizeof(TypeVar));

	ushort Materials = Material.size();

	SaveFile->write((char*)&Materials, sizeof(Materials));

	for(ushort c = 0; c < Materials; c++)
		if(Material[c])
			Material[c]->Save(SaveFile);
		else
		{
			ushort TypeVar = 0;

			SaveFile->write((char*)&TypeVar, sizeof(TypeVar));
		}

	SaveFile->write((char*)&Size, sizeof(Size));
}

void object::Load(std::ifstream* SaveFile)
{
	ushort Materials;

	SaveFile->read((char*)&Materials, sizeof(Materials));

	for(ushort c = 0; c < Materials; c++)
		Material.push_back(prototypesystem::LoadMaterial(SaveFile));

	SaveFile->read((char*)&Size, sizeof(Size));
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

	if(!CMaterial(0)->IsType(DefaultMaterial))
		Temp = CMaterial(0)->Name() + " " + NameSingular();
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
	if(!CMaterial(0)->IsType(DefaultMaterial))
		return NameWithMaterial(Case);
	else
		return NameNormal(Case, Article);
}

std::string object::NameContainer(uchar Case) const
{
	if(!CMaterial(1))
	{
		if(!(Case & PLURAL))
			if(!(Case & DEFINEBIT))
				return std::string("empty ") + CMaterial(0)->Name() + " " + NameSingular();
			else
				if(!(Case & INDEFINEBIT))
					return std::string("the empty ") + CMaterial(0)->Name() + " " + NameSingular();
				else
					return std::string("an empty ") + CMaterial(0)->Name() + " " + NameSingular();
		else
			if(!(Case & DEFINEBIT))
				return std::string("empty ") + CMaterial(0)->Name() + " " + NamePlural();
			else
				if(!(Case & INDEFINEBIT))
					return std::string("the empty ") + CMaterial(0)->Name() + " " + NamePlural();
				else
					return std::string("empty ") + CMaterial(0)->Name() + " " + NamePlural();
	}
	else
	{
		if(!(Case & PLURAL))
			if(!(Case & DEFINEBIT))
				return CMaterial(0)->Name() + " " + NameSingular() + " full of " + CMaterial(1)->Name();
			else
				if(!(Case & INDEFINEBIT))
					return std::string("the ") + CMaterial(0)->Name() + " " + NameSingular() + " full of " + CMaterial(1)->Name();
				else
					return CMaterial(0)->Name(INDEFINITE) + " " + NameSingular() + " full of " + CMaterial(1)->Name();
		else
			if(!(Case & DEFINEBIT))
				return CMaterial(0)->Name() + " " + NamePlural() + " full of " + CMaterial(1)->Name();
			else
				if(!(Case & INDEFINEBIT))
					return std::string("the ") + CMaterial(0)->Name() + " " + NamePlural() + " full of " + CMaterial(1)->Name();
				else
					return CMaterial(0)->Name() + " " + NamePlural() + " full of " + CMaterial(1)->Name();
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
			return NameSingular() + " of " + CMaterial(0)->Name();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + NameSingular() + " of " + CMaterial(0)->Name();
			else
				return Article + " " + NameSingular() + " of " + CMaterial(0)->Name();
	else
		if(!(Case & DEFINEBIT))
			return NamePlural() + " of " + CMaterial(0)->Name();
		else
			if(!(Case & INDEFINEBIT))
				return std::string("the ") + NamePlural() + " of " + CMaterial(0)->Name();
			else
				return NamePlural() + " of " + CMaterial(0)->Name();
}

void object::EraseMaterials(void)
{
	for(ushort c = 0; c < Material.size(); c++)
		delete Material[c];

	Material.resize(0);
}
