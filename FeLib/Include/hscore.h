/*
 *
 *  Iter Vehemens ad Necem
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with
 *  this file for more details
 *
 */

#ifndef __HSCORE_H__
#define __HSCORE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <ctime>

#include "festring.h"

#ifdef LINUX
#define HIGH_SCORE_FILENAME LOCAL_STATE_DIR "/ivan-highscore.scores"
#endif

#if defined(WIN32) || defined(__DJGPP__)
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
  bool LastAddFailed() const;
  void AddToFile(highscore*) const;
  bool MergeToFile(highscore*) const;
  int Find(long, const festring&, time_t, long);
  const festring& GetEntry(int) const;
  long GetScore(int) const;
  long GetSize() const;
 private:
  bool Add(long, const festring&, time_t, long);
  std::vector<festring> Entry;
  std::vector<long> Score;
  std::vector<time_t> Time;
  std::vector<long> RandomID;
  int LastAdd;
};

#endif
