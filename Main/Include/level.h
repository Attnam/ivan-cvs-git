#ifndef __LEVEL_H__
#define __LEVEL_H__

#define FORBIDDEN		1
#define ON_POSSIBLE_ROUTE	2
#define STILL_ON_POSSIBLE_ROUTE	4
#define PREFERRED		8

#define DO_FOR_SQUARES_AROUND(X, Y, BorderX, BorderY, Then)	\
{								\
	ushort DoX, DoY, DoIndex = 0;				\
								\
	if(X && Y)						\
	{							\
		DoX = X - 1; DoY = Y - 1;			\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(Y)							\
	{							\
		DoX = X; DoY = Y - 1;				\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(X < BorderX - 1 && Y)				\
	{							\
		DoX = X + 1; DoY = Y - 1;			\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(X)							\
	{							\
		DoX = X - 1; DoY = Y;				\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(X < BorderX - 1)					\
	{							\
		DoX = X + 1; DoY = Y;				\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(X && Y < BorderY - 1)				\
	{							\
		DoX = X - 1; DoY = Y + 1;			\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(Y < BorderY - 1)					\
	{							\
		DoX = X; DoY = Y + 1;				\
								\
		Then;						\
	}							\
								\
	DoIndex++;						\
								\
	if(X < BorderX - 1 && Y < BorderY - 1)			\
	{							\
		DoX = X + 1; DoY = Y + 1;			\
								\
		Then;						\
	}							\
}

#define DO_RECTANGLE(CenterX, CenterY, ClipLeft, ClipTop, ClipRigth, ClipBottom, Radius, DoOne, DoTwo, DoThree, DoFour)		\
{																\
	long Left    = (CenterX) - (Radius),											\
	    Top     = (CenterY) - (Radius),											\
	    Rigth   = (CenterX) + (Radius),											\
	    Bottom  = (CenterY) + (Radius);											\
																\
	if(Left   < (ClipLeft))		Left   = (ClipLeft);									\
	if(Top    < (ClipTop))		Top    = (ClipTop);									\
	if(Rigth  > (ClipRigth))	Rigth  = (ClipRigth);									\
	if(Bottom > (ClipBottom))	Bottom = (ClipBottom);									\
																\
	if(Left < (ClipRigth) && Top < (ClipBottom) && Rigth > (ClipLeft) && Bottom > (ClipTop))				\
	{															\
		ushort XPointer, YPointer;												\
																\
		for(XPointer = Left; XPointer <= Rigth; XPointer++)								\
		{														\
			DoOne; DoTwo;												\
		}														\
																\
		for(YPointer = Top; YPointer <= Bottom; YPointer++)								\
		{														\
			DoThree; DoFour;											\
		}														\
	}															\
}

#define DO_BIG_RECTANGLE(ClipLeft, ClipTop, ClipRigth, ClipBottom, DoOne, DoTwo)						\
{																\
	ushort XPointer, YPointer;													\
																\
	for(XPointer = (ClipLeft); XPointer <= (ClipRigth); XPointer++)								\
		DoOne;														\
																\
	for(YPointer = (ClipTop); YPointer <= (ClipBottom); YPointer++)								\
		DoTwo;														\
}

#define DO_FILLED_RECTANGLE(CenterX, CenterY, ClipLeft, ClipTop, ClipRigth, ClipBottom, Radius, DoWhat)				\
{																\
	long Left    = (CenterX) - (Radius),											\
	    Top     = (CenterY) - (Radius),											\
	    Rigth   = (CenterX) + (Radius),											\
	    Bottom  = (CenterY) + (Radius);											\
																\
	if(Left   < (ClipLeft))		Left   = (ClipLeft);									\
	if(Top    < (ClipTop))		Top    = (ClipTop);									\
	if(Rigth  > (ClipRigth))	Rigth  = (ClipRigth);									\
	if(Bottom > (ClipBottom))	Bottom = (ClipBottom);									\
																\
	if(Left < (ClipRigth) && Top < (ClipBottom) && Rigth > (ClipLeft) && Bottom > (ClipTop))				\
	{															\
		ushort XPointer, YPointer;												\
																\
		for(XPointer = Left; XPointer <= Rigth; XPointer++)								\
			for(YPointer = Top; YPointer <= Bottom; YPointer++)							\
			{													\
				DoWhat;												\
			}													\
	}															\
}

#include <fstream>

#include "dynarray.h"
#include "typedef.h"
#include "vector.h"

#include "area.h"

class levelsquare;
class vector;
class character;
class area;
class bitmap;

/* Presentation of the level class */

class level : public area
{
public:
	level(ushort, ushort, ushort);
	level(std::ifstream*, ushort);
	~level(void);
	virtual void Generate(void);
	virtual void Draw(void) const;
	virtual void FastAddCharacter(vector, character*);
	virtual void HandleCharacters(void);
	virtual void EmptyFlags(void);
	virtual levelsquare* CLevelSquare(vector Pos) const {return Map[Pos.X][Pos.Y];}
	virtual void PutPlayer(bool);
	virtual void PutPlayerAround(vector Pos);
	virtual dynarray<vector, uchar>* CKeyPoint(void) {return &KeyPoint;} //should be inspector
	virtual void GenerateTunnel(vector, vector, bool);
	virtual void PutStairs(vector);
	virtual void ExpandPossibleRoute(vector, vector, bool);
	virtual void ExpandStillPossibleRoute(vector, vector, bool);
	virtual void Save(std::ofstream*) const;
	virtual void Luxify(void);
	virtual void UpdateLOS(void);
	virtual ushort CPopulation(void) const { return Population; }
	virtual ushort CIdealPopulation(void) const { return 25; }
	virtual void GenerateNewMonsters(ushort);
	virtual void AttachPos(vector);
	virtual void CreateRandomTunnel(void);
	virtual void CreateItems(ushort);
	virtual void CreateMonsters(ushort);
	virtual vector CreateDownStairs(void);
	virtual bool MakeRoom(vector, vector, bool = true, uchar = 0);
	virtual bitmap* CFluidBuffer(void) const { return FluidBuffer; }
protected:
	levelsquare*** Map;
	bitmap* FluidBuffer;
	dynarray<vector, uchar> KeyPoint, Door;
	ushort LevelIndex;
	ushort Population;
};

#endif
