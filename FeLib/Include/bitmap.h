#ifndef __BITMAP_H__
#define __BITMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "vector2d.h"
#include "felibdef.h"

class bitmap;
class outputfile;
class inputfile;

typedef void (*bitmapeditor)(bitmap*);

class bitmap
{
 public:
  bitmap(const std::string&);
  bitmap(bitmap*, uchar = 0, bool = true);
  bitmap(ushort, ushort);
  bitmap(vector2d);
  bitmap(ushort, ushort, ushort);
  bitmap(vector2d, ushort);
  ~bitmap();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Save(const std::string&) const;
  void PutPixel(ushort X, ushort Y, ushort Color) { Image[Y][X] = Color; }
  void PutPixel(vector2d Pos, ushort Color) { Image[Pos.Y][Pos.X] = Color; }
  void SafePutPixelAndResetAlpha(ushort, ushort, ushort);
  void SafePutPixelAndResetAlpha(vector2d, ushort);
  ushort GetPixel(ushort X, ushort Y) const { return Image[Y][X]; }
  ushort GetPixel(vector2d Pos) const { return Image[Pos.Y][Pos.X]; }

  void Fill(ushort, ushort, ushort, ushort, ushort);
  void Fill(vector2d TopLeft, ushort Width, ushort Height, ushort Color) { Fill(TopLeft.X, TopLeft.Y, Width, Height, Color); }
  void Fill(ushort X, ushort Y, vector2d FillSize, ushort Color) { Fill(X, Y, FillSize.X, FillSize.Y, Color); }
  void Fill(vector2d TopLeft, vector2d FillSize, ushort Color) { Fill(TopLeft.X, TopLeft.Y, FillSize.X, FillSize.Y, Color); }

  void ClearToColor(ushort);

  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags = 0) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0) const { Blit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0) const { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags); }
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, uchar Flags = 0) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, uchar Flags = 0) const  { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, uchar Flags = 0) const { Blit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags); }

  void FastBlit(bitmap*) const;

  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong) const;
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ulong Luminance) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ulong Luminance) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ulong Luminance) const { Blit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ulong Luminance) const { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance); }
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ulong Luminance) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ulong Luminance) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ulong Luminance) const  { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, ulong Luminance) const { Blit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance); }

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags, MaskColor); }

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

  void SimpleAlphaBlit(bitmap*, uchar, ushort = TRANSPARENT_COLOR) const;

  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const  { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, uchar Flags = 0, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags, MaskColor); }

  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ulong, ushort = TRANSPARENT_COLOR) const;
  void AlphaBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const  { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ulong Luminance, ushort MaskColor = TRANSPARENT_COLOR) const { AlphaBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

  void DrawLine(ushort, ushort, ushort, ushort, ushort, bool = false);
  void DrawLine(vector2d From, ushort ToX, ushort ToY, ushort Color, bool Wide = false) { DrawLine(From.X, From.Y, ToX, ToY, Color, Wide); }
  void DrawLine(ushort FromX, ushort FromY, vector2d To, ushort Color, bool Wide = false) { DrawLine(FromX, FromY, To.X, To.Y, Color, Wide); }
  void DrawLine(vector2d From, vector2d To, ushort Color, bool Wide = false) { DrawLine(From.X, From.Y, To.X, To.Y, Color, Wide); }

  void DrawVerticalLine(ushort, ushort, ushort, ushort, bool = false);
  void DrawHorizontalLine(ushort, ushort, ushort, ushort, bool = false);

  void StretchBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, char) const;
  void StretchBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, char Stretch) const { StretchBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Stretch); }
  void StretchBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, char Stretch) const { StretchBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Stretch); }
  void StretchBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, char Stretch) const { StretchBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Stretch); }
  void StretchBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, char Stretch) const { StretchBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Stretch); }
  void StretchBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, char Stretch) const { StretchBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Stretch); }
  void StretchBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, char Stretch) const { StretchBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Stretch); }
  void StretchBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, char Stretch) const  { StretchBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Stretch); }
  void StretchBlit(bitmap* Bitmap, char Stretch) const { StretchBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Stretch); }

  void DrawRectangle(ushort, ushort, ushort, ushort, ushort, bool = false);
  void DrawRectangle(vector2d TopLeft, ushort Right, ushort Bottom, ushort Color, bool Wide = false) { DrawRectangle(TopLeft.X, TopLeft.Y, Right, Bottom, Color, Wide); }
  void DrawRectangle(ushort Left, ushort Top, vector2d BottomRight, ushort Color, bool Wide = false) { DrawRectangle(Left, Top, BottomRight.X, BottomRight.Y, Color, Wide); }
  void DrawRectangle(vector2d TopLeft, vector2d BottomRight, ushort Color, bool Wide = false) { DrawRectangle(TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y, Color, Wide); }

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

inline void bitmap::SafePutPixelAndResetAlpha(ushort X, ushort Y, ushort Color)
{
  if(X < XSize && Y < YSize)
    {
      Image[Y][X] = Color;

      if(AlphaMap)
	AlphaMap[Y][X] = 255;
    }
}

inline void bitmap::SafePutPixelAndResetAlpha(vector2d Pos, ushort Color)
{
  if(Pos.X < XSize && Pos.Y < YSize)
    {
      Image[Pos.Y][Pos.X] = Color;

      if(AlphaMap)
	AlphaMap[Pos.Y][Pos.X] = 255;
    }
}

outputfile& operator<<(outputfile&, const bitmap*);
inputfile& operator>>(inputfile&, bitmap*&);

#endif
