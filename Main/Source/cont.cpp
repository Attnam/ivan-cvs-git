#include "cont.h"
#include "strover.h"
#include "save.h"

void continent::Save(std::ofstream& SaveFile) const
{
	SaveFile << Name << Member << Index;
}

void continent::Load(std::ifstream& SaveFile)
{
	SaveFile >> Name >> Member >> Index;
}

void continent::AttachTo(continent& Continent)
{
	for(ulong c = 0; c < Member.size(); c++)
		ContinentBuffer[Member[c].X][Member[c].Y] = Continent.Index;

	if(!Continent.Member.size())
		Continent.Member.swap(Member);
	else
	{
		Continent.Member.insert(Continent.Member.end(), Member.begin(), Member.end());

		Member.clear();
	}
}
