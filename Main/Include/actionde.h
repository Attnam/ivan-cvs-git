#ifndef __ACTIONDE_H__
#define __ACTIONDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "actionba.h"
#include "slot.h"
#include "vector2d.h"

class ACTION
(
  faint,
  action,
  {
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual ushort GetCounter() const { return Counter; }
  virtual void SetCounter(ushort What) { Counter = What; }
  virtual bool IsVoluntary() const { return false; }
  virtual void Terminate(bool);
  virtual bool AllowFaint() const { return false; }
 protected:
  ushort Counter;
);

class ACTION
(
  consume,
  action,
  {
    Consuming.Init(this);
    SetHasEaten(false);
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual void Terminate(bool);
  virtual item* GetConsuming() const;
  virtual void SetConsuming(item*);
  virtual bool GetWasOnGround() const { return WasOnGround; }
  virtual void SetWasOnGround(bool What) { WasOnGround = What; }
  virtual bool GetHasEaten() const { return HasEaten; }
  virtual void SetHasEaten(bool What) { HasEaten = What; }
  virtual bool AllowFaint() const { return false; }
  virtual bool AllowFoodConsumption() const { return false; }
  virtual ulong GetWeight() const;
 protected:
  actionslot Consuming;
  bool WasOnGround;
  bool HasEaten;
);

class ACTION
(
  rest,
  action,
  {
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual short GetGoalHP() const { return GoalHP; }
  virtual void SetGoalHP(short What) { GoalHP = What; }
  virtual void Terminate(bool);
  virtual bool GetRestRegenerationBonus() const { return true; }
 protected:
  short GoalHP;
);

class ACTION
(
  dig,
  action,
  {
    RightBackup.Init(this);
    LeftBackup.Init(this);
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual vector2d GetSquareDug() const { return SquareDug; }
  virtual void SetSquareDug(vector2d What) { SquareDug = What; }
  virtual void Terminate(bool);
  virtual item* GetRightBackup() const;
  virtual void SetRightBackup(item*);
  virtual item* GetLeftBackup() const;
  virtual void SetLeftBackup(item*);
  virtual bool AllowDisplace() const { return false; }
  virtual ulong GetWeight() const;
 protected:
  actionslot RightBackup;
  actionslot LeftBackup;
  vector2d SquareDug;
);

class ACTION
(
  go,
  action,
  {
  },
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual uchar GetDirection() const { return Direction; }
  virtual void SetDirection(uchar What) { Direction = What; }
  virtual bool GetWalkingInOpen() const { return WalkingInOpen; }
  virtual void SetWalkingInOpen(bool What) { WalkingInOpen = What; }
  virtual bool AllowDisplace() const { return false; }
 protected:
  uchar Direction;
  bool WalkingInOpen;
);

#endif

