#include "cont.h"
#include "strover.h"
#include "proto.h"
#include "error.h"
#include "femath.h"
#include "wterraba.h"

ushort** continent::TypeBuffer;
short** continent::AltitudeBuffer;
uchar** continent::ContinentBuffer;

void continent::Save(outputfile& SaveFile) const
{
  SaveFile << Name << Member << Index;
}

void continent::Load(inputfile& SaveFile)
{
  SaveFile >> Name >> Member >> Index;
}

void continent::AttachTo(continent* Continent)
{
  for(ulong c = 0; c < Member.size(); ++c)
    ContinentBuffer[Member[c].X][Member[c].Y] = Continent->Index;

  if(!Continent->Member.size())
    Continent->Member.swap(Member);
  else
    {
      Continent->Member.insert(Continent->Member.end(), Member.begin(), Member.end());
      Member.clear();
    }
}

void continent::GenerateInfo()
{
  GTerrainAmount.resize(protocontainer<gwterrain>::GetProtoAmount() + 1);

  for(ulong c = 0; c < Member.size(); ++c)
    ++GTerrainAmount[TypeBuffer[Member[c].X][Member[c].Y]];

  Name = std::string("number ") + Index;
}

vector2d continent::GetRandomMember(ushort Type)
{
  std::vector<vector2d> TypeContainer;

  if(!GTerrainAmount[Type])
    ABORT("Shortage of terrain!");

  for(ulong c = 0; c < Member.size(); ++c)
    if(TypeBuffer[Member[c].X][Member[c].Y] == Type)
      {
	TypeContainer.push_back(Member[c]);

	if(TypeContainer.size() == GTerrainAmount[Type])
	  break;
      }

  return TypeContainer[RAND() % TypeContainer.size()];
}

