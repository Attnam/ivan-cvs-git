#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

#include "typedef.h"

class vector2d
{
public:
        vector2d() {}
        vector2d(ushort X, ushort Y) : X(X), Y(Y) {}
	vector2d	operator +  (const vector2d& H) const	{ return vector2d(X + H.X, Y + H.Y); }
	vector2d&	operator += (const vector2d& H)		{ X += H.X; Y += H.Y; return *this; }
	vector2d	operator -  (const vector2d& H) const	{ return vector2d(X - H.X, Y - H.Y); }
	vector2d&	operator -= (const vector2d& H)		{ X -= H.X; Y -= H.Y; return *this; }
	vector2d&	operator =  (const vector2d& H)		{ X = H.X; Y = H.Y; return *this; }
	bool	operator == (const vector2d& H) const	{ if(X == H.X && Y == H.Y) return true; else return false; }
	bool	operator != (const vector2d& H) const	{ if(X == H.X && Y == H.Y) return false; else return true; }
	vector2d	operator << (const uchar Shift) const	{ return vector2d(X << Shift, Y << Shift); }
	vector2d	operator >> (const uchar Shift) const	{ return vector2d(X >> Shift, Y >> Shift); }
        ushort X, Y;
};

#endif


