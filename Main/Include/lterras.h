#ifndef __LTERRAS_H__
#define __LTERRAS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "lterra.h"

class OLTERRAIN
(
  wall,
  olterrain,
 public:
  virtual void Break();
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
  virtual void Draw(bitmap*, vector2d, ulong, bool) const { }
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
  virtual bool CanBeOpenedByAI();
  virtual void ReceiveDamage(character*, ushort, uchar);
  virtual void CreateBoobyTrap();
  virtual void ActivateBoobyTrap();
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() { return LockType; }
  virtual bool TryKey(item*, character*);
  virtual void SetParameters(uchar);
  virtual void Lock() { SetIsLocked(true); }
  virtual void HasBeenHitByItem(character*, item*, ushort);
 protected:
  virtual void AddPostFix(std::string&) const;
  virtual void VirtualConstructor(bool);
  virtual bool AddAdjective(std::string&, bool) const;
  virtual void Break();
  virtual vector2d GetBitmapPos(ushort) const;
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
  link,
  olterrain,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Enter(bool) const;
  virtual void StepOn(character*);
  virtual void SetAttachedArea(uchar What) { AttachedArea = What; }
  virtual void SetAttachedEntry(uchar What) { AttachedEntry = What; }
  uchar GetAttachedArea() const { return AttachedArea; }
  uchar GetAttachedEntry() const { return AttachedEntry; }
 protected:
  virtual void VirtualConstructor(bool);
  uchar AttachedArea;
  uchar AttachedEntry;
);

class OLTERRAIN
(
  portal,
  link,
 protected:
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
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
  virtual void Draw(bitmap*, vector2d, ulong, bool) const;
 protected:
  virtual void AddPostFix(std::string&) const;
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
  olterraincontainer,
  olterrain,
 public:
  virtual ~olterraincontainer();
  virtual bool Open(character*);
  virtual bool CanBeOpened() const { return true; }
  virtual stack* GetContained() const { return Contained; }
  virtual void Load(inputfile&);
  virtual void Save(outputfile&) const;
  virtual void SetItemsInside(const std::vector<contentscript<item> >&, ushort);
  virtual void Break();
  virtual bool AllowContentEmitation() const { return false; }
 protected:
  virtual void VirtualConstructor(bool);
  stack* Contained;
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
  virtual bool IsDipDestination() const;
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material*, ushort = 0);
  virtual void ChangeContainedMaterial(material*, ushort = 0);
  void InitMaterials(material*, material*, bool = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetMaterials() const { return 2; }
  virtual material* GetMaterial(ushort) const;
 protected:
  virtual bool IsSparkling(ushort) const;
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  virtual void AddPostFix(std::string& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(std::string&, bool) const;
  virtual vector2d GetBitmapPos(ushort) const;
  material* ContainedMaterial;
);

class OLTERRAIN
(
  brokendoor,
  door,
 public:
  virtual void BeKicked(character*, ushort);
  virtual void ReceiveDamage(character*, ushort, uchar);
  virtual void HasBeenHitByItem(character*, item*, ushort);
  virtual void Break() { olterrain::Break(); }
 protected:
  virtual vector2d GetBitmapPos(ushort) const;
);

class GLTERRAIN
(
  liquidterrain,
  glterrain,
 public:
  virtual bool IsWalkable(const character*) const;
  virtual std::string SurviveMessage() const;
  virtual std::string MonsterSurviveMessage() const;
  virtual std::string DeathMessage() const;
  virtual std::string MonsterDeathVerb() const;
  virtual std::string ScoreEntry() const;
  virtual bool IsFatalToStay() const { return true; }
 protected:
  virtual ushort GetClassAnimationFrames() const { return 32; }
  //virtual void VirtualConstructor(bool);
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class OLTERRAIN
(
  boulder,
  olterrain,
 public:
  virtual void Break();
);

class OLTERRAIN
(
  sign,
  olterrain,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetText(const std::string& What) { Text = What; }
  virtual std::string GetText() const { return Text; }
  virtual void AddPostFix(std::string&) const;
  virtual void StepOn(character*);
 protected:
  std::string Text;
);

#endif
