#ifndef __AREA_H__
#define __AREA_H__

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

#include "typedef.h"
#include "vector.h"

class character;
class square;
class bitmap;

class area
{
public:
	area(ushort, ushort);
	area(void) {}
	virtual ~area(void);
	virtual void Generate(void) = 0;
	virtual void Draw(void) const = 0;
	virtual void AddCharacter(vector, character*);
	virtual void RemoveCharacter(vector);
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual ushort GetFlag(vector Pos) const { return FlagMap[Pos.X][Pos.Y]; }
	virtual square* operator [] (vector Pos) const { return Map[Pos.X][Pos.Y]; }
	virtual square* GetSquare(vector Pos) const { return Map[Pos.X][Pos.Y]; }
	virtual ushort GetXSize(void) const { return XSize; }
	virtual ushort GetYSize(void) const { return YSize; }
	virtual bitmap* GetMemorized(void) const { return Memorized; }
	virtual void UpdateLOS(void);
	virtual void EmptyFlags(void);
protected:
	square*** Map;
	bitmap* Memorized;
	ushort** FlagMap;
	ushort XSize, YSize;
	ulong XSizeTimesYSize;
};

#endif

