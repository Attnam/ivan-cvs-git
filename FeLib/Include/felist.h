#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <vector>

#include "vector2d.h"
#include "felibdef.h"

class outputfile;
class inputfile;
class colorizablebitmap;
class bitmap;
struct felistentry;
struct felistdescription;

class felist
{
 public:
  felist();
  felist(const felist&);
  felist(const std::string&, ushort = WHITE, ushort = 0);
  ~felist();
  void AddEntry(const std::string&, ushort, ushort = 0, bitmap* = 0, bool = true);
  void AddEntry(const std::string&, ushort, ushort, const std::vector<bitmap*>&, bool = true);
  void AddDescription(const std::string&, ushort = WHITE);
  ushort Draw();
  void QuickDraw(vector2d, ushort, ushort = 20) const;
  void Empty();
  std::string GetEntry(ushort Index) const;
  ushort GetColor(ushort Index) const;
  void SetColor(ushort Index, ushort What);
  ushort GetLength() const;
  ushort GetLastEntryIndex() const;
  void Load(inputfile&);
  void Save(outputfile&) const;
  bool IsEmpty() const;
  ushort GetSelected() const { return Selected; }
  void SetSelected(ushort What) { Selected = What; }
  void EditSelected(short What) { Selected += What; }
  bool DrawPage(bitmap*) const;
  void Pop();
  static void CreateQuickDrawFontCaches(colorizablebitmap*, ushort, ushort);
  void PrintToFile(const std::string&);
  void SetPos(vector2d What) { Pos = What; }
  void SetWidth(ushort What) { Width = What; }
  void SetPageLength(ushort What) { PageLength = What; }
  void SetBackColor(ushort What) { BackColor = What; }
  void SetFlags(ushort What) { Flags = What; }
  void AddFlags(ushort What) { Flags |= What; }
  void RemoveFlags(ushort What) { Flags &= ~What; }
 private:
  void DrawDescription(bitmap*, vector2d, ushort, ushort) const;
  std::vector<felistentry*> Entry;
  std::vector<felistdescription*> Description;
  ushort PageBegin;
  ushort Maximum;
  ushort Selected;
  vector2d Pos;
  ushort Width;
  ushort PageLength;
  ushort BackColor;
  ushort Flags;
};

#endif
