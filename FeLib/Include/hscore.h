#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define HIGHSCORE_VERSION 110	// Increment this if changes make highscores incompatible

#include <vector>

#include "typedef.h"

#ifdef WIN32
#define HIGHSCORE_FILENAME "HScore.dat"
#endif

#ifdef __DJGPP__
#define HIGHSCORE_FILENAME "HScore.dat"
#endif

#ifdef USE_SDL
#define HIGHSCORE_FILENAME "/var/lib/games/ivan-highscores.scores"
#endif

class highscore
{
 public:
  highscore(const std::string& File = HIGHSCORE_FILENAME) : LastAdd(0xFF) { Load(File); }
  bool Add(long, const std::string&);
  void Draw() const;
  void Save(const std::string& = HIGHSCORE_FILENAME) const;
  void Load(const std::string& = HIGHSCORE_FILENAME);
 private:
  std::vector<std::string> Entry;
  std::vector<long> Score;
  uchar LastAdd;
};

#endif
