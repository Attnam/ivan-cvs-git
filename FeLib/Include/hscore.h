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
  bool LastAddFailed() const { return LastAdd == 100; }
  bool MergeToFile(highscore*) const;
  ulong Find(long, const festring&, time_t, long);
  const festring& GetEntry(ulong c) const { return Entry[c]; }
  long GetScore(ulong c) const { return Score[c]; }
  long GetSize() const { return Entry.size(); }
 private:
  bool Add(long, const festring&, time_t, long);
  std::vector<festring> Entry;
  std::vector<long> Score;
  std::vector<time_t> Time;
  std::vector<long> RandomID;
  uchar LastAdd;
};

#endif
