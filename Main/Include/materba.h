#ifndef __MATERBA_H__
#define __MATERBA_H__

#define GOOD 0
#define NEUTRAL 1
#define EVIL 2

/* ConsumeTypes */

#define ODD 		0
#define FRUIT 		1
#define MEAT 		2
#define SPOILED 	3
#define HARD 		4
#define SCHOOLFOOD 	5
#define CONTAINER	6
#define LIQUID		7
#define BONE		8

#define EFOOD			1
#define ESCHOOLFOOD		2
#define EOMLEURINE		3
#define EDARKNESS		4
#define EFIRE			5
#define EPEPSI			6

#include <string>

#include "typedef.h"

#include "typeable.h"
#include "proto.h"

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#include "igraph.h"

#endif

/* Presentation of material class */

class character;
class item;
class object;
class outputfile;
class inputfile;

class material : public typeable
{
public:
	material() : Volume(0), MotherObject(0) {}
	virtual ~material() {}
	virtual std::string Name(uchar Case = 0) const;
	virtual ushort GetHitValue() const = 0;
	virtual uchar GetConsumeType() const			{ return ODD; }
	virtual ulong GetVolume() const			{ return Volume; }
	virtual ulong GetWeight() const			{ return ulong(float(Volume) * GetDensity() / 1000); }
	virtual ushort GetDensity() const = 0;
	virtual ushort TakeDipVolumeAway();
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual ushort GetArmorValue() const { return GetHitValue(); }
	virtual void SetVolume(ulong What) { Volume = What; }
	virtual ushort GetEmitation() const { return 0; }
	virtual ushort OfferValue() const = 0;
	virtual uchar Alignment() const				{ return NEUTRAL; }
	virtual void EatEffect(character*, float, float = 1.0);
	virtual void HitEffect(character*)			{ }
	virtual short NutritionValue() const			{ return 0; }
	virtual void MinusAmount(float Amount)			{ SetVolume(GetVolume() > Amount ? GetVolume() - Amount : 0); }
	virtual material* Clone(ulong Volume) const = 0;
	virtual material* Clone() const = 0;
	virtual bool IsType(ushort QType) const { return Type() == QType; }
	virtual bool IsSolid() const { return false; }
	virtual void Be() {}
	virtual ushort GetColor() const = 0;
	virtual bool CanBeWished() const { return true; }
	virtual material* CreateWishedMaterial(ulong) const;
	virtual bool IsFlesh() { return false; }
	virtual object* GetMotherObject() const { return MotherObject; }
	virtual void SetMotherObject(object* What) { MotherObject = What; }
	virtual ulong RawPrice() const { return 0; }
	virtual bool GetIsBadFoodForAI() const { return false; }
	virtual bool CanBeDigged() const { return true; }
protected:
	virtual std::string NameStem() const = 0;
	virtual std::string Article() const { return "a"; }
	virtual void NormalFoodEffect(character*, float, float);
	ulong Volume;
	object* MotherObject;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define MATERIAL_PROTOINSTALLER(name, base)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<material>::Add(new name)) { }\
		ushort GetIndex() const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
	const material* const name::GetPrototype() { return protocontainer<material>::GetProto(StaticType()); }\
	ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define MATERIAL_PROTOINSTALLER(name, base)

#endif

#define MATERIAL(name, base, data)\
\
name : public base\
{\
public:\
	name(ulong IVolume) { Volume = IVolume; }\
	name() {}\
	virtual material* Clone(ulong Volume) const { return new name(Volume); }\
	virtual material* Clone() const { return new name; }\
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { material* Mat = new name; Mat->Load(SaveFile); return Mat; }\
	static ushort StaticType();\
	static const material* const GetPrototype();\
	virtual std::string ClassName() const { return #name; }\
protected:\
	virtual ushort Type() const;\
	data\
}; MATERIAL_PROTOINSTALLER(name, base)

#endif
