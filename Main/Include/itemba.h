#ifndef __ITEMBA_H__
#define __ITEMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cmath>

#include "typedef.h"
#include "vector2d.h"
#include "object.h"
#include "lsquare.h"
#include "slot.h"

class felist;
class bitmap;
class character;
class humanoid;
class material;
class stack;
class outputfile;
class inputfile;
class slot;
class item;
class felist;
template <class type> class contentscript;
template <class type> class database;

struct itemdatabase
{
  void InitDefaults(ushort);
  ushort Possibility;
  vector2d InHandsPic;
  long Score;
  bool IsDestroyable;
  bool CanBeWished;
  bool IsMaterialChangeable;
  uchar WeaponCategory;
  bool IsPolymorphSpawnable;
  bool IsAutoInitializable;
  ulong Category;
  ushort SoundResistance;
  ushort EnergyResistance;
  ushort AcidResistance;
  ushort FireResistance;
  ushort PoisonResistance;
  ushort BulimiaResistance;
  ushort StrengthModifier;
  ushort FormModifier;
  ulong NPModifier;
  ushort DefaultSize;
  ulong DefaultMainVolume;
  ulong DefaultSecondaryVolume;
  ulong DefaultContainedVolume;
  vector2d BitmapPos;
  ulong Price;
  ulong BaseEmitation;
  std::string Article;
  std::string Adjective;
  std::string AdjectiveArticle;
  std::string NameSingular;
  std::string NamePlural;
  std::string PostFix;
  uchar ArticleMode;
  std::vector<long> MainMaterialConfig;
  std::vector<long> SecondaryMaterialConfig;
  std::vector<long> ContainedMaterialConfig;
  std::vector<long> MaterialConfigChances;
  bool IsAbstract;
  bool IsPolymorphable;
  std::vector<std::string> Alias;
  uchar OKVisualEffects;
  bool CanBeGeneratedInContainer;
  uchar ForcedVisualEffects;
  uchar Roundness;
  ushort GearStates;
  bool IsTwoHanded;
  bool CreateDivineConfigurations;
  bool CanBeCloned;
  ushort BeamRange;
  bool CanBeBroken;
  vector2d WallBitmapPos;
  std::string FlexibleNameSingular;
  uchar MinCharges;
  uchar MaxCharges;
  bool CanBePiled;
  ulong StorageVolume;
  ushort MaxGeneratedContainedItems;
  bool AffectsArmStrength;
  bool AffectsLegStrength;
  bool AffectsDexterity;
  bool AffectsAgility;
  bool AffectsEndurance;
  bool AffectsPerception;
  bool AffectsIntelligence;
  bool AffectsWisdom;
  bool AffectsCharisma;
  bool AffectsMana;
  char DefaultEnchantment;
  bool PriceIsProportionalToEnchantment;
  uchar InElasticityPenaltyModifier;
  bool CanBeUsedBySmith;
  bool AffectsCarryingCapacity;
};

class itemprototype
{
 public:
  friend class database<item>;
  itemprototype(itemprototype*, item* (*)(ushort, ushort), const std::string&);
  item* Clone(ushort Config = 0, ushort SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  item* CloneAndLoad(inputfile&) const;
  const std::string& GetClassId() const { return ClassId; }
  ushort GetIndex() const { return Index; }
  const itemprototype* GetBase() const { return Base; }
  const std::map<ushort, itemdatabase>& GetConfig() const { return Config; }
  void CreateSpecialConfigurations() { }
  bool IsAbstract() const { return Config.begin()->second.IsAbstract; }
  const itemdatabase& ChooseBaseForConfig(ushort);
 private:
  ushort Index;
  itemprototype* Base;
  std::map<ushort, itemdatabase> Config;
  item* (*Cloner)(ushort, ushort);
  std::string ClassId;
};

class item : public object
{
 public:
  friend class database<item>;
  typedef itemprototype prototype;
  typedef itemdatabase database;
  typedef std::map<ushort, itemdatabase> databasemap;
  item(donothing);
  item(const item&);
  virtual float GetWeaponStrength() const;
  virtual bool Open(character*);
  virtual bool Consume(character*, long);
  virtual bool IsHeadOfElpuri() const { return false; }
  virtual bool IsPetrussNut() const { return false; }
  virtual bool IsGoldenEagleShirt() const { return false; }
  virtual bool CanBeRead(character*) const { return false; }
  virtual bool Read(character*) { return false; }
  virtual void FinishReading(character*) { }
  virtual bool HitEffect(character*, character*, uchar, uchar, bool) { return false; }
  virtual void DipInto(material*, character*) { }
  virtual material* CreateDipMaterial() { return 0; }
  virtual item* BetterVersion() const { return 0; }
  virtual short GetOfferValue(char) const;
  virtual bool Fly(character*, uchar, ushort);
  virtual bool HitCharacter(character*, character*, float);
  virtual bool DogWillCatchAndConsume() const { return false; }
  virtual bool Apply(character*);
  virtual bool Zap(character*, vector2d, uchar) { return false; }
  virtual bool Polymorph(stack*);
  virtual bool CheckPickUpEffect(character*) { return true; }
  virtual void StepOnEffect(character*) { }
  virtual bool IsTheAvatar() const { return false; }
  virtual void SignalSquarePositionChange(uchar) { }
  virtual bool IsBadFoodForAI(const character*) const;
  virtual std::string GetConsumeVerb() const;
  virtual bool IsExplosive() const { return false; }
  virtual bool CatWillCatchAndConsume() const { return false; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { }
  virtual void SetSize(ushort Value) { Size = Value; }
  virtual ushort GetSize() const { return Size; }
  virtual ulong GetID() const { return ID; }
  virtual void SetID(ulong What) { ID = What; }
  virtual void TeleportRandomly();
  virtual ushort GetStrengthValue() const;
  slot* GetSlot() const { return Slot; }
  void SetSlot(slot* What) { Slot = What; }
  void PlaceToSlot(slot* Slot) { Slot->PutInItem(this); }
  void RemoveFromSlot();
  void MoveTo(stack*);
  static std::string ItemCategoryName(ulong);
  static bool EatableSorter(item* Item, const character* Char) { return Item->IsEatable(Char); }
  static bool DrinkableSorter(item* Item, const character* Char) { return Item->IsDrinkable(Char); }
  static bool OpenableSorter(item* Item, const character* Char) { return Item->IsOpenable(Char); }
  static bool ReadableSorter(item* Item, const character* Char) { return Item->IsReadable(Char); }
  static bool DippableSorter(item* Item, const character* Char) { return Item->IsDippable(Char); }
  static bool DipDestinationSorter(item* Item, const character* Char) { return Item->IsDipDestination(Char); }
  static bool AppliableSorter(item* Item, const character* Char) { return Item->IsAppliable(Char); }
  static bool ZappableSorter(item* Item, const character* Char) { return Item->IsZappable(Char); }
  static bool ChargeableSorter(item* Item, const character* Char) { return Item->IsChargeable(Char); }
  static bool HelmetSorter(item* Item, const character* Char) { return Item->IsHelmet(Char); }
  static bool AmuletSorter(item* Item, const character* Char) { return Item->IsAmulet(Char); }
  static bool CloakSorter(item* Item, const character* Char) { return Item->IsCloak(Char); }
  static bool BodyArmorSorter(item* Item, const character* Char) { return Item->IsBodyArmor(Char); }
  static bool RingSorter(item* Item, const character* Char) { return Item->IsRing(Char); }
  static bool GauntletSorter(item* Item, const character* Char) { return Item->IsGauntlet(Char); }
  static bool BeltSorter(item* Item, const character* Char) { return Item->IsBelt(Char); }
  static bool BootSorter(item* Item, const character* Char) { return Item->IsBoot(Char); }
  static bool WeaponSorter(item* Item, const character* Char) { return Item->IsWeapon(Char); }
  static bool ArmorSorter(item* Item, const character* Char) { return Item->IsArmor(Char); }
  virtual bool IsConsumable(const character*) const;
  virtual bool IsEatable(const character*) const;
  virtual bool IsDrinkable(const character*) const;
  virtual bool IsOpenable(const character*) const { return false; }
  virtual bool IsReadable(const character*) const { return false; }
  virtual bool IsDippable(const character*) const { return false; }
  virtual bool IsDipDestination(const character*) const { return false; }
  virtual bool IsAppliable(const character*) const { return false; }
  virtual bool IsZappable(const character*) const { return false; }
  virtual bool IsChargeable(const character*) const { return false; }
  virtual bool IsHelmet(const character*) const { return false; }
  virtual bool IsAmulet(const character*) const { return false; }
  virtual bool IsCloak(const character*) const { return false; }
  virtual bool IsBodyArmor(const character*) const { return false; }
  virtual bool IsRing(const character*) const { return false; }
  virtual bool IsGauntlet(const character*) const { return false; }
  virtual bool IsBelt(const character*) const { return false; }
  virtual bool IsBoot(const character*) const { return false; }
  virtual bool IsShield(const character*) const { return false; }
  virtual bool IsWeapon(const character*) const { return false; }
  virtual bool IsArmor(const character*) const { return false; }
  virtual bool IsOnGround() const { return GetSlot()->IsOnGround(); }
  virtual ushort GetResistance(uchar) const;
  virtual void GenerateLeftOvers(character*);
  virtual void Be();
  virtual bool RemoveMaterial(uchar) { return true; }
  ushort GetType() const { return GetProtoType()->GetIndex(); }
  virtual bool ReceiveDamage(character*, ushort, uchar);
  virtual void AddConsumeEndMessage(character*) const;
  virtual bool IsEqual(item*) const { return false; }
  virtual bool RaiseTheDead(character*) { return false; }
  virtual uchar GetBodyPartIndex() const { return 0xFF; }
  virtual const prototype* GetProtoType() const { return &item_ProtoType; }
  const database* GetDataBase() const { return DataBase; }
  virtual bool CanOpenLockType(uchar) const { return false; }
  virtual bool IsWhip() const { return false; }
  virtual DATA_BASE_VALUE(vector2d, InHandsPic);
  virtual DATA_BASE_VALUE(long, Score);
  virtual DATA_BASE_BOOL(IsDestroyable);
  virtual DATA_BASE_BOOL(CanBeWished);
  virtual DATA_BASE_BOOL(IsMaterialChangeable);
  DATA_BASE_VALUE(uchar, WeaponCategory);
  virtual DATA_BASE_BOOL(IsAutoInitializable);
  DATA_BASE_VALUE(ulong, Category);
  virtual DATA_BASE_VALUE(ushort, SoundResistance);
  virtual DATA_BASE_VALUE(ushort, EnergyResistance);
  virtual DATA_BASE_VALUE(ushort, AcidResistance);
  virtual DATA_BASE_VALUE(ushort, FireResistance);
  virtual DATA_BASE_VALUE(ushort, PoisonResistance);
  virtual DATA_BASE_VALUE(ushort, BulimiaResistance);
  virtual DATA_BASE_VALUE(ushort, StrengthModifier);
  virtual DATA_BASE_VALUE(ushort, FormModifier);
  virtual DATA_BASE_VALUE(ulong, NPModifier);
  DATA_BASE_VALUE(ushort, DefaultSize);
  DATA_BASE_VALUE(ulong, DefaultMainVolume);
  DATA_BASE_VALUE(ulong, DefaultSecondaryVolume);
  DATA_BASE_VALUE(ulong, DefaultContainedVolume);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(vector2d, BitmapPos, ushort);
  virtual DATA_BASE_VALUE(ulong, Price);
  virtual DATA_BASE_VALUE(ulong, BaseEmitation);
  virtual DATA_BASE_VALUE(const std::string&, Article);
  virtual DATA_BASE_VALUE(const std::string&, Adjective);
  virtual DATA_BASE_VALUE(const std::string&, AdjectiveArticle);
  virtual DATA_BASE_VALUE(const std::string&, NameSingular);
  virtual DATA_BASE_VALUE(const std::string&, NamePlural);
  virtual DATA_BASE_VALUE(const std::string&, PostFix);
  virtual DATA_BASE_VALUE(uchar, ArticleMode);
  DATA_BASE_VALUE(const std::vector<long>&, MainMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, SecondaryMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, ContainedMaterialConfig);
  DATA_BASE_VALUE(const std::vector<long>&, MaterialConfigChances);
  virtual DATA_BASE_BOOL(IsPolymorphable);
  virtual DATA_BASE_VALUE(const std::vector<std::string>&, Alias);
  virtual DATA_BASE_VALUE(uchar, OKVisualEffects);
  virtual DATA_BASE_BOOL(CanBeGeneratedInContainer);
  virtual DATA_BASE_VALUE(uchar, ForcedVisualEffects);
  virtual DATA_BASE_VALUE(uchar, Roundness);
  virtual DATA_BASE_VALUE(ushort, GearStates);
  virtual DATA_BASE_BOOL(IsTwoHanded);
  virtual DATA_BASE_BOOL(CanBeBroken);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(vector2d, WallBitmapPos, ushort);
  virtual DATA_BASE_VALUE(const std::string&, FlexibleNameSingular);
  virtual DATA_BASE_BOOL(CanBePiled);
  DATA_BASE_BOOL(AffectsArmStrength);
  DATA_BASE_BOOL(AffectsLegStrength);
  DATA_BASE_BOOL(AffectsDexterity);
  DATA_BASE_BOOL(AffectsAgility);
  DATA_BASE_BOOL(AffectsEndurance);
  DATA_BASE_BOOL(AffectsPerception);
  DATA_BASE_BOOL(AffectsIntelligence);
  DATA_BASE_BOOL(AffectsWisdom);
  DATA_BASE_BOOL(AffectsCharisma);
  DATA_BASE_BOOL(AffectsMana);
  DATA_BASE_BOOL(AffectsCarryingCapacity);
  DATA_BASE_VALUE(char, DefaultEnchantment);
  virtual DATA_BASE_BOOL(PriceIsProportionalToEnchantment);
  virtual DATA_BASE_VALUE(uchar, MaxCharges);
  virtual DATA_BASE_VALUE(uchar, MinCharges);
  DATA_BASE_VALUE(uchar, InElasticityPenaltyModifier);
  virtual DATA_BASE_VALUE(ulong, StorageVolume);
  virtual DATA_BASE_VALUE(ushort, MaxGeneratedContainedItems);
  virtual DATA_BASE_BOOL(CanBeCloned);
  virtual DATA_BASE_VALUE(ushort, BeamRange);
  virtual DATA_BASE_BOOL(CanBeUsedBySmith);
  static item* Clone(ushort, ushort) { return 0; }
  virtual bool CanBeSoldInLibrary(character* Librarian) const { return CanBeRead(Librarian); }
  virtual bool TryKey(item*, character*) { return false; }
  virtual bool TryToUnstuck(character*, vector2d) { return true; }
  virtual bool TryToUnstuck(character*, ushort, vector2d) { return false; }
  virtual ulong GetBlockModifier() const;
  virtual bool IsSimiliarTo(item* Item) const { return Item->GetType() == GetType() && Item->GetConfig() == GetConfig(); }
  virtual bool IsPickable(character*) const { return true; }
  virtual bool CanBeSeenByPlayer() const;
  virtual bool CanBeSeenBy(const character*) const;
  virtual std::string GetDescription(uchar) const;
  virtual square* GetSquareUnder() const { return Slot ? Slot->GetSquareUnder() : 0; }
  lsquare* GetLSquareUnder() const { return static_cast<lsquare*>(Slot->GetSquareUnder()); }
  level* GetLevelUnder() const { return static_cast<level*>(Slot->GetSquareUnder()->GetAreaUnder()); }
  area* GetAreaUnder() const { return Slot->GetSquareUnder()->GetAreaUnder(); }
  vector2d GetPos() const { return Slot->GetSquareUnder()->GetPos(); }
  square* GetNearSquare(vector2d Pos) const { return Slot->GetSquareUnder()->GetAreaUnder()->GetSquare(Pos); }
  lsquare* GetNearLSquare(vector2d Pos) const { return static_cast<lsquare*>(Slot->GetSquareUnder()->GetAreaUnder()->GetSquare(Pos)); }
  virtual void SignalVolumeAndWeightChange();
  virtual void CalculateVolumeAndWeight();
  ulong GetVolume() const { return Volume; }
  ulong GetWeight() const { return Weight; }
  virtual void SignalEmitationIncrease(ulong);
  virtual void SignalEmitationDecrease(ulong);
  void CalculateAll();
  virtual void DropEquipment() { }
  virtual bool DangerousToStepOn(const character*) const { return false; } 
  void WeaponSkillHit();
  virtual void SetTeam(ushort) { }
  virtual void SpecialGenerationHandler() { }
  item* Duplicate() const;
  virtual void SetIsActive(bool) { }
  ushort GetBaseMinDamage() const { return ushort(sqrt(GetWeaponStrength() / 20000.0f) * 0.75f); }
  ushort GetBaseMaxDamage() const { return ushort(sqrt(GetWeaponStrength() / 20000.0f) * 1.25f) + 1; }
  ushort GetBaseBlockValue() const { return ushort(GetBlockModifier() * GetBonus() / (100000 + 200 * GetWeight())); }
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual void AddAttackInfo(felist&) const;
  virtual void AddMiscellaneousInfo(felist&) const;
  virtual ulong GetNutritionValue() const;
  virtual void SignalSpoil(material*);
  virtual bool AllowSpoil() const;
  bool CarriedByPlayer() const;
  bool CarriedBy(const character*) const;
  item* DuplicateToStack(stack*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual ulong GetTotalExplosivePower() const { return 0; }
  virtual void Break();
  void Empty();
  virtual void SetEnchantment(char) { }
  virtual void EditEnchantment(char) { }
  virtual void SignalEnchantmentChange();
  virtual ushort GetBonus() const { return 100; }
  virtual void DrawContents(const character*) { }
  bool IsBroken() const { return (Config & BROKEN) != 0; }
  virtual void ReceiveFluidSpill(material*) { }
  virtual char GetEnchantment() const { return 0; }
  virtual ulong GetEnchantedPrice(char) const;
  virtual void Fix();
  virtual ushort GetStrengthRequirement() const;
  virtual ushort GetInElasticityPenalty(ushort) const { return 0; }
  virtual bool IsFixableBySmith(const character*) const { return false; }
  static bool IsFixableBySmithSorter(item* Item, const character* Char) { return Item->IsFixableBySmith(Char); }
  virtual ulong GetFixPrice() const { return 100; } 
  void DonateSlotTo(item*);
  virtual uchar GetFlyAmount() const;
  virtual void SignalSpoilLevelChange(material*);
  virtual void ResetSpoiling();
  virtual void SetItemsInside(const std::vector<contentscript<item> >&, ushort) { }
  virtual short GetCarryingBonus() const { return 0; }
  virtual bool IsBanana() const { return false; }
 protected:
  virtual item* RawDuplicate() const = 0;
  virtual void LoadDataBaseStats();
  virtual void VirtualConstructor(bool) { }
  void Initialize(ushort, ushort);
  virtual void InstallDataBase();
  virtual uchar GetGraphicsContainerIndex() const { return GR_ITEM; }
  virtual bool ShowMaterial() const;
  slot* Slot;
  bool Cannibalised;
  ushort Size;
  ulong ID;
  const database* DataBase;
  static prototype item_ProtoType;
  ulong Volume;
  ulong Weight;
};

#ifdef __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__
#define ITEM_PROTOTYPE(name, baseproto) itemprototype name::name##_ProtoType(baseproto, &name::Clone, #name);
#else
#define ITEM_PROTOTYPE(name, baseproto)
#endif

#define ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(ushort Config = 0, ushort SpecialFlags = 0) : base(donothing()) { Initialize(Config, SpecialFlags); }\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static item* Clone(ushort Config, ushort SpecialFlags) { return new name(Config, SpecialFlags); }\
 protected:\
  virtual item* RawDuplicate() const { return new name(*this); }\
  static itemprototype name##_ProtoType;\
  data\
}; ITEM_PROTOTYPE(name, &base##_ProtoType);

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(donothing D) : base(D) { }\
  virtual const prototype* GetProtoType() const { return &name##_ProtoType; }\
  static item* Clone(ushort, ushort) { return 0; }\
 protected:\
  static prototype name##_ProtoType;\
  data\
}; ITEM_PROTOTYPE(name, &base##_ProtoType);

#endif
