#ifndef __BITMAP_H__
#define __BITMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "vector2d.h"

class bitmap;
class outputfile;
class inputfile;
class festring;

typedef void (*bitmapeditor)(bitmap*);

class bitmap
{
 public:
  friend class cachedfont;
  bitmap(const festring&);
  bitmap(const bitmap*, int = 0, bool = true);
  bitmap(int, int);
  bitmap(vector2d);
  bitmap(int, int, color16);
  bitmap(vector2d, color16);
  ~bitmap();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Save(const festring&) const;
  void PutPixel(int X, int Y, color16 Color) { Image[Y][X] = Color; }
  void PutPixel(vector2d Pos, color16 Color) { Image[Pos.Y][Pos.X] = Color; }
  void PowerPutPixel(int, int, color16, alpha, priority);
  color16 GetPixel(int X, int Y) const { return Image[Y][X]; }
  color16 GetPixel(vector2d Pos) const { return Image[Pos.Y][Pos.X]; }

  void Fill(int, int, int, int, color16);
  void Fill(vector2d, int, int, color16);
  void Fill(int, int, vector2d, color16);
  void Fill(vector2d, vector2d, color16);

  void ClearToColor(color16);

  void NormalBlit(bitmap*, int, int, int, int, int, int, int = 0) const;
  void NormalBlit(bitmap*, vector2d, int, int, int, int, int = 0) const;
  void NormalBlit(bitmap*, int, int, vector2d, int, int, int = 0) const;
  void NormalBlit(bitmap*, int, int, int, int, vector2d, int = 0) const;
  void NormalBlit(bitmap*, vector2d, vector2d, int, int, int = 0) const;
  void NormalBlit(bitmap*, vector2d, int, int, vector2d, int = 0) const;
  void NormalBlit(bitmap*, int, int, vector2d, vector2d, int = 0) const;
  void NormalBlit(bitmap*, vector2d, vector2d, vector2d, int = 0) const;
  void NormalBlit(bitmap*, int = 0) const;

  void FastBlit(bitmap*) const;

  void LuminanceBlit(bitmap*, int, int, int, int, int, int, color24) const;
  void LuminanceBlit(bitmap*, vector2d, int, int, int, int, color24) const;
  void LuminanceBlit(bitmap*, int, int, vector2d, int, int, color24) const;
  void LuminanceBlit(bitmap*, int, int, int, int, vector2d, color24) const;
  void LuminanceBlit(bitmap*, vector2d, vector2d, int, int, color24) const;
  void LuminanceBlit(bitmap*, vector2d, int, int, vector2d, color24) const;
  void LuminanceBlit(bitmap*, int, int, vector2d, vector2d, color24) const;
  void LuminanceBlit(bitmap*, vector2d, vector2d, vector2d, color24) const;
  void LuminanceBlit(bitmap*, color24) const;

  void NormalMaskedBlit(bitmap*, int, int, int, int, int, int, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, vector2d, int, int, int, int, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, int, int, vector2d, int, int, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, int, int, int, int, vector2d, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, vector2d, vector2d, int, int, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, vector2d, int, int, vector2d, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, int, int, vector2d, vector2d, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, vector2d, vector2d, vector2d, int = 0, color16 = TRANSPARENT_COLOR) const;
  void NormalMaskedBlit(bitmap*, int = 0, color16 = TRANSPARENT_COLOR) const;

  void LuminanceMaskedBlit(bitmap*, int, int, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, vector2d, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, int, int, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, int, int, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, vector2d, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, vector2d, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, int, int, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, vector2d, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void LuminanceMaskedBlit(bitmap*, color24, color16 = TRANSPARENT_COLOR) const;

  void SimpleAlphaBlit(bitmap*, alpha, color16 = TRANSPARENT_COLOR) const;

  void AlphaBlit(bitmap*, int, int, int, int, int, int, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, int, int, int, int, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, int, int, vector2d, int, int, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, int, int, int, int, vector2d, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, int, int, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, int, int, vector2d, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, int, int, vector2d, vector2d, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, vector2d, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, color16 = TRANSPARENT_COLOR) const;

  void AlphaBlit(bitmap*, int, int, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, int, int, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, int, int, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, int, int, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, vector2d, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap*, color24, color16 = TRANSPARENT_COLOR) const;

  void DrawLine(int, int, int, int, color16, bool = false);
  void DrawLine(vector2d, int, int, color16, bool = false);
  void DrawLine(int, int, vector2d, color16, bool = false);
  void DrawLine(vector2d, vector2d, color16, bool = false);

  void DrawVerticalLine(int, int, int, color16, bool = false);
  void DrawHorizontalLine(int, int, int, color16, bool = false);

  void StretchBlit(bitmap*, int, int, int, int, int, int, int) const;
  void StretchBlit(bitmap*, vector2d, int, int, int, int, int) const;
  void StretchBlit(bitmap*, int, int, vector2d, int, int, int) const;
  void StretchBlit(bitmap*, int, int, int, int, vector2d, int) const;
  void StretchBlit(bitmap*, vector2d, vector2d, int, int, int) const;
  void StretchBlit(bitmap*, vector2d, int, int, vector2d, int) const;
  void StretchBlit(bitmap*, int, int, vector2d, vector2d, int) const;
  void StretchBlit(bitmap*, vector2d, vector2d, vector2d, int) const;
  void StretchBlit(bitmap*, int) const;

  void DrawRectangle(int, int, int, int, color16, bool = false);
  void DrawRectangle(vector2d, int, int, color16, bool = false);
  void DrawRectangle(int, int, vector2d, color16, bool = false);
  void DrawRectangle(vector2d, vector2d, color16, bool = false);

  void BlitAndCopyAlpha(bitmap*, int = 0) const;

  void MaskedPriorityBlit(bitmap*, int, int, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, int, int, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, int, int, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, int, int, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, vector2d, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void MaskedPriorityBlit(bitmap*, color24, color16 = TRANSPARENT_COLOR) const;

  void AlphaPriorityBlit(bitmap*, int, int, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, int, int, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, int, int, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, int, int, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, vector2d, int, int, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, int, int, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, int, int, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, vector2d, vector2d, vector2d, color24, color16 = TRANSPARENT_COLOR) const;
  void AlphaPriorityBlit(bitmap*, color24, color16 = TRANSPARENT_COLOR) const;

  void FastBlitAndCopyAlpha(bitmap*) const;
  int GetXSize() const { return XSize; }
  int GetYSize() const { return YSize; }
  vector2d GetSize() const { return vector2d(XSize, YSize); }
  void DrawPolygon(int, int, int, int, color16, bool = true, bool = false, double = 0);
  void CreateAlphaMap(alpha);
  bool Fade(long&, packedalpha&, int);
  void SetAlpha(int X, int Y, alpha Alpha) { AlphaMap[Y][X] = Alpha; }
  void SetAlpha(vector2d Pos, alpha Alpha) { AlphaMap[Pos.Y][Pos.X] = Alpha; }
  alpha GetAlpha(int X, int Y) const { return AlphaMap[Y][X]; }
  alpha GetAlpha(vector2d Pos) const { return AlphaMap[Pos.Y][Pos.X]; }
  void Outline(color16, alpha, priority);
  void FadeToScreen(bitmapeditor = 0);
  void CreateFlames(int, color16 = TRANSPARENT_COLOR);
  bool IsValidPos(vector2d What) const { return What.X >= 0 && What.Y >= 0 && What.X < XSize && What.Y < YSize; }
  bool IsValidPos(int X, int Y) const { return X >= 0 && Y >= 0 && X < XSize && Y < YSize; }
  void CreateSparkle(vector2d, int);
  void CreateFlies(ulong, int, int);
  void CreateLightning(ulong, color16);
  bool CreateLightning(vector2d, vector2d, int, color16);
  packedcolor16** GetImage() const { return Image; }
  packedalpha** GetAlphaMap() const { return AlphaMap; }
  static bool PixelVectorHandler(long, long);
  void FillAlpha(alpha);
  void CreatePriorityMap(priority);
  void FillPriority(priority);
  void SafeSetPriority(int, int, priority);
  void SafeSetPriority(vector2d Pos, priority What) { SafeSetPriority(Pos.X, Pos.Y, What); }
  void SafeUpdateRandMap(vector2d, bool);
  void UpdateRandMap(long, bool);
  void InitRandMap();
  vector2d RandomizePixel() const;
  void AlphaPutPixel(int, int, color16, color24, alpha);
  void AlphaPutPixel(vector2d Pos, color16 Color, color24 Luminance, alpha Alpha) { AlphaPutPixel(Pos.X, Pos.Y, Color, Luminance, Alpha); }
  void CalculateRandMap();
  alpha CalculateAlphaAverage() const;
  void ActivateFastFlag() { FastFlag = 1; }
  void DeactivateFastFlag() { FastFlag = 0; }
  void Wobble(int, bool);
  void MoveLineVertically(int, int);
  void MoveLineHorizontally(int, int);
 protected:
  int XSize, YSize;
  ulong XSizeTimesYSize : 31;
  ulong FastFlag : 1;
  packedcolor16** Image;
  packedalpha** AlphaMap;
  packedpriority** PriorityMap;
  bool* RandMap;
};

inline void bitmap::SafeUpdateRandMap(vector2d Pos, bool What)
{
  if(RandMap)
    UpdateRandMap(Pos.Y * XSize + Pos.X, What);
}

inline void bitmap::SafeSetPriority(int x, int y, priority What)
{
  if(PriorityMap)
    PriorityMap[y][x] = What;
}

outputfile& operator<<(outputfile&, const bitmap*);
inputfile& operator>>(inputfile&, bitmap*&);

class cachedfont : public bitmap
{
 public:
  cachedfont(int, int);
  cachedfont(int, int, color16);
  ~cachedfont() { delete [] MaskMap; }
  void PrintCharacter(bitmap*, int, int, int, int) const;
  void CreateMaskMap();
 private:
  packedcolor16** MaskMap;
};

#endif
