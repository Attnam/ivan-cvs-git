#ifndef __BITMAP_H__
#define __BITMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"
#include "felibdef.h"

class bitmap;
class outputfile;
class inputfile;
class festring;

typedef void (*bitmapeditor)(bitmap*);

class bitmap
{
 public:
  bitmap(const festring&);
  bitmap(const bitmap*, uchar = 0, bool = true);
  bitmap(ushort, ushort);
  bitmap(vector2d);
  bitmap(ushort, ushort, ushort);
  bitmap(vector2d, ushort);
  ~bitmap();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Save(const festring&) const;
  void PutPixel(ushort X, ushort Y, ushort Color) { Image[Y][X] = Color; }
  void PutPixel(vector2d Pos, ushort Color) { Image[Pos.Y][Pos.X] = Color; }
  void PowerPutPixel(ushort, ushort, ushort, uchar, uchar);
  ushort GetPixel(ushort X, ushort Y) const { return Image[Y][X]; }
  ushort GetPixel(vector2d Pos) const { return Image[Pos.Y][Pos.X]; }

  void Fill(ushort, ushort, ushort, ushort, ushort);
  void Fill(vector2d, ushort, ushort, ushort);
  void Fill(ushort, ushort, vector2d, ushort);
  void Fill(vector2d, vector2d, ushort);

  void ClearToColor(ushort);

  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
  void Blit(bitmap*, vector2d, ushort, ushort, ushort, ushort, uchar = 0) const;
  void Blit(bitmap*, ushort, ushort, vector2d, ushort, ushort, uchar = 0) const;
  void Blit(bitmap*, ushort, ushort, ushort, ushort, vector2d, uchar = 0) const;
  void Blit(bitmap*, vector2d, vector2d, ushort, ushort, uchar = 0) const;
  void Blit(bitmap*, vector2d, ushort, ushort, vector2d, uchar = 0) const;
  void Blit(bitmap*, ushort, ushort, vector2d, vector2d, uchar = 0) const;
  void Blit(bitmap*, vector2d, vector2d, vector2d, uchar = 0) const ;
  void Blit(bitmap*, uchar = 0) const;

  void FastBlit(bitmap*) const;

  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong) const;
  void Blit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ulong) const;
  void Blit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ulong) const;
  void Blit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ulong) const;
  void Blit(bitmap*, vector2d, vector2d, ushort, ushort, ulong) const;
  void Blit(bitmap*, vector2d, ushort, ushort, vector2d, ulong) const;
  void Blit(bitmap*, ushort, ushort, vector2d, vector2d, ulong) const;
  void Blit(bitmap*, vector2d, vector2d, vector2d, ulong) const;
  void Blit(bitmap*, ulong) const;

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, ushort, ushort, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ushort, ushort, vector2d, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, uchar = 0, ushort = TRANSPARENT_COLOR) const;

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ushort, ushort, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, vector2d, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap*, ulong, ushort = TRANSPARENT_COLOR) const;

  void SimpleAlphaBlit(bitmap*, uchar, ushort = TRANSPARENT_COLOR) const;

  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, ushort, ushort, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, ushort, ushort, vector2d, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, vector2d, vector2d, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, vector2d, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort = TRANSPARENT_COLOR) const;

  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ulong, ushort = TRANSPARENT_COLOR) const;

  void DrawLine(ushort, ushort, ushort, ushort, ushort, bool = false);
  void DrawLine(vector2d, ushort, ushort, ushort, bool = false);
  void DrawLine(ushort, ushort, vector2d, ushort, bool = false);
  void DrawLine(vector2d, vector2d, ushort, bool = false);

  void DrawVerticalLine(ushort, ushort, ushort, ushort, bool = false);
  void DrawHorizontalLine(ushort, ushort, ushort, ushort, bool = false);

  void StretchBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, char) const;
  void StretchBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, char) const;
  void StretchBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, char) const;
  void StretchBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, char) const;
  void StretchBlit(bitmap*, vector2d, vector2d, ushort, ushort, char) const;
  void StretchBlit(bitmap*, vector2d, ushort, ushort, vector2d, char) const;
  void StretchBlit(bitmap*, ushort, ushort, vector2d, vector2d, char) const;
  void StretchBlit(bitmap*, vector2d, vector2d, vector2d, char) const;
  void StretchBlit(bitmap*, char) const;

  void DrawRectangle(ushort, ushort, ushort, ushort, ushort, bool = false);
  void DrawRectangle(vector2d, ushort, ushort, ushort, bool = false);
  void DrawRectangle(ushort, ushort, vector2d, ushort, bool = false);
  void DrawRectangle(vector2d, vector2d, ushort, bool = false);

  void BlitAndCopyAlpha(bitmap*, uchar = 0) const;

  void MaskedPriorityBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, ushort, ushort, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, ulong, ushort = TRANSPARENT_COLOR) const;

  void AlphaPriorityBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, vector2d, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, ushort, ushort, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, ushort, ushort, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, vector2d, vector2d, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, ulong, ushort = TRANSPARENT_COLOR) const;

  void FastBlitAndCopyAlpha(bitmap*) const;
  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  vector2d GetSize() const { return vector2d(XSize, YSize); }
  void DrawPolygon(ushort, ushort, ushort, ushort, ushort, bool = true, bool = false, double = 0);
  void CreateAlphaMap(uchar);
  bool Fade(ulong&, uchar&, uchar);
  void SetAlpha(ushort X, ushort Y, uchar Alpha) { AlphaMap[Y][X] = Alpha; }
  void SetAlpha(vector2d Pos, uchar Alpha) { AlphaMap[Pos.Y][Pos.X] = Alpha; }
  uchar GetAlpha(ushort X, ushort Y) const { return AlphaMap[Y][X]; }
  uchar GetAlpha(vector2d Pos) const { return AlphaMap[Pos.Y][Pos.X]; }
  void Outline(ushort, uchar, uchar);
  void CreateOutlineBitmap(bitmap*, ushort);
  void FadeToScreen(bitmapeditor = 0);
  void CreateFlames(ushort, ushort = TRANSPARENT_COLOR);
  bool IsValidPos(vector2d What) const { return What.X >= 0 && What.Y >= 0 && What.X < XSize && What.Y < YSize; }
  bool IsValidPos(short X, short Y) const { return X >= 0 && Y >= 0 && X < XSize && Y < YSize; }
  void CreateSparkle(vector2d, ushort);
  void CreateFlies(ulong, ushort, uchar);
  void CreateLightning(ulong, ushort);
  bool CreateLightning(vector2d, vector2d, ushort, ushort);
  ushort** GetImage() const { return Image; }
  uchar** GetAlphaMap() const { return AlphaMap; }
  static bool PixelVectorHandler(long, long);
  void FillAlpha(uchar);
  void CreatePriorityMap(uchar);
  void FillPriority(uchar);
  void SafeSetPriority(ushort, ushort, uchar);
  void SafeSetPriority(vector2d Pos, uchar What) { SafeSetPriority(Pos.X, Pos.Y, What); }
  void SafeUpdateRandMap(vector2d, bool);
  void UpdateRandMap(ulong, bool);
  void InitRandMap();
  vector2d RandomizePixel() const;
  void AlphaPutPixel(ushort, ushort, ushort, ulong, uchar);
  void AlphaPutPixel(vector2d Pos, ushort Color, ulong Luminance, uchar Alpha) { AlphaPutPixel(Pos.X, Pos.Y, Color, Luminance, Alpha); }
  void CalculateRandMap();
  uchar CalculateAlphaAverage() const;
 protected:
  ushort XSize, YSize;
  ulong XSizeTimesYSize;
  ushort** Image;
  uchar** AlphaMap;
  uchar** PriorityMap;
  bool* RandMap;
};

inline void bitmap::SafeUpdateRandMap(vector2d Pos, bool What)
{
  if(RandMap)
    UpdateRandMap(Pos.Y * XSize + Pos.X, What);
}

inline void bitmap::SafeSetPriority(ushort x, ushort y, uchar What)
{
  if(PriorityMap)
    PriorityMap[y][x] = What;
}

outputfile& operator<<(outputfile&, const bitmap*);
inputfile& operator>>(inputfile&, bitmap*&);

#endif
