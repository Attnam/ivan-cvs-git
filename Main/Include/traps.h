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

#ifndef __TRAPS_H__
#define __TRAPS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "trap.h"

class bitmap;

class TRAP
(
  web,
  trap,
 public:
  virtual ~web();
  virtual void AddDescription(festring&) const;
  virtual bool TryToUnStick(character*, vector2d);
  virtual int GetTrapBaseModifier() const { return Strength; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void StepOnEffect(character*);
  virtual int GetTrapType() const { return GetType(); }
  virtual ulong GetTrapID() const { return TrapData.TrapID; }
  virtual ulong GetVictimID() const { return TrapData.VictimID; }
  virtual void AddTrapName(festring&, int) const;
  virtual void UnStick() { TrapData.VictimID = 0; }
  virtual void UnStick(int I) { TrapData.BodyParts &= ~(1 << I); }
  void SetStrength(int What) { Strength = What; }
  virtual void Draw(bitmap*, vector2d, color24) const;
  virtual bool IsStuckToBodyPart(int) const;
  virtual void ReceiveDamage(character*, int, int, int);
  virtual void Destroy();
  virtual bool IsDangerousFor(const character* Char) const { return CanBeSeenBy(Char); }
  virtual bool CanBeSeenBy(const character*) const;
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
 protected:
  virtual void VirtualConstructor(bool);
  trapdata TrapData;
  bitmap* Picture;
  int Strength; /* must be more than 0 */
);

#endif
