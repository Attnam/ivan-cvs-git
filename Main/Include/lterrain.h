#ifndef __LTERRAIN_H__
#define __LTERRAIN_H__

#include "bitmap.h"
#include "object.h"
#include "terrain.h"
#include "typedef.h"
#include "vector.h"

#include "proto.h"
#include "material.h"
#include "game.h"

class bitmap;
class character;
class material;
class game;
class object;
class vector;
class levelsquare;
class square;

/* Presentation of the levelterrain class & subclasses */

class levelterrain : public object
{
public:
	levelterrain::levelterrain(bool AddToPool) : object(AddToPool) {}
	virtual void Load(std::ifstream&);
	virtual bool Open(character* Opener);
	virtual bool Close(character* Closer);
	virtual vector GetPos(void) const;
	virtual levelsquare* GetLevelSquareUnder(void) const;
	virtual bool CanBeOpened(void) const { return false; }
	virtual bool CanBeOffered(void) const { return false; }
	virtual std::string Name(uchar Case) const { return NameWithMaterial(Case); }
	virtual bool CanBeDigged(void) const { return false; }
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual uchar GetVisualFlags(void) const { return VisualFlags; }
	virtual void SetVisualFlags(uchar What) { VisualFlags = What; }
	virtual void HandleVisualEffects(void);
	virtual void Save(std::ofstream&) const;
protected:
	uchar VisualFlags;
};

class groundlevelterrain : public levelterrain, public groundterrain
{
public:
	groundlevelterrain::groundlevelterrain(bool = true, bool = true, bool AddToPool = true) : levelterrain(AddToPool) {}
	virtual void DrawToTileBuffer(void) const;
	virtual groundlevelterrain* Clone(bool = true, bool = true) const = 0;
	virtual void Save(std::ofstream& SaveFile) const { levelterrain::Save(SaveFile); }
	virtual void Load(std::ifstream& SaveFile) { levelterrain::Load(SaveFile); };
	virtual std::string Name(uchar Case = 0) const { return levelterrain::Name(Case); }
	/*static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }
protected:
	static ushort ProtoIndexBegin, ProtoIndexEnd;*/
};

class overlevelterrain : public levelterrain, public overterrain
{
public:
	overlevelterrain::overlevelterrain(bool = true, bool = true, bool AddToPool = true) : levelterrain(AddToPool) {}
	virtual void Load(std::ifstream&);
	virtual void DrawToTileBuffer(void) const;
	virtual bool GoUp(character*);
	virtual bool GoDown(character*);
	virtual void Save(std::ofstream&) const;
	virtual uchar GetOwnerGod(void) const { return 0; }
	virtual std::string DigMessage(void) { return "The ground is too hard to dig."; }
	virtual overlevelterrain* Clone(bool = true, bool = true) const = 0;
	virtual std::string Name(uchar Case = 0) const { return levelterrain::Name(Case); }
	/*static ushort GetProtoIndexBegin(void) { return ProtoIndexBegin; }
	static ushort GetProtoIndexEnd(void) { return ProtoIndexEnd; }
	static ushort GetProtoAmount(void) { return ProtoIndexEnd - ProtoIndexBegin; }*/
protected:
	virtual void MakeWalkable(void);
	virtual void MakeNotWalkable(void);
	//static ushort ProtoIndexBegin, ProtoIndexEnd;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define PROTOINSTALLER(name, base, protobase, initmaterials, setstats)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller(void) : Index(protocontainer<protobase>::Add(new name(false, false, false))) {}\
		ushort GetIndex(void) const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	name::name(bool CreateMaterials, bool SetStats, bool AddToPool) : base(false, false, AddToPool) { if(CreateMaterials) initmaterials ; if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
	void name::SetDefaultStats(void) { setstats }\
	ushort name::StaticType(void) { return name##_ProtoInstaller.GetIndex(); }\
	ushort name::Type(void) const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define PROTOINSTALLER(name, base, protobase, initmaterials, setstats)

#endif

#define LEVELTERRAIN(name, base, protobase, initmaterials, setstats, data)\
\
name : public base\
{\
public:\
	name(bool = true, bool = true, bool = true);\
	name(material* Material, bool SetStats = true) : base(false, false) { InitMaterials(Material); if(SetStats) SetDefaultStats(); HandleVisualEffects(); }\
	static ushort StaticType(void);\
protected:\
	virtual void SetDefaultStats(void);\
	virtual ushort Type(void) const;\
	data\
}; PROTOINSTALLER(name, base, protobase, initmaterials, setstats);

#define GROUNDLEVELTERRAIN(name, base, initmaterials, setstats, data)\
\
LEVELTERRAIN(\
	name,\
	base,\
	groundlevelterrain,\
	initmaterials,\
	setstats,\
	virtual groundlevelterrain* Clone(bool CreateMaterials = true, bool SetStats = true) const { return new name(CreateMaterials, SetStats); }\
	virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { groundlevelterrain* G = new name(false, false); G->Load(SaveFile); return G; }\
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
	virtual typeable* CloneAndLoad(std::ifstream& SaveFile) const { overlevelterrain* O = new name(false, false); O->Load(SaveFile); return O; }\
	data\
);

//BEGIN_PROTOTYPING(groundlevelterrain)

class GROUNDLEVELTERRAIN
(
	parquet,
	groundlevelterrain,
	InitMaterials(new wood(1)),
	{
	},
public:
	virtual uchar OKVisualEffects(void) const { return 0; }
protected:
	virtual std::string NameSingular() const				{ return "parquet"; }
	virtual std::string NamePlural() const					{ return "parquette"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, 240); }
);

class GROUNDLEVELTERRAIN
(
	floor,
	groundlevelterrain,
	InitMaterials(new gravel(1)),
	{
	},
public:
	virtual uchar OKVisualEffects(void) const { return MIRROR | FLIP | ROTATE_90; }
protected:
	virtual std::string NameSingular() const				{ return "floor"; }
	virtual std::string NamePlural() const					{ return "floors"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, 352); }
);

//FINISH_PROTOTYPING(groundlevelterrain)

//BEGIN_PROTOTYPING(overlevelterrain)

class OVERLEVELTERRAIN
(
	earth,
	overlevelterrain,
	InitMaterials(new moraine(1)),
	{
		SetIsWalkable(false);
	},
public:
	virtual uchar OKVisualEffects(void) const { return MIRROR | FLIP | ROTATE_90; }
	virtual bool CanBeDigged(void) const { return true; }
	virtual std::string DigMessage(void) { return "The ground is fairly easy to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "earth"; }
	virtual std::string NamePlural() const					{ return "earths"; }
	virtual vector GetBitmapPos(void) const					{ return vector(0, 336); }
);

class OVERLEVELTERRAIN
(
	wall,
	overlevelterrain,
	InitMaterials(new stone(1)),
	{
		SetIsWalkable(false);
	},
public:
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual bool CanBeDigged(void) const { return true; }
	virtual std::string DigMessage(void) { return "The wall is pretty hard, but you still manage to go through it."; }
protected:
	virtual std::string NameSingular() const				{ return "wall"; }
	virtual std::string NamePlural() const					{ return "walls"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, 240); }
);

class OVERLEVELTERRAIN
(
	empty,
	overlevelterrain,
	InitMaterials(new air(1)),
	{
		SetIsWalkable(true);
	},
public:
	virtual std::string DigMessage(void) { return "The square you are trying to dig is empty."; }
protected:
	virtual std::string NameSingular() const				{ return "atmosphere"; }
	virtual std::string NamePlural() const					{ return "atmospheres"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, 480); }
);

class OVERLEVELTERRAIN
(
	door,
	overlevelterrain,
	InitMaterials(new stone(1)),
	{
		SetIsWalkable(false);
	},
public:
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual bool CanBeOpened(void) const { return !GetIsWalkable(); }
	virtual std::string DigMessage(void) { return "The door is too hard to dig through."; }
protected:
	virtual std::string NameSingular() const				{ return "door"; }
	virtual std::string NamePlural() const					{ return "doors"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, GetIsWalkable() ? 48 : 176); }
);

class OVERLEVELTERRAIN
(
	stairsup,
	overlevelterrain,
	InitMaterials(new stone(1)),
	{
		SetIsWalkable(true);
	},
public:
	virtual bool GoUp(character*);
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual std::string DigMessage(void) { return "The stairs are too hard to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "stairway upwards"; }
	virtual std::string NamePlural() const					{ return "stairways upwards"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, 192); }
);

class OVERLEVELTERRAIN
(
	stairsdown,
	overlevelterrain,
	InitMaterials(new stone(1)),
	{
		SetIsWalkable(true);
	},
public:
	virtual bool GoDown(character*);
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual std::string DigMessage(void) { return "The stairs are too hard to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "stairway downwards"; }
	virtual std::string NamePlural() const					{ return "stairways downwards"; }
	virtual vector GetBitmapPos(void) const						{ return vector(0, 208); }
);

class OVERLEVELTERRAIN
(
	altar,
	overlevelterrain,
	InitMaterials(new stone(1)),
	{
		SetIsWalkable(true);
		SetOwnerGod(rand() % game::GetGodNumber() + 1);
	},
public:
	virtual bool CanBeOffered(void) const { return true; }
	virtual std::string Name(uchar) const;
	virtual void DrawToTileBuffer(void) const;
	virtual uchar GetOwnerGod(void) const { return OwnerGod; }
	virtual void SetOwnerGod(uchar What) { OwnerGod = What; }
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual uchar OKVisualEffects(void) const { return 0; }
	virtual std::string DigMessage(void) { return "An invisible wall stops your feeble attempt."; }
protected:
	virtual std::string NameSingular() const		{ return "altar"; }
	virtual std::string NamePlural() const			{ return "altars"; }
	virtual vector GetBitmapPos(void) const				{ return vector(0, 368); }
	uchar OwnerGod;
);

//FINISH_PROTOTYPING(overlevelterrain)

#endif


