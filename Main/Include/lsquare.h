#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#include "dynarray.h"
#include "typedef.h"
#include "vector.h"

#include "square.h"

class game;
class bitmap;
class character;
class stack;
class terrain;
class groundterrain;
class overterrain;
class vector;
class material;
class square;
class item;

/* Presentation of the levelsquare class */

class levelsquare : public square
{
public:
	friend class level;
	levelsquare(level*, vector);
	levelsquare(level*, std::ifstream*, vector);
	~levelsquare(void);
	virtual void FastAddCharacter(character* Guy);
	virtual void AddCharacter(character* Guy);
	virtual void HandleCharacters(void);
	virtual void RemoveCharacter(void);
	virtual stack* CStack(void) const		{return Stack;}
	virtual void AlterLuminance(vector, ushort);
	virtual void EmptyFlag(void)			{Flag = false;}
	virtual void SetFlag(void)			{Flag = true;}
	virtual bool RetrieveFlag(void) const		{return Flag;}
	virtual void Emitate(void);
	virtual void ReEmitate(void);
	virtual stack* CSideStack(uchar Index) const	{return SideStack[Index];}
	virtual void Clean(void);
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual void Save(std::ofstream*) const;
	virtual void SpillFluid(uchar, ulong, ushort = 3, ushort = 32);
	virtual ushort CLuminance(void) const;
	virtual void SignalEmitationIncrease(ushort);
	virtual void SignalEmitationDecrease(ushort);
	virtual ushort CEmitation(void) const			{return Emitation;}
	virtual void ForceEmitterNoxify(void);
	virtual void ForceEmitterEmitation(void);
	virtual void Noxify(void);
	virtual void NoxifyEmitter(vector);
	virtual uchar CalculateBitMask(vector) const;
	virtual std::string CEngraved(void) const { return Engraved; }
	virtual bool Engrave(std::string What) { Engraved = What; return true; }
	virtual void SEmitation(ushort What) { Emitation = What; }
	virtual std::string CRememberedItems(void) const { return RememberedItems; }
	virtual void SRememberedItems(std::string);
	virtual void UpdateItemMemory(void);
	virtual bool CanBeSeen(void) const;
	virtual void Kick(ushort,uchar);
	virtual bool CanBeSeenFrom(vector) const;
	virtual uchar CDivineOwner(void) const { return DivineOwner; }
	virtual void SDivineOwner(uchar NDO) { DivineOwner = NDO; }
	virtual void DrawToTileBuffer(void) const;
	virtual void UpdateMemorizedAndDraw(void);
	virtual level* CMotherLevel(void) const { return MotherLevel; }
	virtual char CanBeDigged(character*, item*) const;
	virtual bool Dig(character*, item*);
private:
	level* MotherLevel;
	ushort CalculateEmitation(void) const;
	struct emitter
	{
		emitter(vector Pos, ushort DilatedEmitation) : Pos(Pos), DilatedEmitation(DilatedEmitation) {}
		emitter(void) {}
		bool operator==(emitter& AE) const {if(Pos == AE.Pos) return true; else return false;}
		emitter& operator=(emitter& AE) {Pos = AE.Pos; DilatedEmitation = AE.DilatedEmitation; return *this;}
		vector Pos;
		ushort DilatedEmitation;
	};
	dynarray<emitter> Emitter;
	stack* Stack, * SideStack[4];
	bool Flag;
	ushort Emitation;
	std::string Engraved, RememberedItems;
	uchar DivineOwner;
	bool Fluided;
};

#endif
