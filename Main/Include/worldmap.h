#ifndef __WORLDMAP_H__
#define __WORLDMAP_H__

class worldmap : public area
{
public:
	worldmap(void);
	~worldmap(void);
protected:
	worldmapsquare* Map;
};

#endif
