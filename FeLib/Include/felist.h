#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "typedef.h"
#include "vector2d.h"
#include "felibdef.h"

class colorizablebitmap;
class inputfile;
class outputfile;
class bitmap;

struct felistentry
{
  felistentry() { }
  felistentry(const std::string& String, ushort Color, bool Selectable) : String(String), Color(Color), Selectable(Selectable) { }
  felistentry(const std::vector<bitmap*>&, const std::string&, ushort, bool);
  std::vector<bitmap*> Bitmap;
  std::string String;
  ushort Color;
  bool Selectable;
};

outputfile& operator<<(outputfile&, felistentry);
inputfile& operator>>(inputfile&, felistentry&);

struct felistdescription
{
  felistdescription() { }
  felistdescription(const std::string& String, ushort Color) : String(String), Color(Color) { }
  std::string String;
  ushort Color;
};

outputfile& operator<<(outputfile&, felistdescription);
inputfile& operator>>(inputfile&, felistdescription&);

class felist
{
 public:
  felist(const std::string& Topic, ushort TopicColor = 0xFFFF, ushort Maximum = 0) : Maximum(Maximum), Selected(0) { AddDescription(Topic, TopicColor); }
  ~felist();
  void AddEntry(const std::string&, ushort, bitmap* = 0, bool = true);
  //void AddEntryToPos(const std::string&, ushort, ushort, bitmap* = 0, bool = true);
  void AddEntry(const std::string&, ushort, const std::vector<bitmap*>&, bool = true);
  //void AddEntryToPos(const std::string&, ushort, ushort, const std::vector<bitmap*>&, bool = true);
  //void RemoveEntryFromPos(ushort);
  void AddDescription(const std::string&, ushort = 0xFFFF);
  ushort Draw(vector2d, ushort, ushort = 20, ushort = MAKE_RGB(0, 0, 16), bool = true, bool = true, bool = true, bool = false);
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
  bool DrawPage(bitmap*) const;
  void Pop() { Entry.pop_back(); }
 protected:
  void DrawDescription(bitmap*, vector2d, ushort, ushort) const;
  std::vector<felistentry> Entry;
  std::vector<felistdescription> Description;
  vector2d Pos;
  ushort PageBegin;
  ushort Maximum;
  ushort Selected;
  ushort Width;
  ushort PageLength;
  ushort BackColor;
  bool Selectable;
};

#endif


