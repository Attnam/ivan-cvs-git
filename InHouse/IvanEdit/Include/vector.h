#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "typedef.h"

class vector2d
{
public:
        vector2d(void) {}
        vector2d(ushort X, ushort Y) : X(X), Y(Y) {}
	vector2d	operator +  (const vector2d& H) const	{ return vector2d(X + H.X, Y + H.Y); }
        vector2d	operator -  (const vector2d& H) const	{ return vector2d(X - H.X, Y - H.Y); }
	vector2d&	operator += (const vector2d& H)		{X += H.X; Y += H.Y; return *this; }
	vector2d&	operator =  (const vector2d& H)		{X = H.X; Y = H.Y; return *this; }
	bool	operator == (const vector2d& H) const	{if(X == H.X && Y == H.Y) return true; else return false; }
	bool	operator != (const vector2d& H) const	{if(X == H.X && Y == H.Y) return false; else return true; }
        ushort X, Y;
};

#endif


