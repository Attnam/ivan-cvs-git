#ifndef __FEBOT_H__
#define __FEBOT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <set>
#include <list>

#include "festring.h"

class outputfile;
class inputfile;

class febot
{
 public:
  void Initialize(ushort);
  void Save(outputfile&) const;
  void Load(inputfile&);
  void BeTalkedTo(festring);
  void Reply(festring&) const;
  void Amnesify() { Initialize(ChainLength); }
  bool IsIgnorant() const { return WordChainSet.size() <= 1; }
 private:
  struct wordchain
  {
    wordchain(ushort ChainLength) : String(ChainLength, festring()) { }
    wordchain(const wordchain*, const festring&);
    bool operator<(const wordchain&) const;
    wordchain* GetRandomLink() const;
    std::list<festring> String;
    std::vector<wordchain*> Link;
  };
  wordchain* CreateWordChain(const wordchain*, const festring&);
  wordchain* GetControlWordChain() const;
  typedef std::set<wordchain> wordchainset;
  wordchainset WordChainSet;
  ushort ChainLength;
};

inline outputfile& operator<<(outputfile& SaveFile, const febot& Febot) { Febot.Save(SaveFile); return SaveFile; }
inline inputfile& operator>>(inputfile& SaveFile, febot& Febot) { Febot.Load(SaveFile); return SaveFile; }

#endif
