#ifndef __HSCORE_H__
#define __HSCORE_H__

#include <string>

#include "dynarray.h"
#include "typedef.h"

class highscore
{
public:
	highscore(std::string File = "HScore.dat") { Load(File); }
	void Add(long, std::string);
	void Draw(void) const;
	void Save(std::string = "HScore.dat") const;
	void Load(std::string = "HScore.dat");
private:
	dynarray<std::string> Entry;
	dynarray<long> Score;
};

#endif
