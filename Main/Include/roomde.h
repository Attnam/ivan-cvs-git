#ifndef __ROOMDE_H__
#define __ROOMDE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "roomba.h"

class ROOM
(
  normalroom,
  room,
  ;
);

class ROOM
(
  shop,
  room,
 public:
  virtual void HandleInstantiatedCharacter(character*);
  virtual void Enter(character*);
  virtual bool PickupItem(character*, item*);
  virtual bool DropItem(character*, item*);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*);
  virtual bool AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual bool DestroyTerrain(character*, olterrain*);
);

class ROOM
(
  temple,
  room,
 public:
  virtual void HandleInstantiatedCharacter(character*);
  virtual void Enter(character*);
);

class ROOM
(
  cathedral,
  room,
 public:
  virtual void Enter(character*);
  virtual bool PickupItem(character*, item*);
  virtual bool DropItem(character*, item*);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*);
  virtual void SetEntered(bool What) { Entered = What; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool AllowDropGifts() const { return false; }
  virtual bool Drink(character*) const;
  virtual bool HasDrinkHandler() const { return true; }
  virtual bool Dip(character*) const;
  virtual bool HasDipHandler() const { return true; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual bool DestroyTerrain(character*, olterrain*);
 protected:
  virtual void VirtualConstructor(bool);
  bool Entered;
);

class ROOM
(
 library,
 room,
 public:
 public:
  virtual void HandleInstantiatedCharacter(character*);
  virtual void Enter(character*);
  virtual bool PickupItem(character*, item*);
  virtual bool DropItem(character*, item*);
  virtual void KickSquare(character*, lsquare*);
  virtual bool ConsumeItem(character*, item*);
  virtual bool AllowDropGifts() const { return false; }
  virtual void TeleportSquare(character*, lsquare*);
  virtual bool DestroyTerrain(character*, olterrain*);
);
#endif

