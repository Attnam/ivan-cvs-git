#ifndef __SCRIPT_H__
#define __SCRIPT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "typedef.h"
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

#define INITMEMBER(name)\
  {\
    name.SetIdentifier(#name);\
    Data.push_back(&name);\
  }

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
  ~datamembertemplate();
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

class basecontentscript : public script
{
 public:
  basecontentscript();
  virtual ~basecontentscript() { }
  virtual void ReadFrom(inputfile&);
  virtual ushort GetContentType() const { return ContentType; }
 protected:
  virtual std::string ClassName() const = 0;
  virtual ushort SearchCodeName(const std::string&) const = 0;
  DATAMEMBER(materialscript, MainMaterial);
  DATAMEMBER(materialscript, SecondaryMaterial);
  DATAMEMBER(materialscript, ContainedMaterial);
  ushort ContentType;
};

template <class type> class contentscripttemplate : public basecontentscript
{
 public:
  virtual type* Instantiate() const;
 protected:
  virtual ushort SearchCodeName(const std::string&) const;
};

template <class type> class contentscript : public contentscripttemplate<type> { };

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
 protected:
  virtual std::string ClassName() const { return "item"; }
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
  ~contentmap() { DeleteContents(); }
  void ReadFrom(inputfile&);
  void DeleteContents();
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
  ~roomscript();
  void ReadFrom(inputfile&, bool = false);
  const std::vector<squarescript*>& GetSquare() { return Square; }
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
  const std::vector<squarescript*>& GetSquare() { return Square; }
  const std::map<uchar, roomscript*>& GetRoom() { return Room; }
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
  const std::map<uchar, levelscript*>& GetLevel() { return Level; }
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
  const std::vector<std::pair<uchar, uchar> >& GetRelation() { return Relation; }
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
  const std::vector<std::pair<uchar, teamscript*> >& GetTeam() { return Team; }
  const std::map<uchar, dungeonscript*>& GetDungeon() { return Dungeon; }
 protected:
  std::vector<std::pair<uchar, teamscript*> > Team;
  std::map<uchar, dungeonscript*> Dungeon;
  DATAMEMBER(dungeonscript, DungeonDefault);
  DATAMEMBER(uchar, Dungeons);
  DATAMEMBER(uchar, Teams);
};

class basedata : public script
{
 public:
  virtual ~basedata() { }
  void ReadFrom(inputfile&);
  ushort GetType() const { return Type; }
  void SetType(ushort What) { Type = What; }
 protected:
  virtual std::string ClassName() const = 0;
  ushort Type;
};

template <class type> class data : public basedata { };

class data<character> : public basedata
{
 public:
  data<character>();
 protected:
  virtual std::string ClassName() const { return "character"; }
  DATAMEMBER(ushort, DefaultAgility);
  DATAMEMBER(ushort, DefaultStrength);
  DATAMEMBER(ushort, DefaultEndurance);
  DATAMEMBER(ushort, DefaultPerception);
  DATAMEMBER(ulong, DefaultMoney);
  DATAMEMBER(ushort, TotalSize);
  DATAMEMBER(bool, CanRead);
  DATAMEMBER(bool, IsCharmable);
  DATAMEMBER(uchar, Sex);
  DATAMEMBER(ulong, BloodColor);
  DATAMEMBER(bool, CanBeGenerated);
  DATAMEMBER(bool, HasInfraVision);
  DATAMEMBER(uchar, CriticalModifier);
  DATAMEMBER(std::string, StandVerb);
  DATAMEMBER(bool, CanOpen);
  DATAMEMBER(bool, CanBeDisplaced);
  DATAMEMBER(ushort, Frequency);
  DATAMEMBER(bool, CanWalk);
  DATAMEMBER(bool, CanSwim);
  DATAMEMBER(bool, CanFly);
  DATAMEMBER(ushort, PhysicalDamageResistance);
  DATAMEMBER(ushort, SoundResistance);
  DATAMEMBER(ushort, EnergyResistance);
  DATAMEMBER(ushort, AcidResistance);
  DATAMEMBER(ushort, FireResistance);
  DATAMEMBER(ushort, PoisonResistance);
  DATAMEMBER(ushort, BulimiaResistance);
  DATAMEMBER(bool, IsUnique);
  DATAMEMBER(ushort, EatFlags);
  DATAMEMBER(ulong, TotalVolume);
  DATAMEMBER(ulong, MeleeStrength);
  DATAMEMBER(std::string, TalkVerb);
  DATAMEMBER(vector2d, HeadBitmapPos);
  DATAMEMBER(vector2d, TorsoBitmapPos);
  DATAMEMBER(vector2d, ArmBitmapPos);
  DATAMEMBER(vector2d, LegBitmapPos);
  DATAMEMBER(vector2d, RightArmBitmapPos);
  DATAMEMBER(vector2d, LeftArmBitmapPos);
  DATAMEMBER(vector2d, RightLegBitmapPos);
  DATAMEMBER(vector2d, LeftLegBitmapPos);
  DATAMEMBER(vector2d, GroinBitmapPos);
  DATAMEMBER(ushort, ClothColor);
  DATAMEMBER(ushort, SkinColor);
  DATAMEMBER(ushort, CapColor);
  DATAMEMBER(ushort, HairColor);
  DATAMEMBER(ushort, EyeColor);
  DATAMEMBER(ushort, TorsoMainColor);
  DATAMEMBER(ushort, BeltColor);
  DATAMEMBER(ushort, TorsoSpecialColor);
  DATAMEMBER(ushort, ArmMainColor);
  DATAMEMBER(ushort, ArmSpecialColor);
  DATAMEMBER(ushort, LegMainColor);
  DATAMEMBER(ushort, LegSpecialColor);
  DATAMEMBER(uchar, HeadBonePercentile);
  DATAMEMBER(uchar, TorsoBonePercentile);
  DATAMEMBER(uchar, ArmBonePercentile);
  DATAMEMBER(uchar, RightArmBonePercentile);
  DATAMEMBER(uchar, LeftArmBonePercentile);
  DATAMEMBER(uchar, GroinBonePercentile);
  DATAMEMBER(uchar, LegBonePercentile);
  DATAMEMBER(uchar, RightLegBonePercentile);
  DATAMEMBER(uchar, LeftLegBonePercentile);
  DATAMEMBER(bool, IsNameable);
  DATAMEMBER(ushort, BaseEmitation);
};

class data<item> : public basedata
{
 public:
  data<item>();
 protected:
  virtual std::string ClassName() const { return "item"; }
  DATAMEMBER(ushort, Possibility);
  DATAMEMBER(vector2d, InHandsPic);
  DATAMEMBER(ulong, OfferModifier);
  DATAMEMBER(long, Score);
  DATAMEMBER(bool, IsDestroyable);
  DATAMEMBER(bool, CanBeWished);
  DATAMEMBER(bool, IsMaterialChangeable);
  DATAMEMBER(uchar, WeaponCategory);
  DATAMEMBER(bool, IsPolymorphSpawnable);
  DATAMEMBER(bool, IsAutoInitializable);
  DATAMEMBER(ushort, OneHandedStrengthPenalty);
  DATAMEMBER(ushort, OneHandedToHitPenalty);
  DATAMEMBER(uchar, Category);
  DATAMEMBER(ushort, SoundResistance);
  DATAMEMBER(ushort, EnergyResistance);
  DATAMEMBER(ushort, AcidResistance);
  DATAMEMBER(ushort, FireResistance);
  DATAMEMBER(ushort, PoisonResistance);
  DATAMEMBER(ushort, BulimiaResistance);
  DATAMEMBER(bool, IsStackable);
  DATAMEMBER(ushort, StrengthModifier);
  DATAMEMBER(ushort, FormModifier);
  DATAMEMBER(ulong, NPModifier);
  DATAMEMBER(ushort, DefaultSize);
  DATAMEMBER(ulong, DefaultMainVolume);
  DATAMEMBER(ulong, DefaultSecondaryVolume);
  DATAMEMBER(ulong, DefaultContainedVolume);
  DATAMEMBER(vector2d, BitmapPos);
  DATAMEMBER(ulong, Price);
  DATAMEMBER(ushort, BaseEmitation);
};

class data<material> : public basedata
{
 public:
  data<material>();
 protected:
  virtual std::string ClassName() const { return "material"; }
  DATAMEMBER(ushort, StrengthValue);
  DATAMEMBER(ushort, ConsumeType);
  DATAMEMBER(ushort, Density);
  DATAMEMBER(ushort, OfferValue);
  DATAMEMBER(ushort, Color);
  DATAMEMBER(ulong, PriceModifier);
  DATAMEMBER(bool, IsSolid);
  DATAMEMBER(ushort, Emitation);
  DATAMEMBER(bool, CanBeWished);
  DATAMEMBER(uchar, Alignment);
  DATAMEMBER(ushort, NutritionValue);
  DATAMEMBER(bool, IsAlive);
  DATAMEMBER(bool, IsBadFoodForAI);
  DATAMEMBER(ushort, ExplosivePower);
  DATAMEMBER(bool, IsFlammable);
  DATAMEMBER(bool, IsFlexible);
  DATAMEMBER(bool, IsExplosive);
};

template <class type> class database
{
 public:
  ~database();
  void ReadFrom(inputfile&);
  void Apply();
 protected:
  std::vector<data<type>*> Data;
};

class scriptsystem
{
 public:
  static void Initialize();
  static gamescript* GetGameScript() { return GameScript; }
 protected:
  static gamescript* GameScript;
  static database<character>* CharacterDataBase;
  static database<item>* ItemDataBase;
  static database<material>* MaterialDataBase;
  
};

#endif
