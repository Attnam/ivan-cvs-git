#ifndef __WTERRAIN_H__
#define __WTERRAIN_H__

#include <fstream>

#include "terrain.h"
#include "vector.h"

class worldmapsquare;

class worldmapterrain
{
public:
	friend class worldmap;
	virtual void Save(std::ofstream* SaveFile) const;
	virtual void Load(std::ifstream*) {}
	virtual vector GetPos(void) const;
	virtual worldmapsquare* GetWorldMapSquareUnder(void) const { return WorldMapSquareUnder; }
	virtual void SetWorldMapSquareUnder(worldmapsquare* What) { WorldMapSquareUnder = What; }
	virtual std::string Name(uchar = 0) const;
protected:
	virtual std::string NameStem(void) const = 0;
	virtual std::string Article(void) const { return "a"; }
	virtual vector GetBitmapPos(void) const = 0;
	virtual ushort Type(void) const = 0;
	worldmapsquare* WorldMapSquareUnder;
};

class groundworldmapterrain : public worldmapterrain, public groundterrain
{
public:
	groundworldmapterrain(bool = true) {}
	virtual void DrawToTileBuffer(void) const;
	virtual groundworldmapterrain* Clone(bool = true) const = 0;
};

class overworldmapterrain : public worldmapterrain, public overterrain
{
public:
	overworldmapterrain(bool = true) {}
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual void DrawToTileBuffer(void) const;
	virtual overworldmapterrain* Clone(bool = true) const = 0;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define WORLDMAPTERRAIN(name, base, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
	protected:\
		virtual void SetDefaultStats(void);\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class proto_##name\
	{\
	public:\
		proto_##name(void) : Index(prototypesystem::AddProtoType(new name(false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static Proto_##name;\
	\
	void name::SetDefaultStats(void) { setstats }\
	ushort name::Type(void) const { return Proto_##name.GetIndex(); }

#else

	#define WORLDMAPTERRAIN(name, base, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
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
	setstats,\
	virtual groundworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	data\
);

#define OVERWORLDMAPTERRAIN(name, base, setstats, data)\
\
WORLDMAPTERRAIN(\
	name,\
	base,\
	setstats,\
	virtual overworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	data\
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
	ocean,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "ocean"; }
	virtual vector GetBitmapPos(void) const { return vector(208, 64); }
);


class GROUNDWORLDMAPTERRAIN
(
	steppe,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem(void) const { return "steppe"; }
	virtual std::string Article(void) const { return "a"; }
	virtual vector GetBitmapPos(void) const { return vector(160, 16); }
);


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


#endif
