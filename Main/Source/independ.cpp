#include "independ.h"

void independency::Save(std::ofstream* SaveFile) const
{
	ushort TypeVar = Type();

	SaveFile->write((char*)&TypeVar, sizeof(TypeVar));
}

std::string independency::NameNormal(uchar Case, std::string Article) const
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

std::string independency::NameProperNoun(uchar Case) const
{
	if(!(Case & PLURAL))
		return NameSingular();
	else
		return NamePlural();
}
