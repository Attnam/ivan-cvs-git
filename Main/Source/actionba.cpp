#include "actionba.h"
#include "charba.h"

void action::Terminate(bool)
{
  GetActor()->SetAction(0);
  delete this;
}
