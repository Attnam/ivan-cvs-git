#ifndef __ROOMDE_H__
#define __ROOMDE_H__

#pragma warning(disable : 4786)

#include "roomba.h"

class ROOM
(
	normalroom,
	room,
	{
	},
	;
);

class ROOM
(
	shop,
	room,
	{
	},
public:
	virtual void HandleInstantiatedCharacter(character*);
	virtual void Enter(character*);
	virtual bool PickupItem(character*, item*);
	virtual bool DropItem(character*, item*);
	virtual void KickSquare(character*, levelsquare*);
	virtual bool ConsumeItem(character*, item*);
	virtual bool AllowDropGifts(void) const { return false; }
);

class ROOM
(
	temple,
	room,
	{
	},
public:
	virtual void HandleInstantiatedCharacter(character*);
	virtual void Enter(character*);
);

class ROOM
(
	cathedral,
	room,
	{
		SetEntered(false);
	},
public:
	virtual void Enter(character*);
	virtual bool PickupItem(character*, item*);
	virtual bool DropItem(character*, item*);
	virtual void KickSquare(character*, levelsquare*);
	virtual bool ConsumeItem(character*, item*);
	virtual void SetEntered(bool What) { Entered = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual bool AllowDropGifts(void) const { return false; }
protected:
	bool Entered;
);

#endif
