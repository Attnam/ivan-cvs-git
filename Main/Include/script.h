#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "rect.h"
#include "save.h"

#define DATA_MEMBER(type, name)\
 public:\
  type* Get##name() const { return name##Holder.GetMember(); }\
 protected:\
  datamember< type > name##Holder;

#define DATA_MEMBER_WITH_BASE(type, name)\
 public:\
  type* Get##name() const { return GetMemberOf(name##Holder, Base, &thistype::Get##name); }\
 protected:\
  datamember< type > name##Holder;

#define DATA_BOOL(name)\
 public:\
  bool* name() const { return name##Holder.GetMember(); }\
 protected:\
  datamember<bool> name##Holder;

#define DATA_BOOL_WITH_BASE(name)\
 public:\
  bool* name() const { return GetMemberOf(name##Holder, Base, &thistype::name); }\
 protected:\
  datamember<bool> name##Holder;

class glterrain;
class olterrain;
class character;
class item;
class material;
class scriptwithbase;

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
  virtual void Load(inputfile&) = 0;
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
  virtual void Load(inputfile&);
 protected:
  type* Member;
};

template <class type, class scripttype> inline type* GetMemberOf(const datamember<type>& Data, const scriptwithbase* Base, type* (scripttype::*MemberRetriever)() const)
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

template <class type> void datamember<type>::Load(inputfile& SaveFile)
{
  if(!Member)
    Member = new type;

  ReadData(*Member, SaveFile);
}

class script
{
 public:
  virtual ~script() { }
  bool LoadData(inputfile&, const std::string&);
  virtual datamemberbase* GetData(const std::string&) = 0;
  virtual void ReadFrom(inputfile&, bool = false) = 0;
};

inline void ReadData(script& Type, inputfile& SaveFile) { Type.ReadFrom(SaveFile); }

class scriptwithbase : public script
{
 public:
  scriptwithbase() : Base(0) { }
  scriptwithbase* GetBase() const { return Base; }
  void SetBase(scriptwithbase* What) { Base = What; }
 protected:
  scriptwithbase* Base;
};

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
  basecontentscript();
  virtual void ReadFrom(inputfile&, bool = false);
  virtual datamemberbase* GetData(const std::string&);
  ushort GetContentType() const { return ContentType; }
  bool IsValid() const { return ContentType || Random; }
 protected:
  virtual ushort SearchCodeName(const std::string&) const = 0;
  virtual const char* GetClassId() const = 0;
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
  virtual const char* GetClassId() const;
  DATA_MEMBER(ushort, Team);
  DATA_MEMBER(std::vector<contentscript<item> >, Inventory);
  DATA_BOOL(IsMaster);
  DATA_MEMBER(std::vector<vector2d>, WayPoint);
};

class contentscript<item> : public contentscripttemplate<item>
{
 public:
  item* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  virtual const char* GetClassId() const;
  DATA_MEMBER(ushort, Team);
  DATA_BOOL(IsActive);
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
 protected:
  virtual const char* GetClassId() const;
};

class contentscript<olterrain> : public contentscripttemplate<olterrain>
{
 public:
  olterrain* Instantiate(ushort = 0) const;
  virtual datamemberbase* GetData(const std::string&);
 protected:
  virtual const char* GetClassId() const;
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
  DATA_BOOL(AttachRequired);
  DATA_MEMBER(uchar, EntryIndex);
};

template <class type, class contenttype = contentscript<type> > class contentmap : public script
{
 public:
  contentmap();
  virtual ~contentmap();
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

class roomscript : public scriptwithbase
{
 public:
  typedef roomscript thistype;
  virtual ~roomscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() const { return Square; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  ulong BufferPos;
  std::vector<squarescript*> Square;
  DATA_MEMBER_WITH_BASE(charactercontentmap, CharacterMap);
  DATA_MEMBER_WITH_BASE(itemcontentmap, ItemMap);
  DATA_MEMBER_WITH_BASE(glterraincontentmap, GTerrainMap);
  DATA_MEMBER_WITH_BASE(olterraincontentmap, OTerrainMap);
  DATA_MEMBER_WITH_BASE(squarescript, WallSquare);
  DATA_MEMBER_WITH_BASE(squarescript, FloorSquare);
  DATA_MEMBER_WITH_BASE(squarescript, DoorSquare);
  DATA_MEMBER_WITH_BASE(vector2d, Size);
  DATA_MEMBER_WITH_BASE(vector2d, Pos);
  DATA_BOOL_WITH_BASE(AltarPossible);
  DATA_BOOL_WITH_BASE(GenerateDoor);
  DATA_BOOL_WITH_BASE(ReCalculate);
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
  typedef levelscript thistype;
  virtual ~levelscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() const { return Square; }
  const std::map<uchar, roomscript*>& GetRoom() const { return Room; }
  virtual datamemberbase* GetData(const std::string&);
 protected:
  ulong BufferPos;
  std::vector<squarescript*> Square;
  std::map<uchar, roomscript*> Room;
  DATA_MEMBER_WITH_BASE(roomscript, RoomDefault);
  DATA_MEMBER_WITH_BASE(squarescript, FillSquare);
  DATA_MEMBER_WITH_BASE(std::string, LevelMessage);
  DATA_MEMBER_WITH_BASE(vector2d, Size);
  DATA_MEMBER_WITH_BASE(ushort, Items);
  DATA_MEMBER_WITH_BASE(uchar, Rooms);
  DATA_BOOL_WITH_BASE(GenerateMonsters);
  DATA_BOOL_WITH_BASE(ReCalculate);
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
  DATA_MEMBER(uchar, Dungeons);
  DATA_MEMBER(uchar, Teams);
};

#endif
