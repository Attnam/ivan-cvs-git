#ifndef __LTERRADE_H__
#define __LTERRADE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "lterraba.h"
#include "materde.h"
#include "game.h"
#include "ivandef.h"

class GLTERRAIN
(
  parquet,
  glterrain,
 public:
  virtual uchar OKVisualEffects() const { return 0; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(WOOD)); }
  virtual std::string GetNameSingular() const { return "parquet"; }
  virtual std::string GetNamePlural() const { return "parquette"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 0); }
);

class GLTERRAIN
(
  floor,
  glterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GRAVEL)); }
  virtual std::string GetNameSingular() const { return "floor"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 0); }
);

class GLTERRAIN
(
  ground,
  glterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GRAVEL)); }
  virtual std::string GetNameSingular() const { return "ground"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 0); }
);

class GLTERRAIN
(
  grassterrain,
  glterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GRASS)); }
  virtual std::string GetNameSingular() const { return "ground"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 16); }
);

class OLTERRAIN
(
  earth,
  olterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE; }
  virtual bool CanBeDug() const { return true; }
  virtual std::string DigMessage() const { return "The ground is fairly easy to dig."; }
  virtual bool IsWalkable() const { return false; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(MORAINE)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(56, 56, 56); }
  virtual std::string GetArticle() const { return "an"; }
  virtual std::string GetNameSingular() const { return "earth"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 336); }
);

class OLTERRAIN
(
  wall,
  olterrain,
 public:
  virtual uchar OKVisualEffects() const { return 0; }
  virtual bool CanBeDug() const { return true; }
  virtual std::string DigMessage() const { return "The wall is pretty hard, but you still manage to go through it."; }
  virtual bool IsWalkable() const { return false; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(STONE)); }
  virtual std::string GetNameSingular() const { return "wall"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(64, 272); }
);

class OLTERRAIN
(
  empty,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "The square you are trying to dig is empty."; }
  virtual bool IsSafeToDestroy() const { return true; }
  virtual bool IsEmpty() const { return true; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(new gas(AIR)); }
  virtual std::string GetNameSingular() const { return "atmosphere"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 480); }
);

class OLTERRAIN
(
  door,
  olterrain,
 public:
  virtual bool Open(character*);
  virtual bool Close(character*);
  virtual bool CanBeOpened() const { return !IsWalkable(); }
  virtual std::string DigMessage() const { return "The break the door."; }
  virtual void Kick(ushort, bool, uchar);
  virtual void SetIsOpened(bool What) { Opened = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsWalkable() const { return Opened; }
  virtual bool IsDoor() const { return true; }
  virtual void SetIsLocked(bool What) { Locked = What; }
  virtual bool IsLocked() const { return Locked; }
  virtual bool CanBeOpenedByAI() { return !IsLocked() && CanBeOpened(); }
  virtual bool ReceiveDamage(character*, short, uchar);
  virtual bool CanBeDug() const { return true; }
  virtual void HasBeenHitBy(item*, float, uchar, bool);
  virtual void CreateBoobyTrap();
  virtual void ActivateBoobyTrap();
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() { return LockType; }
  virtual bool TryKey(item*, character*);
  virtual void SetParameters(uchar);
  virtual void Lock() { SetIsLocked(true); }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(STONE)); }
  virtual void VirtualConstructor(bool);
  virtual std::string GetAdjective(bool) const;
  virtual void Break();
  virtual std::string GetNameSingular() const { return "door"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, IsWalkable() ? 48 : 176); }
  virtual void MakeWalkable();
  virtual void MakeNotWalkable();
  virtual uchar GetBoobyTrap() { return BoobyTrap; }
  virtual void SetBoobyTrap(uchar What) { BoobyTrap = What; }
  bool Opened;
  bool Locked;
  uchar BoobyTrap;
  uchar LockType;
);

class OLTERRAIN
(
  stairsup,
  olterrain,
 public:
  virtual bool GoUp(character*) const;
  virtual uchar OKVisualEffects() const { return 0; }
  virtual std::string DigMessage() const { return "The stairs are too hard to dig."; }
  virtual void StepOn(character*);
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(STONE)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(160, 64, 0); }
  virtual std::string GetNameSingular() const { return "stairway upwards"; }
  virtual std::string GetNamePlural() const { return "stairways upwards"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 192); }
);

class OLTERRAIN
(
  stairsdown,
  olterrain,
 public:
  virtual bool GoDown(character*) const;
  virtual uchar OKVisualEffects() const { return 0; }
  virtual std::string DigMessage() const { return "The stairs are too hard to dig."; }
  virtual void StepOn(character*);
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(STONE)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(160, 64, 0); }
  virtual std::string GetNameSingular() const { return "stairway downwards"; }
  virtual std::string GetNamePlural() const { return "stairways downwards"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 208); }
);

class OLTERRAIN
(
  altar,
  olterrain,
 public:
  virtual bool CanBeOffered() const { return true; }
  virtual void DrawToTileBuffer(bool) const;
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual void SetDivineMaster(uchar What) { DivineMaster = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar OKVisualEffects() const { return 0; }
  virtual std::string DigMessage() const { return "An invisible wall stops your feeble attempt."; }
  virtual void StepOn(character*);
  virtual void Kick(ushort, bool, uchar);
  virtual void ReceiveVomit(character*);
  virtual bool Polymorph(character*);
  virtual bool SitOn(character*);
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(STONE)); }
  virtual void VirtualConstructor(bool);
  virtual std::string GetArticle() const { return "an"; }
  virtual std::string GetNameSingular() const { return "altar"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 368); }
  virtual std::string GetPostFix() const { return GetDivineMasterDescription(DivineMaster); }
  uchar DivineMaster;
);

class OLTERRAIN
(
  throne,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "The throne resists."; }
  virtual bool SitOn(character*);
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(GOLD)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(200, 200, 0); }
  virtual std::string GetNameSingular() const { return "throne"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 416); }
);

class OLTERRAIN
(
  pine,
  olterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR; }
  virtual bool CanBeDug() const { return true; }
  virtual std::string DigMessage() const { return "You chop the tree down."; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(WOOD)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(0, 160, 0); }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetNameSingular() const { return "pine"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 320); }
);

class OLTERRAIN
(
  spruce,
  olterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR; }
  virtual bool CanBeDug() const { return true; }
  virtual std::string DigMessage() const { return "You chop the tree down."; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(WOOD)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(0, 160, 0); }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetNameSingular() const { return "spruce"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 352); }
);

class OLTERRAIN
(
  linden,
  olterrain,
 public:
  virtual uchar OKVisualEffects() const { return MIRROR; }
  virtual bool CanBeDug() const { return true; }
  virtual std::string DigMessage() const { return "You chop the tree down."; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(WOOD)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(0, 160, 0); }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetNameSingular() const { return "lovely linden"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 336); }
);

class OLTERRAIN
(
  carpet,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "You can't force yourself to ruin this wonderful carpet."; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(FABRIC)); }
  virtual std::string GetAdjective() const { return "expensive"; }
  virtual std::string GetAdjectiveArticle() const { return "an"; }
  virtual std::string GetNameSingular() const { return "carpet"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 272); }
);

class OLTERRAIN
(
  couch,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "The destroy the couch."; }
  virtual bool SitOn(character*);
  virtual bool CanBeDug() const { return true; }
  virtual uchar RestModifier() const { return 2; }
  virtual void ShowRestMessage(character*) const;
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(FABRIC)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(200, 200, 0); }
  virtual std::string GetAdjective() const { return "expensive"; }
  virtual std::string GetNameSingular() const { return "couch"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 400); }
);

class OLTERRAIN
(
  bookcase,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "You smash the bookcase into pieces."; }
  virtual bool SitOn(character*);
  virtual bool CanBeDug() const { return true; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(WOOD)); }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetNameSingular() const { return "bookcase"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 272); }
);

class OLTERRAIN
(
  fountain,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "The water splashes a bit."; }
  virtual bool SitOn(character*);
  virtual bool Consume(character*);
  virtual bool HasConsumeEffect() const { return true; } 
  virtual void DryOut();
  virtual bool DipInto(item*, character*);
  virtual bool IsDipDestination() const { return true; }
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material* What) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void ChangeContainedMaterial(material* What) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void InitMaterials(material* M1, material* M2, bool CUP = true) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), ContainedMaterial, M2, GetDefaultContainedVolume(), CUP); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetMaterials() const { return 2; }
  virtual material* GetMaterial(ushort) const;
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(MARBLE), MAKE_MATERIAL(WATER)); }
  virtual ushort GetMaterialColor1(ushort) const;
  virtual uchar GetAlpha1(ushort) const;
  virtual ulong GetDefaultContainedVolume() const { return 10000; }
  virtual std::string GetPostFix() const { return ContainerPostFix(); }
  virtual std::string GetAdjective() const { return GetContainedMaterial() ? "" : "dried out"; }
  virtual std::string GetNameSingular() const { return "fountain"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(GetContainedMaterial() ? 16 : 32, 288); }
  material* ContainedMaterial;
);

class OLTERRAIN
(
  doublebed,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "You smash the bed into pieces."; }
  virtual bool SitOn(character*);
  virtual bool CanBeDug() const { return true; }
  virtual uchar RestModifier() const { return 5; }
  virtual void ShowRestMessage(character*) const;
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(FABRIC)); }
  virtual ushort GetMaterialColor1(ushort) const { return MAKE_RGB(200, 200, 0); }
  virtual std::string GetAdjective() const { return "luxurious"; }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetNameSingular() const { return "double bed"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(48, 304); }
);

class OLTERRAIN
(
  brokendoor,
  door,
 public:
  virtual std::string DigMessage() const { return "You destroy the broken door."; }
  virtual void Kick(ushort, bool, uchar);
  virtual bool ReceiveDamage(character*, short, uchar);
  virtual bool CanBeDug() const { return true; }
  virtual void HasBeenHitBy(item*, float, uchar, bool);
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(STONE)); }
  virtual std::string GetNameSingular() const { return "broken door"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, IsWalkable() ? 48 : 160); }
);

class GLTERRAIN
(
  pool,
  glterrain,
 public:
  virtual bool IsWalkable(character*) const;
  virtual bool SitOn(character*);
  virtual std::string SurviveMessage() const { return "you manage to get out of the pool"; }
  virtual std::string DeathMessage() const { return "you drown"; }
  virtual std::string MonsterDeathVerb() const { return "drowns"; }
  virtual std::string ScoreEntry() const { return "drowned"; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(WATER)); }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetNameSingular() const { return "pool"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 0); }
);

class OLTERRAIN
(
  poolborder,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "The water splashes a bit."; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(MARBLE)); }
  virtual std::string GetNameSingular() const { return ""; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(32, 320); }
);

class OLTERRAIN
(
  poolcorner,
  olterrain,
 public:
  virtual std::string DigMessage() const { return "The water splashes a bit."; }
 protected:
  virtual void GenerateMaterials() { InitMaterials(MAKE_MATERIAL(MARBLE)); }
  virtual std::string GetNameSingular() const { return ""; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(48, 320); }
);

#endif


