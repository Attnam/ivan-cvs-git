#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "action.h"
#include "slot.h"
#include "vector2d.h"

class ACTION
(
  faint,
  action,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual ushort GetCounter() const { return Counter; }
  virtual void SetCounter(ushort What) { Counter = What; }
  virtual bool IsVoluntary() const { return false; }
  virtual void Terminate(bool);
  virtual bool AllowFaint() const { return false; }
  virtual const char* GetDescription() const;
  virtual const char* GetDeathExplanation() const;
 protected:
  ushort Counter;
);

class ACTION
(
  consume,
  action,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual void Terminate(bool);
  virtual item* GetConsuming() const;
  virtual void SetConsuming(item*);
  virtual bool GetWasOnGround() const { return WasOnGround; }
  virtual void SetWasOnGround(bool What) { WasOnGround = What; }
  virtual bool HasEaten() const { return Eaten; }
  virtual void SetHasEaten(bool What) { Eaten = What; }
  virtual bool AllowFaint() const { return false; }
  virtual bool AllowFoodConsumption() const { return false; }
  virtual void DropUsedItems();
  virtual void DeleteUsedItems();
  virtual const char* GetDescription() const;
  virtual void SetDescription(const festring&);
  virtual ulong GetVolume() const;
  virtual ulong GetWeight() const;
  virtual ulong GetEmitation() const;
  virtual long GetScore() const;
 protected:
  virtual void VirtualConstructor(bool);
  festring Description;
  actionslot Consuming;
  bool WasOnGround;
  bool Eaten;
);

class ACTION
(
  rest,
  action,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual short GetGoalHP() const { return GoalHP; }
  virtual void SetGoalHP(short What) { GoalHP = What; }
  virtual void Terminate(bool);
  virtual bool GetRestRegenerationBonus() const { return true; }
  virtual const char* GetDescription() const;
 protected:
  short GoalHP;
);

class ACTION
(
  dig,
  action,
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
  virtual void DropUsedItems();
  virtual void DeleteUsedItems();
  virtual const char* GetDescription() const;
  virtual ulong GetVolume() const;
  virtual ulong GetWeight() const;
  virtual ulong GetEmitation() const;
  virtual long GetScore() const;
  virtual bool ShowEnvironment() const { return false; }
  virtual void SetMoveDigger(bool What) { MoveDigger = What; }
 protected:
  virtual void VirtualConstructor(bool);
  actionslot RightBackup;
  actionslot LeftBackup;
  vector2d SquareDug;
  bool MoveDigger;
);

class ACTION
(
  go,
  action,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual uchar GetDirection() const { return Direction; }
  virtual void SetDirection(uchar What) { Direction = What; }
  virtual bool GetWalkingInOpen() const { return WalkingInOpen; }
  virtual void SetWalkingInOpen(bool What) { WalkingInOpen = What; }
  virtual bool AllowDisplace() const { return false; }
  virtual const char* GetDescription() const;
  virtual bool ShowEnvironment() const { return false; }
 protected:
  uchar Direction;
  bool WalkingInOpen;
);

class ACTION
(
  read,
  action,
 public:
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Handle();
  virtual void Terminate(bool);
  virtual item* GetLiterature() const;
  virtual void SetLiterature(item*);
  virtual void DropUsedItems();
  virtual void DeleteUsedItems();
  virtual const char* GetDescription() const;
  virtual ushort GetCounter() const { return Counter; }
  virtual void SetCounter(ushort What) { Counter = What; }
  virtual ulong GetVolume() const;
  virtual ulong GetWeight() const;
  virtual ulong GetEmitation() const;
  virtual long GetScore() const;
 protected:
  virtual void VirtualConstructor(bool);
  actionslot Literature;
  ushort Counter;
);

#endif
