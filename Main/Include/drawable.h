#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "vector.h"

class bitmap;

class drawable
{
public:
	virtual void DrawToTileBuffer(void) const = 0;
protected:
	virtual bitmap* GetGraphicsContainer(void) const { return 0; } //GGG
	virtual vector GetBitmapPos(void) const = 0;
};

#endif
