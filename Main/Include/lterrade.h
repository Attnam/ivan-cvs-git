#ifndef __LTERRADE_H__
#define __LTERRADE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

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
	ground,
	groundlevelterrain,
	InitMaterials(new gravel),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR | FLIP | ROTATE_90; }
protected:
	virtual std::string NameSingular() const				{ return "ground"; }
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
	virtual vector2d GetBitmapPos() const						{ return vector2d(16, 336); }
);

class OVERLEVELTERRAIN
(
	earth,
	overlevelterrain,
	InitMaterials(2, new moraine, new stone),
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
		SetIsLocked(false);
		UpdatePicture();
	},
public:
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual bool CanBeOpened() const { return !GetIsWalkable(); }
	virtual std::string DigMessage() const { return "The chop the door down."; }
	virtual void Kick(ushort, bool, uchar);
	virtual void SetIsOpen(bool What) { IsOpen = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual bool GetIsWalkable() const { return IsOpen; }
	virtual bool IsDoor() const { return true; }
	virtual std::string Name(uchar) const;
	virtual void SetIsLocked(bool What) { IsLocked = What; }
	virtual bool GetIsLocked() const { return IsLocked; }
	virtual bool CanBeOpenedByAI() { return !GetIsLocked() && CanBeOpened(); }
	virtual bool ReceiveStrike();
	virtual void Lock() { SetIsLocked(true); }
	virtual bool CanBeDigged() const { return true; }
protected:
	virtual std::string NameSingular() const				{ return "door"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, GetIsWalkable() ? 48 : 176); }
	virtual void MakeWalkable();
	virtual void MakeNotWalkable();
	bool IsOpen;
	bool IsLocked;
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
	virtual void ColorChangeSpeciality(uchar, bool) { GraphicId.Color[1] = MAKE_RGB(160, 64, 0); }
	virtual void StepOn(character*);
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
	virtual void ColorChangeSpeciality(uchar, bool) { GraphicId.Color[1] = MAKE_RGB(160, 64, 0); }
	virtual void StepOn(character*);
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
		SetOwnerGod(RAND() % game::GetGodNumber() + 1);
	},
public:
	virtual bool CanBeOffered() const { return true; }
	virtual std::string Name(uchar Case) const { return NameWithMaterial(Case) + OwnerGodDescription(OwnerGod); }
	virtual void DrawToTileBuffer() const;
	virtual uchar GetOwnerGod() const { return OwnerGod; }
	virtual void SetOwnerGod(uchar What) { OwnerGod = What; }
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual uchar OKVisualEffects() const { return 0; }
	virtual std::string DigMessage() const { return "An invisible wall stops your feeble attempt."; }
	virtual void StepOn(character*);
	virtual void Kick(ushort, bool, uchar);
	virtual void ReceiveVomit(character*);
	virtual bool Polymorph(character*);
	virtual void SitOn(character*);
protected:
	virtual std::string NameSingular() const		{ return "altar"; }
	virtual vector2d GetBitmapPos() const				{ return vector2d(0, 368); }
	uchar OwnerGod;
);

class OVERLEVELTERRAIN
(
	throne,
	overlevelterrain,
	InitMaterials(2, new gold, new gold),
	{
	},
public:
	virtual std::string DigMessage() const { return "The throne resists."; }
	virtual void SitOn(character*);
protected:
	virtual std::string NameSingular() const				{ return "throne"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 416); }
);

class OVERLEVELTERRAIN
(
	pine,
	overlevelterrain,
	InitMaterials(2, new leaf, new wood),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() const { return "You chop the tree down."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
protected:
	virtual std::string NameSingular() const				{ return "pine"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(16, 320); }
);

class OVERLEVELTERRAIN
(
	spruce,
	overlevelterrain,
	InitMaterials(2, new leaf, new wood),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() const { return "You chop the tree down."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
protected:
	virtual std::string NameSingular() const				{ return "spruce"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(16, 352); }
);

class OVERLEVELTERRAIN
(
	linden,
	overlevelterrain,
	InitMaterials(2, new leaf, new wood),
	{
	},
public:
	virtual uchar OKVisualEffects() const { return MIRROR; }
	virtual bool CanBeDigged() const { return true; }
	virtual std::string DigMessage() const { return "You chop the tree down."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
protected:
	virtual std::string NameSingular() const				{ return "lovely linden"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(32, 336); }
);

class OVERLEVELTERRAIN
(
	carpet,
	overlevelterrain,
	InitMaterials(new expensivefabric),
	{
	},
public:
	virtual std::string DigMessage() const { return "You can't force yourself to ruin this wonderful carpet."; }
protected:
	virtual std::string NameSingular() const				{ return "carpet"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 272); }
);

class OVERLEVELTERRAIN
(
	couch,
	overlevelterrain,
	InitMaterials(2, new expensivefabric, new gold),
	{
	},
public:
	virtual std::string DigMessage() const { return "The destroy the couch."; }
	virtual void SitOn(character*);
	virtual bool CanBeDigged() const { return true; }
protected:
	virtual std::string NameSingular() const				{ return "couch"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(0, 400); }
);

class OVERLEVELTERRAIN
(
	pool,
	overlevelterrain,
	InitMaterials(3, new stone, new marble, new water),
	{
	},
public:
	virtual std::string DigMessage() const { return "The water splashes a bit."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual void SitOn(character*);
protected:
	virtual std::string NameSingular() const				{ return "pool"; }
	virtual vector2d GetBitmapPos() const					{ return vector2d(16, 304); }
);

class GROUNDLEVELTERRAIN
(
	tilefloor,
	groundlevelterrain,
	InitMaterials(2, new stone, new marble),
	{
	},
public:
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
protected:
	virtual std::string NameSingular() const				{ return "tile floor"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(0, 304); }
);

class OVERLEVELTERRAIN
(
	bookcase,
	overlevelterrain,
	InitMaterials(new wood),
	{
	},
public:
	virtual std::string DigMessage() const { return "You smash the bookcase into pieces."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual void SitOn(character*);
	virtual bool CanBeDigged() const { return true; }
protected:
	virtual std::string NameSingular() const				{ return "bookcase"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(16, 272); }
);

class OVERLEVELTERRAIN
(
	fountain,
	overlevelterrain,
	InitMaterials(2, new marble, new water),
	{
	},
public:
	virtual std::string DigMessage() const { return "The water splashes a bit."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual void SitOn(character*);
	virtual std::string GetConsumeQuestion() const { return std::string("Do want to drink from ") + Name(DEFINITE) + std::string("?"); }
	virtual void Consume(character*);
	virtual bool HasConsumeEffect() const { return true; } 
	virtual void DryOut();
protected:
	virtual std::string NameSingular() const				{ return "fountain"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(16, 288); }
);

class OVERLEVELTERRAIN
(
	doublebed,
	overlevelterrain,
	InitMaterials(2, new gold, new expensivefabric),
	{
	},
public:
	virtual std::string DigMessage() const { return "You smash the bed into pieces."; }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual void SitOn(character*);
	virtual bool CanBeDigged() const { return true; }
protected:
	virtual std::string NameSingular() const				{ return "luxurious double bed"; }
	virtual vector2d GetBitmapPos() const						{ return vector2d(48, 304); }
);

class OVERLEVELTERRAIN
(
	brokendoor,
	door,
	InitMaterials(new stone),
	{
		SetIsOpen(false);
		UpdatePicture();
	},
public:
	virtual std::string DigMessage() const { return "You destroy the broken door."; }
	virtual void Kick(ushort, bool, uchar);
	virtual bool ReceiveStrike();
	virtual bool CanBeDigged() const { return true; }
protected:
	virtual std::string NameSingular() const	{ return "broken door"; }
	virtual vector2d GetBitmapPos() const		{ return vector2d(0, GetIsWalkable() ? 48 : 160); }
);

#endif



