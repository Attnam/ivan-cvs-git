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

#ifndef __LSQUARE_H__
#define __LSQUARE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "square.h"
#include "level.h"
#include "dungeon.h"
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
class liquid;
class rain;
class trap;

typedef std::vector<item*> itemvector;
typedef bool (item::*sorter)(const character*) const;

#ifdef VC
#pragma pack(1)
#endif

struct emitter
{
  emitter(ulong ID, color24 Emitation) : ID(ID), Emitation(Emitation) { }
  explicit emitter() { }
  ulong ID NO_ALIGNMENT;
  color24 Emitation NO_ALIGNMENT;
};

#ifdef VC
#pragma pack()
#endif

inline ulong MakeEmitterID(vector2d Pos)
{
  return Pos.X + (Pos.Y << 8);
}

inline vector2d ExtractPosFromEmitterID(ulong ID)
{
  return vector2d(ID & 0xFF, (ID >> 8) & 0xFF);
}

typedef std::vector<emitter> emittervector;

outputfile& operator<<(outputfile&, const emitter&);
inputfile& operator>>(inputfile&, emitter&);

struct eyecontroller
{
  static bool Handler(int, int);
  static lsquare*** Map;
};

struct pathcontroller
{
  static bool Handler(int, int);
  static lsquare*** Map;
  static const character* Character;
};

struct stackcontroller
{
  static lsquare*** Map;
  static lsquare** Stack;
  static long StackIndex;
  static int LevelXSize, LevelYSize;
  static vector2d Center;
};

struct tickcontroller
{
  static void PrepareShiftedTick();
  static ulong Tick;
  static ulong ShiftedTick[4];
  static ulong ShiftedQuadriTick[4];
};

class lsquare : public square
{
 public:
  friend class level;
  friend struct loscontroller;
  friend struct sunbeamcontroller;
  friend struct areacontroller;
  friend struct emitationcontroller;
  friend struct noxifycontroller;
  lsquare(level*, vector2d);
  virtual ~lsquare();
  virtual void AddCharacter(character*);
  virtual void RemoveCharacter();
  stack* GetStack() const { return Stack; }
  void AlterLuminance(ulong, color24);
  void Emitate() { Emitate(Emitation); }
  void Emitate(color24, ulong = 0);
  void Clean();
  bool Open(character*);
  bool Close(character*);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  void SignalEmitationIncrease(color24);
  void SignalEmitationDecrease(color24);
  color24 GetEmitation() const { return Emitation; }
  void Noxify() { Noxify(Emitation); }
  void Noxify(color24, ulong = 0);
  bool NoxifyEmitter(ulong);
  const char* GetEngraved() const { return Engraved; }
  bool Engrave(const festring&);
  void UpdateMemorizedDescription(bool = false);
  bool BeKicked(character*, item*, bodypart*, double, double, int, int, bool, bool);
  int GetDivineMaster() const;
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
  /*void SetGLTerrain(glterrain*);
  void SetOLTerrain(olterrain*);*/
  void ApplyScript(const squarescript*, room*);
  virtual bool CanBeSeenByPlayer(bool = false) const;
  virtual bool CanBeSeenFrom(vector2d, long, bool = false) const ;
  void StepOn(character*, lsquare**);
  uint GetRoomIndex() const { return RoomIndex; }
  void SetRoomIndex(uint What) { RoomIndex = What; }
  void ReceiveVomit(character*, liquid*);
  room* GetRoom() const { return RoomIndex ? GetLevel()->GetRoom(RoomIndex) : 0; }
  void SetTemporaryEmitation(color24);
  color24 GetTemporaryEmitation() const { return TemporaryEmitation; }
  void ChangeOLTerrainAndUpdateLights(olterrain*);
  void DrawParticles(long, bool = true);
  vector2d DrawLightning(vector2d, long, int, bool = true);
  bool DipInto(item*, character*);
  bool TryKey(item*, character*);
  void SignalSeen(ulong);
  void CalculateLuminance();
  void DrawStaticContents(bitmap*, vector2d, color24, bool) const;
  void DrawMemorized();
  void DrawMemorizedCharacter();
  void SendMemorizedUpdateRequest();
  lsquare* GetNeighbourLSquare(int) const;
  lsquare* GetNearLSquare(vector2d Pos) const { return static_cast<lsquare*>(AreaUnder->GetSquare(Pos)); }
  bool IsDangerousForAIToStepOn(const character*) const;
  bool IsScaryForAIToStepOn(const character*) const;
  stack* GetStackOfAdjacentSquare(int) const;
  void KickAnyoneStandingHereAway();
  bool IsDark() const;
  void AddItem(item*);
  static bool (lsquare::*GetBeamEffect(int))(const beamdata&);
  bool Polymorph(const beamdata&);
  bool Strike(const beamdata&);
  bool FireBall(const beamdata&);
  bool Teleport(const beamdata&);
  bool Haste(const beamdata&);
  bool Slow(const beamdata&);
  bool Resurrect(const beamdata&);
  bool Invisibility(const beamdata&);
  bool Duplicate(const beamdata&);
  bool Lightning(const beamdata&);
  bool DoorCreation(const beamdata&);
  bool AcidRain(const beamdata&);
  bool Necromancy(const beamdata&);
  int GetLevelIndex() const { return static_cast<level*>(AreaUnder)->GetIndex(); }
  int GetDungeonIndex() const { return static_cast<level*>(AreaUnder)->GetDungeon()->GetIndex(); }
  dungeon* GetDungeon() const { return static_cast<level*>(AreaUnder)->GetDungeon(); }
  bool CheckKick(const character*) const;
  void GetHitByExplosion(const explosion*);
  int GetSpoiledItems() const;
  void SortAllItems(itemvector&, const character* = 0, sorter = 0);
  bool LowerEnchantment(const beamdata&);
  void RemoveSmoke(smoke*);
  void AddSmoke(gas*);
  bool IsFlyable() const { return !OLTerrain || (OLTerrain->GetWalkability() & FLY); }
  bool IsTransparent() const { return !!(Flags & IS_TRANSPARENT); }
  void SignalSmokeAlphaChange(int);
  void ShowSmokeMessage() const;
  void DisplaySmokeInfo(festring&) const;
  bool IsDipDestination() const;
  void ReceiveEarthQuakeDamage();
  bool IsDangerous(character*) const;
  bool CanBeFeltByPlayer() const;
  void PreProcessForBone();
  void PostProcessForBone(double&, int&);
  void DisplayEngravedInfo(festring&) const;
  bool EngravingsCanBeReadByPlayer();
  bool HasEngravings() const { return !!Engraved; }
  void FinalProcessForBone();
  bool IsFreezed() const { return !!(Flags & FREEZED); }
  bool IsDangerousForAIToBreathe(const character*) const;
  bool IsScaryForAIToBreathe(const character*) const;
  int GetWalkability() const;
  int GetTheoreticalWalkability() const { return OLTerrain ? OLTerrain->GetTheoreticalWalkability() & GLTerrain->GetTheoreticalWalkability() : GLTerrain->GetTheoreticalWalkability(); }
  virtual int GetSquareWalkability() const { return GetWalkability(); }
  void CalculateBorderPartners();
  void RequestForBorderPartnerUpdates();
  void SpillFluid(character*, liquid*, bool = false, bool = true);
  void DisplayFluidInfo(festring&) const;
  void RemoveFluid(fluid*);
  void AddFluid(liquid*);
  void DrawStacks(bitmap*, vector2d, color24, bool) const;
  bool FluidIsVisible() const { return SmokeAlphaSum < 175; }
  void RemoveRain(rain*);
  void AddRain(liquid*, vector2d, int, bool);
  bool IsInside() const { return !!(Flags & INSIDE); }
  void RemoveSunLight();
  void CheckIfIsSecondarySunLightEmitter();
  void CalculateNeighbourLSquares();
  const emittervector& GetEmitter() const { return Emitter; }
  void EnableGlobalRain();
  void DisableGlobalRain();
  void Freeze() { Flags |= FREEZED; }
  void UnFreeze() { Flags &= ~FREEZED; }
  void InitLastSeen();
  void GetSideItemDescription(festring&, bool = false) const;
  void AddSunLightEmitter(ulong);
  void SendSunLightSignals();
  const sunemittervector& GetSunEmitter() const { return SunEmitter; }
  void ZeroReSunEmitatedFlags();
  virtual bool HasBeenSeen() const { return !!Memorized; }
  bool CalculateIsTransparent();
  void CalculateSunLightLuminance(ulong);
  void CreateMemorized();
  bool DetectMaterial(const material*) const;
  void Reveal(ulong, bool = false);
  void DestroyMemorized();
  void SwapMemorized(lsquare*);
  bool HasNoBorderPartners() const { return !(BorderPartnerInfo >> 24); }
  lsquare* GetRandomAdjacentSquare() const;
  void SignalPossibleTransparencyChange();
  bool AddTrap(trap*);
  void RemoveTrap(trap*);
  void DisplayTrapInfo(festring&) const;
 protected:
  void ChangeLuminance(color24&, color24);
  void RemoveLuminance(color24&);
  void CalculateEmitation();
  fluid* Fluid;
  smoke* Smoke;
  rain* Rain;
  trap* Trap;
  emittervector Emitter;
  sunemittervector SunEmitter;
  glterrain* GLTerrain;
  olterrain* OLTerrain;
  stack* Stack;
  bitmap* Memorized;
  bitmap* FowMemorized;
  char* Engraved;
  olterrain* BorderPartnerTerrain[8];
  ulong BorderPartnerInfo;
  ulong SquarePartEmitationTick;
  ulong SquarePartLastSeen;
  color24 Emitation;
  int SmokeAlphaSum;
  lsquare* NeighbourLSquare[8];
  color24 AmbientLuminance;
  color24 SunLightLuminance;
  color24 TemporaryEmitation;
  color24 SecondarySunLightEmitation;
  ushort LastExplosionID;
  uchar RoomIndex;
};

inline bool lsquare::IsDark() const
{
  color24 Light = Luminance;

  return !Light
      || ((Light & 0xFF0000) < (LIGHT_BORDER << 16)
       && (Light & 0x00FF00) < (LIGHT_BORDER << 8)
       && (Light & 0x0000FF) < LIGHT_BORDER);
}

inline bool eyecontroller::Handler(int x, int y)
{
  return Map[x][y]->IsTransparent();
}

inline lsquare* lsquare::GetNeighbourLSquare(int I) const
{
  return I < 8 ? NeighbourLSquare[I] : const_cast<lsquare*>(this);
}

#endif
