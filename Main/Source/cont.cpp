/* Compiled through wmapset.cpp */

ushort** continent::TypeBuffer;
short** continent::AltitudeBuffer;
uchar** continent::ContinentBuffer;

continent::continent() { }
continent::continent(ushort Index) : Index(Index) { }

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
