#ifndef __ITEMBA_H__
#define __ITEMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define HELMET	    0
#define AMULET	    1
#define CLOAK	    2
#define BODYARMOR   3
#define WEAPON	    4
#define SHIELD	    5
#define RING	    6
#define GAUNTLET    7
#define BELT	    8
#define BOOT	    9
#define FOOD	    10
#define POTION	    11
#define SCROLL	    12
#define BOOK	    13
#define WAND	    14
#define TOOL	    15
#define VALUABLE    16
#define MISC	    17

#include "typedef.h"
#include "vector2d.h"

#include "object.h"
#include "materba.h"
#include "igraph.h"
#include "proto.h"
#include "lterrade.h"

class bitmap;
class character;
class humanoid;
class material;
class object;
class stack;
class outputfile;
class inputfile;
class slot;

/* Presentation of the item class */

class item : public object
{
 public:
  item(bool, bool, bool = true);
  virtual float GetWeaponStrength() const;
  virtual void DrawToTileBuffer() const;
  virtual void DrawToTileBuffer(vector2d Pos) const;
  virtual void PositionedDrawToTileBuffer(uchar) const;
  virtual ushort GetEmitation() const;
  virtual vector2d GetInHandsPic() const { return vector2d(0,0); }
  virtual item* TryToOpen(character*) { return 0; }
  virtual ulong GetWeight() const;
  virtual bool Consume(character*, float);
  //virtual ushort GetArmorValue() const { return 100; }
  virtual bool IsHeadOfElpuri() const { return false; }
  virtual bool IsPetrussNut() const { return false; }
  virtual bool IsGoldenEagleShirt() const { return false; }
  virtual bool CanBeRead(character*) const { return false; }
  virtual bool Read(character*) { return false; }
  virtual void ReceiveHitEffect(character*, character*) { }
  virtual void DipInto(material*, character*) { }
  virtual material* CreateDipMaterial() { return 0; }
  virtual bool CanBeWorn() const { return false; }
  //virtual bool Consumable(character*) const;
  virtual item* BetterVersion() const { return 0; }
  virtual bool ImpactDamage(ushort) { return false; }
  virtual short CalculateOfferValue(char) const;
  virtual float OfferModifier() const { return 0; }
  virtual long Score() const { return 0; }
  virtual bool Destroyable() const { return true; }
  virtual bool Fly(uchar, ushort, stack*, bool = true);
  virtual bool HitCharacter(character*, float, character*);
  virtual bool DogWillCatchAndConsume() const { return false; }
  virtual item* PrepareForConsuming(character*);
  virtual item* Clone(bool = true, bool = true) const = 0;
  virtual ushort Possibility() const = 0;
  virtual bool CanBeWished() const { return true; }
  virtual item* CreateWishedItem() const;
  virtual bool Apply(character*);
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool CanBeZapped() const { return false; }
  virtual bool Polymorph(stack*);
  virtual bool ReceiveSound(float) { return false; }
  virtual bool IsMaterialChangeable() const { return true; }
  virtual void ChangeMainMaterial(material*);
  virtual void CheckPickUpEffect(character*) { }
  virtual uchar GetWeaponCategory() const;
  virtual bool StruckByWandOfStriking(character*, std::string);
  virtual float GetThrowGetStrengthModifier() const { return 1; }
  virtual bool UseThrowGetStrengthModifier() const { return false; }
  virtual bool GetStepOnEffect(character*) { return false; }
  virtual ulong Price() const { return 0; }
  virtual bool IsTheAvatar() const { return false; }
  virtual void SignalSquarePositionChange(bool) { }
  virtual ulong ConsumeLimit() const { return GetConsumeMaterial() ? GetConsumeMaterial()->GetVolume() : 0; }
  virtual bool IsBadFoodForAI(character*) const;
  virtual std::string GetConsumeVerb() const { return std::string("eating"); }
  virtual bool PolymorphSpawnable() const { return true; }
  virtual bool IsExplosive() const { return false; }
  virtual bool ReceiveFireDamage(character*, std::string, stack*, long) { return false; }
  virtual bool CatWillCatchAndConsume() const { return false; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { }
  virtual bool IsChargable() const { return false; }
  virtual void SetSize(ushort Value) { Size = Value; }
  virtual ushort GetSize() const { return Size; }
  virtual ulong GetID() const { return ID; }
  virtual void SetID(ulong What) { ID = What; }
  virtual void Teleport();
  virtual ushort GetStrengthValue() const;
  virtual bool AutoInitializable() const { return true; }
  virtual ulong GetVolume() const;
  virtual slot* GetSlot() const { return Slot; }
  virtual void SetSlot(slot* What) { Slot = What; }
  virtual void PlaceToSlot(slot*);
  virtual void RemoveFromSlot();
  virtual void MoveTo(stack*);
  virtual uchar GetLockType() const { return 0; }
  virtual void DonateSlotTo(item*);
  virtual ushort GetOneHandedStrengthPenalty(character*) { return 0; }
  virtual ushort GetOneHandedToHitPenalty(character*) { return 0; }
  virtual uchar GetCategory() const = 0;
  static uchar ItemCategories() { return 18; }
  static std::string ItemCategoryName(uchar);
  bool ConsumableSorter(character* Char) const { return IsConsumable(Char); }
  bool OpenableSorter(character* Char) const { return IsOpenable(Char); }
  bool ReadableSorter(character* Char) const { return IsReadable(Char); }
  bool DippableSorter(character* Char) const { return IsDippable(Char); }
  bool DipDestinationSorter(character* Char) const { return IsDipDestination(Char); }
  bool AppliableSorter(character* Char) const { return IsAppliable(Char); }
  bool ZappableSorter(character* Char) const { return IsZappable(Char); }
  bool ChargeableSorter(character* Char) const { return IsChargeable(Char); }
  bool HelmetSorter(character* Char) const { return IsHelmet(Char); }
  bool AmuletSorter(character* Char) const { return IsAmulet(Char); }
  bool CloakSorter(character* Char) const { return IsCloak(Char); }
  bool BodyArmorSorter(character* Char) const { return IsBodyArmor(Char); }
  bool RingSorter(character* Char) const { return IsRing(Char); }
  bool GauntletSorter(character* Char) const { return IsGauntlet(Char); }
  bool BeltSorter(character* Char) const { return IsBelt(Char); }
  bool BootSorter(character* Char) const { return IsBoot(Char); }
  virtual bool IsConsumable(character*) const;
  virtual bool IsOpenable(character*) const { return false; }
  virtual bool IsReadable(character*) const { return false; }
  virtual bool IsDippable(character*) const { return false; }
  virtual bool IsDipDestination(character*) const { return false; }
  virtual bool IsAppliable(character*) const { return false; }
  virtual bool IsZappable(character*) const { return false; }
  virtual bool IsChargeable(character*) const { return false; }
  virtual bool IsHelmet(character*) const { return false; }
  virtual bool IsAmulet(character*) const { return false; }
  virtual bool IsCloak(character*) const { return false; }
  virtual bool IsBodyArmor(character*) const { return false; }
  virtual bool IsRing(character*) const { return false; }
  virtual bool IsGauntlet(character*) const { return false; }
  virtual bool IsBelt(character*) const { return false; }
  virtual bool IsBoot(character*) const { return false; }
 protected:
  virtual ushort GetStrengthModifier() const = 0;
  virtual uchar GetGraphicsContainerIndex() const { return GRITEM; }
  virtual void SetDefaultStats() = 0;
  virtual ushort GetFormModifier() const { return 0; }
  virtual float NPModifier() const { return 1.0f; }
  virtual bool ShowMaterial() const { return true; }
  slot* Slot;
  bool Cannibalised;
  ushort Size;
  ulong ID;
};

#ifdef __FILE_OF_STATIC_ITEM_PROTOTYPE_DECLARATIONS__

#define ITEM_PROTOINSTALLER(name, base, initmaterials, setstats)\
  \
  static class name##_protoinstaller\
  {\
   public:\
    name##_protoinstaller() : Index(protocontainer<item>::Add(new name(false, false, false))) { }\
    ushort GetIndex() const { return Index; }\
   private:\
    ushort Index;\
  } name##_ProtoInstaller;\
  \
  name::name(bool CreateMaterials, bool SetStats, bool AddToPool) : base(false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); }\
  name::name(material* FirstMaterial, bool SetStats) : base(false, false) { initmaterials ; SetMaterial(0, FirstMaterial); if(SetStats) SetDefaultStats(); }\
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
  const item* const name::GetPrototype() { return protocontainer<item>::GetProto(StaticType()); }\
  ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

#define ITEM_PROTOINSTALLER(name, base, initmaterials, setstats)

#endif

#define ITEM(name, base, initmaterials, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool = true, bool = true, bool = true);\
  name(material*, bool = true);\
  virtual item* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
  virtual type* CloneAndLoad(inputfile& SaveFile) const { item* Item = new name(false, false); Item->Load(SaveFile); return Item; }\
  static ushort StaticType();\
  static const item* const GetPrototype();\
  virtual std::string ClassName() const { return #name; }\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; ITEM_PROTOINSTALLER(name, base, initmaterials, setstats)

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool CreateMaterials, bool SetStats, bool AddToPool = true) : base(CreateMaterials, SetStats, AddToPool) { }\
  data\
};

#endif



