#ifndef __ITEMBA_H__
#define __ITEMBA_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "vector2d.h"

#include "object.h"
#include "materba.h"
#include "igraph.h"
#include "proto.h"

class bitmap;
class character;
class material;
class object;
class stack;
class outputfile;
class inputfile;

/* Presentation of the item class */

class item : public object
{
public:
	item(bool, bool, bool = true);
	virtual float GetWeaponStrength() const;
	virtual void DrawToTileBuffer() const;
	virtual void PositionedDrawToTileBuffer(uchar) const;
	virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
	virtual ushort GetEmitation() const;
	virtual vector2d GetInHandsPic() const { return vector2d(0,0); }
	virtual item* TryToOpen(character*, stack*) { return 0; }
	virtual ulong GetWeight() const;
	virtual bool Consume(character*, float);
	virtual ushort GetArmorValue() const { return 100; }
	virtual bool IsHeadOfElpuri() const { return false; }
	virtual bool IsPetrussNut() const { return false; }
	virtual bool IsGoldenEagleShirt() const { return false; }
	virtual bool CanBeRead(character*) const { return false; }
	virtual bool Read(character*) { return false; }
	virtual void ReceiveHitEffect(character*, character*) {}
	virtual bool CanBeDippedInto(item*) const { return false; }
	virtual void DipInto(item*) {}
	virtual material* BeDippedInto() { return 0; }
	virtual bool CanBeDipped() const { return false; }
	virtual bool CanBeWorn() const { return false; }
	virtual bool Consumable(character*) const;
	virtual item* BetterVersion() const { return 0; }
	virtual bool ImpactDamage(ushort, bool, stack*) { return false; }
	virtual short CalculateOfferValue(char) const;
	virtual float OfferModifier() const { return 0; }
	virtual long Score() const { return 0; }
	virtual bool Destroyable() const { return true; }
	virtual bool Fly(uchar, ushort, stack*, bool = true);
	virtual bool HitCharacter(character*, float, character*);
	virtual bool DogWillCatchAndConsume() const { return false; }
	virtual uchar GetDipMaterialNumber() const { return GetMaterials() - 1; }
	virtual item* PrepareForConsuming(character*, stack*); // Stack where the item IS
	virtual item* Clone(bool = true, bool = true) const = 0;
	virtual ushort Possibility() const = 0;
	virtual bool CanBeWished() const { return true; }
	virtual item* CreateWishedItem() const;
	virtual bool Apply(character*, stack*);
	virtual bool Zap(character*, vector2d, uchar);
	virtual bool CanBeZapped() const { return false; }
	virtual bool Polymorph(stack*);
	virtual bool ReceiveSound(float, bool, stack*) { return false; }
	virtual uchar GetGraphicsContainerIndex() const { return GITEM; }
	virtual bool IsMaterialChangeable() const { return true; }
	virtual void ChangeMainMaterial(material*);
	virtual void CheckPickUpEffect(character*) {}
	virtual uchar GetWeaponCategory() const;
	virtual bool StruckByWandOfStriking(character*, stack*);
	virtual float GetThrowStrengthModifier() const { return 1; }
	virtual bool UseThrowStrengthModifier() const { return false; }
	virtual bool GetStepOnEffect(character*) { return false; }
	virtual ulong Price() const { return 0; }
	virtual bool IsTheAvatar() const RET(false)
	virtual void SignalSquarePositionChange(bool) {}
	virtual ulong ConsumeLimit() const { return GetMaterial(0)->GetVolume(); }
	virtual uchar GetConsumeType() const RET(GetMaterial(0)->GetConsumeType())
	virtual bool IsBadFoodForAI(character*) const;
	virtual uchar GetConsumeMaterial() const { return 0; }
	virtual std::string GetConsumeVerb() const { return std::string("eating"); }
	virtual bool PolymorphSpawnable() const { return true; }
	virtual bool IsExplosive() const { return false; }
	virtual bool ReceiveFireDamage(character*, stack*, long) { return false; }
	virtual bool CatWillCatchAndConsume() const { return false; }
protected:
	virtual void SetDefaultStats() = 0;
	virtual ushort GetFormModifier() const { return 0; }
	virtual float NPModifier() const { return 1.0f; }
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define ITEM_PROTOINSTALLER(name, base, initmaterials, setstats)\
	\
	static class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<item>::Add(new name(false, false, false))) {}\
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
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { item* Item = new name(false, false); Item->Load(SaveFile); return Item; }\
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
	name(bool CreateMaterials, bool SetStats, bool AddToPool = true) : base(CreateMaterials, SetStats, AddToPool) {}\
	data\
};

#endif



