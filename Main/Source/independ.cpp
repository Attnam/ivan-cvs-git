#include "independ.h"

void independency::Save(std::ofstream* SaveFile)
{
	ushort TypeVar = Type();

	SaveFile->write((char*)&TypeVar, sizeof(TypeVar));
}

std::string independency::NameNormal(uchar Case, std::string Article)
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

std::string independency::NameProperNoun(uchar Case)
{
	if(!(Case & PLURAL))
		return NameSingular();
	else
		return NamePlural();
}
