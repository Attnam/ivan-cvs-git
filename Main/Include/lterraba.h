#ifndef __LTERRABA_H__
#define __LTERRABA_H__

#pragma warning(disable : 4786)

#include "bitmap.h"
#include "object.h"
#include "terra.h"
#include "typedef.h"
#include "vector2d.h"

#include "proto.h"
#include "materde.h"
#include "game.h"
#include "igraph.h"

class bitmap;
class character;
class material;
class game;
class object;
class levelsquare;
class square;
class outputfile;
class inputfile;

/* Presentation of the levelterrain class & subclasses */

class levelterrain : public object
{
public:
	levelterrain(bool AddToPool) : object(AddToPool) {}
	virtual void Load(inputfile&);
	virtual bool Open(character* Opener);
	virtual bool Close(character* Closer);
	virtual vector2d GetPos() const;
	virtual bool CanBeOpened() const { return false; }
	virtual bool CanBeOffered() const { return false; }
	virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
	virtual bool CanBeDigged() const { return false; }
	virtual uchar OKVisualEffects() const { return 0; }
	virtual uchar GetVisualFlags() const { return VisualFlags; }
	virtual void SetVisualFlags(uchar What) { VisualFlags = What; }
	virtual void HandleVisualEffects();
	virtual void Save(outputfile&) const;
	virtual uchar GetGraphicsContainerIndex() const { return GLTERRAIN; }
	virtual ulong GetDefaultVolume(ushort Index) const { if(!Index) return 10000000; else return 0; }
	virtual void ReceiveVomit(character*) {}
	virtual bool CanBeOpenedByAI() { return false; }
	virtual bool ReceiveStrike() { return false; }
	virtual bool GetIsLocked() const { return false; }
	virtual bool Polymorph(character*) { return false; }
protected:
	uchar VisualFlags;
};

class groundlevelterrain : public levelterrain, public groundterrain
{
public:
	groundlevelterrain(bool = true, bool = true, bool AddToPool = true) : levelterrain(AddToPool) {}
	virtual void DrawToTileBuffer() const;
	virtual groundlevelterrain* Clone(bool = true, bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return levelterrain::Name(Case); }
};

class overlevelterrain : public levelterrain, public overterrain
{
public:
	overlevelterrain(bool = true, bool = true, bool AddToPool = true) : levelterrain(AddToPool) {}
	virtual void DrawToTileBuffer() const;
	virtual bool GoUp(character*) const;
	virtual bool GoDown(character*) const;
	virtual uchar GetOwnerGod() const { return 0; }
	virtual std::string DigMessage() const { return "The ground is too hard to dig."; }
	virtual overlevelterrain* Clone(bool = true, bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return levelterrain::Name(Case); }
	virtual void Kick(ushort, bool, uchar) {}
	virtual bool IsDoor() const { return false; }
	virtual void SitOn(character*);
	virtual bool HasConsumeEffect() const { return false; } 
	virtual std::string GetConsumeQuestion() const { return std::string("Do you want to consume ") + Name(DEFINITE) + std::string("?"); }
	virtual void Consume(character*) {}
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define LEVELTERRAIN_PROTOINSTALLER(name, base, protobase, initmaterials, setstats)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<protobase>::Add(new name(false, false, false))) {}\
		ushort GetIndex() const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	name::name(bool CreateMaterials, bool SetStats, bool AddToPool) : base(false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
	name::name(material* FirstMaterial, bool SetStats) : base(false, false) { initmaterials ; SetMaterial(0, FirstMaterial); if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
	void name::SetDefaultStats() { setstats }\
	ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
	const protobase* const name::GetPrototype() { return protocontainer<protobase>::GetProto(StaticType()); }\
	ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define LEVELTERRAIN_PROTOINSTALLER(name, base, protobase, initmaterials, setstats)

#endif

#define LEVELTERRAIN(name, base, protobase, initmaterials, setstats, data)\
\
name : public base\
{\
public:\
	name(bool = true, bool = true, bool = true);\
	name(material*, bool = true);\
	static ushort StaticType();\
	static const protobase* const GetPrototype();\
	virtual std::string ClassName() const { return #name; }\
protected:\
	virtual void SetDefaultStats();\
	virtual ushort Type() const;\
	data\
}; LEVELTERRAIN_PROTOINSTALLER(name, base, protobase, initmaterials, setstats)

#define GROUNDLEVELTERRAIN(name, base, initmaterials, setstats, data)\
\
LEVELTERRAIN(\
	name,\
	base,\
	groundlevelterrain,\
	initmaterials,\
	setstats,\
	virtual groundlevelterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { groundlevelterrain* G = new name(false, false); G->Load(SaveFile); return G; }\
	data\
);

#define OVERLEVELTERRAIN(name, base, initmaterials, setstats, data)\
\
LEVELTERRAIN(\
	name,\
	base,\
	overlevelterrain,\
	initmaterials,\
	setstats,\
	virtual overlevelterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { overlevelterrain* O = new name(false, false); O->Load(SaveFile); return O; }\
	data\
);

#endif
