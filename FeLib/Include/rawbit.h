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
class cachedfont;
class festring;

typedef std::map<color16, std::pair<cachedfont*, cachedfont*> > fontcache;

class rawbitmap
{
 public:
  rawbitmap(const festring&);
  rawbitmap(int, int);
  ~rawbitmap();
  void Save(const festring&);

  void MaskedBlit(bitmap*, int, int, int, int, int, int, packedcolor16*) const;
  void MaskedBlit(bitmap*, vector2d, int, int, int, int, packedcolor16*) const;
  void MaskedBlit(bitmap*, int, int, vector2d, int, int, packedcolor16*) const;
  void MaskedBlit(bitmap*, int, int, int, int, vector2d, packedcolor16*) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, int, int, packedcolor16*) const;
  void MaskedBlit(bitmap*, vector2d, int, int, vector2d, packedcolor16*) const;
  void MaskedBlit(bitmap*, int, int, vector2d, vector2d, packedcolor16*) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, vector2d, packedcolor16*) const;
  void MaskedBlit(bitmap*, packedcolor16*) const;

  void Printf(bitmap*, int, int, packedcolor16, const char*, ...) const;
  void PrintfUnshaded(bitmap*, int, int, packedcolor16, const char*, ...) const;
  cachedfont* Colorize(const packedcolor16*, alpha = 255, const packedalpha* = 0) const;
  bitmap* Colorize(vector2d, vector2d, vector2d, const packedcolor16*, alpha = 255, const packedalpha* = 0, const uchar* = 0, bool = true) const;
  int GetXSize() const { return XSize; }
  int GetYSize() const { return YSize; }
  vector2d GetSize() const { return vector2d(XSize, YSize); }

  void AlterGradient(int, int, int, int, int, int, bool);
  void AlterGradient(vector2d, int, int, int, int, bool);
  void AlterGradient(int, int, vector2d, int, int, bool);
  void AlterGradient(vector2d, vector2d, int, int, bool);

  void SwapColors(int, int, int, int, int, int);
  void SwapColors(vector2d, int, int, int, int);
  void SwapColors(int, int, vector2d, int, int);
  void SwapColors(vector2d, vector2d, int, int);

  void Roll(int, int, int, int, int, int, paletteindex*);
  void Roll(vector2d, int, int, int, int, paletteindex*);
  void Roll(int, int, vector2d, int, int, paletteindex*);
  void Roll(int, int, int, int, vector2d, paletteindex*);
  void Roll(vector2d, vector2d, int, int, paletteindex*);
  void Roll(vector2d, int, int, vector2d, paletteindex*);
  void Roll(int, int, vector2d, vector2d, paletteindex*);
  void Roll(vector2d, vector2d, vector2d, paletteindex*);

  void CreateFontCache(packedcolor16);
  static bool IsMaterialColor(int Color) { return Color >= 192; }
  static int GetMaterialColorIndex(int Color) { return Color - 192 >> 4; }
  int GetMaterialColorIndex(int X, int Y) const { return PaletteBuffer[Y][X] - 192 >> 4; }
  //uchar& AccessPaletteEntry(int X, int Y) const { return PaletteBuffer[Y][X]; }
  //uchar& AccessPaletteEntry(vector2d Pos) const { return PaletteBuffer[Pos.Y * XSize + Pos.X]; }
  bool IsTransparent(vector2d) const;
  bool IsMaterialColor1(vector2d) const;
  vector2d RandomizeSparklePos(const vector2d*, vector2d*, vector2d, vector2d, int, bool*) const;
  void CopyPaletteFrom(rawbitmap*);
  void PutPixel(int X, int Y, paletteindex Color) { PaletteBuffer[Y][X] = Color; }
  void PutPixel(vector2d Pos, paletteindex Color) { PaletteBuffer[Pos.Y][Pos.X] = Color; }
  paletteindex GetPixel(int X, int Y) const { return PaletteBuffer[Y][X]; }
  paletteindex GetPixel(vector2d Pos) const { return PaletteBuffer[Pos.Y][Pos.X]; }
  void Clear();
  void NormalBlit(rawbitmap*, int, int, int, int, int, int, int = 0) const;
 protected:
  int XSize, YSize;
  uchar* Palette;
  paletteindex** PaletteBuffer;
  fontcache FontCache;
};

#endif
