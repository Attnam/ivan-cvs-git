#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#include <string>

#include "dynarray.h"
#include "typedef.h"
#include "vector2d.h"

#include "square.h"

class game;
class bitmap;
class character;
class stack;
class levelterrain;
class groundlevelterrain;
class overlevelterrain;
class material;
class square;
class item;
class outputfile;
class inputfile;
class room;

/* Presentation of the levelsquare class */

class levelsquare : public square
{
public:
	friend class level;
	levelsquare(level*, vector2d);
	virtual ~levelsquare();
	virtual void FastAddCharacter(character* Guy);
	virtual void AddCharacter(character* Guy);
	virtual void RemoveCharacter();
	virtual stack* GetStack() const		{ return Stack; }
	virtual void AlterLuminance(vector2d, ushort);
	virtual void Emitate();
	virtual void ReEmitate();
	virtual stack* GetSideStack(uchar Index) const	{ return SideStack[Index]; }
	virtual void Clean();
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
	virtual ushort GetLuminance() const;
	virtual void SignalEmitationIncrease(ushort);
	virtual void SignalEmitationDecrease(ushort);
	virtual ushort GetEmitation() const			{ return Emitation; }
	virtual void ForceEmitterNoxify();
	virtual void ForceEmitterEmitation();
	virtual void Noxify();
	virtual void NoxifyEmitter(vector2d);
	virtual uchar CalculateBitMask(vector2d) const;
	virtual std::string GetEngraved() const { return Engraved; }
	virtual bool Engrave(std::string What) { Engraved = What; return true; }
	virtual void SetEmitation(ushort What) { Emitation = What; }
	virtual void UpdateMemorizedDescription(bool = false);
	virtual bool Kick(ushort,uchar, character*);
	virtual uchar GetDivineOwner() const { return DivineOwner; }
	virtual void SetDivineOwner(uchar What) { DivineOwner = What; }
	virtual bool DrawTerrain() const;
	virtual bool DrawStacks() const;
	virtual bool DrawCharacters() const;
	virtual void Draw();
	virtual void UpdateMemorized();
	virtual bool CanBeDigged(character*, item*) const;
	virtual bool Dig(character*, item*);
	virtual void HandleFluids();
	virtual void SetGroundLevelTerrain(groundlevelterrain*);
	virtual void SetOverLevelTerrain(overlevelterrain*);
	virtual groundterrain* GetGroundTerrain() const;
	virtual overterrain* GetOverTerrain() const;
	virtual groundlevelterrain* GetGroundLevelTerrain() const { return GroundLevelTerrain; }
	virtual overlevelterrain* GetOverLevelTerrain() const { return OverLevelTerrain; }
	virtual void ChangeLevelTerrain(groundlevelterrain*, overlevelterrain*);
	virtual level* GetLevelUnder() const { return (level*)AreaUnder; }
	virtual void SetLevelUnder(level* What) { AreaUnder = (area*)What; }
	virtual void ChangeGroundLevelTerrain(groundlevelterrain*);
	virtual void ChangeOverLevelTerrain(overlevelterrain*);
	virtual bitmap* GetFluidBuffer() const { return FluidBuffer; }
	virtual void ApplyScript(squarescript*, room*);
	virtual bool CanBeSeen(bool = false) const;
	virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
	virtual void MoveCharacter(levelsquare*);
	virtual ushort GetRawLuminance() const;
	virtual void StepOn(character*, square*);
	virtual uchar GetRoom() const { return Room; }
	virtual void SetRoom(uchar What) { Room = What; }
	virtual void SwapCharacter(levelsquare*);
	virtual void ReceiveVomit(character*);
	virtual room* GetRoomClass() const;
protected:
	groundlevelterrain* GroundLevelTerrain;
	overlevelterrain* OverLevelTerrain;
	ushort CalculateEmitation() const;
	struct emitter
	{
		emitter(vector2d Pos, ushort DilatedEmitation) : Pos(Pos), DilatedEmitation(DilatedEmitation) {}
		emitter() {}
		bool operator==(emitter& AE) const {if(Pos == AE.Pos) return true; else return false; }
		emitter& operator=(emitter& AE) {Pos = AE.Pos; DilatedEmitation = AE.DilatedEmitation; return *this; }
		vector2d Pos;
		ushort DilatedEmitation;
	};
	dynarray<emitter> Emitter;
	stack* Stack, * SideStack[4];
	ushort Emitation;
	std::string Engraved;
	uchar DivineOwner;
	bool Fluided;
	bitmap* FluidBuffer;
	uchar Room;
};

#endif
