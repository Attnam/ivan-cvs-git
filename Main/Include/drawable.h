#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "vector2d.h"

class bitmap;

class drawable
{
public:
	virtual void DrawToTileBuffer() const = 0;
protected:
	virtual bitmap* GetGraphicsContainer() const { return 0; } //GGG
	virtual vector2d GetBitmapPos() const = 0;
};

#endif
