#define __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#include "proto.h"
#include "materba.h"

std::vector<material::prototype*> protocontainer<material>::ProtoData;
valuemap protocontainer<material>::CodeNameMap;

MATERIAL_PROTOTYPE(material, 0);

#include "materde.h"

#undef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#include "charba.h"
#include "message.h"
#include "lsquare.h"
#include "save.h"

bool organicsubstance::Be()
{
  return true;
}

void organicsubstance::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << SpoilCounter;
}

void organicsubstance::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> SpoilCounter;
}

void flesh::Save(outputfile& SaveFile) const
{
  organicsubstance::Save(SaveFile);
  SaveFile << SkinColor;
}

void flesh::Load(inputfile& SaveFile)
{
  organicsubstance::Load(SaveFile);
  SaveFile >> SkinColor;
}

