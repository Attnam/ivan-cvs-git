#define __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#include "proto.h"
#include "materba.h"

std::vector<material::prototype*>	protocontainer<material>::ProtoData;
std::map<std::string, ushort>		protocontainer<material>::CodeNameMap;

#include "materde.h"

#undef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DECLARATIONS__

#include "charba.h"
#include "message.h"
#include "lsquare.h"

void schoolfood::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceiveSchoolFoodEffect(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void schoolfood::HitEffect(character* Enemy)
{
  Enemy->ReceiveSchoolFoodEffect(Volume);
}

void schoolfood::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddSchoolFoodConsumeEndMessage();
}

void frogflesh::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->Darkness(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void frogflesh::HitEffect(character* Enemy)
{
  Enemy->Darkness(Volume);
}

void frogflesh::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddDarknessConsumeEndMessage();
}

void omleurine::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceiveOmleUrineEffect(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void omleurine::HitEffect(character* Enemy)
{
  Enemy->ReceiveOmleUrineEffect(Volume);
}

void omleurine::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddOmleUrineConsumeEndMessage();
}

void pepsi::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceivePepsiEffect(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void pepsi::HitEffect(character* Enemy)
{
  Enemy->ReceivePepsiEffect(Volume);
}

void pepsi::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddPepsiConsumeEndMessage();
}

void bone::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddBoneConsumeEndMessage();
}

void koboldflesh::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceiveKoboldFleshEffect(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void koboldflesh::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddKoboldFleshConsumeEndMessage();
}

void healingliquid::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceiveHeal(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void healingliquid::AddConsumeEndMessage(character* Eater) const
{
  Eater->AddHealingLiquidConsumeEndMessage();
}

void healingliquid::HitEffect(character* Enemy)
{
  Enemy->ReceiveHeal(Volume);
}

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
