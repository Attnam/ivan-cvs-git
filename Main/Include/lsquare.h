#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "square.h"
#include "level.h"
#include "dungeon.h"
#include "felibdef.h"
#include "lterra.h"

#ifndef LIGHT_BORDER
#define LIGHT_BORDER 80
#endif

class stack;
class glterrain;
class olterrain;
class fluid;
class material;
class item;
class smoke;
class gas;
class bodypart;

typedef std::vector<item*> itemvector;

struct emitter
{
  emitter(vector2d Pos, ulong DilatedEmitation) : Pos(Pos), DilatedEmitation(DilatedEmitation) { }
  emitter() { }
  vector2d Pos;
  ulong DilatedEmitation;
};

outputfile& operator<<(outputfile&, const emitter&);
inputfile& operator>>(inputfile&, emitter&);

struct borderpartner
{
  olterrain* Terrain;
  ushort SquareIndex;
};

class lsquare : public square
{
 public:
  friend class level;
  lsquare(level*, vector2d);
  virtual ~lsquare();
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  stack* GetStack() const { return Stack; }
  void AlterLuminance(vector2d, ulong);
  void Emitate();
  void ReEmitate(ulong);
  stack* GetSideStack(ushort Index) const { return SideStack[Index]; }
  void Clean();
  bool Open(character*);
  bool Close(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SpillFluid(uchar, ulong, ushort = 5, ushort = 32);
  void SignalEmitationIncrease(ulong);
  void SignalEmitationDecrease(ulong);
  ulong GetEmitation() const { return Emitation; }
  void ForceEmitterNoxify();
  void ForceEmitterEmitation();
  void Noxify();
  void NoxifyEmitter(vector2d);
  uchar CalculateBitMask(vector2d) const;
  festring GetEngraved() const { return Engraved; }
  bool Engrave(const festring& What) { Engraved = What; return true; }
  void SetEmitation(ulong What) { Emitation = What; }
  void UpdateMemorizedDescription(bool = false);
  bool BeKicked(character*, item*, bodypart*, float, float, short, uchar, bool, bool);
  uchar GetDivineMaster() const;
  void Draw();
  void UpdateMemorized();
  bool CanBeDug() const;
  virtual gterrain* GetGTerrain() const { return GLTerrain; }
  virtual oterrain* GetOTerrain() const { return OLTerrain; }
  glterrain* GetGLTerrain() const { return GLTerrain; }
  olterrain* GetOLTerrain() const { return OLTerrain; }
  void ChangeLTerrain(glterrain*, olterrain*);
  level* GetLevel() const { return static_cast<level*>(AreaUnder); }
  void SetLevelUnder(level* What) { AreaUnder = What; }
  void ChangeGLTerrain(glterrain*);
  void ChangeOLTerrain(olterrain*);
  void SetLTerrain(glterrain*, olterrain*);
  void SetGLTerrain(glterrain*);
  void SetOLTerrain(olterrain*);
  void ApplyScript(const squarescript*, room*);
  virtual bool CanBeSeenByPlayer(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, ulong, bool = false) const;
  ulong GetRawLuminance() const;
  void StepOn(character*, lsquare**);
  uchar GetRoomIndex() const { return RoomIndex; }
  void SetRoomIndex(uchar What) { RoomIndex = What; }
  void ReceiveVomit(character*, ushort);
  room* GetRoom() const { return RoomIndex ? GetLevel()->GetRoom(RoomIndex) : 0; }
  void SetTemporaryEmitation(ulong);
  ulong GetTemporaryEmitation() const { return TemporaryEmitation; }
  void ChangeOLTerrainAndUpdateLights(olterrain*);
  void DrawParticles(ulong, bool = true);
  vector2d DrawLightning(vector2d, ulong, uchar, bool = true);
  fluid* GetFluid() const { return Fluid; }
  void SetFluid(fluid* What) { Fluid = What; }
  void RemoveFluid();
  bool DipInto(item*, character*);
  bool TryKey(item*, character*);
  void SetLastSeen(ulong);
  void CalculateLuminance();
  void DrawStaticContents(bitmap*, vector2d, ulong, bool) const;
  void DrawMemorized();
  void DrawMemorizedCharacter();
  bitmap* GetMemorized() const { return Memorized; }
  void SendMemorizedUpdateRequest();
  lsquare* GetNeighbourLSquare(ushort Index) const { return static_cast<level*>(AreaUnder)->GetNeighbourLSquare(Pos, Index); }
  lsquare* GetNearLSquare(vector2d Pos) const { return static_cast<lsquare*>(AreaUnder->GetSquare(Pos)); }
  bool IsDangerousForAIToStepOn(const character*) const;
  stack* GetSideStackOfAdjacentSquare(ushort) const;
  void KickAnyoneStandingHereAway();
  void SpillFluid(character*, material*, ushort);
  bool IsDark() const;
  void AddItem(item*);
  static bool (lsquare::*GetBeamEffect(ushort))(character*, const festring&, uchar);
  bool Polymorph(character*, const festring&, uchar);
  bool Strike(character*, const festring&, uchar);
  bool FireBall(character*, const festring&, uchar);
  bool Teleport(character*, const festring&, uchar);
  bool Haste(character*, const festring&, uchar);
  bool Slow(character*, const festring&, uchar);
  bool Resurrect(character*, const festring&, uchar);
  bool Invisibility(character*, const festring&, uchar);
  bool Clone(character*, const festring&, uchar);
  bool Lightning(character*, const festring&, uchar);
  bool DoorCreation(character*, const festring&, uchar);
  stack* GetFirstSideStackUnderAttack(uchar) const;
  stack* GetSecondSideStackUnderAttack(uchar) const;
  uchar GetLevelIndex() const { return static_cast<level*>(AreaUnder)->GetIndex(); }
  uchar GetDungeonIndex() const { return static_cast<level*>(AreaUnder)->GetDungeon()->GetIndex(); }
  dungeon* GetDungeon() const { return static_cast<level*>(AreaUnder)->GetDungeon(); }
  bool CheckKick(const character*) const;
  void GetHitByExplosion(const explosion*);
  ushort GetSpoiledItems() const;
  void SortAllItems(itemvector&, const character* = 0, bool (*)(const item*, const character*) = 0);
  bool LowerEnchantment(character*, const festring&, uchar);
  void RemoveSmoke(smoke*);
  void AddSmoke(gas*);
  bool IsFlyable() const { return !OLTerrain || (OLTerrain->GetWalkability() & FLY); }
  bool IsTransparent() const { return (!OLTerrain || OLTerrain->IsTransparent()) && SmokeAlphaSum < 175; }
  void SignalSmokeAlphaChange(short);
  void ShowSmokeMessage() const;
  void DisplaySmokeInfo(festring&) const;
  bool IsDipDestination() const;
  void ReceiveEarthQuakeDamage();
  bool IsDangerous(character*) const;
  bool CanBeFeltByPlayer() const;
  void PreProcessForBone();
  void PostProcessForBone(float&, ushort&);
  void DisplayEngravedInfo(festring&) const;
  bool EngravingsCanBeReadByPlayer();
  bool HasEngravings() const { return !Engraved.IsEmpty(); }
  void FinalProcessForBone();
  void SetIsFreezed(bool What) { Freezed = What; }
  bool IsDangerousForAIToBreathe(const character*) const;
  uchar GetWalkability() const { return OLTerrain ? OLTerrain->GetWalkability() & GLTerrain->GetWalkability() : GLTerrain->GetWalkability(); }
  uchar GetTheoreticalWalkability() const { return OLTerrain ? OLTerrain->GetTheoreticalWalkability() & GLTerrain->GetTheoreticalWalkability() : GLTerrain->GetTheoreticalWalkability(); }
  virtual uchar GetSquareWalkability() const { return GetWalkability(); }
  void CalculateBorderPartners();
  void RequestForBorderPartnerUpdates();
 protected:
  glterrain* GLTerrain;
  olterrain* OLTerrain;
  void CalculateEmitation();
  std::vector<emitter> Emitter;
  stack* Stack;
  stack* SideStack[4];
  ulong Emitation;
  festring Engraved;
  uchar RoomIndex;
  ulong TemporaryEmitation;
  fluid* Fluid;
  bitmap* Memorized;
  bool MemorizedUpdateRequested;
  ulong LastExplosionID;
  std::vector<smoke*> Smoke;
  ushort SmokeAlphaSum;
  bool Freezed;
  borderpartner BorderPartner[8];
};

inline bool lsquare::IsDark() const
{
  return !Luminance || (GetRed24(Luminance) < LIGHT_BORDER && GetGreen24(Luminance) < LIGHT_BORDER && GetBlue24(Luminance) < LIGHT_BORDER);
}

#endif
