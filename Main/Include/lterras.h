/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

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
  ;
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
  door,
  olterrain,
 public:
  virtual bool Open(character*);
  virtual bool Close(character*);
  virtual bool CanBeOpened() const { return !Opened; }
  virtual void BeKicked(character*, int, int);
  virtual void SetIsOpened(bool What) { Opened = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsDoor() const { return true; }
  virtual void SetIsLocked(bool What) { Locked = What; }
  virtual bool IsLocked() const { return Locked; }
  virtual bool CanBeOpenedByAI();
  virtual void ReceiveDamage(character*, int, int);
  virtual void CreateBoobyTrap();
  virtual void ActivateBoobyTrap();
  virtual bool TryKey(item*, character*);
  virtual void SetParameters(int);
  virtual void Lock() { SetIsLocked(true); }
  virtual void HasBeenHitByItem(character*, item*, int);
  virtual bool IsTransparent() const;
  virtual int GetWalkability() const;
  virtual int GetTheoreticalWalkability() const;
  virtual void BeDestroyed();
 protected:
  virtual void VirtualConstructor(bool);
  virtual bool AddAdjective(festring&, bool) const;
  virtual void Break();
  virtual vector2d GetBitmapPos(int) const;
  virtual void MakeWalkable();
  virtual void MakeNotWalkable();
  virtual int GetBoobyTrap() { return BoobyTrap; }
  virtual void SetBoobyTrap(int What) { BoobyTrap = What; }
  bool Opened;
  bool Locked;
  int BoobyTrap;
);

class OLTERRAIN
(
  stairs,
  olterrain,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool Enter(bool) const;
  virtual void StepOn(character*);
  virtual void SetAttachedArea(int What) { AttachedArea = What; }
  virtual void SetAttachedEntry(int What) { AttachedEntry = What; }
  int GetAttachedArea() const { return AttachedArea; }
  int GetAttachedEntry() const { return AttachedEntry; }
 protected:
  virtual void VirtualConstructor(bool);
  int AttachedArea;
  int AttachedEntry;
);

class OLTERRAIN
(
  portal,
  stairs,
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual vector2d GetBitmapPos(int) const;
);

class OLTERRAIN
(
  altar,
  olterrain,
 public:
  virtual bool AcceptsOffers() const { return true; }
  virtual void StepOn(character*);
  virtual void BeKicked(character*, int, int);
  virtual bool ReceiveVomit(character*, liquid*);
  virtual bool Polymorph(character*);
  virtual bool SitOn(character*);
  virtual void Draw(bitmap*, vector2d, color24, int, bool) const;
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
  virtual void SetItemsInside(const fearray<contentscript<item> >&, int);
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
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, int = 0);
  virtual void ChangeSecondaryMaterial(material*, int = 0);
  void InitMaterials(material*, material*, bool = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual int GetMaterials() const { return 2; }
  virtual material* GetMaterial(int) const;
  virtual void InitMaterials(const materialscript*, const materialscript*, bool);
  virtual bool IsFountainWithWater() const;
 protected:
  virtual bool IsSparkling(int) const;
  virtual void GenerateMaterials();
  virtual color16 GetMaterialColorB(int) const;
  virtual void AddPostFix(festring& String) const { AddContainerPostFix(String); }
  virtual bool AddAdjective(festring&, bool) const;
  virtual vector2d GetBitmapPos(int) const;
  material* SecondaryMaterial;
);

class OLTERRAIN
(
  brokendoor,
  door,
 public:
  virtual void BeKicked(character*, int, int);
  virtual void ReceiveDamage(character*, int, int);
  virtual void HasBeenHitByItem(character*, item*, int);
  virtual void Break() { olterrain::Break(); }
);

class GLTERRAIN
(
  liquidterrain,
  glterrain,
 public:
  virtual const char* SurviveMessage() const;
  virtual const char* MonsterSurviveMessage() const;
  virtual const char* DeathMessage() const;
  virtual const char* MonsterDeathVerb() const;
  virtual const char* ScoreEntry() const;
  virtual bool IsFatalToStay() const { return true; }
  virtual bool DipInto(item*, character*);
  virtual bool IsDipDestination() const { return true; }
  virtual void SurviveEffect(character*);
 protected:
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual vector2d GetBitmapPos(int) const;
);

class OLTERRAIN
(
  boulder,
  olterrain,
  ;
);

class OLTERRAIN
(
  sign,
  olterrain,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SetText(const festring& What) { Text = What; }
  virtual festring GetText() const;
  virtual void AddPostFix(festring&) const;
  virtual void StepOn(character*);
 protected:
  festring Text;
);

class OLTERRAIN
(
  earth,
  olterrain,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
 protected:
  virtual void VirtualConstructor(bool);
  virtual vector2d GetBitmapPos(int) const;
  int PictureIndex;
);

class OLTERRAIN
(
  monsterportal,
  decoration,
 protected:
  void VirtualConstructor(bool);
  virtual int GetClassAnimationFrames() const { return 32; }
  virtual vector2d GetBitmapPos(int) const;
);

#endif
