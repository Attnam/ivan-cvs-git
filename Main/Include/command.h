#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

#include "typedef.h"

class character;

class command
{
public:
	command(bool (character::*LinkedFunction)(void), std::string Description, char Key, bool CanBeUsedInWilderness) : LinkedFunction(LinkedFunction), Description(Description), Key(Key), CanBeUsedInWilderness(CanBeUsedInWilderness) {}
	bool (character::*GetLinkedFunction(void) const)(void) { return LinkedFunction; }
	std::string GetDescription(void) const { return Description; }
	char GetKey(void) const { return Key; }
	bool GetCanBeUsedInWilderness(void) const { return CanBeUsedInWilderness; }
private:
	bool (character::*LinkedFunction)(void);
	std::string Description;
	char Key;
	bool CanBeUsedInWilderness;
};

#endif
