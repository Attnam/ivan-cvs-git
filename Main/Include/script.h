#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "save.h"
#include "vector2d.h"
#include "error.h"

#define DATAMEMBER(type, name)\
 public:\
  type* Get##name(bool AbortOnError = true) const { return name.GetMember(Base, #name, AbortOnError); }\
 protected:\
  datamember< type > name;

#define PROTONAMEDMEMBER(type, name)\
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
  DATAMEMBER(vector2d, Vector);
  DATAMEMBER(uchar, Flags);
  DATAMEMBER(rect, Borders);
};

class materialscript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  void SetConfig(ushort What) { Config = What; }
  material* Instantiate() const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATAMEMBER(ulong, Volume);
  ushort Config;
};

class basecontentscript : public script
{
 public:
  basecontentscript() : ContentType(0), Config(0) { }
  virtual void ReadFrom(inputfile&, bool = false);
  virtual datamemberbase* GetData(const std::string&);
  ushort GetContentType() const { return ContentType; }
  bool IsValid() const { return ContentType || Random; }
 protected:
  virtual const std::string& GetClassId() const = 0;
  virtual ushort SearchCodeName(const std::string&) const = 0;
  DATAMEMBER(materialscript, MainMaterial);
  DATAMEMBER(materialscript, SecondaryMaterial);
  DATAMEMBER(materialscript, ContainedMaterial);
  DATAMEMBER(ulong, Parameters);
  ushort ContentType;
  ushort Config;
  bool Random;
};

inline bool IsValidScript(const basecontentscript& S) { return S.IsValid(); }

template <class type> class contentscripttemplate : public basecontentscript
{
 protected:
  virtual const std::string& GetClassId() const;
  void BasicInstantiate(std::vector<type*>&, ulong, ushort, uchar = 100) const;
  virtual ushort SearchCodeName(const std::string&) const;
};

template <class type> class contentscript;

class contentscript<character> : public contentscripttemplate<character>
{
 public:
  void Instantiate(std::vector<character*>&, ulong, ushort = 0) const;
  character* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATAMEMBER(ushort, Team);
  DATAMEMBER(std::vector<contentscript<item> >, Inventory);
};

class contentscript<item> : public contentscripttemplate<item>
{
 public:
  void Instantiate(std::vector<item*>&, ulong, ushort = 0) const;
  item* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATAMEMBER(ushort, Team);
  DATAMEMBER(bool, Active);
  DATAMEMBER(uchar, SideStackIndex);
  DATAMEMBER(short, Enchantment);
  DATAMEMBER(ulong, MinPrice);
  DATAMEMBER(ulong, MaxPrice);
  DATAMEMBER(ulong, Category);
  DATAMEMBER(std::vector<contentscript<item> >, ItemsInside);
  DATAMEMBER(uchar, Chance);
};

class contentscript<glterrain> : public contentscripttemplate<glterrain>
{
 public:
  void Instantiate(std::vector<glterrain*>&, ulong, ushort = 0) const;
  glterrain* Instantiate(ushort = 0) const;
};

class contentscript<olterrain> : public contentscripttemplate<olterrain>
{
 public:
  void Instantiate(std::vector<olterrain*>&, ulong, ushort = 0) const;
  olterrain* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATAMEMBER(uchar, VisualEffects);
  DATAMEMBER(uchar, AttachedArea);
  DATAMEMBER(uchar, AttachedEntry);
};

class squarescript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  virtual datamemberbase* GetData(const std::string&);
 protected:
  DATAMEMBER(posscript, Position);
  DATAMEMBER(contentscript<character>, Character);
  DATAMEMBER(std::vector<contentscript<item> >, Items);
  DATAMEMBER(contentscript<glterrain>, GTerrain);
  DATAMEMBER(contentscript<olterrain>, OTerrain);
  DATAMEMBER(uchar, Times);
  DATAMEMBER(bool, AttachRequired);
  DATAMEMBER(uchar, EntryIndex);
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
  DATAMEMBER(vector2d, Size);
  DATAMEMBER(vector2d, Pos);
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
  DATAMEMBER(charactercontentmap, CharacterMap);
  DATAMEMBER(itemcontentmap, ItemMap);
  DATAMEMBER(glterraincontentmap, GTerrainMap);
  DATAMEMBER(olterraincontentmap, OTerrainMap);
  DATAMEMBER(squarescript, WallSquare);
  DATAMEMBER(squarescript, FloorSquare);
  DATAMEMBER(squarescript, DoorSquare);
  DATAMEMBER(vector2d, Size);
  DATAMEMBER(vector2d, Pos);
  DATAMEMBER(bool, AltarPossible);
  DATAMEMBER(bool, GenerateDoor);
  DATAMEMBER(bool, ReCalculate);
  DATAMEMBER(bool, GenerateTunnel);
  PROTONAMEDMEMBER(god, DivineMaster);
  DATAMEMBER(bool, GenerateLanterns);
  PROTONAMEDMEMBER(room, Type);
  DATAMEMBER(bool, GenerateFountains);
  DATAMEMBER(bool, AllowLockedDoors);
  DATAMEMBER(bool, AllowBoobyTrappedDoors);
  DATAMEMBER(uchar, Shape);
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
  DATAMEMBER(roomscript, RoomDefault);
  DATAMEMBER(squarescript, FillSquare);
  DATAMEMBER(std::string, LevelMessage);
  DATAMEMBER(vector2d, Size);
  DATAMEMBER(ushort, Items);
  DATAMEMBER(uchar, Rooms);
  DATAMEMBER(bool, GenerateMonsters);
  DATAMEMBER(bool, ReCalculate);
  DATAMEMBER(bool, OnGround);
  DATAMEMBER(uchar, TeamDefault);
  DATAMEMBER(ulong, AmbientLight);
  DATAMEMBER(std::string, Description);
  DATAMEMBER(uchar, LOSModifier);
  DATAMEMBER(bool, IgnoreDefaultSpecialSquares);
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
  DATAMEMBER(levelscript, LevelDefault);
  DATAMEMBER(uchar, Levels);
};

class teamscript : public script
{
 public:
  virtual void ReadFrom(inputfile&, bool = false);
  const std::vector<std::pair<uchar, uchar> >& GetRelation() const { return Relation; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  std::vector<std::pair<uchar, uchar> > Relation;
  DATAMEMBER(ushort, AttackEvilness);
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
  DATAMEMBER(dungeonscript, DungeonDefault);
  DATAMEMBER(uchar, Dungeons);
  DATAMEMBER(uchar, Teams);
};

#endif
