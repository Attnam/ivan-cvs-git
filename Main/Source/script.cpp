#include "script.h"
#include "lterraba.h"
#include "charba.h"
#include "allocate.h"
#include "team.h"
#include "itemba.h"
#include "roomba.h"
#include "godba.h"
#include "error.h"
#include "game.h"
#include "proto.h"
#include "save.h"
#include "materba.h"
#include "femath.h"

#define ANALYZE_MEMBER(name)\
{\
  if(Identifier == #name)\
    return &name##Holder;\
}

bool script::LoadData(inputfile& SaveFile, const std::string& Word)
{
  datamemberbase* Data = GetData(Word);

  if(Data)
    {
      Data->Load(SaveFile);
      return true;
    }
  else
    return false;
}

datamemberbase* posscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Vector);
  ANALYZE_MEMBER(Flags);
  ANALYZE_MEMBER(Borders);
  return 0;
}

void posscript::ReadFrom(inputfile& SaveFile, bool)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "Pos")
    {
      Random = false;
      VectorHolder.Load(SaveFile);
    }

  if(Word == "Random")
    {
      Random = true;
      FlagsHolder.Load(SaveFile);
    }

  if(Word == "BoundedRandom")
    {
      Random = true;
      BordersHolder.Load(SaveFile);
      FlagsHolder.Load(SaveFile);
    }
}

datamemberbase* materialscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Volume);
  return 0;
}

void materialscript::ReadFrom(inputfile& SaveFile, bool)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    Word = SaveFile.ReadWord();

  valuemap::const_iterator Iterator = game::GetGlobalValueMap().find(Word);

  if(Iterator != game::GetGlobalValueMap().end())
    Config = Iterator->second;
  else
    ABORT("Unconfigured material script detected at line %d!", SaveFile.TellLine());

  if(SaveFile.ReadWord() != "{")
    return;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    if(!LoadData(SaveFile, Word))
      ABORT("Odd script term %s encountered in material script line %d!", Word.c_str(), SaveFile.TellLine());
}

material* materialscript::Instantiate() const
{
  material* Instance = MAKE_MATERIAL(Config);
  ulong* Volume = GetVolume();

  if(Volume)
    Instance->SetVolume(*Volume);

  return Instance;
}

datamemberbase* basecontentscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(MainMaterial);
  ANALYZE_MEMBER(SecondaryMaterial);
  ANALYZE_MEMBER(ContainedMaterial);
  ANALYZE_MEMBER(Parameters);
  return 0;
}

void basecontentscript::ReadFrom(inputfile& SaveFile, bool)
{
  std::string Word = SaveFile.ReadWord();

  if(Word == "=" || Word == ",")
    Word = SaveFile.ReadWord();

  valuemap::const_iterator Iterator = game::GetGlobalValueMap().find(Word);

  if(Iterator != game::GetGlobalValueMap().end())
    {
      if(!GetMainMaterial())
	MainMaterialHolder.SetMember(new materialscript);

      GetMainMaterial()->SetConfig(Iterator->second);
      Word = SaveFile.ReadWord();
      Iterator = game::GetGlobalValueMap().find(Word);

      if(Iterator != game::GetGlobalValueMap().end())
	{
	  if(!GetSecondaryMaterial())
	    SecondaryMaterialHolder.SetMember(new materialscript);

	  GetSecondaryMaterial()->SetConfig(Iterator->second);
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
	if(!LoadData(SaveFile, Word))
	  ABORT("Odd script term %s encountered in %s content script, file %s line %d!", Word.c_str(), GetClassId(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
      }
  else
    if(Word != ";" && Word != ",")
      ABORT("Odd terminator %s encountered in %s content script, file %s line %d!", Word.c_str(), GetClassId(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

template <class type> type* contentscripttemplate<type>::BasicInstantiate(ushort SpecialFlags) const
{
  type* Instance;
  const typename type::prototype* Proto = protocontainer<type>::GetProto(ContentType);

  if(!Config && Proto->IsAbstract())
    {
      const typename type::databasemap& Config = Proto->GetConfig();
      ushort ChosenConfig = 1 + RAND() % (Config.size() - 1);
      typename type::databasemap::const_iterator i;
      for(i = Config.begin(); ChosenConfig; ++i, --ChosenConfig);
      Instance = Proto->Clone(i->first, SpecialFlags|NO_PIC_UPDATE);
    }
  else
    Instance = Proto->Clone(Config, SpecialFlags|NO_PIC_UPDATE);

  ulong* Parameters = GetParameters();

  if(Parameters)
    Instance->SetParameters(*Parameters);

  materialscript* MainMaterial = GetMainMaterial();

  if(MainMaterial)
    Instance->ChangeMainMaterial(MainMaterial->Instantiate(), SpecialFlags|NO_PIC_UPDATE);

  materialscript* SecondaryMaterial = GetSecondaryMaterial();

  if(SecondaryMaterial)
    Instance->ChangeSecondaryMaterial(SecondaryMaterial->Instantiate(), SpecialFlags|NO_PIC_UPDATE);

  materialscript* ContainedMaterial = GetContainedMaterial();

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

datamemberbase* contentscript<character>::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Team);
  ANALYZE_MEMBER(Inventory);
  ANALYZE_MEMBER(IsMaster);
  ANALYZE_MEMBER(WayPoint);
  return contentscripttemplate<character>::GetData(Identifier);
}

character* contentscript<character>::Instantiate(ushort SpecialFlags) const
{
  character* Instance = contentscripttemplate<character>::BasicInstantiate(SpecialFlags);
  ushort* Team = GetTeam();

  if(Team)
    Instance->SetTeam(game::GetTeam(*Team));

  std::vector<contentscript<item> >* Inventory = GetInventory();

  if(Inventory)
    Instance->AddToInventory(*Inventory, SpecialFlags);

  std::vector<vector2d>* WayPoint = GetWayPoint();

  if(WayPoint)
    Instance->SetWayPoints(*WayPoint);

  Instance->RestoreHP();
  return Instance;
}

datamemberbase* contentscript<item>::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Team);
  ANALYZE_MEMBER(IsActive);
  ANALYZE_MEMBER(SideStackIndex);
  ANALYZE_MEMBER(Enchantment);
  ANALYZE_MEMBER(MinPrice);
  ANALYZE_MEMBER(MaxPrice);
  ANALYZE_MEMBER(Category);
  ANALYZE_MEMBER(ItemsInside);
  ANALYZE_MEMBER(Chance);
  return contentscripttemplate<item>::GetData(Identifier);
}

item* contentscript<item>::Instantiate(ushort SpecialFlags) const
{
  uchar* Chance = GetChance();

  if(Chance && *Chance <= RAND() % 100)
    return 0;

  item* Instance;

  if(Random)
    {
      ulong* MinPrice = GetMinPrice();
      ulong* MaxPrice = GetMaxPrice();
      ulong* Category = GetCategory();
      Instance = protosystem::BalancedCreateItem(MinPrice ? *MinPrice : 0, MaxPrice ? *MaxPrice : MAX_PRICE, Category ? *Category : ANY_CATEGORY);
    }
  else
    Instance = contentscripttemplate<item>::BasicInstantiate(SpecialFlags);

  ushort* Team = GetTeam();

  if(Team)
    Instance->SetTeam(*Team);

  bool* Active = IsActive();

  if(Active)
    Instance->SetIsActive(*Active);

  short* Enchantment = GetEnchantment();

  if(Enchantment)
    Instance->SetEnchantment(*Enchantment);

  std::vector<contentscript<item> >* ItemsInside = GetItemsInside();

  if(ItemsInside)
    Instance->SetItemsInside(*ItemsInside, SpecialFlags);

  return Instance;
}

datamemberbase* contentscript<olterrain>::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(VisualEffects);
  ANALYZE_MEMBER(AttachedArea);
  ANALYZE_MEMBER(AttachedEntry);
  ANALYZE_MEMBER(Text);
  ANALYZE_MEMBER(ItemsInside);
  return contentscripttemplate<olterrain>::GetData(Identifier);
}

olterrain* contentscript<olterrain>::Instantiate(ushort SpecialFlags) const
{
  olterrain* Instance = contentscripttemplate<olterrain>::BasicInstantiate(SpecialFlags);

  uchar* VisualEffects = GetVisualEffects();

  if(VisualEffects)
    {
      Instance->SetVisualEffects(*VisualEffects);
      Instance->UpdatePictures();
    }

  uchar* AttachedArea = GetAttachedArea();

  if(AttachedArea)
    Instance->SetAttachedArea(*AttachedArea);

  uchar* AttachedEntry = GetAttachedEntry();

  if(AttachedEntry)
    Instance->SetAttachedEntry(*AttachedEntry);

  std::string* Text = GetText();

  if(Text)
    Instance->SetText(*Text);

  std::vector<contentscript<item> >* ItemsInside = GetItemsInside();

  if(ItemsInside)
    Instance->SetItemsInside(*ItemsInside, SpecialFlags);

  return Instance;
}

datamemberbase* squarescript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Position);
  ANALYZE_MEMBER(Character);
  ANALYZE_MEMBER(Items);
  ANALYZE_MEMBER(GTerrain);
  ANALYZE_MEMBER(OTerrain);
  ANALYZE_MEMBER(Times);
  ANALYZE_MEMBER(AttachRequired);
  ANALYZE_MEMBER(EntryIndex);
  return 0;
}

void squarescript::ReadFrom(inputfile& SaveFile, bool)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word != "=")
    {
      PositionHolder.Load(SaveFile);

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in square script line %d!", SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	if(!LoadData(SaveFile, Word))
	  ABORT("Odd script term %s encountered in square script line %d!", Word.c_str(), SaveFile.TellLine());
    }
  else
    {
      GTerrainHolder.Load(SaveFile);
      OTerrainHolder.Load(SaveFile);
    }
}

template <class type, class contenttype> datamemberbase* contentmap<type, contenttype>::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Size);
  ANALYZE_MEMBER(Pos);
  return 0;
}

template <class type, class contenttype> void contentmap<type, contenttype>::DeleteContents()
{
  for(ushort y1 = 0; y1 < GetSize()->Y; ++y1)
    for(ushort x1 = 0; x1 < GetSize()->X; ++x1)
      {
	contenttype* CS = ContentMap[x1][y1];

	for(ushort y2 = 0; y2 < GetSize()->Y; ++y2)
	  for(ushort x2 = 0; x2 < GetSize()->X; ++x2)
	    if(ContentMap[x2][y2] == CS)
	      ContentMap[x2][y2] = 0;

	delete CS;
      }
}

template <class type, class contenttype> void contentmap<type, contenttype>::ReadFrom(inputfile& SaveFile, bool)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

  std::map<char, contenttype*> SymbolMap;
  SymbolMap.insert(std::pair<char, contenttype*>('.', 0));
  std::string Word1, Word2;

  for(Word1 = SaveFile.ReadWord(); Word1 != "}"; Word1 = SaveFile.ReadWord())
    {
      if(Word1 == "Types")
	{
	  if(SaveFile.ReadWord() != "{")
	    ABORT("Missing bracket in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

	  for(Word2 = SaveFile.ReadWord(); Word2 != "}"; Word2 = SaveFile.ReadWord())
	    {
	      contenttype* Content = new contenttype;
	      ReadData(*Content, SaveFile);

	      if(IsValidScript(*Content))
		SymbolMap.insert(std::pair<char, contenttype*>(Word2[0], Content));
	      else
		{
		  SymbolMap.insert(std::pair<char, contenttype*>(Word2[0], 0));
		  delete Content;
		}
	    }

	  continue;
	}

      if(!LoadData(SaveFile, Word1))
	ABORT("Odd script term %s encountered in %s content script line %d!", Word1.c_str(), protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
    }

  if(!ContentMap)
    Alloc2D(ContentMap, GetSize()->X, GetSize()->Y);
  else
    DeleteContents();

  if(SaveFile.ReadWord() != "{")
    ABORT("Missing bracket in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

  for(ushort y = 0; y < GetSize()->Y; ++y)
    for(ushort x = 0; x < GetSize()->X; ++x)
      {
	char Char = SaveFile.ReadLetter();
	typename std::map<char, contenttype*>::iterator Iterator = SymbolMap.find(Char);

	if(Iterator != SymbolMap.end())
	  ContentMap[x][y] = Iterator->second;
	else
	  ABORT("Illegal content %c in %s content map line %d!", Char, protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
      }

  if(SaveFile.ReadWord() != "}")
    ABORT("Missing bracket in %s content map script line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());
}

template charactercontentmap;
template itemcontentmap;
template glterraincontentmap;
template olterraincontentmap;

datamemberbase* roomscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(CharacterMap);
  ANALYZE_MEMBER(ItemMap);
  ANALYZE_MEMBER(GTerrainMap);
  ANALYZE_MEMBER(OTerrainMap);
  ANALYZE_MEMBER(WallSquare);
  ANALYZE_MEMBER(FloorSquare);
  ANALYZE_MEMBER(DoorSquare);
  ANALYZE_MEMBER(Size);
  ANALYZE_MEMBER(Pos);
  ANALYZE_MEMBER(AltarPossible);
  ANALYZE_MEMBER(GenerateDoor);
  ANALYZE_MEMBER(ReCalculate);
  ANALYZE_MEMBER(GenerateTunnel);
  ANALYZE_MEMBER(DivineMaster);
  ANALYZE_MEMBER(GenerateLanterns);
  ANALYZE_MEMBER(Type);
  ANALYZE_MEMBER(GenerateFountains);
  ANALYZE_MEMBER(AllowLockedDoors);
  ANALYZE_MEMBER(AllowBoobyTrappedDoors);
  ANALYZE_MEMBER(Shape);
  return 0;
}

roomscript::~roomscript()
{
  for(std::vector<squarescript*>::iterator i = Square.begin(); i != Square.end(); ++i)
    delete *i;
}

void roomscript::ReadFrom(inputfile& SaveFile, bool ReRead)
{
  if(ReRead)
    {
      roomscript* RoomBase = static_cast<roomscript*>(Base);

      if(RoomBase)
	{
	  bool* ReCalculateBool = RoomBase->ReCalculate();

	  if(ReCalculateBool && *ReCalculateBool)
	    RoomBase->ReadFrom(SaveFile, true);
	}

      bool* ReCalculateBool = ReCalculate();

      if(ReCalculateBool && *ReCalculateBool)
	{
	  SaveFile.ClearFlags();
	  SaveFile.SeekPosBegin(BufferPos);
	}
      else
	return;
    }
  else
    BufferPos = SaveFile.TellPos();

  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in room script line %d!", SaveFile.TellLine());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Square")
	{
	  squarescript* SS = new squarescript;
	  SS->ReadFrom(SaveFile);

	  if(!ReRead)
	    Square.push_back(SS);
	  else
	    delete SS;

	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in room script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

datamemberbase* levelscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(RoomDefault);
  ANALYZE_MEMBER(FillSquare);
  ANALYZE_MEMBER(LevelMessage);
  ANALYZE_MEMBER(Size);
  ANALYZE_MEMBER(Items);
  ANALYZE_MEMBER(Rooms);
  ANALYZE_MEMBER(GenerateMonsters);
  ANALYZE_MEMBER(ReCalculate);
  ANALYZE_MEMBER(IsOnGround);
  ANALYZE_MEMBER(TeamDefault);
  ANALYZE_MEMBER(AmbientLight);
  ANALYZE_MEMBER(Description);
  ANALYZE_MEMBER(LOSModifier);
  ANALYZE_MEMBER(IgnoreDefaultSpecialSquares);
  ANALYZE_MEMBER(DifficultyBase);
  ANALYZE_MEMBER(DifficultyDelta);
  ANALYZE_MEMBER(MonsterAmountBase);
  ANALYZE_MEMBER(MonsterAmountDelta);
  ANALYZE_MEMBER(MonsterGenerationIntervalBase);
  ANALYZE_MEMBER(MonsterGenerationIntervalDelta);
  ANALYZE_MEMBER(AutoReveal);
  ANALYZE_MEMBER(ShortDescription);
  return 0;
}

levelscript::~levelscript()
{
  for(std::vector<squarescript*>::iterator i1 = Square.begin(); i1 != Square.end(); ++i1)
    delete *i1;

  for(std::map<uchar, roomscript*>::iterator i2 = Room.begin(); i2 != Room.end(); ++i2)
    delete i2->second;
}

void levelscript::ReadFrom(inputfile& SaveFile, bool ReRead)
{
  if(ReRead)
    {
      levelscript* LevelBase = static_cast<levelscript*>(Base);

      if(LevelBase)
	{
	  bool* ReCalculateBool = LevelBase->ReCalculate();

	  if(ReCalculateBool && *ReCalculateBool)
	    LevelBase->ReadFrom(SaveFile, true);
	}

      bool* ReCalculateBool = ReCalculate();

      if(ReCalculateBool && *ReCalculateBool)
	{
	  SaveFile.ClearFlags();
	  SaveFile.SeekPosBegin(BufferPos);
	}
      else
	return;
    }
  else
    BufferPos = SaveFile.TellPos();

  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in level script line %d!", SaveFile.TellLine());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Square")
	{
	  squarescript* SS = new squarescript;
	  SS->ReadFrom(SaveFile);

	  if(!ReRead)
	    Square.push_back(SS);
	  else
	    delete SS;

	  continue;
	}

      if(Word == "Room")
	{
	  ushort Index = SaveFile.ReadNumber();
	  std::map<uchar, roomscript*>::iterator Iterator = Room.find(Index);
	  roomscript* RS = Iterator == Room.end() ? new roomscript : Iterator->second;
	  roomscript* RoomDefault = GetRoomDefault();

	  if(RoomDefault)
	    RS->SetBase(RoomDefault);

	  RS->ReadFrom(SaveFile, ReRead);
	  Room.insert(std::pair<uchar, roomscript*>(Index, RS));
	  continue;
	}

      /*if(Word == "Variable")
	{
	  SaveFile.ReadWord(Word);
	  ValueMap[Word] = SaveFile.ReadNumber(ValueMap);
	  continue;
	}*/

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in level script line %d!", Word.c_str(), SaveFile.TellLine());
    }

  levelscript* LevelBase = static_cast<levelscript*>(Base);

  if(LevelBase && RoomDefaultHolder.GetMember())
    RoomDefaultHolder.GetMember()->SetBase(LevelBase->RoomDefaultHolder.GetMember());
}

datamemberbase* dungeonscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(LevelDefault);
  ANALYZE_MEMBER(Levels);
  ANALYZE_MEMBER(Description);
  ANALYZE_MEMBER(ShortDescription);
  return 0;
}

dungeonscript::~dungeonscript()
{
  for(std::map<uchar, levelscript*>::iterator i = Level.begin(); i != Level.end(); ++i)
    delete i->second;
}

void dungeonscript::ReadFrom(inputfile& SaveFile, bool)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in dungeon script line %d!", SaveFile.TellLine());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      if(Word == "Level")
	{
	  ushort Index = SaveFile.ReadNumber();
	  std::map<uchar, levelscript*>::iterator Iterator = Level.find(Index);
	  levelscript* LS = Iterator == Level.end() ? new levelscript : Iterator->second;
	  levelscript* LevelDefault = GetLevelDefault();

	  if(LevelDefault)
	    LS->SetBase(LevelDefault);

	  LS->ReadFrom(SaveFile);
	  Level.insert(std::pair<uchar, levelscript*>(Index, LS));
	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in dungeon script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

datamemberbase* teamscript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(KillEvilness);
  return 0;
}

void teamscript::ReadFrom(inputfile& SaveFile, bool)
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

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in team script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

datamemberbase* gamescript::GetData(const std::string& Identifier)
{
  ANALYZE_MEMBER(Dungeons);
  ANALYZE_MEMBER(Teams);
  return 0;
}

gamescript::~gamescript()
{
  for(std::vector<std::pair<uchar, teamscript*> >::iterator i1 = Team.begin(); i1 != Team.end(); ++i1)
    delete i1->second;

  for(std::map<uchar, dungeonscript*>::iterator i2 = Dungeon.begin(); i2 != Dungeon.end(); ++i2)
    delete i2->second;
}

void gamescript::ReadFrom(inputfile& SaveFile, bool)
{
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word == "Dungeon")
	{
	  ushort Index = SaveFile.ReadNumber();
	  std::map<uchar, dungeonscript*>::iterator Iterator = Dungeon.find(Index);
	  dungeonscript* DS = Iterator == Dungeon.end() ? new dungeonscript : Iterator->second;
	  DS->ReadFrom(SaveFile);
	  Dungeon.insert(std::pair<uchar, dungeonscript*>(Index, DS));
	  continue;
	}

      if(Word == "Team")
	{
	  ushort Index = SaveFile.ReadNumber();
	  teamscript* TS = new teamscript;
	  TS->ReadFrom(SaveFile);
	  Team.push_back(std::pair<uchar, teamscript*>(Index, TS));
	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in game script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}
