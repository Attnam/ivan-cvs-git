#ifndef __LTERRADE_H__
#define __LTERRADE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "lterraba.h"
#include "materde.h"
#include "game.h"
#include "ivandef.h"

class OLTERRAIN
(
  wall,
  olterrain,
 public:
  virtual bool IsWalkable() const { return false; }
);

class OLTERRAIN
(
  decoration,
  olterrain,
  ;
);

class GLTERRAIN
(
  solidterrain,
  glterrain,
  ;
);

class OLTERRAIN
(
  empty,
  olterrain,
 public:
  virtual void Draw(bitmap*, vector2d, ushort, bool, bool) const { }
  virtual void UpdatePictures() { }
);

class OLTERRAIN
(
  door,
  olterrain,
 public:
  virtual bool Open(character*);
  virtual bool Close(character*);
  virtual bool CanBeOpened() const { return !IsWalkable(); }
  virtual void BeKicked(character*, ushort);
  virtual void SetIsOpened(bool What) { Opened = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsWalkable() const { return Opened; }
  virtual bool IsDoor() const { return true; }
  virtual void SetIsLocked(bool What) { Locked = What; }
  virtual bool IsLocked() const { return Locked; }
  virtual bool CanBeOpenedByAI() { return !IsLocked() && CanBeOpened(); }
  virtual bool ReceiveDamage(character*, short, uchar);
  virtual void HasBeenHitBy(item*, float, uchar);
  virtual void CreateBoobyTrap();
  virtual void ActivateBoobyTrap();
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() { return LockType; }
  virtual bool TryKey(item*, character*);
  virtual void SetParameters(uchar);
  virtual void Lock() { SetIsLocked(true); }
 protected:
  virtual void AddPostFix(std::string& String) const { AddLockPostFix(String, LockType); }
  virtual void VirtualConstructor(bool);
  virtual bool AddAdjective(std::string&, bool) const;
  virtual void Break();
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
  virtual void StepOn(character*);
);

class OLTERRAIN
(
  stairsdown,
  olterrain,
 public:
  virtual bool GoDown(character*) const;
  virtual void StepOn(character*);
);

class OLTERRAIN
(
  altar,
  olterrain,
 public:
  virtual bool AcceptsOffers() const { return true; }
  virtual void StepOn(character*);
  virtual void BeKicked(character*, ushort);
  virtual void ReceiveVomit(character*);
  virtual bool Polymorph(character*);
  virtual bool SitOn(character*);
  virtual void Draw(bitmap*, vector2d, ushort, bool, bool) const;
 protected:
  virtual void AddPostFix(std::string& String) const { AddDivineMasterDescription(String, GetConfig()); }
);

class OLTERRAIN
(
  throne,
  decoration,
 public:
  virtual bool SitOn(character*);
);

class OLTERRAIN
(
  container,
  olterrain,
  ;
);

class OLTERRAIN
(
  fountain,
  olterrain,
 public:
  virtual ~fountain();
  virtual bool SitOn(character*);
  virtual bool Drink(character*);
  virtual bool HasDrinkEffect() const { return true; } 
  virtual void DryOut();
  virtual bool DipInto(item*, character*);
  virtual bool IsDipDestination() const { return true; }
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material* What) { SetMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  virtual void ChangeContainedMaterial(material* What) { ChangeMaterial(ContainedMaterial, What, GetDefaultContainedVolume()); }
  void InitMaterials(material* M1, material* M2, bool CUP = true) { ObjectInitMaterials(MainMaterial, M1, GetDefaultMainVolume(), ContainedMaterial, M2, GetDefaultContainedVolume(), CUP); }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual uchar GetMaterials() const { return 2; }
 protected:
  virtual material*& GetMaterialReference(ushort);
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  virtual void AddPostFix(std::string& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(std::string&, bool) const;
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(GetContainedMaterial() ? 16 : 32, 288); }
  material* ContainedMaterial;
);

class OLTERRAIN
(
  brokendoor,
  door,
 public:
  virtual void BeKicked(character*, ushort);
  virtual bool ReceiveDamage(character*, short, uchar);
  virtual void HasBeenHitBy(item*, float, uchar);
 protected:
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, IsWalkable() ? 48 : 160); }
);

class GLTERRAIN
(
  liquidterrain,
  glterrain,
 public:
  virtual bool IsWalkable(character*) const;
  virtual std::string SurviveMessage() const { return "you manage to get out of the pool"; }
  virtual std::string DeathMessage() const { return "you drown"; }
  virtual std::string MonsterDeathVerb() const { return "drowns"; }
  virtual std::string ScoreEntry() const { return "drowned"; }
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBitmapPos(ushort Frame) const { return vector2d(48 + Frame * 16, 0); } // gum solution, should come from script
);

#endif
