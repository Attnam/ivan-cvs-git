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
#include "wskill.h"
#include "save.h"
#include "materba.h"

template <class type> datamembertemplate<type>::~datamembertemplate<type>()
{
  delete Member;
}

template <class type> type* datamembertemplate<type>::GetMember(bool AbortOnError) const
{
  if(Member)
    return Member;
  else
    if(Base)
      return Base->GetMember(AbortOnError);
    else
    {
      if(AbortOnError)
	ABORT("Undefined script member %s sought!", Identifier.c_str());

      return 0;
    }
}

/* Forced instantiations are here to decrease compilation time, at least in the future */

template bool* datamembertemplate<bool>::GetMember(bool) const;
template uchar* datamembertemplate<uchar>::GetMember(bool) const;
template ushort* datamembertemplate<ushort>::GetMember(bool) const;
template vector2d* datamembertemplate<vector2d>::GetMember(bool) const;
template std::string* datamembertemplate<std::string>::GetMember(bool) const;
template posscript* datamembertemplate<posscript>::GetMember(bool) const;
template squarescript* datamembertemplate<squarescript>::GetMember(bool) const;
template levelscript* datamembertemplate<levelscript>::GetMember(bool) const;
template dungeonscript* datamembertemplate<dungeonscript>::GetMember(bool) const;
template contentscript<character>* datamembertemplate<contentscript<character> >::GetMember(bool) const;
template contentscript<item>* datamembertemplate<contentscript<item> >::GetMember(bool) const;
template contentscript<glterrain>* datamembertemplate<contentscript<glterrain> >::GetMember(bool) const;
template contentscript<olterrain>* datamembertemplate<contentscript<olterrain> >::GetMember(bool) const;
template contentmap<character>* datamembertemplate<contentmap<character> >::GetMember(bool) const;
template contentmap<item>* datamembertemplate<contentmap<item> >::GetMember(bool) const;
template contentmap<glterrain>* datamembertemplate<contentmap<glterrain> >::GetMember(bool) const;
template contentmap<olterrain>* datamembertemplate<contentmap<olterrain> >::GetMember(bool) const;

template <class type> void datamembertemplate<type>::SetBase(datamemberbase* What)
{
  /* No type checking here, since only scriptwithbase<basetype>::SetBase uses this */

  Base = (datamembertemplate<type>*)What;
}

template <class type> bool datamember<type>::Load(const std::string& Word, inputfile& SaveFile, const valuemap& ValueMap)
{
  if(Word == Identifier)
    {
      if(!Member)
	Member = new type;

      ReadData(*Member, SaveFile, ValueMap);
      return true;
    }

  return false;
}

template <class type> void datamember<type>::Load(inputfile& SaveFile, const valuemap& ValueMap)
{
  if(!Member)
    Member = new type;

  ReadData(*Member, SaveFile, ValueMap);
}

template <class type> bool protonamedmember<type>::Load(const std::string& Word, inputfile& SaveFile, const valuemap&)
{
  if(Word == Identifier)
    {
      if(!Member)
	Member = new ushort;

      ReadData(*Member, SaveFile, protocontainer<type>::GetCodeNameMap());
      return true;
    }

  return false;
}

template <class type> void protonamedmember<type>::Load(inputfile& SaveFile, const valuemap&)
{
  if(!Member)
    Member = new ushort;

  ReadData(*Member, SaveFile, protocontainer<type>::GetCodeNameMap());
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
      Vector.Load(SaveFile, ValueMap);
    }

  if(Word == "Random")
    {
      Random = true;

      if(SaveFile.ReadWord() != "{")
	ABORT("Script error in level script!");

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	{
	  ushort c;

	  for(c = 0; c < Data.size(); ++c)
	    if(Data[c]->Load(Word, SaveFile, ValueMap))
	      break;

	  if(c == Data.size())
	    ABORT("Odd script term %s encountered in position script!", Word.c_str());
	}
    }
}

materialscript::materialscript() : Type(0)
{
  INITMEMBER(Volume);
}

void materialscript::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    Word = SaveFile.ReadWord();

  Type = protocontainer<material>::SearchCodeName(Word);

  if(!Type)
    ABORT("Odd script term %s encountered in material script!", Word.c_str());

  if(SaveFile.ReadWord() != "{")
    return;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in material script!", Word.c_str());
    }
}

material* materialscript::Instantiate() const
{
  material* Instance = protocontainer<material>::GetProto(Type)->Clone();

  if(GetVolume(false))
    Instance->SetVolume(*GetVolume());

  return Instance;
}

template <class type> basecontentscript<type>::basecontentscript<type>()
{
  INITMEMBER(MainMaterial);
  INITMEMBER(SecondaryMaterial);
  INITMEMBER(ContainedMaterial);
}

template <class type> void basecontentscript<type>::ReadFrom(inputfile& SaveFile)
{
  std::string Word = SaveFile.ReadWord();

  if(Word == "=")
    Word = SaveFile.ReadWord();

  ushort Index = protocontainer<material>::SearchCodeName(Word);

  if(Index)
    {
      if(!GetMainMaterial(false))
	MainMaterial.SetMember(new materialscript);

      GetMainMaterial()->SetType(Index);
      Word = SaveFile.ReadWord();
    }

  ContentType = protocontainer<type>::SearchCodeName(Word);

  if(ContentType || Word == "0")
    Word = SaveFile.ReadWord();
  else
    ABORT("Odd script term %s encountered content script of %s!", Word.c_str(), typeid(type).name());

  ValueMap["NONE"] = NONE;
  ValueMap["MIRROR"] = MIRROR;
  ValueMap["FLIP"] = FLIP;
  ValueMap["ROTATE"] = ROTATE_90;

  if(Word == "{")
    for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
      {
	ushort c;

	for(c = 0; c < Data.size(); ++c)
	  if(Data[c]->Load(Word, SaveFile, ValueMap))
	    break;

	if(c == Data.size())
	  ABORT("Odd script term %s encountered content script of %s!", Word.c_str(), typeid(type).name());
      }
  else
    if(Word != ";" && Word != ",")
      ABORT("Script error: Odd terminator %s encountered in content script of %s!", Word.c_str(), typeid(type).name());
}

template <class type> type* basecontentscript<type>::Instantiate() const
{
  type* Instance = protocontainer<type>::GetProto(ContentType)->Clone();

  if(GetMainMaterial(false))
    Instance->ChangeMainMaterial(GetMainMaterial()->Instantiate());

  if(GetSecondaryMaterial(false))
    Instance->ChangeSecondaryMaterial(GetSecondaryMaterial()->Instantiate());

  if(GetContainedMaterial(false))
    Instance->ChangeContainedMaterial(GetContainedMaterial()->Instantiate());

  return Instance;
}

contentscript<character>::contentscript<character>()
{
  INITMEMBER(Team);
}

character* contentscript<character>::Instantiate() const
{
  character* Instance = basecontentscript<character>::Instantiate();

  if(GetTeam(false))
    Instance->SetTeam(game::GetTeam(*GetTeam()));

  return Instance;
}

contentscript<olterrain>::contentscript<olterrain>()
{
  INITMEMBER(Locked);
  INITMEMBER(VisualFlags);
}

olterrain* contentscript<olterrain>::Instantiate() const
{
  olterrain* Instance = basecontentscript<olterrain>::Instantiate();

  if(GetLocked(false) && *GetLocked())
    Instance->Lock();

  if(GetVisualFlags(false))
    Instance->SetVisualFlags(*GetVisualFlags());

  return Instance;
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
      Position.Load(SaveFile, ValueMap);

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in square script!");

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	{
	  ushort c;

	  for(c = 0; c < Data.size(); ++c)
	    if(Data[c]->Load(Word, SaveFile, ValueMap))
	      break;

	  if(c == Data.size())
	    ABORT("Odd script term %s encountered in square script!", Word.c_str());
	}
    }
  else
    {
      GTerrain.Load(SaveFile, ValueMap);
      OTerrain.Load(SaveFile, ValueMap);
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
    ABORT("Bracket missing in content map script of %s!", typeid(type).name());

  std::map<char, contentscript<type>*> SymbolMap;

  SymbolMap['.'] = 0;

  for(std::string Word = SaveFile.ReadWord(); Word != "}"; Word = SaveFile.ReadWord())
    {
      if(Word == "Types")
	{
	  if(SaveFile.ReadWord() != "{")
	    ABORT("Missing bracket in content map script of %s!", typeid(type).name());

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

      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in content script of %s!", Word.c_str(), typeid(type).name());
    }

  if(!ContentScriptMap)
    Alloc2D(ContentScriptMap, GetSize()->X, GetSize()->Y);
  else
    DeleteContents();

  if(SaveFile.ReadWord() != "{")
    ABORT("Missing bracket in content map script of %s!", typeid(type).name());

  for(ushort y = 0; y < GetSize()->Y; ++y)
    for(ushort x = 0; x < GetSize()->X; ++x)
      {
	char Char = SaveFile.ReadLetter();

	std::map<char, contentscript<type>*>::iterator Iterator = SymbolMap.find(Char);

	if(Iterator != SymbolMap.end())
	  ContentScriptMap[x][y] = Iterator->second;
	else
	  ABORT("Illegal content %c in content map of %s!", Char, typeid(type).name());
      }

  if(SaveFile.ReadWord() != "}")
    ABORT("Missing bracket in content map script of %s!", typeid(type).name());
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
    ABORT("Bracket missing in room script!");

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

      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in room script!", Word.c_str());
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
    ABORT("Bracket missing in level script!");

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

      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in level script!", Word.c_str());
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
    ABORT("Bracket missing in dungeon script!");

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

      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in dungeon script!", Word.c_str());
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
  ValueMap["HOSTILE"] = HOSTILE;
  ValueMap["NEUTRAL"] = NEUTRAL;
  ValueMap["FRIEND"] = FRIEND;

  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in team script!");

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

      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in team script!", Word.c_str());
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

      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in game script!", Word.c_str());
    }
}

template <class type> void basedata<type>::ReadFrom(inputfile& SaveFile)
{
  if(SaveFile.ReadWord() != "{")
    ABORT("Bracket missing in the data script of %s!", typeid(type).name());

  std::string Word;

  for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
    {
      ushort c;

      for(c = 0; c < Data.size(); ++c)
	if(Data[c]->Load(Word, SaveFile, ValueMap))
	  break;

      if(c == Data.size())
	ABORT("Odd script term %s encountered in the data script of %s!", Word.c_str(), typeid(type).name());
    }
}

data<character>::data<character>()
{
  INITMEMBER(DefaultAgility);
  INITMEMBER(DefaultStrength);
  INITMEMBER(DefaultEndurance);
  INITMEMBER(DefaultPerception);
  INITMEMBER(DefaultMoney);
  INITMEMBER(TotalSize);
  INITMEMBER(CanRead);
  INITMEMBER(IsCharmable);
  INITMEMBER(Sex);
  INITMEMBER(BloodColor);
  INITMEMBER(CanBeGenerated);
  INITMEMBER(HasInfraVision);
  INITMEMBER(CriticalModifier);
  INITMEMBER(StandVerb);
  INITMEMBER(CanOpen);
  INITMEMBER(CanBeDisplaced);
  INITMEMBER(Frequency);
  INITMEMBER(CanWalk);
  INITMEMBER(CanSwim);
  INITMEMBER(CanFly);
  INITMEMBER(PhysicalDamageResistance);
  INITMEMBER(SoundResistance);
  INITMEMBER(EnergyResistance);
  INITMEMBER(AcidResistance);
  INITMEMBER(FireResistance);
  INITMEMBER(PoisonResistance);
  INITMEMBER(BulimiaResistance);
  INITMEMBER(IsUnique);
  INITMEMBER(EatFlags);
  INITMEMBER(TotalVolume);
  //INITMEMBER(BitmapPos);
  INITMEMBER(MeleeStrength);
  INITMEMBER(TalkVerb);
  INITMEMBER(HeadBitmapPos);
  INITMEMBER(TorsoBitmapPos);
  INITMEMBER(ArmBitmapPos);
  INITMEMBER(LegBitmapPos);
  INITMEMBER(RightArmBitmapPos);
  INITMEMBER(LeftArmBitmapPos);
  INITMEMBER(RightLegBitmapPos);
  INITMEMBER(LeftLegBitmapPos);
  INITMEMBER(GroinBitmapPos);
  INITMEMBER(ClothColor);
  INITMEMBER(SkinColor);
  INITMEMBER(CapColor);
  INITMEMBER(HairColor);
  INITMEMBER(EyeColor);
  INITMEMBER(TorsoMainColor);
  INITMEMBER(BeltColor);
  INITMEMBER(TorsoSpecialColor);
  INITMEMBER(ArmMainColor);
  INITMEMBER(ArmSpecialColor);
  INITMEMBER(LegMainColor);
  INITMEMBER(LegSpecialColor);
  INITMEMBER(HeadBonePercentile);
  INITMEMBER(TorsoBonePercentile);
  INITMEMBER(ArmBonePercentile);
  INITMEMBER(RightArmBonePercentile);
  INITMEMBER(LeftArmBonePercentile);
  INITMEMBER(GroinBonePercentile);
  INITMEMBER(LegBonePercentile);
  INITMEMBER(RightLegBonePercentile);
  INITMEMBER(LeftLegBonePercentile);
  INITMEMBER(IsNameable);
  INITMEMBER(BaseEmitation);
}

data<item>::data<item>()
{
  INITMEMBER(Possibility);
  INITMEMBER(InHandsPic);
  INITMEMBER(OfferModifier);
  INITMEMBER(Score);
  INITMEMBER(IsDestroyable);
  INITMEMBER(CanBeWished);
  INITMEMBER(IsMaterialChangeable);
  INITMEMBER(WeaponCategory);
  INITMEMBER(IsPolymorphSpawnable);
  INITMEMBER(IsAutoInitializable);
  INITMEMBER(OneHandedStrengthPenalty);
  INITMEMBER(OneHandedToHitPenalty);
  INITMEMBER(Category);
  INITMEMBER(SoundResistance);
  INITMEMBER(EnergyResistance);
  INITMEMBER(AcidResistance);
  INITMEMBER(FireResistance);
  INITMEMBER(PoisonResistance);
  INITMEMBER(BulimiaResistance);
  INITMEMBER(IsStackable);
  INITMEMBER(StrengthModifier);
  INITMEMBER(FormModifier);
  INITMEMBER(NPModifier);
  INITMEMBER(DefaultSize);
  INITMEMBER(DefaultMainVolume);
  INITMEMBER(DefaultSecondaryVolume);
  INITMEMBER(DefaultContainedVolume);
  INITMEMBER(BitmapPos);
  INITMEMBER(Price);
  INITMEMBER(BaseEmitation);
}

data<material>::data<material>()
{
  INITMEMBER(StrengthValue);
  INITMEMBER(ConsumeType);
  INITMEMBER(Density);
  INITMEMBER(OfferValue);
  INITMEMBER(Color);
  INITMEMBER(PriceModifier);
  INITMEMBER(IsSolid);
  INITMEMBER(Emitation);
  INITMEMBER(CanBeWished);
  INITMEMBER(Alignment);
  INITMEMBER(NutritionValue);
  INITMEMBER(IsAlive);
  INITMEMBER(IsBadFoodForAI);
  INITMEMBER(ExplosivePower);
  INITMEMBER(IsFlammable);
  INITMEMBER(IsFlexible);
  INITMEMBER(IsExplosive);
}

template class database<character>;
template class database<item>;
template class database<material>;

template <class type> database<type>::~database<type>()
{
  for(std::map<std::string, data<type>*>::iterator i = Data.begin(); i != Data.end(); ++i)
    delete i->second;
}

template <class type> void database<type>::ReadFrom(inputfile& SaveFile)
{
  std::string Word;
  valuemap ValueMap;

  ValueMap["ODD"] = ODD;
  ValueMap["FRUIT"] = FRUIT;
  ValueMap["MEAT"] = MEAT;
  ValueMap["METAL"] = METAL;
  ValueMap["MINERAL"] = MINERAL;
  ValueMap["LIQUID"] = LIQUID;
  ValueMap["BONE"] = BONE;
  ValueMap["PROCESSED"] = PROCESSED;
  ValueMap["MISC_ORGANIC"] = MISC_ORGANIC;
  ValueMap["GAS"] = GAS;

  ValueMap["GOOD"] = GOOD;
  ValueMap["NEUTRAL"] = NEUTRAL;
  ValueMap["EVIL"] = EVIL;

  ValueMap["UNCATEGORIZED"] = UNCATEGORIZED;
  ValueMap["UNARMED"] = UNARMED;
  ValueMap["DAGGERS"] = DAGGERS;
  ValueMap["SMALL_SWORDS"] = SMALL_SWORDS;
  ValueMap["LARGE_SWORDS"] = LARGE_SWORDS;
  ValueMap["CLUBS"] = CLUBS;
  ValueMap["HAMMERS"] = HAMMERS;
  ValueMap["MACES"] = MACES;
  ValueMap["FLAILS"] = FLAILS;
  ValueMap["AXES"] = AXES;
  ValueMap["HALBERDS"] = HALBERDS;
  ValueMap["SPEARS"] = SPEARS;
  ValueMap["WHIPS"] = WHIPS;

  ValueMap["HELMET"] = HELMET;
  ValueMap["AMULET"] = AMULET;
  ValueMap["CLOAK"] = CLOAK;
  ValueMap["BODYARMOR"] = BODYARMOR;
  ValueMap["WEAPON"] = WEAPON;
  ValueMap["SHIELD"] = SHIELD;
  ValueMap["RING"] = RING;
  ValueMap["GAUNTLET"] = GAUNTLET;
  ValueMap["BELT"] = BELT;
  ValueMap["BOOT"] = BOOT;
  ValueMap["FOOD"] = FOOD;
  ValueMap["POTION"] = POTION;
  ValueMap["SCROLL"] = SCROLL;
  ValueMap["BOOK"] = BOOK;
  ValueMap["WAND"] = WAND;
  ValueMap["TOOL"] = TOOL;
  ValueMap["VALUABLE"] = VALUABLE;
  ValueMap["MISC"] = MISC;

  ValueMap["UNDEFINED"] = UNDEFINED;
  ValueMap["MALE"] = MALE;
  ValueMap["FEMALE"] = FEMALE;
  ValueMap["TRANSSEXUAL"] = TRANSSEXUAL;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      ushort Index = protocontainer<type>::SearchCodeName(Word);

      if(!Index)
	ABORT("Odd term %s encountered in %s datafile!", Word.c_str(), typeid(type).name());

      data<type>* DataElement = new data<type>;
      DataElement->SetType(Index);
      DataElement->SetValueMap(ValueMap);
      DataElement->ReadFrom(SaveFile);
      Data[Word] = DataElement;
    }
}

#define SETDATA(data)\
{\
  if(DataElement->Get##data(false))\
    DataBase.data = *DataElement->Get##data();\
  else\
    ABORT("Obligatory data member " #data " missing in the database of %s!", Iterator->first.c_str());\
}

#define SETDATAWITHDEFAULT(data, defaultvalue)\
{\
  if(DataElement->Get##data(false))\
    DataBase.data = *DataElement->Get##data();\
  else\
    DataBase.data = defaultvalue;\
}

void database<character>::Apply()
{
  for(ushort c = 1; c < protocontainer<character>::GetProtoAmount(); ++c)
    {
      std::map<std::string, data<character>*>::iterator Iterator = Data.find(protocontainer<character>::GetProto(c)->ClassName());

      if(Iterator != Data.end())
	{
	  data<character>* DataElement = Iterator->second;
	  character_database& DataBase = protocontainer<character>::GetProto(c)->GetDataBase();
	  SETDATA(DefaultAgility);
	  SETDATA(DefaultStrength);
	  SETDATA(DefaultEndurance);
	  SETDATA(DefaultPerception);
	  SETDATAWITHDEFAULT(DefaultMoney, 0);
	  SETDATA(TotalSize);
	  SETDATAWITHDEFAULT(CanRead, false);
	  SETDATAWITHDEFAULT(IsCharmable, true);
	  SETDATAWITHDEFAULT(Sex, MALE);
	  SETDATAWITHDEFAULT(BloodColor, MAKE_RGB(75, 0, 0));
	  SETDATAWITHDEFAULT(CanBeGenerated, true);
	  SETDATAWITHDEFAULT(HasInfraVision, false);
	  SETDATAWITHDEFAULT(CriticalModifier, 20);
	  SETDATAWITHDEFAULT(StandVerb, "standing");
	  SETDATAWITHDEFAULT(CanOpen, true);
	  SETDATAWITHDEFAULT(CanBeDisplaced, true);
	  SETDATAWITHDEFAULT(Frequency, 10000);
	  SETDATAWITHDEFAULT(CanWalk, true);
	  SETDATAWITHDEFAULT(CanSwim, false);
	  SETDATAWITHDEFAULT(CanFly, false);
	  SETDATAWITHDEFAULT(PhysicalDamageResistance, 0);
	  SETDATAWITHDEFAULT(SoundResistance, 0);
	  SETDATAWITHDEFAULT(EnergyResistance, 0);
	  SETDATAWITHDEFAULT(AcidResistance, 0);
	  SETDATAWITHDEFAULT(FireResistance, 0);
	  SETDATAWITHDEFAULT(PoisonResistance, 0);
	  SETDATAWITHDEFAULT(BulimiaResistance, 0);
	  SETDATAWITHDEFAULT(IsUnique, false);
	  SETDATAWITHDEFAULT(EatFlags, FRUIT|MEAT|LIQUID|PROCESSED);
	  SETDATA(TotalVolume);
	  SETDATA(MeleeStrength);
	  SETDATAWITHDEFAULT(TalkVerb, "grunts");
	  SETDATAWITHDEFAULT(HeadBitmapPos, vector2d(96, 0));
	  SETDATAWITHDEFAULT(TorsoBitmapPos, vector2d(32, 0));
	  SETDATAWITHDEFAULT(ArmBitmapPos, vector2d(64, 0));
	  SETDATAWITHDEFAULT(LegBitmapPos, vector2d(0, 0));
	  SETDATAWITHDEFAULT(RightArmBitmapPos, DataBase.ArmBitmapPos);
	  SETDATAWITHDEFAULT(LeftArmBitmapPos, DataBase.ArmBitmapPos);
	  SETDATAWITHDEFAULT(RightLegBitmapPos, DataBase.LegBitmapPos);
	  SETDATAWITHDEFAULT(LeftLegBitmapPos, DataBase.LegBitmapPos);
	  SETDATAWITHDEFAULT(GroinBitmapPos, DataBase.LegBitmapPos);
	  SETDATAWITHDEFAULT(ClothColor, MAKE_RGB(111, 74, 37));
	  SETDATAWITHDEFAULT(SkinColor, MAKE_RGB(180, 120, 90));
	  SETDATAWITHDEFAULT(CapColor, DataBase.ClothColor);
	  SETDATAWITHDEFAULT(HairColor, MAKE_RGB(160, 80, 0));
	  SETDATAWITHDEFAULT(EyeColor, MAKE_RGB(112, 72, 42));
	  SETDATAWITHDEFAULT(TorsoMainColor, DataBase.ClothColor);
	  SETDATAWITHDEFAULT(BeltColor, MAKE_RGB(48, 48, 48));
	  SETDATAWITHDEFAULT(TorsoSpecialColor, 0);
	  SETDATAWITHDEFAULT(ArmMainColor, DataBase.ClothColor);
	  SETDATAWITHDEFAULT(ArmSpecialColor, 0);
	  SETDATAWITHDEFAULT(LegMainColor, DataBase.ClothColor);
	  SETDATAWITHDEFAULT(LegSpecialColor, 0);
	  SETDATAWITHDEFAULT(HeadBonePercentile, 20);
	  SETDATAWITHDEFAULT(TorsoBonePercentile, 10);
	  SETDATAWITHDEFAULT(ArmBonePercentile, 30);
	  SETDATAWITHDEFAULT(RightArmBonePercentile, DataBase.ArmBonePercentile);
	  SETDATAWITHDEFAULT(LeftArmBonePercentile, DataBase.ArmBonePercentile);
	  SETDATAWITHDEFAULT(GroinBonePercentile, 40);
	  SETDATAWITHDEFAULT(LegBonePercentile, 30);
	  SETDATAWITHDEFAULT(RightLegBonePercentile, DataBase.LegBonePercentile);
	  SETDATAWITHDEFAULT(LeftLegBonePercentile, DataBase.LegBonePercentile);
	  SETDATAWITHDEFAULT(IsNameable, true);
	  SETDATAWITHDEFAULT(BaseEmitation, 0);
	}
      else
	{
	  /* Remove these! */

	  character_database& DataBase = protocontainer<character>::GetProto(c)->GetDataBase();

	  DataBase.DefaultMoney = 0;
	  DataBase.CanRead = false;
	  DataBase.IsCharmable = true;
	  DataBase.Sex = MALE;
	  DataBase.BloodColor = MAKE_RGB(75, 0, 0);
	  DataBase.CanBeGenerated = true;
	  DataBase.HasInfraVision = false;
	  DataBase.CriticalModifier = 20;
	  DataBase.StandVerb = "standing";
	  DataBase.CanOpen = true;
	  DataBase.CanBeDisplaced = true;
	  DataBase.Frequency = 10000;
	  DataBase.CanWalk = true;
	  DataBase.CanSwim = false;
	  DataBase.CanFly = false;
	  DataBase.PhysicalDamageResistance = 0;
	  DataBase.SoundResistance = 0;
	  DataBase.EnergyResistance = 0;
	  DataBase.AcidResistance = 0;
	  DataBase.FireResistance = 0;
	  DataBase.PoisonResistance = 0;
	  DataBase.BulimiaResistance = 0;
	  DataBase.IsUnique = false;
	  DataBase.EatFlags = FRUIT|MEAT|LIQUID|PROCESSED;
	  DataBase.TalkVerb = "grunts";
	  DataBase.HeadBitmapPos = vector2d(96, 0);
	  DataBase.TorsoBitmapPos = vector2d(32, 0);
	  DataBase.ArmBitmapPos = vector2d(64, 0);
	  DataBase.LegBitmapPos = vector2d(0, 0);
	  DataBase.RightArmBitmapPos = DataBase.ArmBitmapPos;
	  DataBase.LeftArmBitmapPos = DataBase.ArmBitmapPos;
	  DataBase.RightLegBitmapPos = DataBase.LegBitmapPos;
	  DataBase.LeftLegBitmapPos = DataBase.LegBitmapPos;
	  DataBase.GroinBitmapPos = DataBase.LegBitmapPos;
	  DataBase.ClothColor = MAKE_RGB(111, 74, 37);
	  DataBase.SkinColor = MAKE_RGB(180, 120, 90);
	  DataBase.CapColor = DataBase.ClothColor;
	  DataBase.HairColor = MAKE_RGB(160, 80, 0);
	  DataBase.EyeColor = MAKE_RGB(112, 72, 42);
	  DataBase.TorsoMainColor = DataBase.ClothColor;
	  DataBase.BeltColor = MAKE_RGB(48, 48, 48);
	  DataBase.TorsoSpecialColor = 0;
	  DataBase.ArmMainColor = DataBase.ClothColor;
	  DataBase.ArmSpecialColor = 0;
	  DataBase.LegMainColor = DataBase.ClothColor;
	  DataBase.LegSpecialColor = 0;
	  DataBase.HeadBonePercentile = 20;
	  DataBase.TorsoBonePercentile = 10;
	  DataBase.ArmBonePercentile = 30;
	  DataBase.RightArmBonePercentile = DataBase.ArmBonePercentile;
	  DataBase.LeftArmBonePercentile = DataBase.ArmBonePercentile;
	  DataBase.GroinBonePercentile = 40;
	  DataBase.LegBonePercentile = 30;
	  DataBase.RightLegBonePercentile = DataBase.LegBonePercentile;
	  DataBase.LeftLegBonePercentile = DataBase.LegBonePercentile;
	  DataBase.IsNameable = true;
	  DataBase.BaseEmitation = 0;
	}
    }
}

void database<item>::Apply()
{
  for(ushort c = 1; c < protocontainer<item>::GetProtoAmount(); ++c)
    {
      std::map<std::string, data<item>*>::iterator Iterator = Data.find(protocontainer<item>::GetProto(c)->ClassName());

      if(Iterator != Data.end())
	{
	  data<item>* DataElement = Iterator->second;
	  item_database& DataBase = protocontainer<item>::GetProto(c)->GetDataBase();
	  SETDATAWITHDEFAULT(Possibility, 0);
	  SETDATAWITHDEFAULT(InHandsPic, vector2d(160, 144));
	  SETDATAWITHDEFAULT(OfferModifier, 0);
	  SETDATAWITHDEFAULT(Score, 0);
	  SETDATAWITHDEFAULT(IsDestroyable, true);
	  SETDATAWITHDEFAULT(CanBeWished, true);
	  SETDATAWITHDEFAULT(IsMaterialChangeable, true);
	  SETDATAWITHDEFAULT(WeaponCategory, UNCATEGORIZED);
	  SETDATAWITHDEFAULT(IsPolymorphSpawnable, true);
	  SETDATAWITHDEFAULT(IsAutoInitializable, true);
	  SETDATAWITHDEFAULT(OneHandedStrengthPenalty, 0);
	  SETDATAWITHDEFAULT(OneHandedToHitPenalty, 0);
	  SETDATA(Category);
	  SETDATAWITHDEFAULT(SoundResistance, 0);
	  SETDATAWITHDEFAULT(EnergyResistance, 0);
	  SETDATAWITHDEFAULT(AcidResistance, 0);
	  SETDATAWITHDEFAULT(FireResistance, 0);
	  SETDATAWITHDEFAULT(PoisonResistance, 0);
	  SETDATAWITHDEFAULT(BulimiaResistance, 0);
	  SETDATAWITHDEFAULT(IsStackable, true);
	  SETDATA(StrengthModifier);
	  SETDATA(FormModifier);
	  SETDATAWITHDEFAULT(NPModifier, 10000);
	  SETDATA(DefaultSize);
	  SETDATA(DefaultMainVolume);
	  SETDATAWITHDEFAULT(DefaultSecondaryVolume, 0);
	  SETDATAWITHDEFAULT(DefaultContainedVolume, 0);
	  SETDATA(BitmapPos);
	  SETDATAWITHDEFAULT(Price, 0);
	  SETDATAWITHDEFAULT(BaseEmitation, 0);
	}
      else
	{
	  /* Remove these! */

	  item_database& DataBase = protocontainer<item>::GetProto(c)->GetDataBase();
	  DataBase.Possibility = 0;
          DataBase.InHandsPic = vector2d(160, 144);
	  DataBase.OfferModifier = 0;
	  DataBase.Score = 0;
	  DataBase.IsDestroyable = true;
	  DataBase.CanBeWished = true;
	  DataBase.IsMaterialChangeable = true;
	  DataBase.WeaponCategory = UNCATEGORIZED;
	  DataBase.IsPolymorphSpawnable = true;
	  DataBase.IsAutoInitializable = true;
	  DataBase.OneHandedStrengthPenalty = 0;
	  DataBase.OneHandedToHitPenalty = 0;
	  DataBase.SoundResistance = 0;
	  DataBase.EnergyResistance = 0;
	  DataBase.AcidResistance = 0;
	  DataBase.FireResistance = 0;
	  DataBase.PoisonResistance = 0;
	  DataBase.BulimiaResistance = 0;
	  DataBase.IsStackable = true;
	  DataBase.NPModifier = 10000;
	  DataBase.DefaultSecondaryVolume = 0;
	  DataBase.DefaultContainedVolume = 0;
	  DataBase.Price = 0;
	  DataBase.BaseEmitation = 0;
	}
    }
}

void database<material>::Apply()
{
  for(ushort c = 1; c < protocontainer<material>::GetProtoAmount(); ++c)
    {
      std::map<std::string, data<material>*>::iterator Iterator = Data.find(protocontainer<material>::GetProto(c)->ClassName());

      if(Iterator != Data.end())
	{
	  data<material>* DataElement = Iterator->second;
	  material_database& DataBase = protocontainer<material>::GetProto(c)->GetDataBase();
	  SETDATA(StrengthValue);
	  SETDATA(ConsumeType);
	  SETDATA(Density);
	  SETDATA(OfferValue);
	  SETDATA(Color);
	  SETDATAWITHDEFAULT(PriceModifier, 0);
	  SETDATAWITHDEFAULT(IsSolid, false);
	  SETDATAWITHDEFAULT(Emitation, 0);
	  SETDATAWITHDEFAULT(CanBeWished, true);
	  SETDATAWITHDEFAULT(Alignment, NEUTRAL);
	  SETDATAWITHDEFAULT(NutritionValue, 0);
	  SETDATAWITHDEFAULT(IsAlive, false);
	  SETDATAWITHDEFAULT(IsBadFoodForAI, false);
	  SETDATAWITHDEFAULT(ExplosivePower, 0);
	  SETDATAWITHDEFAULT(IsFlammable, false);
	  SETDATAWITHDEFAULT(IsFlexible, false);
	  SETDATAWITHDEFAULT(IsExplosive, false);
	}
      else
	{
	  /* Remove these! */

	  material_database& DataBase = protocontainer<material>::GetProto(c)->GetDataBase();
	  DataBase.PriceModifier = 0;
	  DataBase.IsSolid = false;
	  DataBase.Emitation = 0;
	  DataBase.CanBeWished = true;
	  DataBase.Alignment = NEUTRAL;
	  DataBase.NutritionValue = 0;
	  DataBase.IsAlive = false;
	  DataBase.IsBadFoodForAI = false;
	  DataBase.ExplosivePower = 0;
	  DataBase.IsFlammable = false;
	  DataBase.IsFlexible = false;
	  DataBase.IsExplosive = false;
	}
    }
}
