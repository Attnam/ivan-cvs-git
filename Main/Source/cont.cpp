/* Compiled through wmapset.cpp */

ushort** continent::TypeBuffer;
short** continent::AltitudeBuffer;
uchar** continent::ContinentBuffer;

continent::continent() { }
continent::continent(ushort Index) : Index(Index) { }
ulong continent::GetSize() const { return Member.size(); }
ushort continent::GetGTerrainAmount(ushort Type) const { return GTerrainAmount[Type]; }
vector2d continent::GetMember(ushort Index) const { return Member[Index]; }

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

  Name = "number ";
  Name += Index;
}

vector2d continent::GetRandomMember(ushort Type)
{
  if(!GTerrainAmount[Type])
    ABORT("Shortage of terrain!");

  vector2d* TypeContainer = new vector2d[Member.size()];
  ulong Index = 0;

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
