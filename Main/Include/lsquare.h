#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "dynarray.h"
#include "typedef.h"
#include "vector2d.h"
#include "square.h"

class game;
class bitmap;
class character;
class stack;
class lterrain;
class glterrain;
class olterrain;
class material;
class square;
class item;
class outputfile;
class inputfile;
class room;
class fluid;

/* Presentation of the lsquare class */

class lsquare : public square
{
 public:
  friend class level;
  lsquare(level*, vector2d);
  virtual ~lsquare();
  virtual void FastAddCharacter(character* Guy);
  virtual void AddCharacter(character* Guy);
  virtual void RemoveCharacter();
  virtual stack* GetStack() const { return Stack; }
  virtual void AlterLuminance(vector2d, ushort);
  virtual void Emitate();
  virtual void ReEmitate();
  virtual stack* GetSideStack(uchar Index) const { return SideStack[Index]; }
  virtual void Clean();
  virtual bool Open(character*);
  virtual bool Close(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  virtual ushort GetLuminance() const;
  virtual void SignalEmitationIncrease(ushort);
  virtual void SignalEmitationDecrease(ushort);
  virtual ushort GetEmitation() const { return Emitation; }
  virtual void ForceEmitterNoxify();
  virtual void ForceEmitterEmitation();
  virtual void Noxify();
  virtual void NoxifyEmitter(vector2d);
  virtual uchar CalculateBitMask(vector2d) const;
  virtual std::string GetEngraved() const { return Engraved; }
  virtual bool Engrave(const std::string& What) { Engraved = What; return true; }
  virtual void SetEmitation(ushort What) { Emitation = What; }
  virtual void UpdateMemorizedDescription(bool = false);
  virtual bool Kick(character*, ushort, uchar);
  virtual uchar GetDivineMaster() const { return DivineMaster; }
  virtual void SetDivineMaster(uchar What) { DivineMaster = What; }
  virtual bool DrawTerrain(bool) const;
  virtual bool DrawStacks(bool) const;
  virtual bool DrawCharacters(bool) const;
  virtual void Draw();
  virtual void UpdateMemorized();
  virtual bool CanBeDug() const;
  virtual bool Dig(character*, item*);
  virtual gterrain* GetGTerrain() const;
  virtual oterrain* GetOTerrain() const;
  virtual glterrain* GetGLTerrain() const { return GLTerrain; }
  virtual olterrain* GetOLTerrain() const { return OLTerrain; }
  virtual void ChangeLTerrain(glterrain*, olterrain*);
  virtual level* GetLevelUnder() const { return (level*)AreaUnder; }
  virtual void SetLevelUnder(level* What) { AreaUnder = (area*)What; }
  virtual void ChangeGLTerrain(glterrain*);
  virtual void ChangeOLTerrain(olterrain*);
  virtual void SetLTerrain(glterrain*, olterrain*);
  virtual void SetGLTerrain(glterrain*);
  virtual void SetOLTerrain(olterrain*);
  virtual void ApplyScript(squarescript*, room*);
  virtual bool CanBeSeen(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
  virtual void MoveCharacter(lsquare*);
  virtual ushort GetRawLuminance() const;
  virtual void StepOn(character*, square*);
  virtual uchar GetRoom() const { return Room; }
  virtual void SetRoom(uchar What) { Room = What; }
  virtual void SwapCharacter(lsquare*);
  virtual void ReceiveVomit(character*);
  virtual room* GetRoomClass() const;
  virtual void SetTemporaryEmitation(ushort);
  virtual ushort GetTemporaryEmitation() const { return TemporaryEmitation; }
  virtual void ChangeOLTerrainAndUpdateLights(olterrain*);
  virtual void DrawParticles(ushort, uchar);
  virtual void PolymorphEverything(character*);
  virtual void StrikeEverything(character*, const std::string&, short, uchar);
  virtual fluid* GetFluid() const { return Fluid; }
  virtual void SetFluid(fluid* What) { Fluid = What; }
  virtual void RemoveFluid();
  virtual void HasBeenHitBy(item*, float, uchar, bool);
  virtual void TeleportEverything(character*);
  virtual bool ReceiveApply(item*, character*);
  virtual bool DipInto(item*, character*);
  virtual void DrawCharacterSymbols(vector2d, ushort);
  virtual bool LockEverything(character*);
 protected:
  glterrain* GLTerrain;
  olterrain* OLTerrain;
  ushort CalculateEmitation() const;
  struct emitter
  {
    emitter(vector2d Pos, ushort DilatedEmitation) : Pos(Pos), DilatedEmitation(DilatedEmitation) { }
    emitter() { }
    bool operator==(const emitter& AE) const { if(Pos == AE.Pos) return true; else return false; }
    emitter& operator=(const emitter& AE) { Pos = AE.Pos; DilatedEmitation = AE.DilatedEmitation; return *this; }
    vector2d Pos;
    ushort DilatedEmitation;
  };
  dynarray<emitter> Emitter;
  stack* Stack, * SideStack[4];
  ushort Emitation;
  std::string Engraved;
  uchar DivineMaster;
  uchar Room;
  ushort TemporaryEmitation;
  fluid* Fluid;
};

#endif


