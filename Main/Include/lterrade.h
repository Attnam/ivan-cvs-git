#ifndef __LTERRADE_H__
#define __LTERRADE_H__

#include "lterraba.h"

class GROUNDLEVELTERRAIN
(
	parquet,
	groundlevelterrain,
	InitMaterials(new wood(1)),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return 0; }
protected:
	virtual std::string NameSingular() const				{ return "parquet"; }
	virtual std::string NamePlural() const					{ return "parquette"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 240); }
);

class GROUNDLEVELTERRAIN
(
	floor,
	groundlevelterrain,
	InitMaterials(new gravel(1)),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
protected:
	virtual std::string NameSingular() const				{ return "floor"; }
	virtual std::string NamePlural() const					{ return "floors"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 352); }
);

class OVERLEVELTERRAIN
(
	earth,
	overlevelterrain,
	InitMaterials(new moraine(1)),
	{
		SetIsWalkable(false);
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() { return "The ground is fairly easy to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "earth"; }
	virtual std::string NamePlural() const					{ return "earths"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 336); }
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
	virtual uchar OKVisualEffects() const { return 0; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() { return "The wall is pretty hard, but you still manage to go through it."; }
protected:
	virtual std::string NameSingular() const				{ return "wall"; }
	virtual std::string NamePlural() const					{ return "walls"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 240); }
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
	virtual std::string DigMessage() { return "The square you are trying to dig is empty."; }
protected:
	virtual std::string NameSingular() const				{ return "atmosphere"; }
	virtual std::string NamePlural() const					{ return "atmospheres"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 480); }
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
	virtual bool CanBeOpened() const { return !GetIsWalkable(); }
	virtual std::string DigMessage() { return "The door is too hard to dig through."; }
	virtual void Kick(ushort, bool, uchar);
protected:
	virtual std::string NameSingular() const				{ return "door"; }
	virtual std::string NamePlural() const					{ return "doors"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, GetIsWalkable() ? 48 : 176); }
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
	virtual bool GoUp(character*) const;
	virtual uchar OKVisualEffects() const { return 0; }
	virtual std::string DigMessage() { return "The stairs are too hard to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "stairway upwards"; }
	virtual std::string NamePlural() const					{ return "stairways upwards"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 192); }
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
	virtual bool GoDown(character*) const;
	virtual uchar OKVisualEffects() const { return 0; }
	virtual std::string DigMessage() { return "The stairs are too hard to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "stairway downwards"; }
	virtual std::string NamePlural() const					{ return "stairways downwards"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 208); }
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
	virtual bool CanBeOffered() const { return true; }
	virtual std::string Name(uchar) const;
	virtual void DrawToTileBuffer() const;
	virtual uchar GetOwnerGod() const { return OwnerGod; }
	virtual void SetOwnerGod(uchar What) { OwnerGod = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual uchar OKVisualEffects() const { return 0; }
	virtual std::string DigMessage() { return "An invisible wall stops your feeble attempt."; }
protected:
	virtual std::string NameSingular() const		{ return "altar"; }
	virtual std::string NamePlural() const			{ return "altars"; }
	virtual vector2d GetBitmapPos() const				{ return vector2d(0, 368); }
	uchar OwnerGod;
);

class OVERLEVELTERRAIN
(
	throne,
	overlevelterrain,
	InitMaterials(new gold(1)),
	{
		SetIsWalkable(true);
	},
public:
	virtual bool CanBeDigged() const { return false; }
	virtual std::string DigMessage() { return "The throne resists."; }
protected:
	virtual std::string NameSingular() const				{ return "throne"; }
	virtual std::string NamePlural() const					{ return "thrones"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 304); }
);

#endif
