#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "save.h"
#include "vector2d.h"
#include "error.h"

#define DATA_MEMBER(type, name)\
 public:\
  type* Get##name(bool AbortOnError = true) const { return name.GetMember(Base, #name, AbortOnError); }\
 protected:\
  datamember< type > name;

#define PROTO_NAMED_MEMBER(type, name)\
 public:\
  ushort* Get##name(bool AbortOnError = true) const { return name.GetMember(Base, #name, AbortOnError); }\
 protected:\
  protonamedmember< type > name;

class inputfile;
class glterrain;
class olterrain;
class character;
class item;
class god;
class room;
class material;
class script;

template <class type> inline bool IsValidScript(const std::vector<type>& Vector)
{
  for(ushort c = 0; c < Vector.size(); ++c)
    if(IsValidScript(Vector[c]))
      return true;

  return false;
}

class datamemberbase
{
 public:
  virtual ~datamemberbase() { }
  virtual void Load(inputfile&, const valuemap&) = 0;
};

template <class type> class datamembertemplate : public datamemberbase
{
 public:
  virtual ~datamembertemplate() { delete Member; }
  datamembertemplate() : Member(0) { }
  datamembertemplate(const datamembertemplate& Data) : datamemberbase(Data), Member(Data.Member ? new type(*Data.Member) : 0) {  }
  datamembertemplate& operator=(const datamembertemplate&);
  type* GetMember() const { return Member; }
  type* GetMember(const script*, const std::string&, bool) const;
  void SetMember(type* What) { Member = What; }
 protected:
  type* Member;
};

template <class type> inline datamembertemplate<type>& datamembertemplate<type>::operator=(const datamembertemplate<type>& Data)
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
  else
    {
      if(Data.Member)
	Member = new type(*Data.Member);
    }

  return *this;
}

template <class type> inline type* datamembertemplate<type>::GetMember(const script* Base, const std::string& Identifier, bool AbortOnError) const
{
  if(Member)
    return Member;
  else
    if(Base)
      return static_cast<const datamembertemplate<type>*>(Base->GetConstData(Identifier))->GetMember(Base->GetBase(), Identifier, AbortOnError);
    else
    {
      if(AbortOnError)
	ABORT("Undefined script member %s sought!", Identifier.c_str());

      return 0;
    }
}

template <class type> class datamember : public datamembertemplate<type>
{
 public:
  virtual void Load(inputfile&, const valuemap&);
};

template <class type> class protonamedmember : public datamembertemplate<ushort>
{
 public:
  virtual void Load(inputfile&, const valuemap&);
};

class script
{
 public:
  script() : Base(0) { }
  virtual ~script() { }
  const valuemap& GetValueMap() const { return ValueMap; }
  void SetValueMap(const valuemap& What) { ValueMap = What; }
  bool LoadData(inputfile&, const std::string&);
  virtual datamemberbase* GetData(const std::string&) = 0;
  const datamemberbase* GetConstData(const std::string& Identifier) const { return const_cast<script*>(this)->GetData(Identifier); }
  script* GetBase() const { return Base; }
  void SetBase(script* What) { Base = What; }
  virtual void ReadFrom(inputfile&, bool = false) = 0;
 protected:
  valuemap ValueMap;
  script* Base;
};

inline void ReadData(script& Type, inputfile& SaveFile, const valuemap& ValueMap)
{
  Type.SetValueMap(ValueMap);
  Type.ReadFrom(SaveFile);
}

class posscript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  bool GetRandom() const { return Random; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  bool Random;
  DATA_MEMBER(vector2d, Vector);
  DATA_MEMBER(uchar, Flags);
  DATA_MEMBER(rect, Borders);
};

class materialscript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  void SetConfig(ushort What) { Config = What; }
  material* Instantiate() const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATA_MEMBER(ulong, Volume);
  ushort Config;
};

class basecontentscript : public script
{
 public:
  basecontentscript() : ContentType(0), Config(0), Random(false) { }
  virtual void ReadFrom(inputfile&, bool = false);
  virtual datamemberbase* GetData(const std::string&);
  ushort GetContentType() const { return ContentType; }
  bool IsValid() const { return ContentType || Random; }
 protected:
  virtual const std::string& GetClassId() const = 0;
  virtual ushort SearchCodeName(const std::string&) const = 0;
  DATA_MEMBER(materialscript, MainMaterial);
  DATA_MEMBER(materialscript, SecondaryMaterial);
  DATA_MEMBER(materialscript, ContainedMaterial);
  DATA_MEMBER(ulong, Parameters);
  ushort ContentType;
  ushort Config;
  bool Random;
};

inline bool IsValidScript(const basecontentscript& S) { return S.IsValid(); }

template <class type> class contentscripttemplate : public basecontentscript
{
 protected:
  virtual const std::string& GetClassId() const;
  type* BasicInstantiate(ushort) const;
  virtual ushort SearchCodeName(const std::string&) const;
};

template <class type> class contentscript;

class contentscript<character> : public contentscripttemplate<character>
{
 public:
  character* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATA_MEMBER(ushort, Team);
  DATA_MEMBER(std::vector<contentscript<item> >, Inventory);
  DATA_MEMBER(bool, IsMaster);
  DATA_MEMBER(std::vector<vector2d>, WayPoint);
};

class contentscript<item> : public contentscripttemplate<item>
{
 public:
  item* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATA_MEMBER(ushort, Team);
  DATA_MEMBER(bool, Active);
  DATA_MEMBER(uchar, SideStackIndex);
  DATA_MEMBER(short, Enchantment);
  DATA_MEMBER(ulong, MinPrice);
  DATA_MEMBER(ulong, MaxPrice);
  DATA_MEMBER(ulong, Category);
  DATA_MEMBER(std::vector<contentscript<item> >, ItemsInside);
  DATA_MEMBER(uchar, Chance);
};

class contentscript<glterrain> : public contentscripttemplate<glterrain>
{
 public:
  glterrain* Instantiate(ushort SpecialFlags = 0) const { return contentscripttemplate<glterrain>::BasicInstantiate(SpecialFlags); }
};

class contentscript<olterrain> : public contentscripttemplate<olterrain>
{
 public:
  olterrain* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATA_MEMBER(uchar, VisualEffects);
  DATA_MEMBER(uchar, AttachedArea);
  DATA_MEMBER(uchar, AttachedEntry);
  DATA_MEMBER(std::string, Text);
  DATA_MEMBER(std::vector<contentscript<item> >, ItemsInside);
};

class squarescript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATA_MEMBER(posscript, Position);
  DATA_MEMBER(contentscript<character>, Character);
  DATA_MEMBER(std::vector<contentscript<item> >, Items);
  DATA_MEMBER(contentscript<glterrain>, GTerrain);
  DATA_MEMBER(contentscript<olterrain>, OTerrain);
  DATA_MEMBER(uchar, Times);
  DATA_MEMBER(bool, AttachRequired);
  DATA_MEMBER(uchar, EntryIndex);
};

template <class type, class contenttype = contentscript<type> > class contentmap : public script
{
 public:
  contentmap() : ContentMap(0) { }
  virtual ~contentmap() { DeleteContents(); }
  virtual void ReadFrom(inputfile&, bool = false);
  void DeleteContents();
  const contenttype* GetContentScript(ushort X, ushort Y) const { return ContentMap[X][Y]; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  contenttype*** ContentMap;
  DATA_MEMBER(vector2d, Size);
  DATA_MEMBER(vector2d, Pos);
};

typedef contentmap<item, std::vector<contentscript<item> > > itemcontentmap;
typedef contentmap<character> charactercontentmap;
typedef contentmap<glterrain> glterraincontentmap;
typedef contentmap<olterrain> olterraincontentmap;

class roomscript : public script
{
 public:
  virtual ~roomscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() const { return Square; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  ulong BufferPos;
  std::vector<squarescript*> Square;
  DATA_MEMBER(charactercontentmap, CharacterMap);
  DATA_MEMBER(itemcontentmap, ItemMap);
  DATA_MEMBER(glterraincontentmap, GTerrainMap);
  DATA_MEMBER(olterraincontentmap, OTerrainMap);
  DATA_MEMBER(squarescript, WallSquare);
  DATA_MEMBER(squarescript, FloorSquare);
  DATA_MEMBER(squarescript, DoorSquare);
  DATA_MEMBER(vector2d, Size);
  DATA_MEMBER(vector2d, Pos);
  DATA_MEMBER(bool, AltarPossible);
  DATA_MEMBER(bool, GenerateDoor);
  DATA_MEMBER(bool, ReCalculate);
  DATA_MEMBER(bool, GenerateTunnel);
  DATA_MEMBER(uchar, DivineMaster);
  DATA_MEMBER(bool, GenerateLanterns);
  PROTO_NAMED_MEMBER(room, Type);
  DATA_MEMBER(bool, GenerateFountains);
  DATA_MEMBER(bool, AllowLockedDoors);
  DATA_MEMBER(bool, AllowBoobyTrappedDoors);
  DATA_MEMBER(uchar, Shape);
};

class levelscript : public script
{
 public:
  virtual ~levelscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() const { return Square; }
  const std::map<uchar, roomscript*>& GetRoom() const { return Room; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  ulong BufferPos;
  std::vector<squarescript*> Square;
  std::map<uchar, roomscript*> Room;
  DATA_MEMBER(roomscript, RoomDefault);
  DATA_MEMBER(squarescript, FillSquare);
  DATA_MEMBER(std::string, LevelMessage);
  DATA_MEMBER(vector2d, Size);
  DATA_MEMBER(ushort, Items);
  DATA_MEMBER(uchar, Rooms);
  DATA_MEMBER(bool, GenerateMonsters);
  DATA_MEMBER(bool, ReCalculate);
  DATA_MEMBER(bool, OnGround);
  DATA_MEMBER(uchar, TeamDefault);
  DATA_MEMBER(ulong, AmbientLight);
  DATA_MEMBER(std::string, Description);
  DATA_MEMBER(uchar, LOSModifier);
  DATA_MEMBER(bool, IgnoreDefaultSpecialSquares);
  DATA_MEMBER(short, DifficultyBase);
  DATA_MEMBER(short, DifficultyDelta);
  DATA_MEMBER(short, MonsterAmountBase);
  DATA_MEMBER(short, MonsterAmountDelta);
  DATA_MEMBER(short, MonsterGenerationIntervalBase);
  DATA_MEMBER(short, MonsterGenerationIntervalDelta);
  DATA_MEMBER(bool, AutoReveal);
  DATA_MEMBER(std::string, ShortDescription);
};

class dungeonscript : public script
{
 public:
  virtual ~dungeonscript();
  virtual void ReadFrom(inputfile&, bool = false);
  const std::map<uchar, levelscript*>& GetLevel() const { return Level; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  std::map<uchar, levelscript*> Level;
  DATA_MEMBER(levelscript, LevelDefault);
  DATA_MEMBER(uchar, Levels);
  DATA_MEMBER(std::string, Description);
  DATA_MEMBER(std::string, ShortDescription);
};

class teamscript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  const std::vector<std::pair<uchar, uchar> >& GetRelation() const { return Relation; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  std::vector<std::pair<uchar, uchar> > Relation;
  DATA_MEMBER(ushort, KillEvilness);
};

class gamescript : public script
{
 public:
  virtual ~gamescript();
  virtual void ReadFrom(inputfile&, bool = false);
  const std::vector<std::pair<uchar, teamscript*> >& GetTeam() const { return Team; }
  const std::map<uchar, dungeonscript*>& GetDungeon() const { return Dungeon; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  std::vector<std::pair<uchar, teamscript*> > Team;
  std::map<uchar, dungeonscript*> Dungeon;
  DATA_MEMBER(dungeonscript, DungeonDefault);
  DATA_MEMBER(uchar, Dungeons);
  DATA_MEMBER(uchar, Teams);
};

#endif
