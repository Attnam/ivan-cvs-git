#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"
#include "bitmap.h"
#include "save.h"

class colorizablebitmap;
class inputfile;
class outputfile;

struct felistentry
{
  felistentry() { }
  felistentry(std::string String, ushort Color, bool Selectable) : Bitmap(0), String(String), Color(Color), Selectable(Selectable) { }
  felistentry(bitmap* Bitmap, std::string String, ushort Color, bool Selectable) : Bitmap(Bitmap), String(String), Color(Color), Selectable(Selectable) { }
  bitmap* Bitmap;
  std::string String;
  ushort Color;
  bool Selectable;
};

struct felistdescription
{
  felistdescription() { }
  felistdescription(std::string String, ushort Color) : String(String), Color(Color) { }
  std::string String;
  ushort Color;
};

class felist
{
 public:
  felist(std::string Topic, ushort TopicColor = 0xFFFF, ushort PageLength = 20, ushort Maximum = 0, bool Selectable = true, bool InverseMode = false) : PageLength(PageLength), Maximum(Maximum), Selectable(Selectable), Selected(0), InverseMode(InverseMode) { AddDescription(Topic, TopicColor); }
  ~felist();
  void AddEntry(std::string, ushort, bitmap* = 0, bool = true);
  void AddDescription(std::string, ushort = 0xFFFF);
  void DrawDescription(bitmap*) const;
  ushort Draw(bool = true, bool = true, bool = false) const;
  void Empty();
  std::string GetEntry(ushort Index) { return Entry[Index].String; }
  ushort Length() const { return Entry.size(); }
  void Load(inputfile&);
  void Save(outputfile&) const;
  bool IsEmpty() { return (Length() == 0); }
  void SetSelected(ushort What) { Selected = What; }
 protected:
  std::vector<felistentry> Entry;
  std::vector<felistdescription> Description;
  ushort PageLength;
  ushort Maximum;
  ushort Selected;
  bool Selectable;
  bool InverseMode;
};

inline outputfile& operator<<(outputfile& SaveFile, felistentry Entry)
{
  SaveFile << Entry.Bitmap << Entry.String << Entry.Color << Entry.Selectable;
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, felistentry& Entry)
{
  Entry.Bitmap = 0;
  SaveFile >> Entry.Bitmap >> Entry.String >> Entry.Color >> Entry.Selectable;
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, felistdescription Desc)
{
  SaveFile << Desc.String << Desc.Color;
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, felistdescription& Desc)
{
  SaveFile >> Desc.String >> Desc.Color;
  return SaveFile;
}

#endif


