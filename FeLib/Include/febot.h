#ifndef __FEBOT_H__
#define __FEBOT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <set>

#include "festring.h"

class outputfile;
class inputfile;

class febot
{
 public:
  febot();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void BeTalkedTo(festring);
  void Reply(festring&) const;
  void Amnesify();
 private:
  struct fword
  {
    fword(const festring& String) : String(String) { }
    bool operator<(const fword& W) const { return String < W.String; }
    fword* GetRandomLink() const;
    festring String;
    std::vector<fword*> Link;
  };
  fword* CreateFWord(const festring&);
  fword* GetControlFWord() const;
  typedef std::set<fword> fwordset;
  fwordset FWordSet;
};

inline outputfile& operator<<(outputfile& SaveFile, const febot& Febot) { Febot.Save(SaveFile); return SaveFile; }
inline inputfile& operator>>(inputfile& SaveFile, febot& Febot) { Febot.Load(SaveFile); return SaveFile; }

#endif
