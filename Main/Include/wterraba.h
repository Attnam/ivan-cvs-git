#ifndef __WTERRABA_H__
#define __WTERRABA_H__

#include "vector2d.h"
#include "terra.h"
#include "proto.h"

class worldmapsquare;
class worldmap;

class worldmapterrain : public typeable
{
public:
	virtual vector2d GetPos() const;
	virtual worldmapsquare* GetWorldMapSquareUnder() const;
	virtual worldmap* GetWorldMapUnder() const;
	virtual std::string Name(uchar = 0) const;
protected:
	virtual std::string NameStem() const = 0;
	virtual std::string Article() const { return "a"; }
	virtual vector2d GetBitmapPos() const = 0;
};

class groundworldmapterrain : public worldmapterrain, public groundterrain
{
public:
	groundworldmapterrain(bool = true) {}
	virtual void DrawToTileBuffer() const;
	virtual groundworldmapterrain* Clone(bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return worldmapterrain::Name(Case); }
	virtual uchar Priority() const = 0;
};

class overworldmapterrain : public worldmapterrain, public overterrain
{
public:
	overworldmapterrain(bool = true) {}
	virtual void DrawToTileBuffer() const;
	virtual overworldmapterrain* Clone(bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return worldmapterrain::Name(Case); }
	virtual bool GoUp(character*) const;
	virtual bool GoDown(character*) const;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define WORLDMAPTERRAIN_PROTOINSTALLER(name, base, protobase, setstats)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<protobase>::Add(new name(false))) {}\
		ushort GetIndex() const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	void name::SetDefaultStats() { setstats }\
	ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
	const protobase* const name::GetPrototype() { return protocontainer<protobase>::GetProto(StaticType()); }\
	ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define WORLDMAPTERRAIN_PROTOINSTALLER(name, base, protobase, setstats)

#endif

#define WORLDMAPTERRAIN(name, base, protobase, setstats, data)\
\
name : public base\
{\
public:\
	name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
	static ushort StaticType();\
	static const protobase* const GetPrototype();\
	virtual std::string ClassName() const { return #name; }\
protected:\
	virtual void SetDefaultStats();\
	virtual ushort Type() const;\
	data\
}; WORLDMAPTERRAIN_PROTOINSTALLER(name, base, protobase, setstats)

#define GROUNDWORLDMAPTERRAIN(name, base, setstats, data)\
\
WORLDMAPTERRAIN(\
	name,\
	base,\
	groundworldmapterrain,\
	setstats,\
	virtual groundworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { groundworldmapterrain* G = new name(false); G->Load(SaveFile); return G; }\
	data\
);

#define OVERWORLDMAPTERRAIN(name, base, setstats, data)\
\
WORLDMAPTERRAIN(\
	name,\
	base,\
	overworldmapterrain,\
	setstats,\
	virtual overworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { overworldmapterrain* O = new name(false); O->Load(SaveFile); return O; }\
	data\
);

#endif
