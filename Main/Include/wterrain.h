#ifndef __WTERRAIN_H__
#define __WTERRAIN_H__

#include <fstream>

#include "game.h"
#include "independ.h"
#include "terrain.h"
#include "vector.h"

class worldmapsquare;

class worldmapterrain : public independency
{
public:
	virtual void Save(std::ofstream* SaveFile) const { independency::Save(SaveFile); }
	virtual void Load(std::ifstream*) {}
	virtual vector GetPos(void) const;
	virtual worldmapsquare* GetWorldMapSquareUnder(void) const { return WorldMapSquareUnder; }
	virtual void SetWorldMapSquareUnder(worldmapsquare* What) { WorldMapSquareUnder = What; }
protected:
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
	//virtual void SetIsWalkable(bool What)			{ IsWalkable = What; }
	//virtual bool GetIsWalkable(void) const			{ return IsWalkable; }
	virtual overworldmapterrain* Clone(bool = true) const = 0;
//protected:
//	bool IsWalkable;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define WORLDMAPTERRAIN(name, base, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
	protected:\
		virtual void SetDefaultStats(void) { setstats }\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class proto_##name\
	{\
	public:\
		proto_##name(void) : Index(game::AddProtoType(new name(false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static Proto_##name;\
	\
	ushort name::Type(void) const { return Proto_##name.GetIndex(); }

#else

	#define WORLDMAPTERRAIN(name, base, setstats, data)\
	\
	name : public base\
	{\
	public:\
		name(bool SetStats = true) : base(false) { if(SetStats) SetDefaultStats(); }\
	protected:\
		virtual void SetDefaultStats(void) { setstats }\
		virtual ushort Type(void) const;\
		data\
	};

#endif

#define WORLDMAPGROUNDTERRAIN(name, base, setstats, data)\
\
WORLDMAPTERRAIN(\
	name,\
	base,\
	setstats,\
	virtual groundworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	data\
);

#define WORLDMAPOVERTERRAIN(name, base, setstats, data)\
\
WORLDMAPTERRAIN(\
	name,\
	base,\
	setstats,\
	virtual overworldmapterrain* Clone(bool SetStats = true) const { return new name(SetStats); }\
	data\
);

class WORLDMAPGROUNDTERRAIN
(
	glacier,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "glacier"; }
	virtual std::string NamePlural(void) const { return "glaciers"; }
	virtual vector GetBitmapPos(void) const { return vector(16, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	desert,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "desert"; }
	virtual std::string NamePlural(void) const { return "deserts"; }
	virtual vector GetBitmapPos(void) const { return vector(64, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	snow,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "snowy terrain"; }
	virtual std::string NamePlural(void) const { return "snowy terrains"; }
	virtual vector GetBitmapPos(void) const { return vector(112, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	jungle,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "jungle"; }
	virtual std::string NamePlural(void) const { return "jungles"; }
	virtual vector GetBitmapPos(void) const { return vector(208, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	swamp,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "swamp"; }
	virtual std::string NamePlural(void) const { return "swamps"; }
	virtual vector GetBitmapPos(void) const { return vector(256, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	leafyforest,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "leafy forest"; }
	virtual std::string NamePlural(void) const { return "leafy forests"; }
	virtual vector GetBitmapPos(void) const { return vector(304, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	evergreenforest,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "evergreen forest"; }
	virtual std::string NamePlural(void) const { return "evergreen forests"; }
	virtual vector GetBitmapPos(void) const { return vector(352, 16); }
);

class WORLDMAPGROUNDTERRAIN
(
	ocean,
	groundworldmapterrain,
	{
	},
	virtual std::string NameSingular(void) const { return "ocean"; }
	virtual std::string NamePlural(void) const { return "oceans"; }
	virtual vector GetBitmapPos(void) const { return vector(208, 64); }
);

#endif
