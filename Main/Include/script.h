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
  type* Get##name(bool AbortOnError = true) const { return name.GetMember(AbortOnError); }\
 protected:\
  datamember< type > name ;

#define PROTONAMEDMEMBER(type, name)\
 public:\
  ushort* Get##name(bool AbortOnError = true) const { return name.GetMember(AbortOnError); }\
 protected:\
  protonamedmember< type > name ;

class inputfile;
class glterrain;
class olterrain;
class character;
class item;
class god;
class room;
class material;

class datamemberbase
{
 public:
  virtual ~datamemberbase() { }
  void SetIdentifier(const std::string& What) { Identifier = What; }
  virtual bool Load(const std::string&, inputfile&, const valuemap&, bool = true) = 0;
  virtual void SetBase(datamemberbase*) = 0;
 protected:
  std::string Identifier;
};

template <class type> class datamembertemplate : public datamemberbase
{
 public:
  virtual ~datamembertemplate();
  datamembertemplate() : Base(0), Member(0) { }
  type* GetMember(bool) const;
  void SetMember(type* What) { Member = What; }
  virtual void SetBase(datamemberbase*);
 protected:
  datamembertemplate<type>* Base;
  type* Member;
};

template <class type> inline type* datamembertemplate<type>::GetMember(bool AbortOnError) const
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

template <class type> class datamember : public datamembertemplate<type>
{
 public:
  virtual bool Load(const std::string&, inputfile&, const valuemap&, bool = true);
};

template <class type> class protonamedmember : public datamembertemplate<ushort>
{
 public:
  virtual bool Load(const std::string&, inputfile&, const valuemap&, bool = true);
};

class script
{
 public:
  const valuemap& GetValueMap() const { return ValueMap; }
  void SetValueMap(const valuemap& What) { ValueMap = What; }
  datamemberbase* GetDataMember(ushort Index) const { return Data[Index]; }
  bool LoadData(inputfile&, const std::string&);
 protected:
  std::vector<datamemberbase*> Data;
  valuemap ValueMap;
};

template <class basetype> class scriptwithbase : public script
{
 public:
  scriptwithbase() : Base(0) { }
  basetype* GetBase() const { return Base; }
  void SetBase(basetype*);
 protected:
  basetype* Base;
};

class posscript : public script
{
 public:
  posscript();
  void ReadFrom(inputfile&);
  bool GetRandom() const { return Random; }
 protected:
  bool Random;
  DATAMEMBER(vector2d, Vector);
  DATAMEMBER(bool, Walkable);
  DATAMEMBER(bool, InRoom);
};

class materialscript : public script
{
 public:
  materialscript();
  void ReadFrom(inputfile&);
  void SetConfig(ushort What) { Config = What; }
  material* Instantiate() const;
 protected:
  DATAMEMBER(ulong, Volume);
  ushort Config;
};

class basecontentscript : public script
{
 public:
  basecontentscript();
  virtual ~basecontentscript() { }
  void ReadFrom(inputfile&);
  ushort GetContentType() const { return ContentType; }
 protected:
  virtual std::string ClassName() const = 0;
  virtual ushort SearchCodeName(const std::string&) const = 0;
  DATAMEMBER(materialscript, MainMaterial);
  DATAMEMBER(materialscript, SecondaryMaterial);
  DATAMEMBER(materialscript, ContainedMaterial);
  DATAMEMBER(ulong, Parameters);
  ushort ContentType;
  ushort Config;
};

template <class type> class contentscripttemplate : public basecontentscript
{
 public:
  virtual type* Instantiate() const;
 protected:
  virtual ushort SearchCodeName(const std::string&) const;
};

template <class type> class contentscript;

class contentscript<character> : public contentscripttemplate<character>
{
 public:
  contentscript<character>();
  virtual character* Instantiate() const;
 protected:
  virtual std::string ClassName() const { return "character"; }
  DATAMEMBER(ushort, Team);
};

class contentscript<item> : public contentscripttemplate<item>
{
 public:
  contentscript<item>();
  virtual item* Instantiate() const;
 protected:
  virtual std::string ClassName() const { return "item"; }
  DATAMEMBER(bool, IsVisible);
};

class contentscript<glterrain> : public contentscripttemplate<glterrain>
{
 protected:
  virtual std::string ClassName() const { return "glterrain"; }
};

class contentscript<olterrain> : public contentscripttemplate<olterrain>
{
 public:
  contentscript<olterrain>();
  virtual olterrain* Instantiate() const;
 protected:
  virtual std::string ClassName() const { return "olterrain"; }
  DATAMEMBER(uchar, VisualEffects);
};

class squarescript : public script
{
 public:
  squarescript();
  void ReadFrom(inputfile&);
 protected:
  DATAMEMBER(posscript, Position);
  DATAMEMBER(contentscript<character>, Character);
  DATAMEMBER(contentscript<item>, Item);
  DATAMEMBER(contentscript<glterrain>, GTerrain);
  DATAMEMBER(contentscript<olterrain>, OTerrain);
  DATAMEMBER(bool, IsUpStairs);
  DATAMEMBER(bool, IsDownStairs);
  DATAMEMBER(bool, IsWorldMapEntry);
  DATAMEMBER(uchar, Times);
};

template <class type> class contentmap : public script
{
 public:
  contentmap();
  ~contentmap() { DeleteContents(); }
  void ReadFrom(inputfile&);
  void DeleteContents();
  const contentscript<type>* GetContentScript(ushort X, ushort Y) const { return ContentScriptMap[X][Y]; }
 protected:
  contentscript<type>*** ContentScriptMap;
  DATAMEMBER(vector2d, Size);
  DATAMEMBER(vector2d, Pos);
};

class roomscript : public scriptwithbase<roomscript>
{
 public:
  roomscript();
  ~roomscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() const { return Square; }
 protected:
  ulong BufferPos;
  std::vector<squarescript*> Square;
  DATAMEMBER(contentmap<character>, CharacterMap);
  DATAMEMBER(contentmap<item>, ItemMap);
  DATAMEMBER(contentmap<glterrain>, GTerrainMap);
  DATAMEMBER(contentmap<olterrain>, OTerrainMap);
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
};

class levelscript : public scriptwithbase<levelscript>
{
 public:
  levelscript();
  ~levelscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() const { return Square; }
  const std::map<uchar, roomscript*>& GetRoom() const { return Room; }
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
  DATAMEMBER(bool, GenerateUpStairs);
  DATAMEMBER(bool, GenerateDownStairs);
  DATAMEMBER(bool, OnGround);
  DATAMEMBER(uchar, TeamDefault);
  DATAMEMBER(ushort, AmbientLight);
  DATAMEMBER(std::string, Description);
  DATAMEMBER(uchar, LOSModifier);
};

class dungeonscript : public scriptwithbase<dungeonscript>
{
 public:
  dungeonscript();
  ~dungeonscript();
  void ReadFrom(inputfile&);
  const std::map<uchar, levelscript*>& GetLevel() const { return Level; }
 protected:
  std::map<uchar, levelscript*> Level;
  DATAMEMBER(levelscript, LevelDefault);
  DATAMEMBER(uchar, Levels);
};

class teamscript : public script
{
 public:
  teamscript();
  void ReadFrom(inputfile&);
  const std::vector<std::pair<uchar, uchar> >& GetRelation() const { return Relation; }
 protected:
  std::vector<std::pair<uchar, uchar> > Relation;
  DATAMEMBER(ushort, AttackEvilness);
};

class gamescript : public script
{
 public:
  gamescript();
  ~gamescript();
  void ReadFrom(inputfile&);
  const std::vector<std::pair<uchar, teamscript*> >& GetTeam() const { return Team; }
  const std::map<uchar, dungeonscript*>& GetDungeon() const { return Dungeon; }
 protected:
  std::vector<std::pair<uchar, teamscript*> > Team;
  std::map<uchar, dungeonscript*> Dungeon;
  DATAMEMBER(dungeonscript, DungeonDefault);
  DATAMEMBER(uchar, Dungeons);
  DATAMEMBER(uchar, Teams);
};

#endif

