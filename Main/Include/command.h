#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class character;

class command
{
public:
	command(bool (character::*LinkedFunction)(), std::string Description, char Key, bool CanBeUsedInWilderness, bool WizardModeFunction = false) : LinkedFunction(LinkedFunction), Description(Description), Key(Key), CanBeUsedInWilderness(CanBeUsedInWilderness), WizardModeFunction(WizardModeFunction) {}
	bool (character::*GetLinkedFunction() const)() { return LinkedFunction; }
	std::string GetDescription() const { return Description; }
	char GetKey() const { return Key; }
	bool GetCanBeUsedInWilderness() const { return CanBeUsedInWilderness; }
	bool GetWizardModeFunction() const { return WizardModeFunction; }
private:
	bool (character::*LinkedFunction)();
	std::string Description;
	char Key;
	bool CanBeUsedInWilderness, WizardModeFunction;
};

#endif



