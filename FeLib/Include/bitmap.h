#ifndef __BITMAP_H__
#define __BITMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#include <string>
#include <list>

#include "typedef.h"
#include "vector2d.h"
#include "save.h"
#include "graphics.h"

class outputfile;
class inputfile;

class bitmap
{
 public:
  friend class graphics;
  friend class colorizablebitmap;
  bitmap(std::string);
  bitmap(bitmap*);
  bitmap(ushort, ushort);
  bitmap(vector2d);
  bitmap(ushort, ushort, ushort);
  bitmap(vector2d, ushort);
  bitmap(bitmap*, ushort, ushort, ushort, ushort);
  bitmap(bitmap*, vector2d, vector2d);
  ~bitmap();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Save(std::string) const;
  void PutPixel(ushort, ushort, ushort);
  void PutPixel(vector2d Pos, ushort Color) { PutPixel(Pos.X, Pos.Y, Color); }
  ushort GetPixel(ushort, ushort) const;
  ushort GetPixel(vector2d Pos) const { return GetPixel(Pos.X, Pos.Y); }

  void Fill(ushort, ushort, ushort, ushort, ushort);
  void Fill(vector2d TopLeft, ushort Width, ushort Height, ushort Color) { Fill(TopLeft.X, TopLeft.Y, Width, Height, Color); }
  void Fill(ushort X, ushort Y, vector2d FillSize, ushort Color) { Fill(X, Y, FillSize.X, FillSize.Y, Color); }
  void Fill(vector2d TopLeft, vector2d FillSize, ushort Color) { Fill(TopLeft.X, TopLeft.Y, FillSize.X, FillSize.Y, Color); }
  void Fill(ushort Color) { Fill(0, 0, XSize, YSize, Color); }

  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags = 0) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0) const { Blit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0) const { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags); }
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort Height, uchar Flags = 0) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, uchar Flags = 0) const  { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags); }
  void Blit(bitmap* Bitmap, uchar Flags = 0) const { Blit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags); }

  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort) const;
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ushort Luminance) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ushort Luminance) const { Blit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ushort Luminance) const { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance); }
  void Blit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ushort Luminance) const { Blit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort Height, ushort Luminance) const { Blit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ushort Luminance) const  { Blit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance); }
  void Blit(bitmap* Bitmap, ushort Luminance) const { Blit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance); }

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0, ushort = DEFAULT_TRANSPARENT) const;
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort Height, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, uchar Flags = 0, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags, MaskColor); }

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort, ushort = DEFAULT_TRANSPARENT) const;
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort Height, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
  void MaskedBlit(bitmap* Bitmap, ushort Luminance, ushort MaskColor = DEFAULT_TRANSPARENT) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar, ushort = DEFAULT_TRANSPARENT) const;
  void AlphaBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort Height, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const  { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Alpha, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, uchar Alpha, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Alpha, MaskColor); }

  void AlphaBlit(bitmap*, ushort, ushort, ushort = DEFAULT_TRANSPARENT) const;
  void AlphaBlit(bitmap* Bitmap, vector2d Dest, ushort MaskColor = DEFAULT_TRANSPARENT) const  { AlphaBlit(Bitmap, Dest.X, Dest.Y, MaskColor); }
  void AlphaBlit(bitmap* Bitmap, ushort MaskColor = DEFAULT_TRANSPARENT) const { AlphaBlit(Bitmap, 0, 0, MaskColor); }

  void DrawLine(ushort, ushort, ushort, ushort, ushort, bool = false);
  void DrawLine(vector2d From, ushort ToX, ushort ToY, ushort Color, bool Wide = false) { DrawLine(From.X, From.Y, ToX, ToY, Color, Wide); }
  void DrawLine(ushort FromX, ushort FromY, vector2d To, ushort Color, bool Wide = false) { DrawLine(FromX, FromY, To.X, To.Y, Color, Wide); }
  void DrawLine(vector2d From, vector2d To, ushort Color, bool Wide = false) { DrawLine(From.X, From.Y, To.X, To.Y, Color, Wide); }

  void FillWithGradient(ushort, ushort, ushort, ushort, ushort, ushort);
  void FillWithGradient(vector2d Pos, ushort Width, ushort Height, ushort Color1, ushort Color2) { FillWithGradient(Pos.X, Pos.Y, Width, Height, Color1, Color2); }
  void FillWithGradient(ushort X, ushort Y, vector2d FillSize, ushort Color1, ushort Color2) { FillWithGradient(X, Y, FillSize.X, FillSize.Y, Color1, Color2); }
  void FillWithGradient(vector2d Pos, vector2d FillSize, ushort Color1, ushort Color2) { FillWithGradient(Pos.X, Pos.Y, FillSize.X, FillSize.Y, Color1, Color2); }

  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  void DrawPolygon(vector2d, ushort, ushort, ushort, bool = true, bool = false, double = 0);
  void CreateAlphaMap(uchar);
  bool ChangeAlpha(char);
  void SetAlpha(ushort, ushort, uchar);
  void SetAlpha(vector2d Pos, uchar Alpha) { SetAlpha(Pos.X, Pos.Y, Alpha); }
  uchar GetAlpha(ushort, ushort) const;
  uchar GetAlpha(vector2d Pos) const { return GetAlpha(Pos.X, Pos.Y); }
  void Outline(ushort);
  void CreateOutlineBitmap(bitmap*, ushort);
  void FadeToScreen(void (*)(bitmap*) = 0);
 protected:
  ushort XSize, YSize;
  bool IsIndependent;
  ushort** GetImage() const { return Data.Base.Image; }
  void SetImage(ushort** What) { Data.Base.Image = What; }
  uchar** GetAlphaMap() const { return Data.Base.AlphaMap; }
  void SetAlphaMap(uchar** What) { Data.Base.AlphaMap = What; }
  bitmap* GetMotherBitmap() const { return Data.Child.MotherBitmap; }
  void SetMotherBitmap(bitmap* What) { Data.Child.MotherBitmap = What; }
  ushort GetXPos() const { return Data.Child.XPos; }
  void SetXPos(ushort What) { Data.Child.XPos = What; }
  ushort GetYPos() const { return Data.Child.YPos; }
  void SetYPos(ushort What) { Data.Child.YPos = What; }
  union data
  {
    struct base
    {
      ushort** Image;
      uchar** AlphaMap;
    } Base;
    struct child
    {
      bitmap* MotherBitmap;
      ushort XPos, YPos;
    } Child;
  } Data;
};

inline outputfile& operator<<(outputfile& SaveFile, bitmap* Bitmap)
{
  if(Bitmap)
    {
      SaveFile.Put(1);
      SaveFile << Bitmap->GetXSize() << Bitmap->GetYSize();
      Bitmap->Save(SaveFile);
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, bitmap*& Bitmap)
{
  if(SaveFile.Get())
    {
      ushort XSize, YSize;
      SaveFile >> XSize >> YSize;
      Bitmap = new bitmap(XSize, YSize);
      Bitmap->Load(SaveFile);
    }

  return SaveFile;
}

#endif
