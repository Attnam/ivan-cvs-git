#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

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

#define INITMEMBER(name)\
  {\
    name.SetIdentifier(#name);\
    Data.push_back(&name);\
  }

#include <vector>

#include "typedef.h"
#include "vector2d.h"

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
  virtual void SetBase(datamemberbase*) = 0;
  virtual bool Load(const std::string&, inputfile&, const valuemap&) = 0;
  virtual void Load(inputfile&, const valuemap&) = 0;
  void SetIdentifier(const std::string& What) { Identifier = What; }
 protected:
  std::string Identifier;
};

template <class type> class datamembertemplate : public datamemberbase
{
 public:
  datamembertemplate() : Base(0), Member(0) { }
  type* GetMember(bool) const;
  void SetMember(type* What) { Member = What; }
  virtual void SetBase(datamemberbase*);
 protected:
  datamembertemplate<type>* Base;
  type* Member;
};

template <class type> class datamember : public datamembertemplate<type>
{
 public:
  virtual bool Load(const std::string&, inputfile&, const valuemap&);
  virtual void Load(inputfile&, const valuemap&);
};

template <class type> class protonamedmember : public datamembertemplate<ushort>
{
 public:
  virtual bool Load(const std::string&, inputfile&, const valuemap&);
  virtual void Load(inputfile&, const valuemap&);
};

class script
{
 public:
  valuemap& GetValueMap() { return ValueMap; }
  void SetValueMap(const valuemap& What) { ValueMap = What; }
  datamemberbase* GetDataMember(ushort Index) const { return Data[Index]; }
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
  void SetType(ushort What) { Type = What; }
  ushort GetType() const { return Type; }
  material* Instantiate() const;
 protected:
  DATAMEMBER(ulong, Volume);
  ushort Type;
};

template <class type> class basecontentscript : public script
{
 public:
  basecontentscript();
  virtual ~basecontentscript() { }
  virtual void ReadFrom(inputfile&);
  virtual ushort GetContentType() const { return ContentType; }
  virtual type* Instantiate() const;
 protected:
  DATAMEMBER(materialscript, MainMaterial);
  DATAMEMBER(materialscript, SecondaryMaterial);
  DATAMEMBER(materialscript, ContainedMaterial);
  ushort ContentType;
};

template <class type> class contentscript : public basecontentscript<type> { };

class contentscript<character> : public basecontentscript<character>
{
 public:
  contentscript<character>();
  virtual character* Instantiate() const;
 protected:
  DATAMEMBER(ushort, Team);
};

class contentscript<olterrain> : public basecontentscript<olterrain>
{
 public:
  contentscript<olterrain>();
  virtual olterrain* Instantiate() const;
 protected:
  DATAMEMBER(bool, Locked);
  DATAMEMBER(uchar, VisualFlags);
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
  void ReadFrom(inputfile&);
  contentscript<type>* GetContentScript(ushort X, ushort Y) { return ContentScriptMap[X][Y]; }
 protected:
  contentscript<type>*** ContentScriptMap;
  DATAMEMBER(vector2d, Size);
  DATAMEMBER(vector2d, Pos);
};

class roomscript : public scriptwithbase<roomscript>
{
 public:
  roomscript();
  void ReadFrom(inputfile&, bool = false);
  std::vector<squarescript*>& GetSquare() { return Square; }
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
  void ReadFrom(inputfile&, bool = false);
  std::vector<squarescript*>& GetSquare() { return Square; }
  std::map<uchar, roomscript*>& GetRoom() { return Room; }
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
  void ReadFrom(inputfile&);
  std::map<uchar, levelscript*>& GetLevel() { return Level; }
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
  std::vector<std::pair<uchar, uchar> >& GetRelation() { return Relation; }
 protected:
  std::vector<std::pair<uchar, uchar> > Relation;
  DATAMEMBER(ushort, AttackEvilness);
};

class gamescript : public script
{
 public:
  gamescript();
  void ReadFrom(inputfile&);
  std::map<uchar, dungeonscript*>& GetDungeon() { return Dungeon; }
  std::vector<std::pair<uchar, teamscript*> >& GetTeam() { return Team; }
 protected:
  std::map<uchar, dungeonscript*> Dungeon;
  std::vector<std::pair<uchar, teamscript*> > Team;
  DATAMEMBER(dungeonscript, DungeonDefault);
  DATAMEMBER(uchar, Dungeons);
  DATAMEMBER(uchar, Teams);
};

#endif

