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
class item;

class item_prototype
{
 public:
  virtual item* Clone(bool = true, bool = true) const = 0;
  virtual item* CloneAndLoad(inputfile&) const = 0;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  virtual bool PolymorphSpawnable() const = 0;
  virtual ushort Possibility() const = 0;
  virtual bool CanBeWished() const = 0;
  virtual item* CreateWishedItem() const = 0;
  virtual bool AutoInitializable() const = 0;
 protected:
  ushort Index;
};

/* Presentation of the item class */

class item : public object
{
 public:
  typedef item_prototype prototype;
  item(bool, bool);
  virtual float GetWeaponStrength() const;
  virtual void DrawToTileBuffer() const;
  virtual void DrawToTileBuffer(vector2d Pos) const;
  virtual void PositionedDrawToTileBuffer(uchar) const;
  virtual ushort GetEmitation() const;
  virtual vector2d GetInHandsPic() const { return vector2d(0,0); }
  virtual item* TryToOpen(character*) { return 0; }
  virtual bool Consume(character*, float);
  virtual bool IsHeadOfElpuri() const { return false; }
  virtual bool IsPetrussNut() const { return false; }
  virtual bool IsGoldenEagleShirt() const { return false; }
  virtual bool CanBeRead(character*) const { return false; }
  virtual bool Read(character*) { return false; }
  virtual void ReceiveHitEffect(character*, character*) { }
  virtual void DipInto(material*, character*) { }
  virtual material* CreateDipMaterial() { return 0; }
  virtual bool CanBeWorn() const { return false; }
  virtual item* BetterVersion() const { return 0; }
  virtual short CalculateOfferValue(char) const;
  virtual float OfferModifier() const { return 0; }
  virtual long Score() const { return 0; }
  virtual bool Destroyable() const { return true; }
  virtual bool Fly(character*, uchar, ushort);
  virtual bool HitCharacter(character*, character*, float);
  virtual bool DogWillCatchAndConsume() const { return false; }
  virtual item* PrepareForConsuming(character*);
  virtual item* Clone(bool = true, bool = true) const = 0;
  static bool CanBeWished() { return true; }
  static item* CreateWishedItem() { return 0; } // never called
  virtual bool Apply(character*);
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool CanBeZapped() const { return false; }
  virtual bool Polymorph(stack*);
  virtual bool IsMaterialChangeable() const { return true; }
  virtual void ChangeMainMaterial(material*);
  virtual void CheckPickUpEffect(character*) { }
  virtual uchar GetWeaponCategory() const;
  virtual float GetThrowGetStrengthModifier() const { return 1; }
  virtual bool UseThrowGetStrengthModifier() const { return false; }
  virtual bool GetStepOnEffect(character*) { return false; }
  virtual ulong Price() const { return 0; }
  virtual bool IsTheAvatar() const { return false; }
  virtual void SignalSquarePositionChange(bool) { }
  //virtual ulong ConsumeLimit() const { return GetConsumeMaterial() ? GetConsumeMaterial()->GetVolume() : 0; }
  virtual bool IsBadFoodForAI(character*) const;
  virtual std::string GetConsumeVerb() const { return "eating"; }
  static bool PolymorphSpawnable() { return true; }
  virtual bool IsExplosive() const { return false; }
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
  static bool AutoInitializable() { return true; }
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
  static bool ConsumableSorter(item* Item, character* Char) { return Item->IsConsumable(Char); }
  static bool OpenableSorter(item* Item, character* Char) { return Item->IsOpenable(Char); }
  static bool ReadableSorter(item* Item, character* Char) { return Item->IsReadable(Char); }
  static bool DippableSorter(item* Item, character* Char) { return Item->IsDippable(Char); }
  static bool DipDestinationSorter(item* Item, character* Char) { return Item->IsDipDestination(Char); }
  static bool AppliableSorter(item* Item, character* Char) { return Item->IsAppliable(Char); }
  static bool ZappableSorter(item* Item, character* Char) { return Item->IsZappable(Char); }
  static bool ChargeableSorter(item* Item, character* Char) { return Item->IsChargeable(Char); }
  static bool HelmetSorter(item* Item, character* Char) { return Item->IsHelmet(Char); }
  static bool AmuletSorter(item* Item, character* Char) { return Item->IsAmulet(Char); }
  static bool CloakSorter(item* Item, character* Char) { return Item->IsCloak(Char); }
  static bool BodyArmorSorter(item* Item, character* Char) { return Item->IsBodyArmor(Char); }
  static bool RingSorter(item* Item, character* Char) { return Item->IsRing(Char); }
  static bool GauntletSorter(item* Item, character* Char) { return Item->IsGauntlet(Char); }
  static bool BeltSorter(item* Item, character* Char) { return Item->IsBelt(Char); }
  static bool BootSorter(item* Item, character* Char) { return Item->IsBoot(Char); }
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
  virtual bool IsOnGround() const;
  virtual ushort GetResistance(uchar) const;
  virtual void GenerateLeftOvers(character*);
  virtual void Be();
  virtual bool RemoveMaterial(uchar);
  virtual ushort SoundResistance() const { return 0; }
  virtual ushort EnergyResistance() const { return 0; }
  virtual ushort AcidResistance() const { return 0; }
  virtual ushort FireResistance() const { return 0; }
  virtual ushort PoisonResistance() const { return 0; }
  virtual ushort BulimiaResistance() const { return 0; }
  static bool SpecialWishedItem() { return false; }
  virtual ushort GetType() const { return Type(); }
  virtual void SetDivineMaster(uchar) { }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual void AddConsumeEndMessage(character*) const;
  virtual bool IsStackable() const { return false; }
  virtual bool IsEqual(item*) const { return false; }
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

#define ITEM_PROTOTYPE(name, base, initmaterials, setstats)\
  \
  static class name##_prototype : public item::prototype\
  {\
   public:\
    name##_prototype() { Index = protocontainer<item>::Add(this); }\
    virtual item* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
    virtual item* CloneAndLoad(inputfile& SaveFile) const { item* Item = new name(false, false); Item->Load(SaveFile); return Item; }\
    virtual std::string ClassName() const { return #name; }\
    virtual bool PolymorphSpawnable() const { return name::PolymorphSpawnable(); }\
    virtual ushort Possibility() const { return name::Possibility(); }\
    virtual bool CanBeWished() const { return name::CanBeWished(); }\
    virtual item* CreateWishedItem() const;\
    virtual bool AutoInitializable() const { return name::AutoInitializable(); }\
  } name##_ProtoType;\
  \
  name::name(bool CreateMaterials, bool SetStats) : base(false, false) { if(SetStats) SetDefaultStats(); if(CreateMaterials) initmaterials ; }\
  name::name(material* FirstMaterial, bool SetStats) : base(false, false) { if(SetStats) SetDefaultStats(); initmaterials ; SetMaterial(0, FirstMaterial); }\
  void name::SetDefaultStats() { setstats }\
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const item::prototype* const name::GetPrototype() { return &name##_ProtoType; }\
  ushort name::Type() const { return name##_ProtoType.GetIndex(); }\
  item* name##_prototype::CreateWishedItem() const { if(!name::SpecialWishedItem()) return Clone(); else return name::CreateWishedItem(); }

#else

#define ITEM_PROTOTYPE(name, base, initmaterials, setstats)

#endif

#define ITEM(name, base, initmaterials, setstats, data)\
\
name : public base\
{\
 public:\
  name(bool = true, bool = true);\
  name(material*, bool = true);\
  virtual item* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
  static ushort StaticType();\
  static const item::prototype* const GetPrototype();\
 protected:\
  virtual void SetDefaultStats();\
  virtual ushort Type() const;\
  data\
}; ITEM_PROTOTYPE(name, base, initmaterials, setstats)

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool CreateMaterials, bool SetStats) : base(CreateMaterials, SetStats) { }\
  data\
};

#endif




