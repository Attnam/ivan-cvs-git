#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#define OVER_TERRAIN_TYPES	7
#define GROUND_TERRAIN_TYPES	3

#define ITERRAIN	0xFFFF

#define IOVERTERRAIN	0xFFFF
#define IEARTH		1
#define IWALL		2
#define IEMPTY		3
#define IDOOR		4
#define ISTAIRSUP	5
#define ISTAIRSDOWN	6
#define IALTAR		7

#define IGROUNDTERRAIN	0xFFFF
#define IPARQUET	1
#define IFLOORY		2

#include "typedef.h"
#include "vector.h"
#include "bitmap.h"
#include "game.h"
#include "object.h"

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
	terrain(material**, vector);
	terrain(std::ifstream* SaveFile, ushort M = 1) : object(SaveFile) {}
	virtual void SSquareUnder(square*);
	virtual bool Open(character* Opener);
	virtual bool Close(character* Closer);
	virtual vector CPos(void);
	virtual square* CSquareUnder(void) { return SquareUnder; }
	virtual levelsquare* CLevelSquareUnder(void) { return (levelsquare*)SquareUnder; }
	virtual bool CanBeOpened(void) { return false; }
	virtual bool CanBeOffered(void) { return false; }
	virtual std::string Name(uchar Case) {return NameWithMaterial(Case);}
	virtual bool CanBeDigged(void) { return false; }
	virtual uchar OKVisualEffects(void) { return MIRROR | FLIP | ROTATE_90; }
	virtual uchar CVisualFlags(void) { return VisualFlags; }
	virtual void SVisualFlags(uchar What) { VisualFlags = What; }
	virtual void HandleVisualEffects(void);
protected:
	virtual std::string NameSingular() const				{return "terrain";}
	virtual std::string NamePlural() const					{return "terrains";}
	virtual ushort Type(void) {return ITERRAIN;}
	square* SquareUnder;
	uchar VisualFlags;
};

class overterrain : public terrain
{
public:
	overterrain(material** Material, vector BitmapPos, bool IW) : terrain(Material, BitmapPos), IsWalkable(IW) {}
	overterrain(std::ifstream*, ushort);
	virtual void DrawToTileBuffer(void);
	virtual bool GoUp(character*);
	virtual bool GoDown(character*);
	virtual bool CIsWalkable(void)			{return IsWalkable;}
	virtual void Save(std::ofstream*);
	virtual uchar COwnerGod(void) { return 0; }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The ground is too hard to dig."); }
protected:
	virtual void MakeWalkable(void);
	virtual void MakeNotWalkable(void);
	virtual ushort Type(void) {return IOVERTERRAIN;}
	bool IsWalkable;
};

class groundterrain : public terrain
{
public:
	groundterrain(material** Material, vector BitmapPos) : terrain(Material, BitmapPos)	{}
	groundterrain(std::ifstream*, ushort);
	virtual void DrawToTileBuffer(void);
	virtual void Save(std::ofstream*);
protected:
	virtual ushort Type(void) {return IGROUNDTERRAIN;}
};

class earth : public overterrain
{
public:
	earth(material** Material) : overterrain(Material, vector(0, 336), false) {HandleVisualEffects();}
	earth(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : overterrain(SaveFile, MaterialQuantity) {}
protected:
	virtual ushort Type(void) {return IEARTH;}
	virtual std::string NameSingular() const				{return "earth";}
	virtual std::string NamePlural() const					{return "earths";}
	virtual vector CBitmapPos(void)						{ return vector(0, 336); }
	virtual bool CanBeDigged(void) { return true; }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The ground is fairly easy to dig."); }
};

class wall : public overterrain
{
public:
	wall(material** Material) : overterrain(Material, vector(0, 240), false) {HandleVisualEffects();}
	wall(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : overterrain(SaveFile, MaterialQuantity) {}
	virtual uchar OKVisualEffects(void) { return 0; }
protected:
	virtual std::string NameSingular() const				{return "wall";}
	virtual std::string NamePlural() const					{return "walls";}
	virtual ushort Type(void) {return IWALL;}
	virtual vector CBitmapPos(void)						{ return vector(0, 240); }
	virtual bool CanBeDigged(void) { return true; }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The wall is pretty hard, but you still manage to go through it."); }
};

class empty : public overterrain
{
public:
	empty(material** Material) : overterrain(Material, vector(0, 480), true) {HandleVisualEffects();}
	empty(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : overterrain(SaveFile, MaterialQuantity) {}
protected:
	virtual std::string NameSingular() const				{return "atmosphere";}
	virtual std::string NamePlural() const					{return "atmospheres";}
	virtual ushort Type(void) {return IEMPTY;}
	virtual vector CBitmapPos(void)						{ return vector(0, 480); }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The square you are trying to dig is empty."); }
};

class door : public overterrain
{
public:
	door(material**, const bool = false);
	door(std::ifstream*);
	virtual bool Open(character*);
	virtual bool Close(character*);
	virtual bool CanBeOpened(void) { return !CIsWalkable(); }
	virtual uchar OKVisualEffects(void) { return MIRROR; }
protected:
	virtual std::string NameSingular() const				{return "door";}
	virtual std::string NamePlural() const					{return "doors";}
	virtual ushort Type(void) {return IDOOR;}
	virtual vector CBitmapPos(void)						{ return vector(0, CIsWalkable() ? 48 : 176); }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The door is too hard to dig through."); }
};

class stairsup : public overterrain
{
public:
	stairsup(material** Material) : overterrain(Material, vector(0, 192), true) {HandleVisualEffects();}
	stairsup(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : overterrain(SaveFile, MaterialQuantity) {}
	virtual bool GoUp(character*);
	virtual uchar OKVisualEffects(void) { return 0; }
protected:
	virtual std::string NameSingular() const				{return "stairway upwards";}
	virtual std::string NamePlural() const					{return "stairways upwards";}
	virtual ushort Type(void) {return ISTAIRSUP;}
	virtual vector CBitmapPos(void)						{ return vector(0, 192); }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The stairs are too hard to dig."); }
};

class stairsdown : public overterrain
{
public:
	stairsdown(material** Material) : overterrain(Material, vector(0, 208), true) {HandleVisualEffects();}
	stairsdown(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : overterrain(SaveFile, MaterialQuantity) {}
	virtual bool GoDown(character*);
	virtual uchar OKVisualEffects(void) { return 0; }
protected:
	virtual std::string NameSingular() const				{return "stairway downwards";}
	virtual std::string NamePlural() const					{return "stairways downwards";}
	virtual ushort Type(void) {return ISTAIRSDOWN;}
	virtual vector CBitmapPos(void)						{ return vector(0, 208); }
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("The stairs are too hard to dig."); }
};

class parquet : public groundterrain
{
public:
	parquet(material** Material) : groundterrain(Material, vector(0, 240)) {HandleVisualEffects();}
	parquet(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : groundterrain(SaveFile, MaterialQuantity) {}
	virtual uchar OKVisualEffects(void) { return 0; }
protected:
	virtual std::string NameSingular() const				{return "parquet";}
	virtual std::string NamePlural() const					{return "parquette";}
	virtual ushort Type(void) {return IPARQUET;}
	virtual vector CBitmapPos(void)						{ return vector(0, 240); }
};

class floory : public groundterrain
{
public:
	floory(material** Material) : groundterrain(Material, vector(0, 352)) {HandleVisualEffects();}
	floory(std::ifstream* SaveFile, ushort MaterialQuantity = 1) : groundterrain(SaveFile, MaterialQuantity) {}
protected:
	virtual std::string NameSingular() const				{return "floor";}
	virtual std::string NamePlural() const					{return "floors";}
	virtual ushort Type(void) {return IFLOORY;}
	virtual vector CBitmapPos(void)						{ return vector(0, 352); }
};

class altar : public overterrain
{
public:
	altar(material** Material, uchar OwnerGod) : overterrain(Material, vector(0, 368),true), OwnerGod(OwnerGod) {HandleVisualEffects();}
	altar(std::ifstream*, ushort = 1);
	bool CanBeOffered(void) { return true; }
	virtual std::string Name(uchar);
	virtual void DrawToTileBuffer(void);
	virtual uchar COwnerGod(void) { return OwnerGod; }
	virtual void Save(std::ofstream*);
	virtual void ShowDigMessage(character* Who, item*) { if(Who == game::CPlayer()) ADD_MESSAGE("An invisible wall stops your feeble attempt."); }
	virtual uchar OKVisualEffects(void) { return 0; }
protected:
	virtual std::string NameSingular() const		{return "altar";}
	virtual std::string NamePlural() const			{return "altars";}
	virtual ushort Type(void) {return IALTAR;}
	virtual vector CBitmapPos(void)				{ return vector(0, 368); }
	uchar OwnerGod;
};

#endif
