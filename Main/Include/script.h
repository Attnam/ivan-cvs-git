#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>
#include <map>

#include "rect.h"
#include "femath.h"
#include "festring.h"

#define SCRIPT_MEMBER(type, name)\
 public:\
  const type* Get##name() const { return name##Holder.GetMember(); }\
 protected:\
  scriptmember< type > name##Holder;

#define SCRIPT_MEMBER_WITH_BASE(type, name)\
 public:\
  const type* Get##name() const { return GetMemberOf(name##Holder, Base, &scripttype::Get##name); }\
 protected:\
  scriptmember< type > name##Holder;

#define SCRIPT_BOOL(name)\
 public:\
  const bool* name() const { return name##Holder.GetMember(); }\
 protected:\
  scriptmember<bool> name##Holder;

#define SCRIPT_BOOL_WITH_BASE(name)\
 public:\
  const bool* name() const { return GetMemberOf(name##Holder, Base, &scripttype::name); }\
 protected:\
  scriptmember<bool> name##Holder;

class glterrain;
class olterrain;
class character;
class item;
class material;
class scriptwithbase;
class outputfile;
class inputfile;

class scriptmemberbase
{
 public:
  virtual ~scriptmemberbase() { }
  virtual void ReadFrom(inputfile&) = 0;
  virtual void Save(outputfile&) const = 0;
  virtual void Load(inputfile&) = 0;
  virtual void Replace(scriptmemberbase&) = 0;
};

template <class type> class scriptmember : public scriptmemberbase
{
 public:
  virtual ~scriptmember() { delete Member; }
  scriptmember() : Member(0) { }
  scriptmember(const scriptmember& Data) : scriptmemberbase(Data), Member(Data.Member ? new type(*Data.Member) : 0) {  }
  scriptmember& operator=(const scriptmember&);
  type* GetMember() const { return Member; }
  void SetMember(type* What) { Member = What; }
  virtual void ReadFrom(inputfile&);
  virtual void Replace(scriptmemberbase&);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  type* Member;
};

template <class type, class scripttype> inline const type* GetMemberOf(const scriptmember<type>& Data, const scriptwithbase* Base, const type* (scripttype::*MemberRetriever)() const)
{
  return Data.GetMember() ? Data.GetMember() : Base ? (static_cast<const scripttype*>(Base)->*MemberRetriever)() : 0;
}

template <class type> inline scriptmember<type>& scriptmember<type>::operator=(const scriptmember<type>& Data)
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

class script
{
 public:
  typedef std::map<const char*, scriptmemberbase script::*, charcomparer> datamap;
  virtual ~script() { }
  virtual void ReadFrom(inputfile&) = 0;
  virtual void Save(outputfile& SaveFile) const { SaveDataMap(GetDataMap(), SaveFile); }
  virtual void Load(inputfile& SaveFile) { LoadDataMap(GetDataMap(), SaveFile); }
 protected:
  bool ReadMember(inputfile&, const festring&);
  virtual scriptmemberbase* GetDataFromMap(const datamap&, const char*);
  virtual scriptmemberbase* GetData(const char* String) { return GetDataFromMap(GetDataMap(), String); }
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
  SCRIPT_MEMBER(vector2d, Vector);
  SCRIPT_MEMBER(uchar, Flags);
  SCRIPT_MEMBER(rect, Borders);
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
  SCRIPT_MEMBER(ulong, Volume);
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
  virtual scriptmemberbase* GetData(const char*);
  virtual ushort SearchCodeName(const festring&) const = 0;
  virtual const char* GetClassID() const = 0;
  static datamap DataMap;
  SCRIPT_MEMBER(materialscript, MainMaterial);
  SCRIPT_MEMBER(materialscript, SecondaryMaterial);
  SCRIPT_MEMBER(materialscript, ContainedMaterial);
  SCRIPT_MEMBER(ulong, Parameters);
  ushort ContentType;
  ushort Config;
  bool Random;
};

inline bool IsValidScript(const basecontentscript* S) { return S->IsValid(); }

template <class type> class contentscripttemplate : public basecontentscript
{
 protected:
  type* BasicInstantiate(ushort) const;
  virtual ushort SearchCodeName(const festring&) const;
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
  virtual const char* GetClassID() const;
  static datamap DataMap;
  SCRIPT_MEMBER(ushort, Team);
  SCRIPT_MEMBER(std::list<contentscript<item> >, Inventory);
  SCRIPT_BOOL(IsMaster);
  SCRIPT_MEMBER(std::vector<vector2d>, WayPoint);
};

class contentscript<item> : public contentscripttemplate<item>
{
 public:
  typedef contentscript<item> scripttype;
  item* Instantiate(ushort = 0) const;
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  virtual const char* GetClassID() const;
  static datamap DataMap;
  SCRIPT_MEMBER(ushort, Team);
  SCRIPT_BOOL(IsActive);
  SCRIPT_MEMBER(uchar, SideStackIndex);
  SCRIPT_MEMBER(short, Enchantment);
  SCRIPT_MEMBER(ulong, MinPrice);
  SCRIPT_MEMBER(ulong, MaxPrice);
  SCRIPT_MEMBER(ulong, Category);
  SCRIPT_MEMBER(std::list<contentscript<item> >, ItemsInside);
  SCRIPT_MEMBER(uchar, Chance);
  SCRIPT_MEMBER(ushort, ConfigFlags);
};

bool IsValidScript(const std::list<contentscript<item> >*);

class contentscript<glterrain> : public contentscripttemplate<glterrain>
{
 public:
  typedef contentscript<glterrain> scripttype;
  glterrain* Instantiate(ushort SpecialFlags = 0) const { return contentscripttemplate<glterrain>::BasicInstantiate(SpecialFlags); }
  static void InitDataMap() { }
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  virtual const char* GetClassID() const;
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
  virtual const char* GetClassID() const;
  SCRIPT_MEMBER(uchar, VisualEffects);
  SCRIPT_MEMBER(uchar, AttachedArea);
  SCRIPT_MEMBER(uchar, AttachedEntry);
  SCRIPT_MEMBER(festring, Text);
  SCRIPT_MEMBER(std::list<contentscript<item> >, ItemsInside);
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
  SCRIPT_MEMBER(posscript, Position);
  SCRIPT_MEMBER(contentscript<character>, Character);
  SCRIPT_MEMBER(std::list<contentscript<item> >, Items);
  SCRIPT_MEMBER(contentscript<glterrain>, GTerrain);
  SCRIPT_MEMBER(contentscript<olterrain>, OTerrain);
  SCRIPT_MEMBER(interval, Times);
  SCRIPT_BOOL(AttachRequired);
  SCRIPT_MEMBER(uchar, EntryIndex);
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
  SCRIPT_MEMBER(vector2d, Size);
  SCRIPT_MEMBER(vector2d, Pos);
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
  const std::list<squarescript>& GetSquare() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::list<squarescript> Square;
  SCRIPT_MEMBER_WITH_BASE(charactercontentmap, CharacterMap);
  SCRIPT_MEMBER_WITH_BASE(itemcontentmap, ItemMap);
  SCRIPT_MEMBER_WITH_BASE(glterraincontentmap, GTerrainMap);
  SCRIPT_MEMBER_WITH_BASE(olterraincontentmap, OTerrainMap);
  SCRIPT_MEMBER_WITH_BASE(squarescript, WallSquare);
  SCRIPT_MEMBER_WITH_BASE(squarescript, FloorSquare);
  SCRIPT_MEMBER_WITH_BASE(squarescript, DoorSquare);
  SCRIPT_MEMBER_WITH_BASE(region, Size);
  SCRIPT_MEMBER_WITH_BASE(region, Pos);
  SCRIPT_BOOL_WITH_BASE(AltarPossible);
  SCRIPT_BOOL_WITH_BASE(GenerateDoor);
  SCRIPT_BOOL_WITH_BASE(GenerateTunnel);
  SCRIPT_MEMBER_WITH_BASE(uchar, DivineMaster);
  SCRIPT_BOOL_WITH_BASE(GenerateLanterns);
  SCRIPT_MEMBER_WITH_BASE(ushort, Type);
  SCRIPT_BOOL_WITH_BASE(GenerateFountains);
  SCRIPT_BOOL_WITH_BASE(AllowLockedDoors);
  SCRIPT_BOOL_WITH_BASE(AllowBoobyTrappedDoors);
  SCRIPT_MEMBER_WITH_BASE(uchar, Shape);
};

class levelscript : public scriptwithbase
{
 public:
  typedef levelscript scripttype;
  void ReadFrom(inputfile&);
  const std::list<squarescript>& GetSquare() const;
  const std::list<roomscript>& GetRoom() const;
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
  SCRIPT_MEMBER_WITH_BASE(roomscript, RoomDefault);
  SCRIPT_MEMBER_WITH_BASE(squarescript, FillSquare);
  SCRIPT_MEMBER_WITH_BASE(festring, LevelMessage);
  SCRIPT_MEMBER_WITH_BASE(vector2d, Size);
  SCRIPT_MEMBER_WITH_BASE(interval, Items);
  SCRIPT_MEMBER_WITH_BASE(interval, Rooms);
  SCRIPT_BOOL_WITH_BASE(GenerateMonsters);
  SCRIPT_BOOL_WITH_BASE(IsOnGround);
  SCRIPT_MEMBER_WITH_BASE(uchar, TeamDefault);
  SCRIPT_MEMBER_WITH_BASE(ulong, AmbientLight);
  SCRIPT_MEMBER_WITH_BASE(festring, Description);
  SCRIPT_MEMBER_WITH_BASE(uchar, LOSModifier);
  SCRIPT_BOOL_WITH_BASE(IgnoreDefaultSpecialSquares);
  SCRIPT_MEMBER_WITH_BASE(short, DifficultyBase);
  SCRIPT_MEMBER_WITH_BASE(short, DifficultyDelta);
  SCRIPT_MEMBER_WITH_BASE(short, MonsterAmountBase);
  SCRIPT_MEMBER_WITH_BASE(short, MonsterAmountDelta);
  SCRIPT_MEMBER_WITH_BASE(short, MonsterGenerationIntervalBase);
  SCRIPT_MEMBER_WITH_BASE(short, MonsterGenerationIntervalDelta);
  SCRIPT_BOOL_WITH_BASE(AutoReveal);
  SCRIPT_MEMBER_WITH_BASE(festring, ShortDescription);
  SCRIPT_BOOL_WITH_BASE(CanGenerateBone);
  SCRIPT_MEMBER_WITH_BASE(ushort, ItemMinPriceBase);
  SCRIPT_MEMBER_WITH_BASE(ushort, ItemMinPriceDelta);
  SCRIPT_MEMBER_WITH_BASE(ushort, Type);
};

class dungeonscript : public script
{
 public:
  typedef dungeonscript scripttype;
  dungeonscript();
  virtual ~dungeonscript();
  virtual void ReadFrom(inputfile&);
  const std::map<uchar, levelscript>& GetLevel() const;
  void RandomizeLevels();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual const datamap& GetDataMap() const { return DataMap; }
  static void InitDataMap();
 protected:
  static datamap DataMap;
  std::map<uchar, levelscript> Level;
  std::list<std::pair<interval, levelscript> > RandomLevel;
  SCRIPT_MEMBER(levelscript, LevelDefault);
  SCRIPT_MEMBER(uchar, Levels);
  SCRIPT_MEMBER(festring, Description);
  SCRIPT_MEMBER(festring, ShortDescription);
};

class teamscript : public script
{
 public:
  typedef teamscript scripttype;
  virtual void ReadFrom(inputfile&);
  const std::vector<std::pair<uchar, uchar> >& GetRelation() const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::vector<std::pair<uchar, uchar> > Relation;
  SCRIPT_MEMBER(ushort, KillEvilness);
};

class gamescript : public script
{
 public:
  typedef gamescript scripttype;
  virtual void ReadFrom(inputfile&);
  const std::list<std::pair<uchar, teamscript> >& GetTeam() const;
  const std::map<uchar, dungeonscript>& GetDungeon() const;
  void RandomizeLevels();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  static void InitDataMap();
 protected:
  virtual const datamap& GetDataMap() const { return DataMap; }
  static datamap DataMap;
  std::list<std::pair<uchar, teamscript> > Team;
  std::map<uchar, dungeonscript> Dungeon;
  SCRIPT_MEMBER(uchar, Dungeons);
  SCRIPT_MEMBER(uchar, Teams);
};

outputfile& operator<<(outputfile&, const gamescript*);
inputfile& operator>>(inputfile&, gamescript*&);

class scriptsystem
{
 public:
  static void Initialize();
};

#endif
