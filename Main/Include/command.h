#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

class character;

class command
{
 public:
  command(bool (character::*LinkedFunction)(), const std::string& Description, char Key, bool UsableInWilderness, bool WizardModeFunction = false) : LinkedFunction(LinkedFunction), Description(Description), Key(Key), UsableInWilderness(UsableInWilderness), WizardModeFunction(WizardModeFunction) { }
  bool (character::*GetLinkedFunction() const)() { return LinkedFunction; }
  std::string GetDescription() const { return Description; }
  char GetKey() const { return Key; }
  bool IsUsableInWilderness() const { return UsableInWilderness; }
  bool GetWizardModeFunction() const { return WizardModeFunction; }
 private:
  bool (character::*LinkedFunction)();
  std::string Description;
  char Key;
  bool UsableInWilderness, WizardModeFunction;
};

#endif


