#include "actionba.h"
#include "charba.h"
#include "save.h"

void action::Terminate(bool)
{
  GetActor()->SetAction(0);
  delete this;
}

void action::Save(outputfile& SaveFile) const
{
  SaveFile << Type();
}
