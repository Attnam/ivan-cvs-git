/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

/* Compiled through wmapset.cpp */

uchar** continent::TypeBuffer;
short** continent::AltitudeBuffer;
uchar** continent::ContinentBuffer;

continent::continent() { }
continent::continent(int Index) : Index(Index) { }
long continent::GetSize() const { return Member.size(); }
int continent::GetGTerrainAmount(int Type) const { return GTerrainAmount[Type]; }
vector2d continent::GetMember(int I) const { return Member[I]; }

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
  GTerrainAmount.resize(protocontainer<gwterrain>::GetSize() + 1);

  for(ulong c = 0; c < Member.size(); ++c)
    ++GTerrainAmount[TypeBuffer[Member[c].X][Member[c].Y]];

  Name = CONST_S("number ");
  Name << Index;
}

vector2d continent::GetRandomMember(int Type)
{
  if(!GTerrainAmount[Type])
    ABORT("Shortage of terrain!");

  vector2d* TypeContainer = new vector2d[Member.size()];
  long Index = 0;

  for(ulong c = 0; c < Member.size(); ++c)
    if(TypeBuffer[Member[c].X][Member[c].Y] == Type)
      {
	TypeContainer[Index++] = Member[c];

	if(Index == GetGTerrainAmount(Type))
	  break;
      }

  vector2d Return = TypeContainer[RAND() % Index];
  delete [] TypeContainer;
  return Return;
}

outputfile& operator<<(outputfile& SaveFile, const continent* Continent)
{
  if(Continent)
    {
      SaveFile.Put(1);
      Continent->Save(SaveFile);
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, continent*& Continent)
{
  if(SaveFile.Get())
    {
      Continent = new continent;
      Continent->Load(SaveFile);
    }

  return SaveFile;
}

void continent::Add(vector2d Pos)
{
  Member.push_back(Pos);
  ContinentBuffer[Pos.X][Pos.Y] = Index;
}
