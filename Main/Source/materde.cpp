#define __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "materba.h"

std::vector<material::prototype*> protocontainer<material>::ProtoData;
valuemap protocontainer<material>::CodeNameMap;

MATERIAL_PROTOTYPE(material, 0);

#include "materde.h"

#undef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "charba.h"
#include "message.h"
#include "lsquare.h"
#include "save.h"

void organicsubstance::Be()
{
  if(MotherEntity->AllowSpoil())
    {
      if((SpoilCounter += 10) < GetSpoilModifier())
	{
	  if(SpoilCounter << 1 >= GetSpoilModifier())
	    {
	      uchar NewSpoilLevel = ((SpoilCounter << 4) / GetSpoilModifier()) - 7;

	      if(NewSpoilLevel != SpoilLevel)
		{
		  SpoilLevel = NewSpoilLevel;
		  MotherEntity->SignalSpoilLevelChange(this);
		}
	    }
	}
      else
	MotherEntity->SignalSpoil(this);
    }
}

void organicsubstance::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << SpoilCounter << SpoilLevel;
}

void organicsubstance::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> SpoilCounter >> SpoilLevel;
}

void organicsubstance::VirtualConstructor(bool Load)
{
  if(!Load)
    {
      SpoilCounter = (RAND() % GetSpoilModifier()) >> 5;
      SpoilLevel = 0;
    }
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

void powder::Be()
{
  if(Wetness > 0)
    --Wetness;
}

void powder::Save(outputfile& SaveFile) const
{
  material::Save(SaveFile);
  SaveFile << Wetness;
}

void powder::Load(inputfile& SaveFile)
{
  material::Load(SaveFile);
  SaveFile >> Wetness;
}
