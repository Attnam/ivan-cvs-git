#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "game.h"
#include "bitmap.h"
#include "object.h"
#include "typedef.h"
#include "vector.h"

class bitmap;
class character;
class material;
class game;
class object;
class vector;
class levelsquare;
class square;

/* Presentation of the terrain class & subclasses */

class terrain : public object
{
public:
	virtual void Load(std::ifstream*);
	virtual void SSquareUnder(square*);
	virtual bool Open(character* Opener);
	virtual bool Close(character* Closer);
	virtual vector CPos(void) const;
	virtual square* CSquareUnder(void) const { return SquareUnder; }
	virtual levelsquare* CLevelSquareUnder(void) const { return (levelsquare*)SquareUnder; }
	virtual bool CanBeOpened(void) const { return false; }
	virtual bool CanBeOffered(void) const { return false; }
	virtual std::string Name(uchar Case) const {return NameWithMaterial(Case);}
	virtual bool CanBeDigged(void) const { return false; }
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual uchar CVisualFlags(void) const { return VisualFlags; }
	virtual void SVisualFlags(uchar What) { VisualFlags = What; }
	virtual void HandleVisualEffects(void);
	virtual void Save(std::ofstream*) const;
protected:
	virtual std::string NameSingular() const				{return "terrain";}
	virtual std::string NamePlural() const					{return "terrains";}
	square* SquareUnder;
	uchar VisualFlags;
};

class groundterrain : public terrain
{
public:
	groundterrain::groundterrain(bool = true, bool = true) {}
	virtual void DrawToTileBuffer(void) const;
	virtual groundterrain* Clone(bool = true, bool = true) const = 0;
};

class overterrain : public terrain
{
public:
	overterrain::overterrain(bool = true, bool = true) {}
	virtual void Load(std::ifstream*);
	virtual void DrawToTileBuffer(void) const;
	virtual bool GoUp(character*);
	virtual bool GoDown(character*);
	virtual void SIsWalkable(bool What)			{ IsWalkable = What; }
	virtual bool CIsWalkable(void) const			{ return IsWalkable; }
	virtual void Save(std::ofstream*) const;
	virtual uchar COwnerGod(void) const { return 0; }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The ground is too hard to dig."); }
	virtual overterrain* Clone(bool = true, bool = true) const = 0;
protected:
	virtual void MakeWalkable(void);
	virtual void MakeNotWalkable(void);
	bool IsWalkable;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define TERRAIN(name, base, initmaterials, setstats, data)\
	\
	class name : public base\
	{\
	public:\
		name(bool CreateMaterials = true, bool SetStats = true) : base(false, false) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
		name(material* Material, bool SetStats = true) : base(false, false) { InitMaterials(Material); if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
	protected:\
		virtual void SetDefaultStats(void) { setstats }\
		virtual ushort Type(void) const;\
		data\
	};\
	\
	class proto_##name\
	{\
	public:\
		proto_##name(void) : Index(game::AddProtoType(new name(false, false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static Proto_##name;\
	\
	ushort name::Type(void) const { return Proto_##name.GetIndex(); }

#else

	#define TERRAIN(name, base, initmaterials, setstats, data)\
	\
	class name : public base\
	{\
	public:\
		name(bool CreateMaterials = true, bool SetStats = true) : base(false, false) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
		name(material* Material, bool SetStats = true) : base(false, false) { InitMaterials(Material); if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
	protected:\
		virtual void SetDefaultStats(void) { setstats }\
		virtual ushort Type(void) const;\
		data\
	};

#endif

#define GROUNDTERRAIN(name, base, initmaterials, setstats, data)\
\
TERRAIN(\
	name,\
	base,\
	initmaterials,\
	setstats,\
	virtual groundterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
	data\
);

#define OVERTERRAIN(name, base, initmaterials, setstats, data)\
\
TERRAIN(\
	name,\
	base,\
	initmaterials,\
	setstats,\
	virtual overterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
	data\
);

OVERTERRAIN(
	earth,
	overterrain,
	InitMaterials(new moraine(1)),
	{
		SIsWalkable(false);
	},
public:
	virtual uchar OKVisualEffects(void) const { return MIRROR | FLIP | ROTATE_90; }
	virtual bool CanBeDigged(void) const { return true; }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The ground is fairly easy to dig."); }
protected:
	virtual std::string NameSingular() const				{return "earth";}
	virtual std::string NamePlural() const					{return "earths";}
	virtual vector CBitmapPos(void) const					{ return vector(0, 336); }
);

OVERTERRAIN(
	wall,
	overterrain,
	InitMaterials(new stone(1)),
	{
		SIsWalkable(false);
	},
public:
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual bool CanBeDigged(void) const { return true; }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The wall is pretty hard, but you still manage to go through it."); }
protected:
	virtual std::string NameSingular() const				{return "wall";}
	virtual std::string NamePlural() const					{return "walls";}
	virtual vector CBitmapPos(void) const						{ return vector(0, 240); }
);

OVERTERRAIN(
	empty,
	overterrain,
	InitMaterials(new air(1)),
	{
		SIsWalkable(true);
	},
public:
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The square you are trying to dig is empty."); }
protected:
	virtual std::string NameSingular() const				{return "atmosphere";}
	virtual std::string NamePlural() const					{return "atmospheres";}
	virtual vector CBitmapPos(void) const						{ return vector(0, 480); }
);

OVERTERRAIN(
	door,
	overterrain,
	InitMaterials(new stone(1)),
	{
		SIsWalkable(false);
	},
public:
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual bool CanBeOpened(void) const { return !CIsWalkable(); }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The door is too hard to dig through."); }
protected:
	virtual std::string NameSingular() const				{return "door";}
	virtual std::string NamePlural() const					{return "doors";}
	virtual vector CBitmapPos(void) const						{ return vector(0, CIsWalkable() ? 48 : 176); }
);

OVERTERRAIN(
	stairsup,
	overterrain,
	InitMaterials(new stone(1)),
	{
		SIsWalkable(true);
	},
public:
	virtual bool GoUp(character*);
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The stairs are too hard to dig."); }
protected:
	virtual std::string NameSingular() const				{return "stairway upwards";}
	virtual std::string NamePlural() const					{return "stairways upwards";}
	virtual vector CBitmapPos(void) const						{ return vector(0, 192); }
);

OVERTERRAIN(
	stairsdown,
	overterrain,
	InitMaterials(new stone(1)),
	{
		SIsWalkable(true);
	},
public:
	virtual bool GoDown(character*);
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("The stairs are too hard to dig."); }
protected:
	virtual std::string NameSingular() const				{return "stairway downwards";}
	virtual std::string NamePlural() const					{return "stairways downwards";}
	virtual vector CBitmapPos(void) const						{ return vector(0, 208); }
);

GROUNDTERRAIN(
	parquet,
	groundterrain,
	InitMaterials(new wood(1)),
	{
	},
public:
	virtual uchar OKVisualEffects(void) const { return 0; }
protected:
	virtual std::string NameSingular() const				{return "parquet";}
	virtual std::string NamePlural() const					{return "parquette";}
	virtual vector CBitmapPos(void) const						{ return vector(0, 240); }
);

GROUNDTERRAIN(
	floory,
	groundterrain,
	InitMaterials(new gravel(1)),
	{
	},
public:
	virtual uchar OKVisualEffects(void) const { return MIRROR | FLIP | ROTATE_90; }
protected:
	virtual std::string NameSingular() const				{return "floor";}
	virtual std::string NamePlural() const					{return "floors";}
	virtual vector CBitmapPos(void) const						{ return vector(0, 352); }
);

OVERTERRAIN(
	altar,
	overterrain,
	InitMaterials(new stone(1)),
	{
		SIsWalkable(true);
		SOwnerGod(rand() % game::CGodNumber() + 1);
	},
public:
	virtual bool CanBeOffered(void) const { return true; }
	virtual std::string Name(uchar) const;
	virtual void DrawToTileBuffer(void) const;
	virtual uchar COwnerGod(void) const { return OwnerGod; }
	virtual void SOwnerGod(uchar What) { OwnerGod = What; }
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual void ShowDigMessage(character* Who, item*) const { if(Who == game::CPlayer()) ADD_MESSAGE("An invisible wall stops your feeble attempt."); }
protected:
	virtual std::string NameSingular() const		{return "altar";}
	virtual std::string NamePlural() const			{return "altars";}
	virtual vector CBitmapPos(void) const				{ return vector(0, 368); }
	uchar OwnerGod;
);

#endif
