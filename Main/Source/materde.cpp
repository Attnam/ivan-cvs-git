#include "materde.h"
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

void omleurine::EatEffect(character* Eater, float Amount, float NPModifier)
{
  if(Amount >= Volume)
    Eater->ReceiveOmleUrineEffect(long(Volume > Amount ? Amount : Volume));

  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void omleurine::HitEffect(character* Enemy)
{
  Enemy->ReceiveOmleUrineEffect(Volume);
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

void bone::EatEffect(character* Eater, float Amount, float NPModifier)
{
  if(Amount >= Volume)
    if(Eater->GetIsPlayer())
      ADD_MESSAGE("You feel like a hippie.");
    else
      if(GetMotherObject()->GetLevelSquareUnder()->CanBeSeen())
	ADD_MESSAGE("%s barks happily.", Eater->CNAME(DEFINITE));

  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}

void koboldflesh::EatEffect(character* Eater, float Amount, float NPModifier)
{
  Eater->ReceiveKoboldFleshEffect(long(Volume > Amount ? Amount : Volume));
  NormalFoodEffect(Eater, Amount, NPModifier);
  MinusAmount(Amount);
}
