#ifndef __ITEMBA_H__
#define __ITEMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "vector2d.h"
#include "object.h"

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

struct item_database
{
  ushort Possibility;
  vector2d InHandsPic;
  ulong OfferModifier;
  long Score;
  bool IsDestroyable;
  bool CanBeWished;
  bool IsMaterialChangeable;
  uchar WeaponCategory;
  bool IsPolymorphSpawnable;
  bool IsAutoInitializable;
  ushort OneHandedStrengthPenalty;
  ushort OneHandedToHitPenalty;
  uchar Category;
  ushort SoundResistance;
  ushort EnergyResistance;
  ushort AcidResistance;
  ushort FireResistance;
  ushort PoisonResistance;
  ushort BulimiaResistance;
  bool IsStackable;
  ushort StrengthModifier;
  ushort FormModifier;
  ulong NPModifier;
  ushort DefaultSize;
  ulong DefaultMainVolume;
  ulong DefaultSecondaryVolume;
  ulong DefaultContainedVolume;
  vector2d BitmapPos;
  ulong Price;
  ushort BaseEmitation;
};

class item_prototype
{
 public:
  item_prototype();
  virtual item* Clone(bool = true) const = 0;
  item* CloneAndLoad(inputfile&) const;
  virtual std::string ClassName() const = 0;
  ushort GetIndex() const { return Index; }
  virtual item_database& GetDataBase() const = 0;
  DATABASEBOOL(IsAutoInitializable);
  DATABASEBOOL(CanBeWished);
  DATABASEBOOL(IsPolymorphSpawnable);
  DATABASEVALUE(ushort, Possibility);
 protected:
  ushort Index;
};

/* Presentation of the item class */

class item : public object
{
 public:
  typedef item_prototype prototype;
  typedef item_database database;
  item();
  virtual float GetWeaponStrength() const;
  virtual void DrawToTileBuffer(bool) const;
  virtual void DrawToTileBuffer(vector2d Pos, bool) const;
  virtual void PositionedDrawToTileBuffer(uchar, bool) const;
  virtual item* TryToOpen(character*);
  virtual bool Consume(character*, float);
  virtual bool IsHeadOfElpuri() const { return false; }
  virtual bool IsPetrussNut() const { return false; }
  virtual bool IsGoldenEagleShirt() const { return false; }
  virtual bool CanBeRead(character*) const { return false; }
  virtual bool Read(character*) { return false; }
  virtual void ReceiveHitEffect(character*, character*) { }
  virtual void DipInto(material*, character*) { }
  virtual material* CreateDipMaterial() { return 0; }
  virtual item* BetterVersion() const { return 0; }
  virtual short CalculateOfferValue(char) const;
  virtual bool Fly(character*, uchar, ushort);
  virtual bool HitCharacter(character*, character*, float);
  virtual bool DogWillCatchAndConsume() const { return false; }
  virtual item* PrepareForConsuming(character*);
  virtual bool Apply(character*);
  virtual bool Zap(character*, vector2d, uchar);
  virtual bool Polymorph(stack*);
  virtual void CheckPickUpEffect(character*) { }
  virtual bool GetStepOnEffect(character*) { return false; }
  virtual bool IsTheAvatar() const { return false; }
  virtual void SignalSquarePositionChange(bool) { }
  virtual bool IsBadFoodForAI(character*) const;
  virtual std::string GetConsumeVerb() const { return "eating"; }
  virtual bool IsExplosive() const { return false; }
  virtual bool CatWillCatchAndConsume() const { return false; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { }
  virtual void SetSize(ushort Value) { Size = Value; }
  virtual ushort GetSize() const { return Size; }
  virtual ulong GetID() const { return ID; }
  virtual void SetID(ulong What) { ID = What; }
  virtual void Teleport();
  virtual ushort GetStrengthValue() const;
  virtual slot* GetSlot() const { return Slot; }
  virtual void SetSlot(slot* What) { Slot = What; }
  virtual void PlaceToSlot(slot*);
  virtual void RemoveFromSlot();
  virtual void MoveTo(stack*);
  virtual void DonateSlotTo(item*);
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
  virtual ushort GetType() const { return GetProtoType().GetIndex(); }
  virtual void SetDivineMaster(uchar) { }
  virtual bool ReceiveDamage(character*, short, uchar) { return false; }
  virtual void AddConsumeEndMessage(character*) const;
  virtual bool IsEqual(item*) const { return false; }
  virtual bool RaiseTheDead(character*) { return false; }
  virtual bool FitsBodyPartIndex(uchar, character*) const { return false; }
  virtual const prototype& GetProtoType() const = 0;
  virtual const database& GetDataBase() const = 0;

  DATABASEVALUE(ushort, Possibility);
  DATABASEVALUE(vector2d, InHandsPic);
  DATABASEVALUE(ulong, OfferModifier);
  DATABASEVALUE(long, Score);
  DATABASEBOOL(IsDestroyable);
  DATABASEBOOL(CanBeWished);
  DATABASEBOOL(IsMaterialChangeable);
  DATABASEVALUE(uchar, WeaponCategory);
  DATABASEBOOL(IsPolymorphSpawnable);
  DATABASEBOOL(IsAutoInitializable);
  DATABASEVALUEWITHPARAMETER(ushort, OneHandedStrengthPenalty, character*);
  DATABASEVALUEWITHPARAMETER(ushort, OneHandedToHitPenalty, character*);
  DATABASEVALUE(uchar, Category);
  DATABASEVALUE(ushort, SoundResistance);
  DATABASEVALUE(ushort, EnergyResistance);
  DATABASEVALUE(ushort, AcidResistance);
  DATABASEVALUE(ushort, FireResistance);
  DATABASEVALUE(ushort, PoisonResistance);
  DATABASEVALUE(ushort, BulimiaResistance);
  DATABASEBOOL(IsStackable);
  DATABASEVALUE(ushort, StrengthModifier);
  DATABASEVALUE(ushort, FormModifier);
  DATABASEVALUE(ulong, NPModifier);
  DATABASEVALUE(ushort, DefaultSize);
  DATABASEVALUE(ulong, DefaultMainVolume);
  DATABASEVALUE(ulong, DefaultSecondaryVolume);
  DATABASEVALUE(ulong, DefaultContainedVolume);
  DATABASEVALUEWITHPARAMETER(vector2d, BitmapPos, ushort);
  DATABASEVALUE(ulong, Price);
  DATABASEVALUE(ushort, BaseEmitation);
  virtual bool SavesLifeWhenWorn() const { return false; }
 protected:
  virtual void LoadDataBaseStats();
  virtual void VirtualConstructor() { }
  virtual void Initialize(bool);
  virtual void GenerateMaterials() = 0;
  virtual uchar GetGraphicsContainerIndex(ushort) const { return GRITEM; }
  virtual bool ShowMaterial() const { return true; }
  slot* Slot;
  bool Cannibalised;
  ushort Size;
  ulong ID;
  graphic_id InHandsGraphicId;
};

#ifdef __FILE_OF_STATIC_ITEM_PROTOTYPE_DECLARATIONS__

#define ITEM_PROTOTYPE(name, base)\
  \
  item::database name##_DataBase;\
  \
  static class name##_prototype : public item::prototype\
  {\
   public:\
    virtual item* Clone(bool CallGenerateMaterials = true) const { return new name(CallGenerateMaterials); }\
    virtual std::string ClassName() const { return #name; }\
    virtual item::database& GetDataBase() const { return name##_DataBase; }\
  } name##_ProtoType;\
  \
  ushort name::StaticType() { return name##_ProtoType.GetIndex(); }\
  const item::prototype& name::GetProtoType() const { return name##_ProtoType; }\
  const item::database& name::GetDataBase() const { return name##_DataBase; }

#else

#define ITEM_PROTOTYPE(name, base)

#endif

#define ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(bool CallGenerateMaterials = true) { Initialize(CallGenerateMaterials); }\
  name(material* FirstMaterial) { Initialize(true); SetMainMaterial(FirstMaterial); }\
  static ushort StaticType();\
  virtual const prototype& GetProtoType() const;\
  virtual const database& GetDataBase() const;\
  data\
}; ITEM_PROTOTYPE(name, base)

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  data\
};

#endif

