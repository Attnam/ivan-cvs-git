#ifndef __LTERRADE_H__
#define __LTERRADE_H__

#include "lterraba.h"

class GROUNDLEVELTERRAIN
(
	parquet,
	groundlevelterrain,
	InitMaterials(new wood),
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
	InitMaterials(new gravel),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
protected:
	virtual std::string NameSingular() const				{ return "floor"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 352); }
);

class GROUNDLEVELTERRAIN
(
	grassterrain,
	groundlevelterrain,
	InitMaterials(new grass),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
	virtual std::string Name(uchar Case) const RET(NameHandleDefaultMaterial(Case, "a", grass::StaticType()))
protected:
	virtual std::string NameSingular() const				{ return "grassy ground"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 352); }
);

class OVERLEVELTERRAIN
(
	earth,
	overlevelterrain,
	InitMaterials(new moraine),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() const { return "The ground is fairly easy to dig."; }
	virtual bool GetIsWalkable() const { return false; }
protected:
	virtual std::string NameSingular() const				{ return "earth"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 336); }
);

class OVERLEVELTERRAIN
(
	wall,
	overlevelterrain,
	InitMaterials(new stone),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return 0; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() const { return "The wall is pretty hard, but you still manage to go through it."; }
	virtual bool GetIsWalkable() const { return false; }
protected:
	virtual std::string NameSingular() const				{ return "wall"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 240); }
);

class OVERLEVELTERRAIN
(
	empty,
	overlevelterrain,
	InitMaterials(new air),
	{
	},
public:
	virtual std::string DigMessage() const { return "The square you are trying to dig is empty."; }
protected:
	virtual std::string NameSingular() const				{ return "atmosphere"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 480); }
);

class OVERLEVELTERRAIN
(
	door,
	overlevelterrain,
	InitMaterials(new stone),
	{
		SetIsOpen(false);
		UpdatePicture();
	},
public:
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual bool CanBeOpened() const { return !GetIsWalkable(); }
	virtual std::string DigMessage() const { return "The door is too hard to dig through."; }
	virtual void Kick(ushort, bool, uchar);
	virtual void SetIsOpen(bool What) { IsOpen = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual bool GetIsWalkable() const { return IsOpen; }
	virtual bool IsDoor() const { return true; }
protected:
	virtual std::string NameSingular() const				{ return "door"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, GetIsWalkable() ? 48 : 176); }
	virtual void MakeWalkable();
	virtual void MakeNotWalkable();
	bool IsOpen;
);

class OVERLEVELTERRAIN
(
	stairsup,
	overlevelterrain,
	InitMaterials(new stone),
	{
	},
public:
	virtual bool GoUp(character*) const;
	virtual uchar OKVisualEffects() const { return 0; }
	virtual std::string DigMessage() const { return "The stairs are too hard to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "stairway upwards"; }
	virtual std::string NamePlural() const					{ return "stairways upwards"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 192); }
);

class OVERLEVELTERRAIN
(
	stairsdown,
	overlevelterrain,
	InitMaterials(new stone),
	{
	},
public:
	virtual bool GoDown(character*) const;
	virtual uchar OKVisualEffects() const { return 0; }
	virtual std::string DigMessage() const { return "The stairs are too hard to dig."; }
protected:
	virtual std::string NameSingular() const				{ return "stairway downwards"; }
	virtual std::string NamePlural() const					{ return "stairways downwards"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 208); }
);

class OVERLEVELTERRAIN
(
	altar,
	overlevelterrain,
	InitMaterials(new stone),
	{
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
	virtual std::string DigMessage() const { return "An invisible wall stops your feeble attempt."; }
	virtual void StepOn(character*);
protected:
	virtual std::string NameSingular() const		{ return "altar"; }
	virtual vector2d GetBitmapPos() const				{ return vector2d(0, 368); }
	uchar OwnerGod;
);

class OVERLEVELTERRAIN
(
	throne,
	overlevelterrain,
	InitMaterials(new gold),
	{
	},
public:
	virtual bool CanBeDigged() const { return false; }
	virtual std::string DigMessage() const { return "The throne resists."; }
	virtual void SitOn(character*);
protected:
	virtual std::string NameSingular() const				{ return "throne"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 416); }
);

#endif
