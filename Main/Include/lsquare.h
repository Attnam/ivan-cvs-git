#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#include <string>
#include <fstream>

#include "dynarray.h"
#include "typedef.h"
#include "vector.h"

#include "square.h"

class game;
class bitmap;
class character;
class stack;
class levelterrain;
class groundlevelterrain;
class overlevelterrain;
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
	virtual ~levelsquare(void);
	virtual void FastAddCharacter(character* Guy);
	virtual void AddCharacter(character* Guy);
	virtual void HandleCharacters(void);
	virtual void RemoveCharacter(void);
	virtual stack* GetStack(void) const		{ return Stack; }
	virtual void AlterLuminance(vector, ushort);
	virtual void Emitate(void);
	virtual void ReEmitate(void);
	virtual stack* GetSideStack(uchar Index) const	{ return SideStack[Index]; }
	virtual void Clean(void);
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual void SpillFluid(uchar, ulong, ushort = 3, ushort = 32);
	virtual ushort GetLuminance(void) const;
	virtual void SignalEmitationIncrease(ushort);
	virtual void SignalEmitationDecrease(ushort);
	virtual ushort GetEmitation(void) const			{ return Emitation; }
	virtual void ForceEmitterNoxify(void);
	virtual void ForceEmitterEmitation(void);
	virtual void Noxify(void);
	virtual void NoxifyEmitter(vector);
	virtual uchar CalculateBitMask(vector) const;
	virtual std::string GetEngraved(void) const { return Engraved; }
	virtual bool Engrave(std::string What) { Engraved = What; return true; }
	virtual void SetEmitation(ushort What) { Emitation = What; }
	virtual std::string GetRememberedItems(void) const { return RememberedItems; }
	virtual void SetRememberedItems(std::string);
	virtual void UpdateItemMemory(void);
	virtual bool CanBeSeen(void) const;
	virtual void Kick(ushort,uchar);
	virtual bool CanBeSeenFrom(vector) const;
	virtual uchar GetDivineOwner(void) const { return DivineOwner; }
	virtual void SetDivineOwner(uchar NDO) { DivineOwner = NDO; }
	virtual void DrawToTileBuffer(void) const;
	virtual void UpdateMemorizedAndDraw(void);
	virtual char CanBeDigged(character*, item*) const;
	virtual bool Dig(character*, item*);
	virtual void HandleFluids(void);
	virtual void SetGroundLevelTerrain(groundlevelterrain*);
	virtual void SetOverLevelTerrain(overlevelterrain*);
	virtual groundlevelterrain* GetGroundLevelTerrain(void) const;
	virtual overlevelterrain* GetOverLevelTerrain(void) const;
	virtual void ChangeLevelTerrain(groundlevelterrain*, overlevelterrain*);
	virtual level* GetMotherLevel(void) const { return (level*)MotherArea; }
	virtual void SetMotherLevel(level* What) { MotherArea = (area*)What; }
protected:
	ushort CalculateEmitation(void) const;
	struct emitter
	{
		emitter(vector Pos, ushort DilatedEmitation) : Pos(Pos), DilatedEmitation(DilatedEmitation) {}
		emitter(void) {}
		bool operator==(emitter& AE) const {if(Pos == AE.Pos) return true; else return false; }
		emitter& operator=(emitter& AE) {Pos = AE.Pos; DilatedEmitation = AE.DilatedEmitation; return *this; }
		vector Pos;
		ushort DilatedEmitation;
	};
	dynarray<emitter> Emitter;
	stack* Stack, * SideStack[4];
	ushort Emitation;
	std::string Engraved;
	std::string RememberedItems;
	uchar DivineOwner;
	bool Fluided;
	ushort TimeFromSpill;
};

#endif

