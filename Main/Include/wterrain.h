#ifndef __WTERRAIN_H__
#define __WTERRAIN_H__

#include <fstream>

#include "vector.h"
#include "terrain.h"
#include "proto.h"

class worldmapsquare;

class worldmapterrain : virtual public typeable, virtual public drawable
{
public:
	friend class worldmap;
	virtual vector GetPos(void) const;
	virtual worldmapsquare* GetWorldMapSquareUnder(void) const;
	virtual std::string Name(uchar = 0) const;
protected:
	virtual std::string NameStem(void) const = 0;
	virtual std::string Article(void) const { return "a"; }
};

class groundworldmapterrain : public worldmapterrain, public groundterrain
{
public:
	groundworldmapterrain(bool = true) {}
	virtual void Save(std::ofstream& SaveFile) const { typeable::Save(SaveFile); }
	virtual void Load(std::ifstream& SaveFile) { typeable::Load(SaveFile); }
	virtual void DrawToTileBuffer(void) const;
	virtual groundworldmapterrain* Clone(bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return worldmapterrain::Name(Case); }
	static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }
protected:
	static ushort ProtoIndexBegin, ProtoIndexEnd;
};

class overworldmapterrain : public worldmapterrain, public overterrain
{
public:
	overworldmapterrain(bool = true) {}
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual void DrawToTileBuffer(void) const;
	virtual overworldmapterrain* Clone(bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return worldmapterrain::Name(Case); }
	static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }
protected:
	static ushort ProtoIndexBegin, ProtoIndexEnd;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define WORLDMAPTERRAIN(name, base, protobase, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
		static ushort StaticType(void);\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller(void) : Index(prototypesystem::Add(new name(false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	void name::SetDefaultStats(void) { setstats }\
	ushort name::StaticType(void) { return name##_ProtoInstaller.GetIndex(); }\
	ushort name::Type(void) const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define WORLDMAPTERRAIN(name, base, protobase, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
		static ushort StaticType(void);\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};

#endif

#define GROUNDWORLDMAPTERRAIN(name, base, setstats, data)\
\
WORLDMAPTERRAIN(\
	name,\
	base,\
	groundworldmapterrain,\
	setstats,\
	virtual groundworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { groundworldmapterrain* G = new name(false); G->Load(SaveFile); return G; }\
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
	virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { overworldmapterrain* O = new name(false); O->Load(SaveFile); return O; }\
	data\
);

BEGIN_PROTOTYPING(groundworldmapterrain)

class GROUNDWORLDMAPTERRAIN
(
	ocean,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "ocean"; }
	virtual std::string Article(void) const { return "an"; }
	virtual vector GetBitmapPos(void) const { return vector(208, 64); }
);

class GROUNDWORLDMAPTERRAIN
(
	glacier,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "glacier"; }
	virtual vector GetBitmapPos(void) const { return vector(16, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	desert,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "desert"; }
	virtual vector GetBitmapPos(void) const { return vector(64, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	snow,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "tundra"; }
	virtual vector GetBitmapPos(void) const { return vector(112, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	jungle,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "jungle"; }
	virtual vector GetBitmapPos(void) const { return vector(208, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	swamp,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "swamp"; }
	virtual vector GetBitmapPos(void) const { return vector(256, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	leafyforest,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "leafy forest"; }
	virtual vector GetBitmapPos(void) const { return vector(304, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	evergreenforest,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "evergreen forest"; }
	virtual std::string Article(void) const { return "an"; }
	virtual vector GetBitmapPos(void) const { return vector(352, 16); }
);

class GROUNDWORLDMAPTERRAIN
(
	steppe,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "steppe"; }
	virtual vector GetBitmapPos(void) const { return vector(160, 16); }
);

FINISH_PROTOTYPING(groundworldmapterrain)

BEGIN_PROTOTYPING(overworldmapterrain)

class OVERWORLDMAPTERRAIN
(
	atmosphere,
	overworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "atmosphere"; }
	virtual std::string Article(void) const { return "an"; }
	virtual vector GetBitmapPos(void) const { return vector(208, 256); }
);

class OVERWORLDMAPTERRAIN
(
	attnam,
	overworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "migthy cathedral reaching the clouds"; }
	virtual vector GetBitmapPos(void) const { return vector(0, 48); }
);

class OVERWORLDMAPTERRAIN
(
	elpuricave,
	overworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "hideous cave entry radiating pure navastating"; }
	virtual vector GetBitmapPos(void) const { return vector(16, 48); }
);

FINISH_PROTOTYPING(overworldmapterrain)

#endif
