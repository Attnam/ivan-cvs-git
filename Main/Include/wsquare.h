#ifndef __WSQUARE_H__
#define __WSQUARE_H__

#include "typedef.h"
#include "vector.h"

#include "square.h"

class area;
class material;

class worldmapsquare : public square
{
public:
	worldmapsquare(area*, vector);
	~worldmapsquare(void);
	virtual void DrawToTileBuffer(void);
	virtual void UpdateMemorizedAndDraw(void);
protected:
};

#endif
