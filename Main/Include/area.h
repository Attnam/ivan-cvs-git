#ifndef __AREA_H__
#define __AREA_H__

#define FORBIDDEN		1
#define ON_POSSIBLE_ROUTE	2
#define STILL_ON_POSSIBLE_ROUTE	4
#define PREFERRED		8

#define DO_FOR_SQUARES_AROUND(X, Y, BorderX, BorderY, Then)		\
{									\
	while(true)							\
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
									\
		break;							\
	}								\
}

#define DO_FOR_SQUARES_AROUND_IN_TWO_PARTS(X, Y, BorderX, BorderY, FirstCommand, SecondCommand, CommonCommand)\
{									\
	while(true)							\
	{								\
		ushort DoX, DoY, DoIndex = 0;				\
									\
		if(X && Y)						\
		{							\
			DoX = X - 1; DoY = Y - 1;			\
									\
			FirstCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(Y)							\
		{							\
			DoX = X; DoY = Y - 1;				\
									\
			FirstCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(X < BorderX - 1 && Y)				\
		{							\
			DoX = X + 1; DoY = Y - 1;			\
									\
			FirstCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(X)							\
		{							\
			DoX = X - 1; DoY = Y;				\
									\
			FirstCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(X < BorderX - 1)					\
		{							\
			DoX = X + 1; DoY = Y;				\
									\
			SecondCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(X && Y < BorderY - 1)				\
		{							\
			DoX = X - 1; DoY = Y + 1;			\
									\
			SecondCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(Y < BorderY - 1)					\
		{							\
			DoX = X; DoY = Y + 1;				\
									\
			SecondCommand;					\
			CommonCommand;					\
		}							\
									\
		DoIndex++;						\
									\
		if(X < BorderX - 1 && Y < BorderY - 1)			\
		{							\
			DoX = X + 1; DoY = Y + 1;			\
									\
			SecondCommand;					\
			CommonCommand;					\
		}							\
									\
		break;							\
	}								\
}

#define DO_RECTANGLE(CenterX, CenterY, ClipLeft, ClipTop, ClipRigth, ClipBottom, Radius, DoOne, DoTwo)				\
{																\
	long	Left    = (CenterX) - (Radius),											\
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
		ushort XPointer, YPointer;											\
																\
		for(XPointer = Left; XPointer <= Rigth; XPointer++)								\
		{														\
			DoOne;													\
		}														\
																\
		for(YPointer = Top; YPointer <= Bottom; YPointer++)								\
		{														\
			DoTwo;													\
		}														\
	}															\
}

/*#define DO_BIG_RECTANGLE(ClipLeft, ClipTop, ClipRigth, ClipBottom, DoOne, DoTwo)						\
{																\
	ushort XPointer, YPointer;												\
																\
	for(XPointer = (ClipLeft); XPointer <= (ClipRigth); XPointer++)								\
		DoOne;														\
																\
	for(YPointer = (ClipTop); YPointer <= (ClipBottom); YPointer++)								\
		DoTwo;														\
}*/

#define DO_FILLED_RECTANGLE(CenterX, CenterY, ClipLeft, ClipTop, ClipRigth, ClipBottom, Radius, DoWhat)				\
{																\
	long	Left    = (CenterX) - (Radius),											\
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
		ushort XPointer, YPointer;											\
																\
		for(XPointer = Left; XPointer <= Rigth; XPointer++)								\
			for(YPointer = Top; YPointer <= Bottom; YPointer++)							\
			{													\
				DoWhat;												\
			}													\
	}															\
}

#include "typedef.h"
#include "vector2d.h"

class character;
class square;
class bitmap;
class outputfile;
class inputfile;

class area
{
public:
	area(ushort, ushort);
	area() {}
	virtual ~area();
	virtual void Draw() const = 0;
	virtual void AddCharacter(vector2d, character*);
	virtual void RemoveCharacter(vector2d);
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual ushort GetFlag(vector2d Pos) const { return FlagMap[Pos.X][Pos.Y]; }
	virtual square* operator [] (vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
	virtual square* GetSquare(vector2d Pos) const { return Map[Pos.X][Pos.Y]; }
	virtual square* GetSquare(ushort x, ushort y) const { return Map[x][y]; }
	virtual ushort GetXSize() const { return XSize; }
	virtual ushort GetYSize() const { return YSize; }
	virtual void UpdateLOS();
	virtual void EmptyFlags();
	virtual void SendNewDrawRequest();
	virtual void Initialize(ushort, ushort);
protected:
	square*** Map;
	ushort** FlagMap;
	ushort XSize, YSize;
	ulong XSizeTimesYSize;
};

inline outputfile& operator<<(outputfile& SaveFile, area* Area)
{
	Area->Save(SaveFile);
	return SaveFile;
}

#endif
