#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

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
class squarescript;

struct emitter
{
  emitter(vector2d Pos, ushort DilatedEmitation) : Pos(Pos), DilatedEmitation(DilatedEmitation) { }
  emitter() { }
  vector2d Pos;
  ushort DilatedEmitation;
};

outputfile& operator<<(outputfile&, const emitter&);
inputfile& operator>>(inputfile&, emitter&);

class lsquare : public square
{
 public:
  friend class level;
  lsquare(level*, vector2d);
  virtual ~lsquare();
  void FastAddCharacter(character*);
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  stack* GetStack() const { return Stack; }
  void AlterLuminance(vector2d, ushort);
  void Emitate();
  void ReEmitate();
  stack* GetSideStack(ushort Index) const { return SideStack[Index]; }
  void Clean();
  bool Open(character*);
  bool Close(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  void SignalEmitationIncrease(ushort);
  void SignalEmitationDecrease(ushort);
  ushort GetEmitation() const { return Emitation; }
  void ForceEmitterNoxify();
  void ForceEmitterEmitation();
  void Noxify();
  void NoxifyEmitter(vector2d);
  uchar CalculateBitMask(vector2d) const;
  std::string GetEngraved() const { return Engraved; }
  bool Engrave(const std::string& What) { Engraved = What; return true; }
  void SetEmitation(ushort What) { Emitation = What; }
  void UpdateMemorizedDescription(bool = false);
  void BeKicked(character*, float, float, short, bool);
  uchar GetDivineMaster() const { return DivineMaster; }
  void SetDivineMaster(uchar What) { DivineMaster = What; }
  void Draw();
  void UpdateMemorized();
  bool CanBeDug() const;
  virtual gterrain* GetGTerrain() const;
  virtual oterrain* GetOTerrain() const;
  glterrain* GetGLTerrain() const { return GLTerrain; }
  olterrain* GetOLTerrain() const { return OLTerrain; }
  void ChangeLTerrain(glterrain*, olterrain*);
  level* GetLevelUnder() const { return (level*)AreaUnder; }
  void SetLevelUnder(level* What) { AreaUnder = (area*)What; }
  void ChangeGLTerrain(glterrain*);
  void ChangeOLTerrain(olterrain*);
  void SetLTerrain(glterrain*, olterrain*);
  void SetGLTerrain(glterrain*);
  void SetOLTerrain(olterrain*);
  void ApplyScript(squarescript*, room*);
  virtual bool CanBeSeenByPlayer(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
  void MoveCharacter(lsquare*);
  ushort GetRawLuminance() const;
  void StepOn(character*, square*);
  uchar GetRoom() const { return Room; }
  void SetRoom(uchar What) { Room = What; }
  void SwapCharacter(lsquare*);
  void ReceiveVomit(character*);
  room* GetRoomClass() const;
  void SetTemporaryEmitation(ushort);
  ushort GetTemporaryEmitation() const { return TemporaryEmitation; }
  void ChangeOLTerrainAndUpdateLights(olterrain*);
  void DrawParticles(ushort, uchar);
  void PolymorphEverything(character*);
  void StrikeEverything(character*, const std::string&, short, uchar);
  fluid* GetFluid() const { return Fluid; }
  void SetFluid(fluid* What) { Fluid = What; }
  void RemoveFluid();
  void HasBeenHitBy(item*, float, uchar);
  void TeleportEverything(character*);
  bool DipInto(item*, character*);
  bool LockEverything(character*);
  bool RaiseTheDead(character*);
  bool TryKey(item*, character*);
  void SetLastSeen(ulong);
  void CalculateLuminance();
  void DrawStaticContents(bitmap*, vector2d, ushort, bool) const;
  void DrawMemorized();
  bitmap* GetMemorized() const { return Memorized; }
  void SendMemorizedUpdateRequest() { MemorizedUpdateRequested = true; }
  lsquare* GetNeighbourLSquare(ushort) const;
 protected:
  glterrain* GLTerrain;
  olterrain* OLTerrain;
  ushort CalculateEmitation() const;
  std::vector<emitter> Emitter;
  stack* Stack;
  stack* SideStack[4];
  ushort Emitation;
  std::string Engraved;
  uchar DivineMaster;
  uchar Room;
  ushort TemporaryEmitation;
  fluid* Fluid;
  bitmap* Memorized;
  bool MemorizedUpdateRequested;
};

#endif
