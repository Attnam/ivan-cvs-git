#ifndef __ITEM_H__
#define __ITEM_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cmath>

#include "object.h"
#include "lsquare.h"
#include "slot.h"

class felist;
class head;
class itemprototype;
template <class type> class contentscript;
template <class type> class databasecreator;

typedef std::vector<item*> itemvector;
typedef std::vector<fluid*> fluidvector;
typedef bool (rawbitmap::*pixelpredicate)(vector2d) const;
typedef bool (material::*materialpredicate)() const;
typedef bool (item::*sorter)(const character*) const;

extern materialpredicate TrueMaterialPredicate;

struct itemdatabase : public databasebase
{
  typedef itemprototype prototype;
  void InitDefaults(const prototype*, int);
  bool AllowRandomInstantiation() const;
  void PostProcess() { }
  const prototype* ProtoType;
  /* Maintained by configcontainer */
  long PartialPossibilitySum;
  long PartialCategoryPossibilitySum;
  int Possibility;
  bool IsDestroyable;
  bool CanBeWished;
  bool IsMaterialChangeable;
  int WeaponCategory;
  bool IsPolymorphSpawnable;
  bool IsAutoInitializable;
  long Category;
  int FireResistance;
  int PoisonResistance;
  int ElectricityResistance;
  int AcidResistance;
  int StrengthModifier;
  int FormModifier;
  int DefaultSize;
  long DefaultMainVolume;
  long DefaultSecondaryVolume;
  vector2d BitmapPos;
  long Price;
  color24 BaseEmitation;
  festring Article;
  festring Adjective;
  festring AdjectiveArticle;
  festring NameSingular;
  festring NamePlural;
  festring PostFix;
  int ArticleMode;
  fearray<long> MainMaterialConfig;
  fearray<long> SecondaryMaterialConfig;
  fearray<long> MaterialConfigChances;
  long MaterialConfigChanceSum;
  bool IsAbstract;
  bool IsPolymorphable;
  fearray<festring> Alias;
  int OKVisualEffects;
  bool CanBeGeneratedInContainer;
  int ForcedVisualEffects;
  int Roundness;
  long GearStates;
  bool IsTwoHanded;
  bool CreateDivineConfigurations;
  bool CanBeCloned;
  bool CanBeMirrored;
  int BeamRange;
  bool CanBeBroken;
  vector2d WallBitmapPos;
  festring FlexibleNameSingular;
  int MinCharges;
  int MaxCharges;
  bool CanBePiled;
  long StorageVolume;
  int MaxGeneratedContainedItems;
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
  int DefaultEnchantment;
  bool PriceIsProportionalToEnchantment;
  int InElasticityPenaltyModifier;
  bool CanBeUsedBySmith;
  bool AffectsCarryingCapacity;
  int DamageDivider;
  bool HandleInPairs;
  bool CanBeEnchanted;
  color16 BeamColor;
  int BeamEffect;
  int BeamStyle;
  int WearWisdomLimit;
  int AttachedGod;
  int BreakEffectRangeSquare;
  vector2d WieldedBitmapPos;
  bool IsQuestItem;
  bool IsGoodWithPlants;
  bool CreateLockConfigurations;
  bool CanBePickedUp;
  int CoverPercentile;
  vector2d TorsoArmorBitmapPos;
  vector2d ArmArmorBitmapPos;
  vector2d LegArmorBitmapPos;
  vector2d HelmetBitmapPos;
  vector2d CloakBitmapPos;
  vector2d BeltBitmapPos;
  vector2d GauntletBitmapPos;
  vector2d BootBitmapPos;
  bool HasSecondaryMaterial;
  bool AllowEquip;
  int ReadDifficulty;
  bool IsValuable;
};

class itemprototype
{
 public:
  friend class databasecreator<item>;
  itemprototype(itemprototype*, item* (*)(int, int), const char*);
  item* Clone(int Config = 0, int SpecialFlags = 0) const { return Cloner(Config, SpecialFlags); }
  item* CloneAndLoad(inputfile&) const;
  const char* GetClassID() const { return ClassID; }
  int GetIndex() const { return Index; }
  const itemprototype* GetBase() const { return Base; }
  int CreateSpecialConfigurations(itemdatabase**, int);
  const itemdatabase* ChooseBaseForConfig(itemdatabase**, int, int);
  const itemdatabase*const* GetConfigData() const { return ConfigData; }
  int GetConfigSize() const { return ConfigSize; }
 private:
  int Index;
  itemprototype* Base;
  itemdatabase** ConfigData;
  itemdatabase** ConfigTable[CONFIG_TABLE_SIZE];
  int ConfigSize;
  item* (*Cloner)(int, int);
  const char* ClassID;
};

class item : public object
{
 public:
  friend class databasecreator<item>;
  typedef itemprototype prototype;
  typedef itemdatabase database;
  item(donothing);
  item(const item&);
  virtual ~item();
  virtual double GetWeaponStrength() const;
  virtual bool Open(character*);
  bool Consume(character*, long);
  virtual bool IsHeadOfElpuri() const { return false; }
  virtual bool IsPetrussNut() const { return false; }
  virtual bool IsGoldenEagleShirt() const { return false; }
  virtual bool CanBeRead(character*) const { return false; }
  virtual bool Read(character*);
  virtual void FinishReading(character*) { }
  virtual bool HitEffect(character*, character*, vector2d, int, int, bool) { return false; }
  virtual void DipInto(liquid*, character*) { }
  virtual liquid* CreateDipLiquid() { return 0; }
  virtual item* BetterVersion() const { return 0; }
  virtual int GetOfferValue(int) const;
  virtual void Fly(character*, int, int);
  int HitCharacter(character*, character*, int, double, int);
  virtual bool DogWillCatchAndConsume(const character*) const { return false; }
  virtual bool Apply(character*);
  virtual bool Zap(character*, vector2d, int) { return false; }
  virtual bool Polymorph(character*, stack*);
  virtual bool CheckPickUpEffect(character*) { return true; }
  virtual void StepOnEffect(character*) { }
  virtual bool IsTheAvatar() const { return false; }
  virtual void SignalSquarePositionChange(int);
  virtual bool CanBeEatenByAI(const character*) const;
  virtual bool IsExplosive() const { return false; }
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void ChargeFully(character*) { }
  void SetSize(int Value) { Size = Value; }
  virtual int GetSize() const { return Size; }
  ulong GetID() const { return ID; }
  virtual void TeleportRandomly();
  virtual int GetStrengthValue() const;
  slot* GetSlot(int I = 0) const { return Slot[I]; }
  void SetMainSlot(slot* What) { Slot[0] = What; }
  void PlaceToSlot(slot* Slot) { Slot->PutInItem(this); }
  void RemoveFromSlot();
  void MoveTo(stack*);
  bool IsMainSlot(const slot* What) const { return Slot[0] == What; }
  static const char* GetItemCategoryName(long);
  virtual bool IsConsumable(const character*) const;
  bool IsEatable(const character*) const;
  bool IsDrinkable(const character*) const;
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
  virtual bool IsEnchantable(const character*) const { return CanBeEnchanted(); }
  virtual bool IsRepairable(const character*) const { return IsBroken() || IsRusted(); }
  virtual bool IsDecosAdShirt(const character*) const { return false; }
  bool CanBeHardened(const character*) const;
  virtual bool HasLock(const character*) const { return false; }
  virtual bool IsOnGround() const;
  int GetResistance(int) const;
  virtual void Be();
  int GetType() const { return GetProtoType()->GetIndex(); }
  virtual bool ReceiveDamage(character*, int, int, int = YOURSELF);
  virtual bool RaiseTheDead(character*) { return false; }
  virtual int GetBodyPartIndex() const { return 0xFF; }
  virtual const prototype* GetProtoType() const;
  const database* GetDataBase() const { return DataBase; }
  virtual bool CanOpenLockType(int) const { return false; }
  virtual bool IsWhip() const { return false; }
  DATA_BASE_VALUE(int, Config);
  virtual DATA_BASE_BOOL_WITH_PARAMETER(IsDestroyable, const character*);
  DATA_BASE_BOOL(IsMaterialChangeable);
  DATA_BASE_VALUE(int, WeaponCategory);
  DATA_BASE_BOOL(IsAutoInitializable);
  DATA_BASE_VALUE(long, Category);
  DATA_BASE_VALUE(int, FireResistance);
  DATA_BASE_VALUE(int, PoisonResistance);
  DATA_BASE_VALUE(int, ElectricityResistance);
  DATA_BASE_VALUE(int, AcidResistance);
  DATA_BASE_VALUE(int, StrengthModifier);
  virtual DATA_BASE_VALUE(int, FormModifier);
  DATA_BASE_VALUE(int, DefaultSize);
  DATA_BASE_VALUE(long, DefaultMainVolume);
  DATA_BASE_VALUE(long, DefaultSecondaryVolume);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(vector2d, BitmapPos, int);
  virtual DATA_BASE_VALUE(long, Price);
  virtual DATA_BASE_VALUE(color24, BaseEmitation);
  virtual DATA_BASE_VALUE(const festring&, Article);
  virtual DATA_BASE_VALUE(const festring&, Adjective);
  virtual DATA_BASE_VALUE(const festring&, AdjectiveArticle);
  virtual DATA_BASE_VALUE(const festring&, NameSingular);
  virtual DATA_BASE_VALUE(const festring&, NamePlural);
  virtual DATA_BASE_VALUE(const festring&, PostFix);
  virtual DATA_BASE_VALUE(int, ArticleMode);
  DATA_BASE_VALUE(const fearray<long>&, MainMaterialConfig);
  DATA_BASE_VALUE(const fearray<long>&, SecondaryMaterialConfig);
  virtual DATA_BASE_VALUE(const fearray<long>&, MaterialConfigChances);
  virtual DATA_BASE_VALUE(long, MaterialConfigChanceSum);
  DATA_BASE_BOOL(IsPolymorphable);
  virtual DATA_BASE_VALUE(int, OKVisualEffects);
  DATA_BASE_BOOL(CanBeGeneratedInContainer);
  virtual DATA_BASE_VALUE(int, ForcedVisualEffects);
  DATA_BASE_VALUE(int, Roundness);
  DATA_BASE_VALUE(long, GearStates);
  DATA_BASE_BOOL(IsTwoHanded);
  DATA_BASE_BOOL(CanBeBroken);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, WallBitmapPos, int);
  DATA_BASE_VALUE(const festring&, FlexibleNameSingular);
  DATA_BASE_BOOL(CanBePiled);
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
  DATA_BASE_VALUE(int, DefaultEnchantment);
  DATA_BASE_BOOL(PriceIsProportionalToEnchantment);
  DATA_BASE_VALUE(int, MaxCharges);
  DATA_BASE_VALUE(int, MinCharges);
  DATA_BASE_VALUE(int, InElasticityPenaltyModifier);
  DATA_BASE_VALUE(long, StorageVolume);
  DATA_BASE_VALUE(int, MaxGeneratedContainedItems);
  virtual DATA_BASE_BOOL(CanBeCloned);
  virtual DATA_BASE_BOOL(CanBeMirrored);
  DATA_BASE_VALUE(int, BeamRange);
  DATA_BASE_BOOL(CanBeUsedBySmith);
  DATA_BASE_VALUE(int, DamageDivider);
  DATA_BASE_BOOL(HandleInPairs);
  DATA_BASE_BOOL(CanBeEnchanted);
  DATA_BASE_VALUE(long, BeamColor);
  DATA_BASE_VALUE(int, BeamEffect);
  DATA_BASE_VALUE(int, BeamStyle);
  DATA_BASE_VALUE(int, WearWisdomLimit);
  DATA_BASE_VALUE(int, BreakEffectRangeSquare);
  virtual DATA_BASE_VALUE_WITH_PARAMETER(vector2d, WieldedBitmapPos, int);
  DATA_BASE_BOOL(IsQuestItem);
  DATA_BASE_BOOL(IsGoodWithPlants);
  DATA_BASE_BOOL(CanBePickedUp);
  DATA_BASE_VALUE(int, CoverPercentile);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, TorsoArmorBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, ArmArmorBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, LegArmorBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, HelmetBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, CloakBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, BeltBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, GauntletBitmapPos, int);
  DATA_BASE_VALUE_WITH_PARAMETER(vector2d, BootBitmapPos, int);
  DATA_BASE_BOOL(AllowEquip);
  DATA_BASE_VALUE(int, ReadDifficulty);
  bool CanBeSoldInLibrary(character* Librarian) const { return CanBeRead(Librarian); }
  virtual bool TryKey(item*, character*) { return false; }
  //virtual bool TryToUnstuck(character*, vector2d) { return true; }
  virtual bool TryToUnstuck(character*, int, vector2d) { return false; }
  long GetBlockModifier() const;
  bool IsSimiliarTo(item*) const;
  virtual bool IsPickable(character*) const { return true; }
  bool CanBeSeenByPlayer() const;
  virtual bool CanBeSeenBy(const character*) const;
  festring GetDescription(int) const;
  virtual square* GetSquareUnderEntity(int = 0) const;
  virtual square* GetSquareUnder(int = 0) const;
  virtual lsquare* GetLSquareUnder(int = 0) const;
  level* GetLevel() const { return static_cast<level*>(Slot[0]->GetSquareUnder()->GetArea()); }
  area* GetArea() const { return Slot[0]->GetSquareUnder()->GetArea(); }
  vector2d GetPos(int I = 0) const { return Slot[I]->GetSquareUnder()->GetPos(); }
  square* GetNearSquare(vector2d Pos) const { return Slot[0]->GetSquareUnder()->GetArea()->GetSquare(Pos); }
  square* GetNearSquare(int x, int y) const { return Slot[0]->GetSquareUnder()->GetArea()->GetSquare(x, y); }
  lsquare* GetNearLSquare(vector2d Pos) const { return static_cast<lsquare*>(Slot[0]->GetSquareUnder()->GetArea()->GetSquare(Pos)); }
  lsquare* GetNearLSquare(int x, int y) const { return static_cast<lsquare*>(Slot[0]->GetSquareUnder()->GetArea()->GetSquare(x, y)); }
  virtual void SignalVolumeAndWeightChange();
  virtual void CalculateVolumeAndWeight();
  long GetVolume() const { return Volume; }
  long GetWeight() const { return Weight; }
  virtual void SignalEmitationIncrease(color24);
  virtual void SignalEmitationDecrease(color24);
  void CalculateAll();
  virtual void DropEquipment() { }
  virtual bool IsDangerousForAI(const character*) const { return false; } 
  virtual bool IsDangerous() const { return false; } 
  void WeaponSkillHit(int);
  virtual void SetTeam(int) { }
  void SpecialGenerationHandler();
  item* Duplicate(ulong = 0);
  virtual void SetIsActive(bool) { }
  int GetBaseMinDamage() const;
  int GetBaseMaxDamage() const;
  int GetBaseToHitValue() const;
  int GetBaseBlockValue() const;
  virtual void AddInventoryEntry(const character*, festring&, int, bool) const;
  long GetNutritionValue() const;
  virtual void SignalSpoil(material*);
  virtual bool AllowSpoil() const;
  item* DuplicateToStack(stack*, ulong = 0);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual long GetTotalExplosivePower() const { return 0; }
  virtual void Break(character*, int = YOURSELF);
  virtual void SetEnchantment(int) { }
  virtual void EditEnchantment(int) { }
  virtual void SignalEnchantmentChange();
  //virtual int GetBonus() const { return 100; }
  virtual double GetTHVBonus() const { return 0.; }
  virtual double GetDamageBonus() const { return 0.; }
  virtual void DrawContents(const character*) { }
  virtual bool IsBroken() const;
  virtual int GetEnchantment() const { return 0; }
  long GetEnchantedPrice(int) const;
  virtual item* Fix();
  int GetStrengthRequirement() const;
  virtual int GetInElasticityPenalty(int) const { return 0; }
  virtual bool IsFixableBySmith(const character*) const { return false; }
  virtual long GetFixPrice() const { return 100; } 
  void DonateSlotTo(item*);
  virtual int GetSpoilLevel() const;
  virtual void SignalSpoilLevelChange(material*);
  void ResetSpoiling();
  virtual void SetItemsInside(const fearray<contentscript<item> >&, int) { }
  virtual int GetCarryingBonus() const { return 0; }
  virtual bool IsBanana() const { return false; }
  virtual bool IsEncryptedScroll() const { return false; }
  const char* GetStrengthValueDescription() const;
  const char* GetBaseToHitValueDescription() const;
  const char* GetBaseBlockValueDescription() const;
  virtual bool IsInCorrectSlot(int) const;
  bool IsInCorrectSlot() const;
  int GetEquipmentIndex() const;
  room* GetRoom(int I = 0) const { return GetLSquareUnder(I)->GetRoom(); }
  virtual bool HasBetterVersion() const { return false; }
  virtual void SortAllItems(itemvector&, const character*, sorter) const;
  virtual bool AllowAlphaEverywhere() const { return false; }
  virtual int GetAttachedGod() const;
  virtual long GetTruePrice() const;
  virtual void Search(const character*, int) { }
  bool IsSparkling() const;
  virtual head* Behead() { return 0; }
  virtual bool IsGorovitsFamilyRelic() const { return false; }
  virtual bool EffectIsGood() const { return false; }
#ifdef WIZARD
  virtual void AddAttackInfo(felist&) const;
  void AddMiscellaneousInfo(felist&) const;
#endif
  virtual void PreProcessForBone();
  virtual void PostProcessForBone();
  virtual void FinalProcessForBone() { }
  virtual bool SuckSoul(character*, character* = 0) { return false; }
  void SetConfig(int, int = 0);
  god* GetMasterGod() const;
  const std::vector<ulong>& GetCloneMotherID() const { return CloneMotherID; }
  virtual void SignalStackAdd(stackslot*, void (stack::*)(item*));
  virtual int GetSquareIndex(vector2d) const { return 0; }
  virtual void Draw(bitmap*, vector2d, color24, int, bool, bool) const;
  vector2d GetLargeBitmapPos(vector2d, int) const;
  void LargeDraw(bitmap*, vector2d, color24, int, bool, bool) const;
  virtual bool BunnyWillCatchAndConsume(const character*) const { return false; }
  void DonateIDTo(item*);
  virtual void SignalRustLevelChange();
  void SendNewDrawAndMemorizedUpdateRequest() const;
  virtual void CalculateSquaresUnder() { SquaresUnder = 1; }
  int GetSquaresUnder() const { return SquaresUnder; }
  virtual void CalculateEmitation();
  void FillFluidVector(fluidvector&, int = 0) const;
  virtual void SpillFluid(character*, liquid*, int = 0);
  virtual void TryToRust(long);
  void RemoveFluid(fluid*);
  void AddFluid(liquid*, festring, int, bool);
  virtual bool IsAnimated() const;
  const rawbitmap* GetRawPicture() const;
  void DrawFluidGearPictures(bitmap*, vector2d, color24, int, bool) const;
  void DrawFluidBodyArmorPictures(bitmap*, vector2d, color24, int, bool) const;
  void CheckFluidGearPictures(vector2d, int, bool);
  void DrawFluids(bitmap*, vector2d, color24, int, bool) const;
  virtual void ReceiveAcid(material*, const festring&, long);
  virtual bool ShowFluids() const { return true; }
  void DonateFluidsTo(item*);
  void Destroy(character*, int);
  virtual bool AllowFluidBe() const { return true; }
  virtual bool IsRusted() const;
  void RemoveRust();
  virtual bool IsBananaPeel() const { return false; }
  void SetSpoilPercentage(int);
  virtual pixelpredicate GetFluidPixelAllowedPredicate() const;
  void RedistributeFluids();
  virtual material* GetConsumeMaterial(const character*, materialpredicate = TrueMaterialPredicate) const;
  virtual material* RemoveMaterial(material*);
  virtual void Cannibalize();
  void InitMaterials(material*, bool = true);
  void SetMainMaterial(material*, int = 0);
  void ChangeMainMaterial(material*, int = 0);
  virtual void GenerateMaterials();
  virtual void InitMaterials(const materialscript*, const materialscript*, bool);
  int GetSquarePosition() const { return (Flags & SQUARE_POSITION_BITS) >> SQUARE_POSITION_SHIFT; }
  virtual bool IsLanternOnWall() const { return false; }
  virtual void DestroyBodyPart(stack*) { SendToHell(); }
  virtual void SetLifeExpectancy(int, int);
  int NeedsBe() const { return !!LifeExpectancy; }
  bool IsVeryCloseToDisappearance() const { return LifeExpectancy && LifeExpectancy < 10; }
  bool IsVeryCloseToSpoiling() const;
  virtual bool IsValuable() const;
 protected:
  virtual bool AllowFluids() const { return false; }
  virtual const char* GetBreakVerb() const;
  virtual long GetMaterialPrice() const;
  virtual item* RawDuplicate() const = 0;
  void LoadDataBaseStats();
  virtual void VirtualConstructor(bool) { }
  void Initialize(int, int);
  virtual int GetGraphicsContainerIndex() const;
  virtual bool ShowMaterial() const;
  virtual bool AllowSparkling() const { return !Fluid; }
  virtual bool WeightIsIrrelevant() const { return false; }
  slot** Slot;
  int Size;
  ulong ID;
  const database* DataBase;
  long Volume;
  long Weight;
  std::vector<ulong> CloneMotherID;
  fluid** Fluid;
  int SquaresUnder;
  int LifeExpectancy;
};

#ifdef __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__
#define ITEM_PROTOTYPE(name, base, baseproto)\
item* name##_Clone(int Config, int SpecialFlags) { return new name(Config, SpecialFlags); }\
itemprototype name##_ProtoType(baseproto, &name##_Clone, #name);\
name::name(int Config, int SpecialFlags) : base(donothing()) { Initialize(Config, SpecialFlags); }\
name::name(donothing D) : base(D) { }\
const itemprototype* name::GetProtoType() const { return &name##_ProtoType; }\
item* name::RawDuplicate() const { return new name(*this); }
#define ABSTRACT_ITEM_PROTOTYPE(name, base, baseproto)\
itemprototype name##_ProtoType(baseproto, 0, #name);\
name::name(donothing D) : base(D) { }\
const itemprototype* name::GetProtoType() const { return &name##_ProtoType; }
#else
#define ITEM_PROTOTYPE(name, base, baseproto)
#define ABSTRACT_ITEM_PROTOTYPE(name, base, baseproto)
#endif

#define ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(int = 0, int = 0);\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
 protected:\
  virtual item* RawDuplicate() const;\
}; ITEM_PROTOTYPE(name, base, &base##_ProtoType);

#define ABSTRACT_ITEM(name, base, data)\
\
name : public base\
{\
 public:\
  name(donothing);\
  virtual const prototype* GetProtoType() const;\
  data\
}; ABSTRACT_ITEM_PROTOTYPE(name, base, &base##_ProtoType);

#endif
