#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "typedef.h"

class vector
{
public:
        vector(void) {}
        vector(ushort X, ushort Y) : X(X), Y(Y) {}
	vector	operator +  (const vector& H) const	{ return vector(X + H.X, Y + H.Y); }
	vector&	operator += (const vector& H)		{X += H.X; Y += H.Y; return *this; }
	vector&	operator =  (const vector& H)		{X = H.X; Y = H.Y; return *this; }
	bool	operator == (const vector& H) const	{if(X == H.X && Y == H.Y) return true; else return false; }
	bool	operator != (const vector& H) const	{if(X == H.X && Y == H.Y) return false; else return true; }
        ushort X, Y;
};

#endif


