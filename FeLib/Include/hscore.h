#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <string>

#include "typedef.h"

#if defined(WIN32) || defined(__DJGPP__)
#define HIGH_SCORE_FILENAME "HScore.dat"
#endif

#ifdef USE_SDL
#define HIGH_SCORE_FILENAME "/var/lib/games/ivan-highscores.scores"
#endif

class highscore
{
 public:
  highscore(const std::string& File = HIGH_SCORE_FILENAME) : LastAdd(0xFF) { Load(File); }
  bool Add(long, const std::string&);
  void Draw() const;
  void Save(const std::string& = HIGH_SCORE_FILENAME) const;
  void Load(const std::string& = HIGH_SCORE_FILENAME);
  bool LastAddFailed() const { return LastAdd == 100; }
 private:
  std::vector<std::string> Entry;
  std::vector<long> Score;
  uchar LastAdd;
};

#endif
