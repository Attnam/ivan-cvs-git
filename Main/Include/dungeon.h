#ifndef __DUNGEON_H__
#define __DUNGEON_H__

/* Presentation of the dungeon class */

class dungeon
{
public:
	dungeon(void)		{};
	~dungeon(void)		{};
	void Generate(void);		/* Well... This shouldn't be void,
private:				   but let's tolerate it for now. */
	uchar*** BlockMap;
	area** Area;
	uchar Areas;
};

#endif


