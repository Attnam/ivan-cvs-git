/* Compiled through dataset.cpp */

script::datamap posscript::DataMap;
script::datamap materialscript::DataMap;
script::datamap basecontentscript::DataMap;
script::datamap contentscript<character>::DataMap;
script::datamap contentscript<item>::DataMap;
script::datamap contentscript<glterrain>::DataMap;
script::datamap contentscript<olterrain>::DataMap;
script::datamap squarescript::DataMap;
script::datamap itemcontentmap::DataMap;
script::datamap charactercontentmap::DataMap;
script::datamap glterraincontentmap::DataMap;
script::datamap olterraincontentmap::DataMap;
script::datamap roomscript::DataMap;
script::datamap levelscript::DataMap;
script::datamap dungeonscript::DataMap;
script::datamap teamscript::DataMap;
script::datamap gamescript::DataMap;

template <class type> inline void datamember<type>::ReadFrom(inputfile& SaveFile)
{
  if(!Member)
    Member = new type;

  ReadData(*Member, SaveFile);
}

template <class type> inline void datamember<type>::Replace(datamemberbase& Base)
{
  datamember<type>& Data = static_cast<datamember<type>&>(Base);

  if(Data.Member)
    {
      delete Member;
      Member = Data.Member;
      Data.Member = 0;
    }
}

template <class type> inline void datamember<type>::Save(outputfile& SaveFile) const
{
  if(Member)
    {
      SaveFile.Put(1);
      SaveFile << *Member;
    }
  else
    SaveFile.Put(0);
}

template <class type> inline void datamember<type>::Load(inputfile& SaveFile)
{
  if(SaveFile.Get())
    {
      Member = new type;
      SaveFile >> *Member;
    }
}

bool script::ReadMember(inputfile& SaveFile, const std::string& Word)
{
  datamemberbase* Data = GetData(Word);

  if(Data)
    {
      Data->ReadFrom(SaveFile);
      return true;
    }
  else
    return false;
}

datamemberbase* script::GetDataFromMap(const datamap& DataMap, const std::string& Identifier)
{
  datamap::const_iterator i = DataMap.find(Identifier);
  return i != DataMap.end() ? &(this->*i->second) : 0;
}

void script::SaveDataMap(const datamap& DataMap, outputfile& SaveFile) const
{
  for(datamap::const_iterator i = DataMap.begin(); i != DataMap.end(); ++i)
    (this->*i->second).Save(SaveFile);
}

void script::LoadDataMap(const datamap& DataMap, inputfile& SaveFile)
{
  for(datamap::const_iterator i = DataMap.begin(); i != DataMap.end(); ++i)
    (this->*i->second).Load(SaveFile);
}

template<class type, class scripttype> inline void InitMember(script::datamap& DataMap, const std::string& Identifier, datamember<type> scripttype::* DataMember)
{
  DataMap.insert(std::pair<std::string, datamemberbase script::*>(Identifier, reinterpret_cast<datamemberbase script::*>(DataMember)));
}

#define INIT_MEMBER(name) InitMember(DataMap, #name, &scripttype::name##Holder)

inline outputfile& operator<<(outputfile& SaveFile, const script& Script) { Script.Save(SaveFile); return SaveFile; }
inline inputfile& operator>>(inputfile& SaveFile, script& Script) { Script.Load(SaveFile); return SaveFile; }

void posscript::InitDataMap()
{
  INIT_MEMBER(Vector);
  INIT_MEMBER(Flags);
  INIT_MEMBER(Borders);
}

void posscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "Pos")
    {
      Random = false;
      VectorHolder.ReadFrom(SaveFile);
    }

  if(Word == "Random")
    {
      Random = true;
      FlagsHolder.ReadFrom(SaveFile);
    }

  if(Word == "BoundedRandom")
    {
      Random = true;
      BordersHolder.ReadFrom(SaveFile);
      FlagsHolder.ReadFrom(SaveFile);
    }
}

void posscript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Random;
}

void posscript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Random;
}

void materialscript::InitDataMap()
{
  INIT_MEMBER(Volume);
}

void materialscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    Word = SaveFile.ReadWord();

  valuemap::const_iterator i = game::GetGlobalValueMap().find(Word);

  if(i != game::GetGlobalValueMap().end())
    Config = i->second;
  else
    ABORT("Unconfigured material script detected at line %d!", SaveFile.TellLine());

  if(SaveFile.ReadWord() != "{")
    return;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    if(!ReadMember(SaveFile, Word))
      ABORT("Odd script term %s encountered in material script line %d!", Word.c_str(), SaveFile.TellLine());
}

material* materialscript::Instantiate() const
{
  const ulong* Volume = GetVolume();
  return !Volume ? MAKE_MATERIAL(Config) : MAKE_MATERIAL(Config, *Volume);
}

void materialscript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Config;
}

void materialscript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Config;
}

void basecontentscript::InitDataMap()
{
  INIT_MEMBER(MainMaterial);
  INIT_MEMBER(SecondaryMaterial);
  INIT_MEMBER(ContainedMaterial);
  INIT_MEMBER(Parameters);
}

basecontentscript::basecontentscript() : ContentType(0), Config(0), Random(false) { }

void basecontentscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word = SaveFile.ReadWord();

  if(Word == "=" || Word == ",")
    Word = SaveFile.ReadWord();

  valuemap::const_iterator i = game::GetGlobalValueMap().find(Word);

  if(i != game::GetGlobalValueMap().end())
    {
      if(!GetMainMaterial())
	MainMaterialHolder.SetMember(new materialscript);

      MainMaterialHolder.GetMember()->SetConfig(i->second);
      Word = SaveFile.ReadWord();
      i = game::GetGlobalValueMap().find(Word);

      if(i != game::GetGlobalValueMap().end())
	{
	  if(!GetSecondaryMaterial())
	    SecondaryMaterialHolder.SetMember(new materialscript);

	  SecondaryMaterialHolder.GetMember()->SetConfig(i->second);
	  Word = SaveFile.ReadWord();
	}
    }

  if(Word == "Random")
    {
      Random = true;
      Word = SaveFile.ReadWord();
    }
  else
    {
      Random = false;
      ContentType = SearchCodeName(Word);

      if(ContentType || Word == "0")
	Word = SaveFile.ReadWord();
      else
	ABORT("Odd script term %s encountered in %s content script, file %s line %d!", Word.c_str(), GetClassId(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
    }

  if(Word == "(")
    {
      Config = SaveFile.ReadNumber();
      Word = SaveFile.ReadWord();
    }
  else
    Config = 0;

  if(Word == "{")
    for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
      {
	if(!ReadMember(SaveFile, Word))
	  ABORT("Odd script term %s encountered in %s content script, file %s line %d!", Word.c_str(), GetClassId(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
      }
  else
    if(Word != ";" && Word != ",")
      ABORT("Odd terminator %s encountered in %s content script, file %s line %d!", Word.c_str(), GetClassId(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

datamemberbase* basecontentscript::GetData(const std::string& String)
{
  datamemberbase* Return = GetDataFromMap(GetDataMap(), String);
  return Return ? Return : GetDataFromMap(DataMap, String);
}

void basecontentscript::Save(outputfile& SaveFile) const
{
  SaveDataMap(GetDataMap(), SaveFile);
  SaveDataMap(DataMap, SaveFile);
  SaveFile << ContentType << Config << Random;
}

void basecontentscript::Load(inputfile& SaveFile)
{
  LoadDataMap(GetDataMap(), SaveFile);
  LoadDataMap(DataMap, SaveFile);
  SaveFile >> ContentType >> Config >> Random;
}

template <class type> type* contentscripttemplate<type>::BasicInstantiate(ushort SpecialFlags) const
{
  type* Instance = 0;
  const typename type::prototype* Proto = protocontainer<type>::GetProto(ContentType);
  const typename type::databasemap& ProtoConfig = Proto->GetConfig();

  if(!Config && ProtoConfig.begin()->second.IsAbstract)
    {
      while(!Instance)
	{
	  ushort ChosenConfig = 1 + RAND() % (ProtoConfig.size() - 1);
	  typename type::databasemap::const_iterator i;
	  for(i = ProtoConfig.begin(); ChosenConfig; ++i, --ChosenConfig);

	  if(i->second.AllowRandomInstantiation())
	    Instance = Proto->Clone(i->first, SpecialFlags|NO_PIC_UPDATE);
	}
    }
  else
    Instance = Proto->Clone(Config, SpecialFlags|NO_PIC_UPDATE);

  const ulong* Parameters = GetParameters();

  if(Parameters)
    Instance->SetParameters(*Parameters);

  const materialscript* MainMaterial = GetMainMaterial();

  if(MainMaterial)
    Instance->ChangeMainMaterial(MainMaterial->Instantiate(), SpecialFlags|NO_PIC_UPDATE);

  const materialscript* SecondaryMaterial = GetSecondaryMaterial();

  if(SecondaryMaterial)
    Instance->ChangeSecondaryMaterial(SecondaryMaterial->Instantiate(), SpecialFlags|NO_PIC_UPDATE);

  const materialscript* ContainedMaterial = GetContainedMaterial();

  if(ContainedMaterial)
    Instance->ChangeContainedMaterial(ContainedMaterial->Instantiate(), SpecialFlags|NO_PIC_UPDATE);

  if(!(SpecialFlags & NO_PIC_UPDATE))
    Instance->UpdatePictures();

  return Instance;
}

/* Called by an inline function in script.h... */

template glterrain* contentscripttemplate<glterrain>::BasicInstantiate(ushort) const;

template <class type> ushort contentscripttemplate<type>::SearchCodeName(const std::string& String) const
{
  return protocontainer<type>::SearchCodeName(String);
}

/* GCC 2.952 SUCKS!!! IT MUST BURN!!! */

template ushort contentscripttemplate<character>::SearchCodeName(const std::string&) const;
template ushort contentscripttemplate<item>::SearchCodeName(const std::string&) const;
template ushort contentscripttemplate<glterrain>::SearchCodeName(const std::string&) const;
template ushort contentscripttemplate<olterrain>::SearchCodeName(const std::string&) const;

const char* contentscript<character>::GetClassId() const { return "character"; }
const char* contentscript<item>::GetClassId() const { return "item"; }
const char* contentscript<glterrain>::GetClassId() const { return "glterrain"; }
const char* contentscript<olterrain>::GetClassId() const { return "olterrain"; }

void contentscript<character>::InitDataMap()
{
  INIT_MEMBER(Team);
  INIT_MEMBER(Inventory);
  INIT_MEMBER(IsMaster);
  INIT_MEMBER(WayPoint);
}

character* contentscript<character>::Instantiate(ushort SpecialFlags) const
{
  character* Instance = contentscripttemplate<character>::BasicInstantiate(SpecialFlags);
  const ushort* Team = GetTeam();

  if(Team)
    Instance->SetTeam(game::GetTeam(*Team));

  const std::list<contentscript<item> >* Inventory = GetInventory();

  if(Inventory)
    Instance->AddToInventory(*Inventory, SpecialFlags);

  const std::vector<vector2d>* WayPoint = GetWayPoint();

  if(WayPoint)
    Instance->SetWayPoints(*WayPoint);

  Instance->RestoreHP();
  return Instance;
}

void contentscript<item>::InitDataMap()
{
  INIT_MEMBER(Team);
  INIT_MEMBER(IsActive);
  INIT_MEMBER(SideStackIndex);
  INIT_MEMBER(Enchantment);
  INIT_MEMBER(MinPrice);
  INIT_MEMBER(MaxPrice);
  INIT_MEMBER(Category);
  INIT_MEMBER(ItemsInside);
  INIT_MEMBER(Chance);
}

item* contentscript<item>::Instantiate(ushort SpecialFlags) const
{
  const uchar* Chance = GetChance();

  if(Chance && *Chance <= RAND() % 100)
    return 0;

  item* Instance;

  if(Random)
    {
      const ulong* MinPrice = GetMinPrice();
      const ulong* MaxPrice = GetMaxPrice();
      const ulong* Category = GetCategory();
      Instance = protosystem::BalancedCreateItem(MinPrice ? *MinPrice : 0, MaxPrice ? *MaxPrice : MAX_PRICE, Category ? *Category : ANY_CATEGORY);
    }
  else
    Instance = contentscripttemplate<item>::BasicInstantiate(SpecialFlags);

  const ushort* Team = GetTeam();

  if(Team)
    Instance->SetTeam(*Team);

  const bool* Active = IsActive();

  if(Active)
    Instance->SetIsActive(*Active);

  const short* Enchantment = GetEnchantment();

  if(Enchantment)
    Instance->SetEnchantment(*Enchantment);

  const std::list<contentscript<item> >* ItemsInside = GetItemsInside();

  if(ItemsInside)
    Instance->SetItemsInside(*ItemsInside, SpecialFlags);

  return Instance;
}

void contentscript<olterrain>::InitDataMap()
{
  INIT_MEMBER(VisualEffects);
  INIT_MEMBER(AttachedArea);
  INIT_MEMBER(AttachedEntry);
  INIT_MEMBER(Text);
  INIT_MEMBER(ItemsInside);
}

olterrain* contentscript<olterrain>::Instantiate(ushort SpecialFlags) const
{
  olterrain* Instance = contentscripttemplate<olterrain>::BasicInstantiate(SpecialFlags);

  const uchar* VisualEffects = GetVisualEffects();

  if(VisualEffects)
    {
      Instance->SetVisualEffects(*VisualEffects);
      Instance->UpdatePictures();
    }

  const uchar* AttachedArea = GetAttachedArea();

  if(AttachedArea)
    Instance->SetAttachedArea(*AttachedArea);

  const uchar* AttachedEntry = GetAttachedEntry();

  if(AttachedEntry)
    Instance->SetAttachedEntry(*AttachedEntry);

  const std::string* Text = GetText();

  if(Text)
    Instance->SetText(*Text);

  const std::list<contentscript<item> >* ItemsInside = GetItemsInside();

  if(ItemsInside)
    Instance->SetItemsInside(*ItemsInside, SpecialFlags);

  return Instance;
}

void squarescript::InitDataMap()
{
  INIT_MEMBER(Position);
  INIT_MEMBER(Character);
  INIT_MEMBER(Items);
  INIT_MEMBER(GTerrain);
  INIT_MEMBER(OTerrain);
  INIT_MEMBER(Times);
  INIT_MEMBER(AttachRequired);
  INIT_MEMBER(EntryIndex);
}

void squarescript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word != "=")
    {
      PositionHolder.ReadFrom(SaveFile);

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in square script line %d!", SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	if(!ReadMember(SaveFile, Word))
	  ABORT("Odd script term %s encountered in square script line %d!", Word.c_str(), SaveFile.TellLine());
    }
  else
    {
      GTerrainHolder.ReadFrom(SaveFile);
      OTerrainHolder.ReadFrom(SaveFile);
    }
}

template <class type, class contenttype> contentmap<type, contenttype>::contentmap<type, contenttype>() : ContentMap(0) { }

template <class type, class contenttype> contentmap<type, contenttype>::~contentmap<type, contenttype>()
{
  delete [] ContentMap;
}

template <class type, class contenttype> void contentmap<type, contenttype>::InitDataMap()
{
  INIT_MEMBER(Size);
  INIT_MEMBER(Pos);
}

template <class type, class contenttype> void contentmap<type, contenttype>::ReadFrom(inputfile& SaveFile)
{
  typedef std::map<char, contenttype> maptype;
  typedef typename maptype::iterator mapiterator;

  if(ContentMap)
    ABORT("Illegal %s content map redefinition on line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

  SymbolMap.insert(std::pair<char, contenttype>('.', contenttype()));
  std::string Word1, Word2;

  for(Word1 = SaveFile.ReadWord(); Word1 != "}"; Word1 = SaveFile.ReadWord())
    {
      if(Word1 == "Types")
	{
	  if(SaveFile.ReadWord() != "{")
	    ABORT("Missing bracket in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

	  for(Word2 = SaveFile.ReadWord(); Word2 != "}"; Word2 = SaveFile.ReadWord())
	    {
	      std::pair<mapiterator, bool> Return = SymbolMap.insert(std::pair<char, contenttype>(Word2[0], contenttype()));

	      if(Return.second)
		ReadData(Return.first->second, SaveFile);
	      else
		ABORT("Symbol %c defined again in %s content map script line %d!", Word2[0], protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
	    }

	  continue;
	}

      if(!ReadMember(SaveFile, Word1))
	ABORT("Odd script term %s encountered in %s content script line %d!", Word1.c_str(), protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
    }

  vector2d Size = *GetSize();
  Alloc2D(ContentMap, Size.X, Size.Y);

  if(SaveFile.ReadWord() != "{")
    ABORT("Missing bracket in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

  for(ushort y = 0; y < Size.Y; ++y)
    for(ushort x = 0; x < Size.X; ++x)
      {
	char Char = SaveFile.ReadLetter();
	typename std::map<char, contenttype>::iterator i = SymbolMap.find(Char);

	if(i != SymbolMap.end())
	  ContentMap[x][y] = std::pair<char, contenttype*>(Char, &i->second);
	else
	  ABORT("Illegal content %c in %s content map line %d!", Char, protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
      }

  if(SaveFile.ReadWord() != "}")
    ABORT("Missing bracket in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
}

template <class type, class contenttype> void contentmap<type, contenttype>::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << SymbolMap;
  vector2d Size = *GetSize();

  for(ushort y = 0; y < Size.Y; ++y)
    for(ushort x = 0; x < Size.X; ++x)
      SaveFile << ContentMap[x][y].first;
}

template <class type, class contenttype> void contentmap<type, contenttype>::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> SymbolMap;
  vector2d Size = *GetSize();
  Alloc2D(ContentMap, Size.X, Size.Y);

  for(ushort y = 0; y < Size.Y; ++y)
    for(ushort x = 0; x < Size.X; ++x)
      {
	char Char = ReadType<char>(SaveFile);
	ContentMap[x][y] = std::pair<char, contenttype*>(Char, &SymbolMap.find(Char)->second);
      }
}

/*template charactercontentmap;
template itemcontentmap;
template glterraincontentmap;
template olterraincontentmap;*/

void roomscript::InitDataMap()
{
  INIT_MEMBER(CharacterMap);
  INIT_MEMBER(ItemMap);
  INIT_MEMBER(GTerrainMap);
  INIT_MEMBER(OTerrainMap);
  INIT_MEMBER(WallSquare);
  INIT_MEMBER(FloorSquare);
  INIT_MEMBER(DoorSquare);
  INIT_MEMBER(Size);
  INIT_MEMBER(Pos);
  INIT_MEMBER(AltarPossible);
  INIT_MEMBER(GenerateDoor);
  INIT_MEMBER(GenerateTunnel);
  INIT_MEMBER(DivineMaster);
  INIT_MEMBER(GenerateLanterns);
  INIT_MEMBER(Type);
  INIT_MEMBER(GenerateFountains);
  INIT_MEMBER(AllowLockedDoors);
  INIT_MEMBER(AllowBoobyTrappedDoors);
  INIT_MEMBER(Shape);
}

void roomscript::ReadFrom(inputfile& SaveFile)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in room script line %d!", SaveFile.TellLine());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Square")
	{
	  Square.push_back(squarescript());
	  Square.back().ReadFrom(SaveFile);
	  continue;
	}

      if(!ReadMember(SaveFile, Word))
	ABORT("Odd script term %s encountered in room script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

void roomscript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Square;
}

void roomscript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Square;
}

void levelscript::InitDataMap()
{
  INIT_MEMBER(RoomDefault);
  INIT_MEMBER(FillSquare);
  INIT_MEMBER(LevelMessage);
  INIT_MEMBER(Size);
  INIT_MEMBER(Items);
  INIT_MEMBER(Rooms);
  INIT_MEMBER(GenerateMonsters);
  INIT_MEMBER(IsOnGround);
  INIT_MEMBER(TeamDefault);
  INIT_MEMBER(AmbientLight);
  INIT_MEMBER(Description);
  INIT_MEMBER(LOSModifier);
  INIT_MEMBER(IgnoreDefaultSpecialSquares);
  INIT_MEMBER(DifficultyBase);
  INIT_MEMBER(DifficultyDelta);
  INIT_MEMBER(MonsterAmountBase);
  INIT_MEMBER(MonsterAmountDelta);
  INIT_MEMBER(MonsterGenerationIntervalBase);
  INIT_MEMBER(MonsterGenerationIntervalDelta);
  INIT_MEMBER(AutoReveal);
  INIT_MEMBER(ShortDescription);
}

void levelscript::ReadFrom(inputfile& SaveFile)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in level script line %d!", SaveFile.TellLine());

  if(Base)
    {
      const vector2d* Size = static_cast<const levelscript*>(Base)->GetSize();

      if(Size)
	{
	  game::GetGlobalValueMap()["XSize"] = Size->X;
	  game::GetGlobalValueMap()["YSize"] = Size->Y;
	}
    }

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Square")
	{
	  Square.push_back(squarescript());
	  Square.back().ReadFrom(SaveFile);
	  continue;
	}

      if(Word == "Room")
	{
	  Room.push_back(roomscript());
	  const roomscript* RoomDefault = GetRoomDefault();

	  if(RoomDefault)
	    Room.back().SetBase(RoomDefault);

	  Room.back().ReadFrom(SaveFile);
	  continue;
	}

      if(!ReadMember(SaveFile, Word))
	ABORT("Odd script term %s encountered in level script line %d!", Word.c_str(), SaveFile.TellLine());

      if(Word == "Size")
	{
	  game::GetGlobalValueMap()["XSize"] = GetSize()->X;
	  game::GetGlobalValueMap()["YSize"] = GetSize()->Y;
	}
    }

  const levelscript* LevelBase = static_cast<const levelscript*>(Base);

  if(LevelBase && RoomDefaultHolder.GetMember())
    RoomDefaultHolder.GetMember()->SetBase(LevelBase->RoomDefaultHolder.GetMember());

  valuemap::iterator i = game::GetGlobalValueMap().find("XSize");

  if(i != game::GetGlobalValueMap().end())
    game::GetGlobalValueMap().erase(i);

  i = game::GetGlobalValueMap().find("YSize");

  if(i != game::GetGlobalValueMap().end())
    game::GetGlobalValueMap().erase(i);
}

void levelscript::Combine(levelscript& Script)
{
  if(!Base)
    Base = Script.Base;

  Square.splice(Square.end(), Script.Square);
  Room.splice(Room.end(), Script.Room);

  for(datamap::const_iterator i = DataMap.begin(); i != DataMap.end(); ++i)
    (this->*i->second).Replace(Script.*i->second);
}

void levelscript::SetBase(const scriptwithbase* What)
{
  const levelscript* LevelBase = static_cast<const levelscript*>(Base = What);
  roomscript* BaseRoomDefault = LevelBase->RoomDefaultHolder.GetMember();

  if(BaseRoomDefault)
    {
      roomscript* ThisRoomDefault = RoomDefaultHolder.GetMember();

      if(!ThisRoomDefault)
	for(std::list<roomscript>::iterator i = Room.begin(); i != Room.end(); ++i)
	  i->SetBase(BaseRoomDefault);
      else
	ThisRoomDefault->SetBase(BaseRoomDefault);
    }
}

void levelscript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Square << Room;
}

void levelscript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Square >> Room;
  const roomscript* RoomDefault = GetRoomDefault();

  if(RoomDefault)
    for(std::list<roomscript>::iterator i = Room.begin(); i != Room.end(); ++i)
      i->SetBase(RoomDefault);
}

void dungeonscript::InitDataMap()
{
  INIT_MEMBER(LevelDefault);
  INIT_MEMBER(Levels);
  INIT_MEMBER(Description);
  INIT_MEMBER(ShortDescription);
}

void dungeonscript::ReadFrom(inputfile& SaveFile)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in dungeon script line %d!", SaveFile.TellLine());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Level")
	{
	  uchar Index = SaveFile.ReadNumber();
	  std::pair<std::map<uchar, levelscript>::iterator, bool> Return = Level.insert(std::pair<uchar, levelscript>(Index, levelscript()));

	  if(Return.second)
	    {
	      levelscript& LS = Return.first->second;
	      const levelscript* LevelDefault = GetLevelDefault();

	      if(LevelDefault)
		LS.SetBase(LevelDefault);

	      LS.ReadFrom(SaveFile);
	    }
	  else
	    ABORT("Level #%d defined again in dungeon script line %d!", Index, SaveFile.TellLine());

	  continue;
	}

      if(Word == "RandomLevel")
	{
	  interval Interval;
	  ReadData(Interval, SaveFile);
	  RandomLevel.push_back(std::pair<interval, levelscript>(Interval, levelscript()));
	  const levelscript* LevelDefault = GetLevelDefault();

	  if(LevelDefault)
	    RandomLevel.back().second.SetBase(LevelDefault);

	  RandomLevel.back().second.ReadFrom(SaveFile);
	  continue;
	}

      if(!ReadMember(SaveFile, Word))
	ABORT("Odd script term %s encountered in dungeon script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

void dungeonscript::RandomizeLevels()
{
  for(std::list<std::pair<interval, levelscript> >::iterator i = RandomLevel.begin(); i != RandomLevel.end(); ++i)
    {
      uchar Index = i->first.Randomize();
      Level[Index].Combine(i->second);
    }

  RandomLevel.clear();
}

void dungeonscript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Level << RandomLevel;
}

void dungeonscript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Level >> RandomLevel;
  const levelscript* LevelDefault = GetLevelDefault();

  if(LevelDefault)
    {
      for(std::map<uchar, levelscript>::iterator i1 = Level.begin(); i1 != Level.end(); ++i1)
	i1->second.SetBase(LevelDefault);

      for(std::list<std::pair<interval, levelscript> >::iterator i2 = RandomLevel.begin(); i2 != RandomLevel.end(); ++i2)
	i2->second.SetBase(LevelDefault);
    }
}

void teamscript::InitDataMap()
{
  INIT_MEMBER(KillEvilness);
}

void teamscript::ReadFrom(inputfile& SaveFile)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in team script line %d!", SaveFile.TellLine());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Relation")
	{
	  std::pair<uchar, uchar> Rel;
	  Rel.first = SaveFile.ReadNumber();
	  Rel.second = SaveFile.ReadNumber();
	  Relation.push_back(Rel);
	  continue;
	}

      if(!ReadMember(SaveFile, Word))
	ABORT("Odd script term %s encountered in team script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

void teamscript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Relation;
}

void teamscript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Relation;
}

void gamescript::InitDataMap()
{
  INIT_MEMBER(Dungeons);
  INIT_MEMBER(Teams);
}

void gamescript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word == "Dungeon")
	{
	  ushort Index = SaveFile.ReadNumber();
	  std::pair<std::map<uchar, dungeonscript>::iterator, bool> Return = Dungeon.insert(std::pair<uchar, dungeonscript>(Index, dungeonscript()));

	  if(Return.second)
	    Return.first->second.ReadFrom(SaveFile);
	  else
	    ABORT("Dungeon #%d defined again in game script line %d!", Index, SaveFile.TellLine());

	  continue;
	}

      if(Word == "Team")
	{
	  ushort Index = SaveFile.ReadNumber();
	  Team.push_back(std::pair<uchar, teamscript>(Index, teamscript()));
	  Team.back().second.ReadFrom(SaveFile);
	  continue;
	}

      if(!ReadMember(SaveFile, Word))
	ABORT("Odd script term %s encountered in game script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

void gamescript::RandomizeLevels()
{
  for(std::map<uchar, dungeonscript>::iterator i = Dungeon.begin(); i != Dungeon.end(); ++i)
    i->second.RandomizeLevels();
}

void gamescript::Save(outputfile& SaveFile) const
{
  script::Save(SaveFile);
  SaveFile << Team << Dungeon;
}

void gamescript::Load(inputfile& SaveFile)
{
  script::Load(SaveFile);
  SaveFile >> Team >> Dungeon;
}

outputfile& operator<<(outputfile& SaveFile, const gamescript* Script)
{
  Script->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, gamescript*& Script)
{
  Script = new gamescript;
  Script->Load(SaveFile);
  return SaveFile;
}

void scriptsystem::Initialize()
{
  posscript::InitDataMap();
  materialscript::InitDataMap();
  basecontentscript::InitDataMap();
  contentscript<character>::InitDataMap();
  contentscript<item>::InitDataMap();
  contentscript<glterrain>::InitDataMap();
  contentscript<olterrain>::InitDataMap();
  squarescript::InitDataMap();
  itemcontentmap::InitDataMap();
  charactercontentmap::InitDataMap();
  glterraincontentmap::InitDataMap();
  olterraincontentmap::InitDataMap();
  roomscript::InitDataMap();
  levelscript::InitDataMap();
  dungeonscript::InitDataMap();
  teamscript::InitDataMap();
  gamescript::InitDataMap();
}
