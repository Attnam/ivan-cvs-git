#include "object.h"
#include "game.h"
#include "error.h"

object::~object(void)
{
	EraseMaterials();
}

void object::Save(std::ofstream* SaveFile)
{
	independency::Save(SaveFile);

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

object::object(std::ifstream* SaveFile) : independency(SaveFile)
{
	ushort Materials;

	SaveFile->read((char*)&Materials, sizeof(Materials));

	for(ushort c = 0; c < Materials; c++)
		Material.push_back(game::LoadMaterial(SaveFile));

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

ushort object::CEmitation(void)
{
	ushort Emitation = 0;

	for(ushort c = 0; c < Material.size(); c++)
		if(Material[c])
			if(Material[c]->CEmitation() > Emitation)
				Emitation = Material[c]->CEmitation();

	return Emitation;
}

std::string object::NameArtifact(uchar Case, uchar DefaultMaterial)
{
	std::string Temp;

	if(CMaterial(0)->Type() != DefaultMaterial)
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

std::string object::NameWithMaterial(uchar Case)
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

std::string object::NameHandleDefaultMaterial(uchar Case, std::string Article, uchar DefaultMaterial)
{
	if(CMaterial(0)->Type() != DefaultMaterial)
		return NameWithMaterial(Case);
	else
		return NameNormal(Case, Article);
}

std::string object::NameContainer(uchar Case)
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

std::string object::NameSized(uchar Case, std::string Article, ushort LillaBorder, ushort StoraBorder)
{
	std::string Temp;

	if(CSize() >= StoraBorder) Temp = "big ";
	if(CSize() >= LillaBorder && Size < StoraBorder) Temp = "standard sized ";
	if(CSize() < LillaBorder) Temp = "small ";            // SWEDISH SUX0R BAD!

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

std::string object::NameThingsThatAreLikeLumps(uchar Case, std::string Article)
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
