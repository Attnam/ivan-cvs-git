#ifndef __ITEMBA_H__
#define __ITEMBA_H__

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
	virtual uchar GetConsumeType() const { return ODD; }
	virtual ushort TryToOpen(stack*) { return 0xFFFF; }
	virtual ulong GetWeight() const;
	virtual bool Consume(character*, float = 100) { return false; };
	virtual ushort GetArmorValue() const { return 100; }
	virtual bool IsHeadOfElpuri() const { return false; }
	virtual bool IsPerttusNut() const { return false; }
	virtual bool IsMaakotkaShirt() const { return false; }
	virtual bool CanBeRead(character*) const { return false; };
	virtual bool Read(character*) { return false; };
	virtual void ReceiveHitEffect(character*, character*) {}
	virtual bool CanBeDippedInto(item*) const { return false; }
	virtual void DipInto(item*) {}
	virtual material* BeDippedInto() { return 0; }
	virtual bool CanBeDipped() const { return false; }
	virtual bool CanBeWorn() const { return false; }
	virtual bool Consumable(character*) const;
	virtual item* BetterVersion() const { return 0; }
	virtual void ImpactDamage(ushort, bool, stack*) {};
	virtual short CalculateOfferValue(char) const;
	virtual float OfferModifier() const { return 0; }
	virtual long Score() const { return 0; }
	virtual bool Destroyable() const { return true; }
	virtual bool Fly(uchar, ushort, stack*, bool = true);
	virtual bool HitCharacter(character*, float);
	virtual bool DogWillCatchAndConsume() const { return false; }
	virtual uchar GetDipMaterialNumber() const { return GetMaterials() - 1; }
	virtual ushort PrepareForConsuming(character*, stack*); // Stack where the item IS
	virtual item* Clone(bool = true, bool = true) const = 0;
	virtual ushort Possibility() const = 0;
	virtual bool CanBeWished() const { return true; }
	virtual item* CreateWishedItem() const;
	virtual bool Apply(character*);
	virtual bool Zap(vector2d, uchar);
	virtual bool CanBeZapped() const { return false; }
	virtual bool Polymorph(stack*);
	virtual bool ReceiveSound(float, bool, stack*) { return false; }
	virtual uchar GetGraphicsContainerIndex() const { return GITEM; }
	virtual bool IsMaterialChangeable() const { return true; }
	virtual void ChangeMainMaterial(material*);
	virtual void CheckPickUpEffect(character*);
	virtual uchar GetWeaponCategory() const;
protected:
	virtual void SetDefaultStats() = 0;
	virtual ushort GetFormModifier() const { return 0; }
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define ITEM_PROTOINSTALLER(name, base, initmaterials, setstats)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<item>::Add(new name(false, false, false))) {}\
		ushort GetIndex() const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	name::name(bool CreateMaterials, bool SetStats, bool AddToPool) : base(false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); }\
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
	name(material* Material, bool SetStats = true) : base(false, false) { InitMaterials(Material); if(SetStats) SetDefaultStats(); }\
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
