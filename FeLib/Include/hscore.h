#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <string>

#include "typedef.h"

#if (defined(WIN32) || defined(__DJGPP__)) && !defined(HIGH_SCORE_FILENAME)
#define HIGH_SCORE_FILENAME "HScore.dat"
#endif

// HIGH_SCORE_FILENAME defined in configure.in in Linux

class highscore
{
 public:
  highscore(const std::string& = HIGH_SCORE_FILENAME);
  bool Add(long, const std::string&);
  void Draw() const;
  void Save(const std::string& = HIGH_SCORE_FILENAME) const;
  void Load(const std::string& = HIGH_SCORE_FILENAME);
  bool LastAddFailed() const { return LastAdd == 100; }
  void AddToFile(highscore*) const;
 private:
  std::vector<std::string> Entry;
  std::vector<long> Score;
  uchar LastAdd;
};

#endif
