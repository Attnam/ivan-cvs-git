#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "rect.h"
#include "save.h"
#include "femath.h"

#define DATA_MEMBER(type, name)\
 public:\
  const type* Get##name() const { return name##Holder.GetMember(); }\
 protected:\
  datamember< type > name##Holder;

#define DATA_MEMBER_WITH_BASE(type, name)\
 public:\
  const type* Get##name() const { return GetMemberOf(name##Holder, Base, &scripttype::Get##name); }\
 protected:\
  datamember< type > name##Holder;

#define DATA_BOOL(name)\
 public:\
  const bool* name() const { return name##Holder.GetMember(); }\
 protected:\
  datamember<bool> name##Holder;

#define DATA_BOOL_WITH_BASE(name)\
 public:\
  const bool* name() const { return GetMemberOf(name##Holder, Base, &scripttype::name); }\
 protected:\
  datamember<bool> name##Holder;

class glterrain;
class olterrain;
class character;
class item;
class material;
class scriptwithbase;

class datamemberbase
{
 public:
  virtual ~datamemberbase() { }
  virtual void ReadFrom(inputfile&) = 0;
  virtual void Save(outputfile&) const = 0;
  virtual void Load(inputfile&) = 0;
  virtual void Replace(datamemberbase&) = 0;
};

template <class type> class datamember : public datamemberbase
{
 public:
  virtual ~datamember() { delete Member; }
  datamember() : Member(0) { }
  datamember(const datamember& Data) : datamemberbase(Data), Member(Data.Member ? new type(*Data.Member) : 0) {  }
  datamember& operator=(const datamember&);
  type* GetMember() const { return Member; }
  void SetMember(type* What) { Member = What; }
  virtual void ReadFrom(inputfile&);
  virtual void Replace(datamemberbase&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  type* Member;
};

template <class type, class scripttype> inline const type* GetMemberOf(const datamember<type>& Data, const scriptwithbase* Base, const type* (scripttype::*MemberRetriever)() const)
{
  return Data.GetMember() ? Data.GetMember() : Base ? (static_cast<const scripttype*>(Base)->*MemberRetriever)() : 0;
}

template <class type> inline datamember<type>& datamember<type>::operator=(const datamember<type>& Data)
{
  if(Member)
    {
      if(Data.Member)
	*Member = *Data.Member;
      else
	{
	  delete Member;
	  Member = 0;
	}
    }
  else if(Data.Member)
    Member = new type(*Data.Member);

  return *this;
}

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

class script
{
 public:
  typedef std::map<std::string, datamemberbase script::*> datamap;
  virtual ~script() { }
  virtual void ReadFrom(inputfile&) = 0;
  virtual void Save(outputfile& SaveFile) const { SaveDataMap(GetDataMap(), SaveFile); }
  virtual void Load(inputfile& SaveFile) { LoadDataMap(GetDataMap(), SaveFile); }
 protected:
  bool ReadMember(inputfile&, const std::string&);
  virtual datamemberbase* GetDataFromMap(const datamap&, const std::string&);
  virtual datamemberbase* GetData(const std::string& String) { return GetDataFromMap(GetDataMap(), String); }
  virtual const datamap& GetDataMap() const = 0;
  virtual void SaveDataMap(const datamap&, outputfile&) const;
  virtual void LoadDataMap(const datamap&, inputfile&);
};

inline void ReadData(script& Type, inputfile& SaveFile) { Type.ReadFrom(SaveFile); }
inline outputfile& operator<<(outputfile& SaveFile, const script& Script) { Script.Save(SaveFile); return SaveFile; }
inline inputfile& operator>>(inputfile& SaveFile, script& Script) { Script.Load(SaveFile); return SaveFile; }

class scriptwithbase : public script
{
 public:
  scriptwithbase() : Base(0) { }
  const scriptwithbase* GetBase() const { return Base; }
  virtual void SetBase(const scriptwithbase* What) { Base = What; }
 protected:
  const scriptwithbase* Base;
};

class posscript : public script
{
 public:
  typedef posscript scripttype;
  virtual void ReadFrom(inputfile&);
  bool GetRandom() const { return Random; }
  static void InitDataMap();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  bool Random;
  DATA_MEMBER(vector2d, Vector);
  DATA_MEMBER(uchar, Flags);
  DATA_MEMBER(rect, Borders);
};

class materialscript : public script
{
 public:
  typedef materialscript scripttype;
  virtual void ReadFrom(inputfile&);
  void SetConfig(ushort What) { Config = What; }
  material* Instantiate() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  DATA_MEMBER(ulong, Volume);
  ushort Config;
};

class basecontentscript : public script
{
 public:
  typedef basecontentscript scripttype;
  basecontentscript();
  virtual void ReadFrom(inputfile&);
  ushort GetContentType() const { return ContentType; }
  bool IsValid() const { return ContentType || Random; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  virtual datamemberbase* GetData(const std::string&);
  virtual ushort SearchCodeName(const std::string&) const = 0;
  virtual const char* GetClassId() const = 0;
  static datamap DataMap;
  DATA_MEMBER(materialscript, MainMaterial);
  DATA_MEMBER(materialscript, SecondaryMaterial);
  DATA_MEMBER(materialscript, ContainedMaterial);
  DATA_MEMBER(ulong, Parameters);
  ushort ContentType;
  ushort Config;
  bool Random;
};

inline bool IsValidScript(const basecontentscript* S) { return S->IsValid(); }

template <class type> inline bool IsValidScript(const std::list<type>* List)
{
  for(typename std::list<type>::const_iterator i = List->begin(); i != List->end(); ++i)
    if(IsValidScript(&*i))
      return true;

  return false;
}

template <class type> class contentscripttemplate : public basecontentscript
{
 protected:
  type* BasicInstantiate(ushort) const;
  virtual ushort SearchCodeName(const std::string&) const;
};

template <class type> class contentscript;

class contentscript<character> : public contentscripttemplate<character>
{
 public:
  typedef contentscript<character> scripttype;
  character* Instantiate(ushort = 0) const;
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  virtual const char* GetClassId() const;
  static datamap DataMap;
  DATA_MEMBER(ushort, Team);
  DATA_MEMBER(std::list<contentscript<item> >, Inventory);
  DATA_BOOL(IsMaster);
  DATA_MEMBER(std::vector<vector2d>, WayPoint);
};

class contentscript<item> : public contentscripttemplate<item>
{
 public:
  typedef contentscript<item> scripttype;
  item* Instantiate(ushort = 0) const;
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  virtual const char* GetClassId() const;
  static datamap DataMap;
  DATA_MEMBER(ushort, Team);
  DATA_BOOL(IsActive);
  DATA_MEMBER(uchar, SideStackIndex);
  DATA_MEMBER(short, Enchantment);
  DATA_MEMBER(ulong, MinPrice);
  DATA_MEMBER(ulong, MaxPrice);
  DATA_MEMBER(ulong, Category);
  DATA_MEMBER(std::list<contentscript<item> >, ItemsInside);
  DATA_MEMBER(uchar, Chance);
};

class contentscript<glterrain> : public contentscripttemplate<glterrain>
{
 public:
  typedef contentscript<glterrain> scripttype;
  glterrain* Instantiate(ushort SpecialFlags = 0) const { return contentscripttemplate<glterrain>::BasicInstantiate(SpecialFlags); }
  static void InitDataMap() { }
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  virtual const char* GetClassId() const;
};

class contentscript<olterrain> : public contentscripttemplate<olterrain>
{
 public:
  typedef contentscript<olterrain> scripttype;
  olterrain* Instantiate(ushort = 0) const;
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  virtual const char* GetClassId() const;
  DATA_MEMBER(uchar, VisualEffects);
  DATA_MEMBER(uchar, AttachedArea);
  DATA_MEMBER(uchar, AttachedEntry);
  DATA_MEMBER(std::string, Text);
  DATA_MEMBER(std::list<contentscript<item> >, ItemsInside);
};

class squarescript : public script
{
 public:
  typedef squarescript scripttype;
  virtual void ReadFrom(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  DATA_MEMBER(posscript, Position);
  DATA_MEMBER(contentscript<character>, Character);
  DATA_MEMBER(std::list<contentscript<item> >, Items);
  DATA_MEMBER(contentscript<glterrain>, GTerrain);
  DATA_MEMBER(contentscript<olterrain>, OTerrain);
  DATA_MEMBER(interval, Times);
  DATA_BOOL(AttachRequired);
  DATA_MEMBER(uchar, EntryIndex);
};

template <class type, class contenttype = contentscript<type> > class contentmap : public script
{
 public:
  typedef contentmap scripttype;
  contentmap();
  virtual ~contentmap();
  virtual void ReadFrom(inputfile&);
  const contenttype* GetContentScript(ushort X, ushort Y) const { return ContentMap[X][Y].second; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::pair<char, contenttype*>** ContentMap;
  std::map<char, contenttype> SymbolMap;
  DATA_MEMBER(vector2d, Size);
  DATA_MEMBER(vector2d, Pos);
};

typedef contentmap<item, std::list<contentscript<item> > > itemcontentmap;
typedef contentmap<character> charactercontentmap;
typedef contentmap<glterrain> glterraincontentmap;
typedef contentmap<olterrain> olterraincontentmap;

class roomscript : public scriptwithbase
{
 public:
  typedef roomscript scripttype;
  void ReadFrom(inputfile&);
  const std::list<squarescript>& GetSquare() const { return Square; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::list<squarescript> Square;
  DATA_MEMBER_WITH_BASE(charactercontentmap, CharacterMap);
  DATA_MEMBER_WITH_BASE(itemcontentmap, ItemMap);
  DATA_MEMBER_WITH_BASE(glterraincontentmap, GTerrainMap);
  DATA_MEMBER_WITH_BASE(olterraincontentmap, OTerrainMap);
  DATA_MEMBER_WITH_BASE(squarescript, WallSquare);
  DATA_MEMBER_WITH_BASE(squarescript, FloorSquare);
  DATA_MEMBER_WITH_BASE(squarescript, DoorSquare);
  DATA_MEMBER_WITH_BASE(region, Size);
  DATA_MEMBER_WITH_BASE(region, Pos);
  DATA_BOOL_WITH_BASE(AltarPossible);
  DATA_BOOL_WITH_BASE(GenerateDoor);
  DATA_BOOL_WITH_BASE(GenerateTunnel);
  DATA_MEMBER_WITH_BASE(uchar, DivineMaster);
  DATA_BOOL_WITH_BASE(GenerateLanterns);
  DATA_MEMBER_WITH_BASE(ushort, Type);
  DATA_BOOL_WITH_BASE(GenerateFountains);
  DATA_BOOL_WITH_BASE(AllowLockedDoors);
  DATA_BOOL_WITH_BASE(AllowBoobyTrappedDoors);
  DATA_MEMBER_WITH_BASE(uchar, Shape);
};

class levelscript : public scriptwithbase
{
 public:
  typedef levelscript scripttype;
  void ReadFrom(inputfile&);
  const std::list<squarescript>& GetSquare() const { return Square; }
  const std::list<roomscript>& GetRoom() const { return Room; }
  void Combine(levelscript&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetBase(const scriptwithbase*);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::list<squarescript> Square;
  std::list<roomscript> Room;
  DATA_MEMBER_WITH_BASE(roomscript, RoomDefault);
  DATA_MEMBER_WITH_BASE(squarescript, FillSquare);
  DATA_MEMBER_WITH_BASE(std::string, LevelMessage);
  DATA_MEMBER_WITH_BASE(vector2d, Size);
  DATA_MEMBER_WITH_BASE(interval, Items);
  DATA_MEMBER_WITH_BASE(interval, Rooms);
  DATA_BOOL_WITH_BASE(GenerateMonsters);
  DATA_BOOL_WITH_BASE(IsOnGround);
  DATA_MEMBER_WITH_BASE(uchar, TeamDefault);
  DATA_MEMBER_WITH_BASE(ulong, AmbientLight);
  DATA_MEMBER_WITH_BASE(std::string, Description);
  DATA_MEMBER_WITH_BASE(uchar, LOSModifier);
  DATA_BOOL_WITH_BASE(IgnoreDefaultSpecialSquares);
  DATA_MEMBER_WITH_BASE(short, DifficultyBase);
  DATA_MEMBER_WITH_BASE(short, DifficultyDelta);
  DATA_MEMBER_WITH_BASE(short, MonsterAmountBase);
  DATA_MEMBER_WITH_BASE(short, MonsterAmountDelta);
  DATA_MEMBER_WITH_BASE(short, MonsterGenerationIntervalBase);
  DATA_MEMBER_WITH_BASE(short, MonsterGenerationIntervalDelta);
  DATA_BOOL_WITH_BASE(AutoReveal);
  DATA_MEMBER_WITH_BASE(std::string, ShortDescription);
};

class dungeonscript : public script
{
 public:
  typedef dungeonscript scripttype;
  dungeonscript();
  virtual ~dungeonscript();
  virtual void ReadFrom(inputfile&);
  const std::map<uchar, levelscript>& GetLevel() const { return Level; }
  void RandomizeLevels();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual const datamap& GetDataMap() const { return DataMap; }
  static void InitDataMap();
 protected:
  static datamap DataMap;
  std::map<uchar, levelscript> Level;
  std::list<std::pair<interval, levelscript> > RandomLevel;
  DATA_MEMBER(levelscript, LevelDefault);
  DATA_MEMBER(uchar, Levels);
  DATA_MEMBER(std::string, Description);
  DATA_MEMBER(std::string, ShortDescription);
};

class teamscript : public script
{
 public:
  typedef teamscript scripttype;
  virtual void ReadFrom(inputfile&);
  const std::vector<std::pair<uchar, uchar> >& GetRelation() const { return Relation; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::vector<std::pair<uchar, uchar> > Relation;
  DATA_MEMBER(ushort, KillEvilness);
};

class gamescript : public script
{
 public:
  typedef gamescript scripttype;
  virtual void ReadFrom(inputfile&);
  const std::list<std::pair<uchar, teamscript> >& GetTeam() const { return Team; }
  const std::map<uchar, dungeonscript>& GetDungeon() const { return Dungeon; }
  void RandomizeLevels();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::list<std::pair<uchar, teamscript> > Team;
  std::map<uchar, dungeonscript> Dungeon;
  DATA_MEMBER(uchar, Dungeons);
  DATA_MEMBER(uchar, Teams);
};

outputfile& operator<<(outputfile&, const gamescript*);
inputfile& operator>>(inputfile&, gamescript*&);

class scriptsystem
{
 public:
  static void Initialize();
};

#endif
