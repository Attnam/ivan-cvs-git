#ifndef __ROOMDE_H__
#define __ROOMDE_H__

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
);

class ROOM
(
	temple,
	room,
public:
	virtual void HandleInstantiatedCharacter(character*);
	virtual void HandleInstantiatedOverLevelTerrain(overlevelterrain*);
	virtual void Enter(character*);
);

#endif
