#include <typeinfo>

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

#define INITMEMBER(name)\
{\
  name.SetIdentifier(#name);\
  Data.push_back(&name);\
}

template <class type> datamembertemplate<type>::~datamembertemplate<type>()
{
  delete Member;
}

template <class type> void datamembertemplate<type>::SetBase(datamemberbase* What)
{
  /* No type checking here, since only scriptwithbase<basetype>::SetBase uses this */

  Base = (datamembertemplate<type>*)What;
}

template <class type> bool datamember<type>::Load(const std::string& Word, inputfile& SaveFile, const valuemap& ValueMap, bool RequireIdentifier)
{
  if(!RequireIdentifier || Word == Identifier)
    {
      if(!Member)
	Member = new type;

      ReadData(*Member, SaveFile, ValueMap);
      return true;
    }

  return false;
}

template <class type> bool protonamedmember<type>::Load(const std::string& Word, inputfile& SaveFile, const valuemap&, bool RequireIdentifier)
{
  if(!RequireIdentifier || Word == Identifier)
    {
      if(!Member)
	Member = new ushort;

      ReadData(*Member, SaveFile, protocontainer<type>::GetCodeNameMap());
      return true;
    }

  return false;
}

bool script::LoadData(inputfile& SaveFile, const std::string& Word)
{
  ushort c;

  for(c = 0; c < Data.size(); ++c)
    if(Data[c]->Load(Word, SaveFile, ValueMap))
      break;

  return c != Data.size();
}

template <class basetype> void scriptwithbase<basetype>::SetBase(basetype* NewBase)
{
  if(NewBase == this)
    return;

  Base = NewBase;

  if(Base)
    for(ushort c = 0; c < Data.size(); ++c)
      Data[c]->SetBase(NewBase->Data[c]);
}

posscript::posscript()
{
  INITMEMBER(Vector);
  INITMEMBER(Walkable);
  INITMEMBER(InRoom);
}

void posscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "Pos")
    {
      Random = false;
      Vector.Load("", SaveFile, ValueMap, false);
    }

  if(Word == "Random")
    {
      Random = true;

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in position script line %d!", SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	if(!LoadData(SaveFile, Word))
	  ABORT("Odd script term %s encountered in position script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

materialscript::materialscript()
{
  INITMEMBER(Volume);
}

void materialscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    Word = SaveFile.ReadWord();

  valuemap::const_iterator Iterator = ValueMap.find(Word);

  if(Iterator != ValueMap.end())
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

  if(GetVolume(false))
    Instance->SetVolume(*GetVolume());

  return Instance;
}

basecontentscript::basecontentscript() : Config(0)
{
  INITMEMBER(MainMaterial);
  INITMEMBER(SecondaryMaterial);
  INITMEMBER(ContainedMaterial);
  INITMEMBER(Parameters);
}

void basecontentscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word = SaveFile.ReadWord();

  if(Word == "=")
    Word = SaveFile.ReadWord();

  valuemap::const_iterator Iterator = ValueMap.find(Word);

  if(Iterator != ValueMap.end())
    {
      if(!GetMainMaterial(false))
	MainMaterial.SetMember(new materialscript);

      GetMainMaterial()->SetConfig(Iterator->second);
      Word = SaveFile.ReadWord();
    }

  ContentType = SearchCodeName(Word);

  if(ContentType || Word == "0")
    Word = SaveFile.ReadWord();
  else
    ABORT("Odd script term %s encountered in %s content script line %d!", Word.c_str(), GetClassId().c_str(), SaveFile.TellLine());

  if(Word == "(")
    {
      Config = SaveFile.ReadNumber(ValueMap);
      Word = SaveFile.ReadWord();
    }

  if(Word == "{")
    for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
      {
	if(!LoadData(SaveFile, Word))
	  ABORT("Odd script term %s encountered in %s content script line %d!", Word.c_str(), GetClassId().c_str(), SaveFile.TellLine());
      }
  else
    if(Word != ";" && Word != ",")
      ABORT("Odd terminator %s encountered in %s content script line %d!", Word.c_str(), GetClassId().c_str(), SaveFile.TellLine());
}

template <class type> void contentscripttemplate<type>::BasicInstantiate(std::vector<type*>& Instance, ulong Amount) const
{
  const typename type::prototype* Proto = protocontainer<type>::GetProto(ContentType);
  Instance.resize(Amount, 0);

  if(!Config && Proto->IsAbstract())
    {
      const typename type::databasemap& Config = Proto->GetConfig();

      for(ulong c = 0; c < Amount; ++c)
	{
	  ushort ChosenConfig = RAND() % Config.size();

	  for(typename type::databasemap::const_iterator i = Config.begin(); i != Config.end(); ++i)
	    if(!ChosenConfig--)
	      {
		Instance[c] = Proto->Clone(i->first);
		break;
	      }
	}
    }
  else
    {
      for(ulong c = 0; c < Amount; ++c)
	Instance[c] = Proto->Clone(Config);
    }

  if(GetParameters(false))
    for(ulong c = 0; c < Amount; ++c)
      Instance[c]->SetParameters(*GetParameters());

  if(GetMainMaterial(false))
    for(ulong c = 0; c < Amount; ++c)
      Instance[c]->ChangeMainMaterial(GetMainMaterial()->Instantiate());

  if(GetSecondaryMaterial(false))
    for(ulong c = 0; c < Amount; ++c)
      Instance[c]->ChangeSecondaryMaterial(GetSecondaryMaterial()->Instantiate());

  if(GetContainedMaterial(false))
    for(ulong c = 0; c < Amount; ++c)
      Instance[c]->ChangeContainedMaterial(GetContainedMaterial()->Instantiate());
}

template <class type> ushort contentscripttemplate<type>::SearchCodeName(const std::string& Word) const
{
  return protocontainer<type>::SearchCodeName(Word);
}

contentscript<character>::contentscript<character>()
{
  INITMEMBER(Team);
}

void contentscript<character>::Instantiate(std::vector<character*>& Instance, ulong Amount) const
{
  contentscripttemplate<character>::BasicInstantiate(Instance, Amount);

  if(GetTeam(false))
    for(ulong c = 0; c < Amount; ++c)
      Instance[c]->SetTeam(game::GetTeam(*GetTeam()));
}

character* contentscript<character>::Instantiate() const
{
  std::vector<character*> Instance;
  Instantiate(Instance, 1);
  return Instance[0];
}

contentscript<item>::contentscript<item>()
{
  INITMEMBER(IsVisible);
}

void contentscript<item>::Instantiate(std::vector<item*>& Instance, ulong Amount) const
{
  contentscripttemplate<item>::BasicInstantiate(Instance, Amount);

  if(GetIsVisible(false))
    for(ulong c = 0; c < Amount; ++c)
      Instance[c]->SetIsVisible(*GetIsVisible());
}

item* contentscript<item>::Instantiate() const
{
  std::vector<item*> Instance;
  Instantiate(Instance, 1);
  return Instance[0];
}

void contentscript<glterrain>::Instantiate(std::vector<glterrain*>& Instance, ulong Amount) const
{
  contentscripttemplate<glterrain>::BasicInstantiate(Instance, Amount);
}

glterrain* contentscript<glterrain>::Instantiate() const
{
  std::vector<glterrain*> Instance;
  Instantiate(Instance, 1);
  return Instance[0];
}

contentscript<olterrain>::contentscript<olterrain>()
{
  INITMEMBER(VisualEffects);
}

void contentscript<olterrain>::Instantiate(std::vector<olterrain*>& Instance, ulong Amount) const
{
  contentscripttemplate<olterrain>::BasicInstantiate(Instance, Amount);

  if(GetVisualEffects(false))
    for(ulong c = 0; c < Amount; ++c)
      {
	Instance[c]->SetVisualEffects(*GetVisualEffects());
	Instance[c]->UpdatePictures();
      }
}

olterrain* contentscript<olterrain>::Instantiate() const
{
  std::vector<olterrain*> Instance;
  Instantiate(Instance, 1);
  return Instance[0];
}

squarescript::squarescript()
{
  INITMEMBER(Position);
  INITMEMBER(Character);
  INITMEMBER(Item);
  INITMEMBER(GTerrain);
  INITMEMBER(OTerrain);
  INITMEMBER(IsUpStairs);
  INITMEMBER(IsDownStairs);
  INITMEMBER(IsWorldMapEntry);
  INITMEMBER(Times);
}

void squarescript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word != "=")
    {
      Position.Load("", SaveFile, ValueMap, false);

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in square script line %d!", SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	if(!LoadData(SaveFile, Word))
	  ABORT("Odd script term %s encountered in square script line %d!", Word.c_str(), SaveFile.TellLine());
    }
  else
    {
      GTerrain.Load("", SaveFile, ValueMap, false);
      OTerrain.Load("", SaveFile, ValueMap, false);
    }
}

template <class type> contentmap<type>::contentmap<type>() : ContentScriptMap(0)
{
  INITMEMBER(Size);
  INITMEMBER(Pos);
}

template <class type> void contentmap<type>::DeleteContents()
{
  for(ushort y1 = 0; y1 < GetSize()->Y; ++y1)
    for(ushort x1 = 0; x1 < GetSize()->X; ++x1)
      {
	contentscript<type>* CS = ContentScriptMap[x1][y1];

	for(ushort y2 = 0; y2 < GetSize()->Y; ++y2)
	  for(ushort x2 = 0; x2 < GetSize()->X; ++x2)
	    if(ContentScriptMap[x2][y2] == CS)
	      ContentScriptMap[x2][y2] = 0;

	delete CS;
      }
}

template <class type> void contentmap<type>::ReadFrom(inputfile& SaveFile)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in %s content map script line %d!", typeid(type).name(), SaveFile.TellLine());

  std::map<char, contentscript<type>*> SymbolMap;

  SymbolMap['.'] = 0;

  for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
    {
      if(Word == "Types")
	{
	  if(SaveFile.ReadWord() != "{")
	    ABORT("Missing bracket in %s content map script line %d!", typeid(type).name(), SaveFile.TellLine());

	  for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
	    {
	      contentscript<type>* ContentScript = new contentscript<type>;
	      ContentScript->SetValueMap(ValueMap);
	      ContentScript->ReadFrom(SaveFile);

	      if(ContentScript->GetContentType())
		SymbolMap[Word[0]] = ContentScript;
	      else
		{
		  SymbolMap[Word[0]] = 0;
		  delete ContentScript;
		}
	    }

	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in %s content script line %d!", Word.c_str(), typeid(type).name(), SaveFile.TellLine());
    }

  if(!ContentScriptMap)
    Alloc2D(ContentScriptMap, GetSize()->X, GetSize()->Y);
  else
    DeleteContents();

  if(SaveFile.ReadWord() != "{")
    ABORT("Missing bracket in %s content map script line %d!", typeid(type).name(), SaveFile.TellLine());

  for(ushort y = 0; y < GetSize()->Y; ++y)
    for(ushort x = 0; x < GetSize()->X; ++x)
      {
	char Char = SaveFile.ReadLetter();

	typename std::map<char, contentscript<type>*>::iterator Iterator = SymbolMap.find(Char);

	if(Iterator != SymbolMap.end())
	  ContentScriptMap[x][y] = Iterator->second;
	else
	  ABORT("Illegal content %c in %s content map line %d!", Char, typeid(type).name(), SaveFile.TellLine());
      }

  if(SaveFile.ReadWord() != "}")
    ABORT("Missing bracket in %s content map script line %d!", typeid(type).name(), SaveFile.TellLine());
}

roomscript::roomscript()
{
  INITMEMBER(CharacterMap);
  INITMEMBER(ItemMap);
  INITMEMBER(GTerrainMap);
  INITMEMBER(OTerrainMap);
  INITMEMBER(WallSquare);
  INITMEMBER(FloorSquare);
  INITMEMBER(DoorSquare);
  INITMEMBER(Size);
  INITMEMBER(Pos);
  INITMEMBER(AltarPossible);
  INITMEMBER(GenerateDoor);
  INITMEMBER(ReCalculate);
  INITMEMBER(GenerateTunnel);
  INITMEMBER(DivineMaster);
  INITMEMBER(GenerateLanterns);
  INITMEMBER(Type);
  INITMEMBER(GenerateFountains);
  INITMEMBER(AllowLockedDoors);
  INITMEMBER(AllowBoobyTrappedDoors);
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
      if(Base)
	if(Base->GetReCalculate(false) && *Base->GetReCalculate(false))
	  Base->ReadFrom(SaveFile, true);

      if(GetReCalculate(false) && *GetReCalculate(false))
	{
	  SaveFile.ClearFlags();
	  SaveFile.SeekPosBeg(BufferPos);
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
	  SS->SetValueMap(ValueMap);
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

levelscript::levelscript()
{
  INITMEMBER(RoomDefault);
  INITMEMBER(FillSquare);
  INITMEMBER(LevelMessage);
  INITMEMBER(Size);
  INITMEMBER(Items);
  INITMEMBER(Rooms);
  INITMEMBER(GenerateMonsters);
  INITMEMBER(ReCalculate);
  INITMEMBER(GenerateUpStairs);
  INITMEMBER(GenerateDownStairs);
  INITMEMBER(OnGround);
  INITMEMBER(TeamDefault);
  INITMEMBER(AmbientLight);
  INITMEMBER(Description);
  INITMEMBER(LOSModifier);
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
      if(Base)
	if(Base->GetReCalculate(false) && *Base->GetReCalculate(false))
	  Base->ReadFrom(SaveFile, true);

      if(GetReCalculate(false) && *GetReCalculate())
	{
	  SaveFile.ClearFlags();
	  SaveFile.SeekPosBeg(BufferPos);
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
	  SS->SetValueMap(ValueMap);
	  SS->ReadFrom(SaveFile);

	  if(!ReRead)
	    Square.push_back(SS);
	  else
	    delete SS;

	  continue;
	}

      if(Word == "Room")
	{
	  ushort Index = SaveFile.ReadNumber(ValueMap);
	  std::map<uchar, roomscript*>::iterator Iterator = Room.find(Index);
	  roomscript* RS = Iterator == Room.end() ? new roomscript : Iterator->second;

	  RS->SetValueMap(ValueMap);

	  if(GetRoomDefault(false))
	    RS->SetBase(GetRoomDefault());

	  RS->ReadFrom(SaveFile, ReRead);
	  Room[Index] = RS;
	  continue;
	}

      if(Word == "Variable")
	{
	  SaveFile.ReadWord(Word);
	  ValueMap[Word] = SaveFile.ReadNumber(ValueMap);
	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in level scrip line %d!", Word.c_str(), SaveFile.TellLine());
    }

  if(Base && Base->GetRoomDefault(false) && GetRoomDefault(false))
    GetRoomDefault(false)->SetBase(Base->GetRoomDefault(false));
}

dungeonscript::dungeonscript()
{
  INITMEMBER(LevelDefault);
  INITMEMBER(Levels);
}

dungeonscript::~dungeonscript()
{
  for(std::map<uchar, levelscript*>::iterator i = Level.begin(); i != Level.end(); ++i)
    delete i->second;
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
	  ushort Index = SaveFile.ReadNumber(ValueMap);
	  std::map<uchar, levelscript*>::iterator Iterator = Level.find(Index);
	  levelscript* LS = Iterator == Level.end() ? new levelscript : Iterator->second;
	  LS->SetValueMap(ValueMap);

	  if(GetLevelDefault(false))
	    LS->SetBase(GetLevelDefault());

	  LS->ReadFrom(SaveFile);
	  Level[Index] = LS;
	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in dungeon script line %d!", Word.c_str(), SaveFile.TellLine());
    }

  if(Base && Base->GetLevelDefault(false) && GetLevelDefault(false))
    GetLevelDefault(false)->SetBase(Base->GetLevelDefault(false));
}

teamscript::teamscript()
{
  INITMEMBER(AttackEvilness);
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
	  Rel.first = SaveFile.ReadNumber(ValueMap);
	  Rel.second = SaveFile.ReadNumber(ValueMap);
	  Relation.push_back(Rel);
	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in team script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

gamescript::gamescript()
{
  INITMEMBER(DungeonDefault);
  INITMEMBER(Dungeons);
  INITMEMBER(Teams);
}

gamescript::~gamescript()
{
  for(std::vector<std::pair<uchar, teamscript*> >::iterator i1 = Team.begin(); i1 != Team.end(); ++i1)
    delete i1->second;

  for(std::map<uchar, dungeonscript*>::iterator i2 = Dungeon.begin(); i2 != Dungeon.end(); ++i2)
    delete i2->second;
}

void gamescript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word == "Dungeon")
	{
	  ushort Index = SaveFile.ReadNumber(ValueMap);
	  std::map<uchar, dungeonscript*>::iterator Iterator = Dungeon.find(Index);
	  dungeonscript* DS = Iterator == Dungeon.end() ? new dungeonscript : Iterator->second;
	  DS->SetValueMap(ValueMap);
	  DS->SetBase(GetDungeonDefault(false));
	  DS->ReadFrom(SaveFile);
	  Dungeon[Index] = DS;
	  continue;
	}

      if(Word == "Team")
	{
	  ushort Index = SaveFile.ReadNumber(ValueMap);
	  teamscript* TS = new teamscript;
	  TS->SetValueMap(ValueMap);
	  TS->ReadFrom(SaveFile);
	  Team.push_back(std::pair<uchar, teamscript*>(Index, TS));
	  continue;
	}

      if(!LoadData(SaveFile, Word))
	ABORT("Odd script term %s encountered in game script line %d!", Word.c_str(), SaveFile.TellLine());
    }
}

