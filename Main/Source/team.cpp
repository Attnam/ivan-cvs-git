#include "team.h"
#include "error.h"
#include "charba.h"
#include "message.h"

void team::SetRelation(team* AnotherTeam, uchar Relation)
{
  this->Relation[AnotherTeam->GetID()] = AnotherTeam->Relation[GetID()] = Relation;
}

uchar team::GetRelation(team* AnotherTeam)
{
  if(AnotherTeam == this)
    return FRIEND;

  std::map<ulong, uchar>::iterator Iterator = Relation.find(AnotherTeam->GetID());

  if(Iterator == Relation.end())
    ABORT("Team %d dismissed!", AnotherTeam->GetID());

  return Iterator->second;
}

void team::Hostility(team* Enemy)
{
  if(this != Enemy && GetRelation(Enemy) != HOSTILE)
    {
      game::Hostility(this, Enemy);

      if(this == game::GetPlayer()->GetTeam())
	{
	  /* This is a gum solution. The message should come from the script. */

	  if(Enemy->GetID() == 2)
	    ADD_MESSAGE("You hear an alarm ringing.");

	  if(Enemy->GetAttackEvilness())
	    game::DoEvilDeed(Enemy->GetAttackEvilness());

	  ADD_MESSAGE("You have a feeling this wasn't a good idea...");
	}

      SetRelation(Enemy, HOSTILE);
    }
}

void team::Save(outputfile& SaveFile) const
{
  SaveFile << ID << Relation << AttackEvilness;
}

void team::Load(inputfile& SaveFile)
{
  SaveFile >> ID >> Relation >> AttackEvilness;
}
