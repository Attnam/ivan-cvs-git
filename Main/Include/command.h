#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

#include "typedef.h"

class character;

class command
{
public:
	command(bool (character::*LinkedFunction)(), std::string Description, char Key, bool CanBeUsedInWilderness) : LinkedFunction(LinkedFunction), Description(Description), Key(Key), CanBeUsedInWilderness(CanBeUsedInWilderness) {}
	bool (character::*GetLinkedFunction() const)() { return LinkedFunction; }
	std::string GetDescription() const { return Description; }
	char GetKey() const { return Key; }
	bool GetCanBeUsedInWilderness() const { return CanBeUsedInWilderness; }
private:
	bool (character::*LinkedFunction)();
	std::string Description;
	char Key;
	bool CanBeUsedInWilderness;
};

#endif
