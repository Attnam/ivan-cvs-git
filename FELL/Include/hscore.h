#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define HIGHSCORE_VERSION 110

#include <string>
#include <vector>

#include "dynarray.h"
#include "typedef.h"

#ifdef WIN32
#define HIGHSCORE_FILENAME "HScore.dat"
#else
#define HIGHSCORE_FILENAME "/var/lib/games/ivan-highscores.scores"
#endif

class highscore
{
public:
	highscore(std::string File = HIGHSCORE_FILENAME) : LastAdd(0xFF) { Load(File); }
	void Add(long, std::string);
	void Draw() const;
	void Save(std::string = HIGHSCORE_FILENAME) const;
	void Load(std::string = HIGHSCORE_FILENAME);
private:
	std::vector<std::string> Entry;
	std::vector<long> Score;
	uchar LastAdd;
};

#endif



