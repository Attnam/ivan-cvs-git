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
  bitmap(bitmap*, uchar = 0, bool = true);
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
  void SafePutPixelAndResetAlpha(ushort, ushort, ushort);
  void SafePutPixelAndResetAlpha(vector2d, ushort);
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

  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, ushort, ushort, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, vector2d, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, ushort, ushort, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, ushort, ushort, vector2d, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, vector2d, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, uchar = 0, ushort = TRANSPARENT_COLOR) const;

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

  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  vector2d GetSize() const { return vector2d(XSize, YSize); }
  void DrawPolygon(vector2d, ushort, ushort, ushort, bool = true, bool = false, double = 0);
  void CreateAlphaMap(uchar);
  bool ChangeAlpha(char);
  void SetAlpha(ushort X, ushort Y, uchar Alpha) { AlphaMap[Y][X] = Alpha; }
  void SetAlpha(vector2d Pos, uchar Alpha) { AlphaMap[Pos.Y][Pos.X] = Alpha; }
  uchar GetAlpha(ushort X, ushort Y) const { return AlphaMap[Y][X]; }
  uchar GetAlpha(vector2d Pos) const { return AlphaMap[Pos.Y][Pos.X]; }

  void Outline(ushort);
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
 protected:
  ushort XSize, YSize;
  ulong XSizeTimesYSize;
  ushort** Image;
  uchar** AlphaMap;
};

outputfile& operator<<(outputfile&, const bitmap*);
inputfile& operator>>(inputfile&, bitmap*&);

#endif
