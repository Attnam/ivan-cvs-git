#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

class vector2d
{
public:
        vector2d()						{}
        vector2d(ushort X, ushort Y) : X(X), Y(Y)		{}
	vector2d	operator +   (vector2d H) const		{ return vector2d(X + H.X, Y + H.Y); }
	vector2d&	operator +=  (vector2d H)		{ X += H.X; Y += H.Y; return *this; }
	vector2d	operator -   (vector2d H) const		{ return vector2d(X - H.X, Y - H.Y); }
	vector2d&	operator -=  (vector2d H)		{ X -= H.X; Y -= H.Y; return *this; }
        vector2d	operator *   (ushort H) const		{ return vector2d(ushort(X * H), ushort(Y * H)); }
        vector2d&	operator *=  (ushort H)			{ X *= H; Y *= H; return *this; }
        vector2d	operator /   (ushort H) const		{ return vector2d(ushort(X / H), ushort(Y / H)); }
        vector2d&	operator /=  (ushort H)			{ X /= H; Y /= H; return *this; }
        vector2d	operator *   (float H) const		{ return vector2d(ushort(X * H), ushort(Y * H)); }
        vector2d&	operator *=  (float H)			{ X = ushort(X * H); Y = ushort(Y * H); return *this; }
        vector2d	operator /   (float H) const		{ return vector2d(ushort(X / H), ushort(Y / H)); }
        vector2d&	operator /=  (float H)			{ X = ushort(X / H); Y = ushort(Y / H); return *this; }
	bool		operator ==  (vector2d H) const		{ if(X == H.X && Y == H.Y) return true; else return false; }
	bool		operator !=  (vector2d H) const		{ if(X == H.X && Y == H.Y) return false; else return true; }
	vector2d	operator <<  (uchar Shift) const	{ return vector2d(X << Shift, Y << Shift); }
	vector2d&	operator <<= (uchar Shift)		{ X <<= Shift; Y <<= Shift; return *this; }
	vector2d	operator >>  (uchar Shift) const	{ return vector2d(X >> Shift, Y >> Shift); }
	vector2d&	operator >>= (uchar Shift)		{ X >>= Shift; Y >>= Shift; return *this; }
        ushort X, Y;
};

#endif



