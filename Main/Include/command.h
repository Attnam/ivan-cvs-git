#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

#include "typedef.h"

class character;

class command
{
public:
	command(bool (character::*LinkedFunction)(void), std::string Description, char Key) : LinkedFunction(LinkedFunction), Description(Description), Key(Key) {}
	bool (character::*CLinkedFunction(void))(void) { return LinkedFunction; }
	std::string CDescription(void) { return Description; }
	char CKey(void) { return Key; }
private:
	bool (character::*LinkedFunction)(void);
	std::string Description;
	char Key;
};

#endif
