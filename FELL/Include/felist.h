#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"
#include "save.h"

class colorizablebitmap;
class inputfile;
class outputfile;
class bitmap;

struct felistentry
{
  felistentry() {}
  felistentry(std::string String, ushort Color) : String(String), Color(Color) {}
  std::string String;
  ushort Color;
};

class felist
{
 public:
  felist(ushort Maximum = 0, bool DrawLetters = true, bool InverseMode = false) : Maximum(Maximum), DrawLetters(DrawLetters), InverseMode(InverseMode) {}
  felist(std::string Topic, ushort TopicColor = 0xFFFF, ushort Maximum = 0, bool DrawLetters = true, bool InverseMode = false) : Maximum(Maximum), DrawLetters(DrawLetters), InverseMode(InverseMode) { AddDescription(Topic, TopicColor); }
  void AddEntry(std::string, ushort);
  void AddDescription(std::string Str, ushort Color = 0xFFFF) { Description.push_back(felistentry(Str, Color)); }
  void DrawDescription(bitmap*) const;
  ushort Draw(bool = true, bool = false) const;
  void Empty();
  std::string GetEntry(ushort Index) { return Entry[Index].String; }
  ushort Length() const { return Entry.size(); }
  void Load(inputfile&);
  void Save(outputfile&) const;
  bool IsEmpty() { return (Length() == 0); }
 protected:
  std::vector<felistentry> Entry;
  std::vector<felistentry> Description;
  ushort Maximum;
  bool DrawLetters;
  bool InverseMode;
};

inline outputfile& operator<<(outputfile& SaveFile, felistentry Entry)
{
  SaveFile << Entry.String << Entry.Color;
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, felistentry& Entry)
{
  SaveFile >> Entry.String >> Entry.Color;
  return SaveFile;
}

#endif
