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
      if(++SpoilCounter < GetSpoilModifier())
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

void organicsubstance::EatEffect(character* Eater, ulong Amount, float NPModifier)
{
  Amount = Volume > Amount ? Amount : Volume;
  Effect(Eater, Amount);
  Eater->ReceiveNutrition(long(float(GetNutritionValue()) * Amount * NPModifier / (100000 * (GetSpoilLevel() + 1))));

  if(GetSpoilLevel() > 0)
    Eater->BeginTemporaryState(CONFUSED, Amount * GetSpoilLevel() * NPModifier / 25000);

  if(GetSpoilLevel() > 4)
    Eater->BeginTemporaryState(POISONED, Amount * (GetSpoilLevel() - 4) * NPModifier / 25000);

  SetVolume(Volume - Amount);
}

void organicsubstance::AddConsumeEndMessage(character* Eater) const
{
  if(Eater->IsPlayer())
    {
      if(GetSpoilLevel() > 0 && GetSpoilLevel() <= 4)
	ADD_MESSAGE("Ugh. This stuff was slightly spoiled.");
      else if(GetSpoilLevel() > 4)
	ADD_MESSAGE("Ugh. This stuff was terribly spoiled!");
    }

  material::AddConsumeEndMessage(Eater);
}
