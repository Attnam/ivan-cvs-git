#ifndef __COLORBIT_H__
#define __COLORBIT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>
#include <vector>

#include "vector2d.h"

class outputfile;
class inputfile;
class bitmap;
class festring;

typedef std::map<ushort, std::pair<bitmap*, bitmap*> > fontcache;

class colorizablebitmap
{
 public:
  colorizablebitmap(const festring&);
  ~colorizablebitmap();
  void Save(const festring&);

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort*) const;
  void MaskedBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ushort*) const;
  void MaskedBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ushort*) const;
  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ushort*) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, ushort, ushort, ushort*) const;
  void MaskedBlit(bitmap*, vector2d, ushort, ushort, vector2d, ushort*) const;
  void MaskedBlit(bitmap*, ushort, ushort, vector2d, vector2d, ushort*) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, vector2d, ushort*) const;
  void MaskedBlit(bitmap*, ushort*) const;

  void Printf(bitmap*, ushort, ushort, ushort, const char*, ...) const;
  void PrintfUnshaded(bitmap*, ushort, ushort, ushort, const char*, ...) const;
  bitmap* Colorize(const ushort*, uchar = 255, const uchar* = 0) const;
  bitmap* Colorize(vector2d, vector2d, vector2d, const ushort*, uchar = 255, const uchar* = 0, const uchar* = 0) const;
  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  vector2d GetSize() const { return vector2d(XSize, YSize); }

  void AlterGradient(ushort, ushort, ushort, ushort, uchar, char, bool);
  void AlterGradient(vector2d, ushort, ushort, uchar, char, bool);
  void AlterGradient(ushort, ushort, vector2d, uchar, char, bool);
  void AlterGradient(vector2d, vector2d, uchar, char, bool);

  void SwapColors(ushort, ushort, ushort, ushort, uchar, uchar);
  void SwapColors(vector2d, ushort, ushort, uchar, uchar);
  void SwapColors(ushort, ushort, vector2d, uchar, uchar);
  void SwapColors(vector2d, vector2d, uchar, uchar);

  void Roll(ushort, ushort, ushort, ushort, short, short, uchar*);
  void Roll(vector2d, ushort, ushort, short, short, uchar*);
  void Roll(ushort, ushort, vector2d, short, short, uchar*);
  void Roll(ushort, ushort, ushort, ushort, vector2d, uchar*);
  void Roll(vector2d, vector2d, short, short, uchar*);
  void Roll(vector2d, ushort, ushort, vector2d, uchar*);
  void Roll(ushort, ushort, vector2d, vector2d, uchar*);
  void Roll(vector2d, vector2d, vector2d, uchar*);

  void CreateFontCache(ushort);
  static bool IsMaterialColor(uchar Color) { return Color >= 192; }
  bool IsTransparent(vector2d Pos) const { return PaletteBuffer[Pos.Y * XSize + Pos.X] == 191; }
  static uchar GetMaterialColorIndex(uchar Color) { return (Color - 192) >> 4; }
  uchar GetPaletteEntry(ushort X, ushort Y) const { return PaletteBuffer[Y * XSize + X]; }
  vector2d RandomizeSparklePos(const vector2d*, vector2d*, vector2d, vector2d, ushort, bool*) const;
 protected:
  ushort XSize, YSize;
  uchar* Palette;
  uchar* PaletteBuffer;
  fontcache FontCache;
};

#endif
