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

#ifndef __LIST_H__
#define __LIST_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "vector2d.h"

class outputfile;
class inputfile;
class rawbitmap;
class bitmap;
class festring;
struct felistentry;
struct felistdescription;

typedef void (*entrydrawer)(bitmap*, vector2d, uint);

class felist
{
 public:
  felist(const festring&, color16 = WHITE, uint = 0);
  ~felist();
  void AddEntry(const festring&, color16, uint = 0, uint = NO_IMAGE, bool = true);
  void AddDescription(const festring&, color16 = WHITE);
  uint Draw();
  void QuickDraw(bitmap*, uint) const;
  void Empty();
  void EmptyDescription();
  festring GetEntry(uint) const;
  color16 GetColor(uint) const;
  void SetColor(uint, color16);
  uint GetLength() const;
  uint GetLastEntryIndex() const;
  void Load(inputfile&);
  void Save(outputfile&) const;
  bool IsEmpty() const;
  uint GetSelected() const { return Selected; }
  void SetSelected(uint What) { Selected = What; }
  void EditSelected(int What) { Selected += What; }
  bool DrawPage(bitmap*) const;
  void Pop();
  static void CreateQuickDrawFontCaches(rawbitmap*, color16, uint);
  void PrintToFile(const festring&);
  void SetPos(vector2d What) { Pos = What; }
  void SetWidth(uint What) { Width = What; }
  void SetPageLength(uint What) { PageLength = What; }
  void SetBackColor(color16 What) { BackColor = What; }
  void SetFlags(uint What) { Flags = What; }
  void AddFlags(uint What) { Flags |= What; }
  void RemoveFlags(uint What) { Flags &= ~What; }
  void SetUpKey(uint What) { UpKey = What; }
  void SetDownKey(uint What) { DownKey = What; }
  void SetEntryDrawer(entrydrawer What) { EntryDrawer = What; }
 private:
  void DrawDescription(bitmap*, vector2d, uint, color16) const;
  std::vector<felistentry*> Entry;
  std::vector<felistdescription*> Description;
  uint PageBegin;
  uint Maximum;
  uint Selected;
  vector2d Pos;
  uint Width;
  uint PageLength;
  color16 BackColor;
  uint Flags;
  uint UpKey;
  uint DownKey;
  entrydrawer EntryDrawer;
};

#endif
