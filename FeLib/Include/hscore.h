#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "festring.h"

/* HIGH_SCORE_FILENAME defined in configure.in in Linux */

#if (defined(WIN32) || defined(__DJGPP__)) && !defined(HIGH_SCORE_FILENAME)
#define HIGH_SCORE_FILENAME CONST_S("HScore.dat")
#endif

class festring;

class highscore
{
 public:
  highscore(const festring& = HIGH_SCORE_FILENAME);
  bool Add(long, const festring&);
  void Draw() const;
  void Save(const festring& = HIGH_SCORE_FILENAME) const;
  void Load(const festring& = HIGH_SCORE_FILENAME);
  bool LastAddFailed() const { return LastAdd == 100; }
  void AddToFile(highscore*) const;
 private:
  std::vector<festring> Entry;
  std::vector<long> Score;
  uchar LastAdd;
};

#endif
