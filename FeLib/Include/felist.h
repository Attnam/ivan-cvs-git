#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "typedef.h"
#include "bitmap.h"
#include "save.h"
#include "vector2d.h"

class colorizablebitmap;
class inputfile;
class outputfile;

struct felistentry
{
  felistentry() { }
  felistentry(const std::string& String, ushort Color, bool Selectable) : Bitmap(0), String(String), Color(Color), Selectable(Selectable) { }
  felistentry(bitmap* Bitmap, const std::string& String, ushort Color, bool Selectable) : Bitmap(Bitmap), String(String), Color(Color), Selectable(Selectable) { }
  bitmap* Bitmap;
  std::string String;
  ushort Color;
  bool Selectable;
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

struct felistdescription
{
  felistdescription() { }
  felistdescription(const std::string& String, ushort Color) : String(String), Color(Color) { }
  std::string String;
  ushort Color;
};

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

class felist
{
 public:
  felist(const std::string& Topic, ushort TopicColor = 0xFFFF, ushort Maximum = 0, bool InverseMode = false) : Maximum(Maximum), Selected(0), InverseMode(InverseMode) { AddDescription(Topic, TopicColor); }
  ~felist();
  void AddEntry(const std::string&, ushort, bitmap* = 0, bool = true);
  void AddEntryToPos(const std::string&, ushort, ushort, bitmap* = 0, bool = true);
  void RemoveEntryFromPos(ushort);
  void AddDescription(const std::string&, ushort = 0xFFFF);
  ushort Draw(vector2d, ushort, ushort = 20, bool = true, bool = true, bool = true, bool = false) const;
  void QuickDraw(vector2d, ushort, ushort = 20) const;
  void Empty();
  std::string GetEntry(ushort Index) const { return Entry[Index].String; }
  ushort GetColor(ushort Index) const { return Entry[Index].Color; }
  void SetColor(ushort Index, ushort What) { Entry[Index].Color = What; }
  ushort Length() const { return Entry.size(); }
  void Load(inputfile&);
  void Save(outputfile&) const;
  bool IsEmpty() const { return (Length() == 0); }
  ushort GetSelected() const { return Selected; }
  void SetSelected(ushort What) { Selected = What; }
  void EditSelected(short What) { Selected += What; }
 protected:
  void DrawDescription(bitmap*, vector2d, ushort, ushort) const;
  std::vector<felistentry> Entry;
  std::vector<felistdescription> Description;
  ushort Maximum;
  ushort Selected;
  bool InverseMode;
};

#endif


