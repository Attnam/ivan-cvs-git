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

#ifndef __ENTITY_H__
#define __ENTITY_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>

#include "typedef.h"

#define EXISTS 1
#define HAS_BE 2
#define ENTITY_FLAGS 3

class square;
class material;
class character;
class festring;
struct vector2d;

class entity
{
 public:
  friend class pool;
  entity(int);
  entity(const entity&);
  virtual ~entity();
  virtual void Be() { }
  bool Exists() const { return !!(Flags & EXISTS); }
  void SendToHell();
  bool IsEnabled() const { return !!(Flags & HAS_BE); }
  void Enable();
  void Disable();
  virtual square* GetSquareUnderEntity(int = 0) const = 0;
  virtual void SignalVolumeAndWeightChange() { }
  color24 GetEmitation() const { return Emitation; }
  virtual void SignalEmitationIncrease(color24) { }
  virtual void SignalEmitationDecrease(color24) { }
  virtual bool ContentsCanBeSeenBy(const character*) const { return false; }
  virtual bool AllowSpoil() const { return false; }
  virtual void SignalSpoil(material*) { }
  virtual void SignalSpoilLevelChange(material*) { }
  virtual bool IsOnGround() const = 0;
  virtual bool AllowContentEmitation() const { return true; }
  virtual void SignalRustLevelChange() { }
  virtual material* RemoveMaterial(material*) { return 0; }
  virtual character* TryNecromancy(character*) { return 0; }
  virtual void SignalDisappearance() { }
  virtual void SpecialEatEffect(character*, int) { }
  virtual ulong GetTrapID() const { return 0; }
  virtual ulong GetVictimID() const { return 0; }
  virtual void AddTrapName(festring&, int) const { }
  virtual void UnStick() { }
  virtual bool TryToUnStick(character*, vector2d);
  virtual int GetTrapType() const { return 0; }
 protected:
  color24 Emitation;
  ulong Flags;
 private:
  entity* Last;
  entity* Next;
};

#endif
