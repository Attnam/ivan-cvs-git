#ifndef __LTERRADE_H__
#define __LTERRADE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "lterraba.h"

#define NUMBER_OF_LOCK_TYPES 3

class GLTERRAIN
(
  parquet,
  glterrain,
  InitMaterials(new wood),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return 0; }
 protected:
  virtual std::string NameSingular() const { return "parquet"; }
  virtual std::string NamePlural() const { return "parquette"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 240); }
);

class GLTERRAIN
(
  floor,
  glterrain,
  InitMaterials(new gravel),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
 protected:
  virtual std::string NameSingular() const { return "floor"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 352); }
);

class GLTERRAIN
(
  ground,
  glterrain,
  InitMaterials(new gravel),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
 protected:
  virtual std::string NameSingular() const { return "ground"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 352); }
);

class GLTERRAIN
(
  grassterrain,
  glterrain,
  InitMaterials(new grass),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
  virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
 protected:
  virtual std::string NameSingular() const { return "ground"; }
  virtual vector2d GetBitmapPos() const { return vector2d(16, 336); }
);

class OLTERRAIN
(
  earth,
  olterrain,
  InitMaterials(2, new moraine, new stone),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
  virtual bool CanBeDigged() const { return true; }
  virtual std::string DigMessage() const { return "The ground is fairly easy to dig."; }
  virtual bool GetIsWalkable() const { return false; }
 protected:
  virtual std::string NameSingular() const { return "earth"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 336); }
);

class OLTERRAIN
(
  wall,
  olterrain,
  InitMaterials(new stone),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return 0; }
  virtual bool CanBeDigged() const { return true; }
  virtual std::string DigMessage() const { return "The wall is pretty hard, but you still manage to go through it."; }
  virtual bool GetIsWalkable() const { return false; }
 protected:
  virtual std::string NameSingular() const { return "wall"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 240); }
);

class OLTERRAIN
(
  empty,
  olterrain,
  InitMaterials(new air),
  {
  },
 public:
  virtual std::string DigMessage() const { return "The square you are trying to dig is empty."; }
 protected:
  virtual std::string NameSingular() const { return ""; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 480); }
);

class OLTERRAIN
(
  door,
  olterrain,
  InitMaterials(new stone),
  {
    SetIsOpen(false);
    SetIsLocked(false);
    SetBoobyTrap(0);
    UpdatePicture();
    SetLockType(RAND() % NUMBER_OF_LOCK_TYPES);
  },
 public:
  virtual bool Open(character*);
  virtual bool Close(character*);
  virtual bool CanBeOpened() const { return !GetIsWalkable(); }
  virtual std::string DigMessage() const { return "The chop the door down."; }
  virtual void Kick(ushort, bool, uchar);
  virtual void SetIsOpen(bool What) { IsOpen = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool GetIsWalkable() const { return IsOpen; }
  virtual bool IsDoor() const { return true; }
  virtual std::string Name(uchar) const;
  virtual void SetIsLocked(bool What) { IsLocked = What; }
  virtual bool GetIsLocked() const { return IsLocked; }
  virtual bool CanBeOpenedByAI() { return !GetIsLocked() && CanBeOpened(); }
  virtual bool ReceiveStrike();
  virtual void Lock() { SetIsLocked(true); }
  virtual bool CanBeDigged() const { return true; }
  virtual void HasBeenHitBy(item*, float, uchar, bool);
  virtual void CreateBoobyTrap();
  virtual void ActivateBoobyTrap();
  virtual void SetLockType(uchar What) { LockType = What; }
  virtual uchar GetLockType() { return LockType; }
  virtual bool ReceiveApply(item*, character*);
 private:
  virtual void Break(bool);
 protected:
  virtual std::string NameSingular() const { return "door"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, GetIsWalkable() ? 48 : 176); }
  virtual void MakeWalkable();
  virtual void MakeNotWalkable();
  virtual uchar GetBoobyTrap() { return BoobyTrap; }
  virtual void SetBoobyTrap(uchar What) { BoobyTrap = What; }
  bool IsOpen;
  bool IsLocked;
  uchar BoobyTrap;
  uchar LockType;
);

class OLTERRAIN
(
  stairsup,
  olterrain,
  InitMaterials(new stone),
  {
  },
 public:
  virtual bool GoUp(character*) const;
  virtual uchar OKVisualEffects() const { return 0; }
  virtual std::string DigMessage() const { return "The stairs are too hard to dig."; }
  //virtual void ColorChangeSpeciality(uchar, bool) { GraphicId.Color[1] = MAKE_RGB(160, 64, 0); }
  virtual void StepOn(character*);
 protected:
  virtual ushort GetMaterialColor1() const { return MAKE_RGB(160, 64, 0); }
  virtual std::string NameSingular() const { return "stairway upwards"; }
  virtual std::string NamePlural() const { return "stairways upwards"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 192); }
);

class OLTERRAIN
(
  stairsdown,
  olterrain,
  InitMaterials(new stone),
  {
  },
 public:
  virtual bool GoDown(character*) const;
  virtual uchar OKVisualEffects() const { return 0; }
  virtual std::string DigMessage() const { return "The stairs are too hard to dig."; }
  //virtual void ColorChangeSpeciality(uchar, bool) { GraphicId.Color[1] = MAKE_RGB(160, 64, 0); }
  virtual void StepOn(character*);
 protected:
  virtual ushort GetMaterialColor1() const { return MAKE_RGB(160, 64, 0); }
  virtual std::string NameSingular() const { return "stairway downwards"; }
  virtual std::string NamePlural() const { return "stairways downwards"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 208); }
);

class OLTERRAIN
(
  altar,
  olterrain,
  InitMaterials(new stone),
  {
    SetOwnerGod(1 + RAND() % (game::GetGods() - 1));
  },
 public:
  virtual bool CanBeOffered() const { return true; }
  virtual std::string Name(uchar Case) const { return NameWithMaterial(Case) + OwnerGodDescription(OwnerGod); }
  virtual void DrawToTileBuffer() const;
  virtual uchar GetOwnerGod() const { return OwnerGod; }
  virtual void SetOwnerGod(uchar What) { OwnerGod = What; }
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
  virtual std::string NameSingular() const { return "altar"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 368); }
  uchar OwnerGod;
);

class OLTERRAIN
(
  throne,
  olterrain,
  InitMaterials(2, new gold, new gold),
  {
  },
 public:
  virtual std::string DigMessage() const { return "The throne resists."; }
  virtual bool SitOn(character*);
 protected:
  virtual std::string NameSingular() const { return "throne"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 416); }
);

class OLTERRAIN
(
  pine,
  olterrain,
  InitMaterials(2, new leaf, new wood),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR; }
  virtual bool CanBeDigged() const { return true; }
  virtual std::string DigMessage() const { return "You chop the tree down."; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
 protected:
  virtual std::string NameSingular() const { return "pine"; }
  virtual vector2d GetBitmapPos() const { return vector2d(16, 320); }
);

class OLTERRAIN
(
  spruce,
  olterrain,
  InitMaterials(2, new leaf, new wood),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR; }
  virtual bool CanBeDigged() const { return true; }
  virtual std::string DigMessage() const { return "You chop the tree down."; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
 protected:
  virtual std::string NameSingular() const { return "spruce"; }
  virtual vector2d GetBitmapPos() const { return vector2d(16, 352); }
);

class OLTERRAIN
(
  linden,
  olterrain,
  InitMaterials(2, new leaf, new wood),
  {
  },
 public:
  virtual uchar OKVisualEffects() const { return MIRROR; }
  virtual bool CanBeDigged() const { return true; }
  virtual std::string DigMessage() const { return "You chop the tree down."; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
 protected:
  virtual std::string NameSingular() const { return "lovely linden"; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 336); }
);

class OLTERRAIN
(
  carpet,
  olterrain,
  InitMaterials(new expensivefabric),
  {
  },
 public:
  virtual std::string DigMessage() const { return "You can't force yourself to ruin this wonderful carpet."; }
 protected:
  virtual std::string NameSingular() const { return "carpet"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 272); }
);

class OLTERRAIN
(
  couch,
  olterrain,
  InitMaterials(2, new expensivefabric, new gold),
  {
  },
 public:
  virtual std::string DigMessage() const { return "The destroy the couch."; }
  virtual bool SitOn(character*);
  virtual bool CanBeDigged() const { return true; }
  virtual uchar RestModifier() const { return 2; }
  virtual void ShowRestMessage(character*) const;
 protected:
  virtual std::string NameSingular() const { return "couch"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 400); }
);

class OLTERRAIN
(
  bookcase,
  olterrain,
  InitMaterials(new wood),
  {
  },
 public:
  virtual std::string DigMessage() const { return "You smash the bookcase into pieces."; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
  virtual bool SitOn(character*);
  virtual bool CanBeDigged() const { return true; }
 protected:
  virtual std::string NameSingular() const { return "bookcase"; }
  virtual vector2d GetBitmapPos() const { return vector2d(16, 272); }
);

class OLTERRAIN
(
  fountain,
  olterrain,
  InitMaterials(2, new marble, new water),
  {
  },
 public:
  virtual std::string DigMessage() const { return "The water splashes a bit."; }
  virtual std::string Name(uchar) const;
  virtual bool SitOn(character*);
  virtual bool Consume(character*);
  virtual bool HasConsumeEffect() const { return true; } 
  virtual void DryOut();
 protected:
  virtual std::string NameSingular() const { return "fountain"; }
  virtual vector2d GetBitmapPos() const { return vector2d(GetMaterial(1) ? 16 : 32, 288); }
);

class OLTERRAIN
(
  doublebed,
  olterrain,
  InitMaterials(2, new gold, new expensivefabric),
  {
  },
 public:
  virtual std::string DigMessage() const { return "You smash the bed into pieces."; }
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
  virtual bool SitOn(character*);
  virtual bool CanBeDigged() const { return true; }
  virtual uchar RestModifier() const { return 5; }
  virtual void ShowRestMessage(character*) const;
 protected:
  virtual std::string NameSingular() const { return "luxurious double bed"; }
  virtual vector2d GetBitmapPos() const { return vector2d(48, 304); }
);

class OLTERRAIN
(
  brokendoor,
  door,
  InitMaterials(new stone),
  {
    SetIsOpen(false);
    UpdatePicture();
  },
 public:
  virtual std::string DigMessage() const { return "You destroy the broken door."; }
  virtual void Kick(ushort, bool, uchar);
  virtual bool ReceiveStrike();
  virtual bool CanBeDigged() const { return true; }
  virtual void HasBeenHitBy(item*, float, uchar, bool);
 protected:
  virtual std::string NameSingular() const { return "broken door"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, GetIsWalkable() ? 48 : 160); }
);

class GLTERRAIN
(
  pool,
  glterrain,
  InitMaterials(new water),
  {
  },
 public:
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
  virtual bool GetIsWalkable(character*) const;
  virtual bool SitOn(character*);
  virtual std::string SurviveMessage() const { return "you manage to get out of the pool"; }
  virtual std::string DeathMessage() const { return "you drown"; }
  virtual std::string MonsterDeathVerb() const { return "drowns"; }
  virtual std::string ScoreEntry() const { return "drowned"; }
 protected:
  virtual std::string NameSingular() const { return "pool"; }
  virtual vector2d GetBitmapPos() const { return vector2d(0, 224); }
);

class OLTERRAIN
(
  poolborder,
  olterrain,
  InitMaterials(new marble),
  {
  },
 public:
  virtual std::string DigMessage() const { return "The water splashes a bit."; }
 protected:
  virtual std::string NameSingular() const { return ""; }
  virtual vector2d GetBitmapPos() const { return vector2d(32, 320); }
);

class OLTERRAIN
(
  poolcorner,
  olterrain,
  InitMaterials(new marble),
  {
  },
 public:
  virtual std::string DigMessage() const { return "The water splashes a bit."; }
 protected:
  virtual std::string NameSingular() const { return ""; }
  virtual vector2d GetBitmapPos() const { return vector2d(48, 320); }
);

#endif

