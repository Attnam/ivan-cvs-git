#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "dynarray.h"
#include "typedef.h"

class highscore
{
public:
	highscore(std::string File = "HScore.dat") : LastAdd(0xFF) { Load(File); }
	void Add(long, std::string);
	void Draw() const;
	void Save(std::string = "HScore.dat") const;
	void Load(std::string = "HScore.dat");
private:
	std::vector<std::string> Entry;
	std::vector<long> Score;
	uchar LastAdd;
};

#endif



