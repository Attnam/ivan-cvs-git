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

class TRAP
(
  web,
  trap,
 public:
  bool TryToUnStick(character*, vector2d);
  int GetTrapBaseModifier() const { return Strength; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void StepOnEffect(character*);
 protected:
  virtual void VirtualConstructor(bool);
  trapdata TrapData;
  int Strength; /* must be more than 0 */
);

#endif
