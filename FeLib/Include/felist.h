#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "vector2d.h"
#include "felibdef.h"

class outputfile;
class inputfile;
class colorizablebitmap;
class bitmap;
class festring;
struct felistentry;
struct felistdescription;

class felist
{
 public:
  felist(const festring&, ushort = WHITE, ushort = 0);
  ~felist();
  void AddEntry(const festring&, ushort, ushort = 0, bitmap* = 0, bool = true);
  void AddEntry(const festring&, ushort, ushort, const bitmap*const*, ushort, bool = true, bool = false);
  void AddDescription(const festring&, ushort = WHITE);
  ushort Draw();
  void QuickDraw(bitmap*, ushort) const;
  void Empty();
  festring GetEntry(ushort) const;
  ushort GetColor(ushort) const;
  void SetColor(ushort, ushort);
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
  void PrintToFile(const festring&);
  void SetPos(vector2d What) { Pos = What; }
  void SetWidth(ushort What) { Width = What; }
  void SetPageLength(ushort What) { PageLength = What; }
  void SetBackColor(ushort What) { BackColor = What; }
  void SetFlags(ushort What) { Flags = What; }
  void AddFlags(ushort What) { Flags |= What; }
  void RemoveFlags(ushort What) { Flags &= ~What; }
  void SetMaskColor(ushort What) { MaskColor = What; }
  void SetUpKey(ushort What) { UpKey = What; }
  void SetDownKey(ushort What) { DownKey = What; }
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
  ushort MaskColor;
  ushort UpKey;
  ushort DownKey;
};

#endif
