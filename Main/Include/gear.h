#ifndef __GEAR_H__
#define __GEAR_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "item.h"

class ITEM
(
  meleeweapon,
  item,
 public:
  meleeweapon(const meleeweapon&);
  virtual ~meleeweapon();
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
  virtual void DipInto(material*, character*);
  virtual ulong GetPrice() const;
  virtual bool IsDippable(const character*) const { return !ContainedMaterial; }
  virtual material* GetSecondaryMaterial() const { return SecondaryMaterial; }
  virtual void SetSecondaryMaterial(material*, ushort = 0);
  virtual void ChangeSecondaryMaterial(material*, ushort = 0);
  virtual material* GetContainedMaterial() const { return ContainedMaterial; }
  virtual void SetContainedMaterial(material*, ushort = 0);
  virtual void ChangeContainedMaterial(material*, ushort = 0);
  void InitMaterials(material*, material*, material*, bool = true);
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual ushort GetMaterials() const { return 3; }
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual void SignalSpoil(material*);
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual void Be();
  virtual bool IsWeapon(const character*) const { return true; }
  virtual char GetEnchantment() const { return Enchantment; }
  virtual void SetEnchantment(char);
  virtual void EditEnchantment(char);
  virtual float GetWeaponStrength() const;
  virtual ushort GetStrengthValue() const;
  virtual ushort GetEffectBonus() const;
  virtual ushort GetAPBonus() const;
  virtual bool IsFixableBySmith(const character*) const { return IsBroken(); }
  virtual ushort GetBonus() const;
  virtual uchar GetSpoilLevel() const;
  virtual material* GetMaterial(ushort) const;
 protected:
  virtual void VirtualConstructor(bool);
  virtual bool IsSparkling(ushort) const;
  virtual void AddPostFix(std::string&) const;
  virtual void GenerateMaterials();
  virtual ushort GetMaterialColorB(ushort) const;
  virtual uchar GetAlphaB(ushort) const;
  material* SecondaryMaterial;
  material* ContainedMaterial;
  char Enchantment;
);

class ITEM
(
  justifier,
  meleeweapon,
 protected:
  virtual void Be() { }
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class ITEM
(
  neercseulb,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
 protected:
  virtual void Be() { }
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class ITEM
(
  pickaxe,
  meleeweapon,
 public:
  virtual bool Apply(character*);
  virtual bool IsAppliable(const character*) const;
);

class ITEM
(
  whip,
  meleeweapon,
 public:
  virtual bool IsWhip() const { return true; }
 protected:
  virtual ushort GetFormModifier() const;
);

class ITEM
(
  flamingsword,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
  virtual uchar GetSpecialFlags() const;
);

class ITEM
(
  mjolak,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
);

class ITEM
(
  vermis,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
);

class ITEM
(
  turox,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
);

class ITEM
(
  whipofthievery,
  whip,
 public:
  virtual ulong GetPrice() const;
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
 protected:
  virtual bool CleptiaHelps(const character*, const character*) const;
);

class ITEM
(
  gorovitshammer,
  meleeweapon,
 public:
  virtual bool IsGorovitsFamilyRelic() const { return true; }
 protected:
  virtual void Be() { }
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class ITEM
(
  gorovitssickle,
  meleeweapon,
 public:
  virtual bool IsGorovitsFamilyRelic() const { return true; }
 protected:
  virtual void Be() { }
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class ITEM
(
  thunderhammer,
  meleeweapon,
 public:
  virtual bool HitEffect(character*, character*, uchar, uchar, bool);
  virtual uchar GetSpecialFlags() const;
  virtual bool ReceiveDamage(character*, ushort, uchar);
);

class ITEM
(
  saalthul,
  meleeweapon,
  ;
);

class ABSTRACT_ITEM
(
  armor,
  item,
 public:
  virtual ulong GetPrice() const;
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual bool IsArmor(const character*) const { return true; }
  virtual char GetEnchantment() const { return Enchantment; }
  virtual void SetEnchantment(char);
  virtual void EditEnchantment(char);
  virtual ushort GetStrengthValue() const;
  virtual bool CanBePiledWith(const item*, const character*) const;
  virtual ushort GetInElasticityPenalty(ushort) const;
  virtual short GetCarryingBonus() const;
  virtual bool IsFixableBySmith(const character*) const { return IsBroken(); }
 protected:
  virtual void AddPostFix(std::string&) const;
  virtual void VirtualConstructor(bool);
  char Enchantment;
);

class ITEM
(
  bodyarmor,
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsBodyArmor(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
 protected:
  virtual const std::string& GetNameSingular() const;
);

class ITEM
(
  goldeneagleshirt,
  bodyarmor,
 public:
  virtual void Be() { }
  virtual bool IsGoldenEagleShirt() const { return true; }
  virtual bool IsConsumable(const character*) const { return false; }
 protected:
  virtual ushort GetClassAnimationFrames() const { return 32; }
  virtual ushort GetOutlineColor(ushort) const;
  virtual bool HasSpecialAnimation() const { return true; }
);

class ITEM
(
  shield,
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsShield(const character*) const { return true; }
  virtual void AddInventoryEntry(const character*, std::string&, ushort, bool) const;
  virtual ushort GetBonus() const;
);

class ITEM
(
  cloak,
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsCloak(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
);

class ITEM
(
  boot,
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsBoot(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
);

class ITEM
(
  gauntlet, 
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsGauntlet(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
);

class ITEM
(
  belt, 
  armor,
 public:
  virtual ulong GetPrice() const;
  virtual bool IsBelt(const character*) const { return true; }
  virtual ushort GetFormModifier() const; 
  virtual bool IsInCorrectSlot(ushort) const;
);

class ITEM
(
  ring, 
  item,
 public:
  virtual bool IsRing(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
);

class ITEM
(
  amulet, 
  item,
 public:
  virtual bool IsAmulet(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
 protected:
  virtual ushort GetMaterialColorB(ushort) const;
);

class ITEM
(
  helmet,
  armor,
 public:
  virtual bool IsGorovitsFamilyRelic() const;
  virtual ulong GetPrice() const;
  virtual bool IsHelmet(const character*) const { return true; }
  virtual bool IsInCorrectSlot(ushort) const;
 protected:
  virtual uchar GetAlphaB(ushort) const;
  virtual ushort GetMaterialColorB(ushort) const;
  virtual ushort GetMaterialColorC(ushort) const;
);

class ITEM 
( 
  chameleonwhip, 
  whip, 
 public: 
  virtual bool HitEffect(character*, character*, uchar, uchar, bool); 
 protected: 
  virtual bool ScabiesHelps(const character*, const character*) const; 
); 

#endif
