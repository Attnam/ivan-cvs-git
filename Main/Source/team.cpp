#include "team.h"
#include "game.h"
#include "error.h"
#include "save.h"

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
	if(GetRelation(Enemy) != HOSTILE)
		game::Hostility(this, Enemy);

	SetRelation(Enemy, HOSTILE);
}

void team::Save(outputfile& SaveFile) const
{
	SaveFile << ID << Relation;
}

void team::Load(inputfile& SaveFile)
{
	SaveFile >> ID >> Relation;
}
