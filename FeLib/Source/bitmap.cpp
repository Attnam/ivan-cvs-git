#include <cmath>
#include <ctime>

#include "bitmap.h"
#include "graphics.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"

/* Blitting must be as fast as possible, even if no optimizations are used;
 * therefore we can't use inline functions inside loops, since they may be left
 * unexpanded. These macros will do the job efficiently, even if they are rather ugly */

#define LOAD_SRC() int SrcCol = *SrcPtr;
#define LOAD_DEST() int DestCol = *DestPtr;
#define LOAD_ALPHA() int Alpha = *AlphaPtr;

#define STORE_COLOR() *DestPtr = Red | Green | Blue;

#define NEW_LUMINATE_RED()\
int Red = (SrcCol & 0xF800) + NewRedLuminance;\
\
if(Red >= 0)\
  {\
    if(Red > 0xF800)\
      Red = 0xF800;\
  }\
else\
  Red = 0;

#define NEW_LUMINATE_GREEN()\
int Green = (SrcCol & 0x7E0) + NewGreenLuminance;\
\
if(Green >= 0)\
  {\
    if(Green > 0x7E0)\
      Green = 0x7E0;\
  }\
else\
  Green = 0;

#define NEW_LUMINATE_BLUE()\
int Blue = (SrcCol & 0x1F) + NewBlueLuminance;\
\
if(Blue >= 0)\
  {\
    if(Blue > 0x1F)\
      Blue = 0x1F;\
  }\
else\
  Blue = 0;

#define NEW_APPLY_ALPHA_RED()\
{\
  int DestRed = (DestCol & 0xF800);\
  Red = (((Red - DestRed) * Alpha >> 8) + DestRed) & 0xF800;\
}
#define NEW_APPLY_ALPHA_GREEN()\
{\
  int DestGreen = (DestCol & 0x7E0);\
  Green = (((Green - DestGreen) * Alpha >> 8) + DestGreen) & 0x7E0;\
}
#define NEW_APPLY_ALPHA_BLUE()\
{\
  int DestBlue = (DestCol & 0x1F);\
  Blue = ((Blue - DestBlue) * Alpha >> 8) + DestBlue;\
}

#define NEW_LOAD_AND_APPLY_ALPHA_RED()\
int Red;\
{\
  int DestRed = DestCol & 0xF800;\
  Red = ((((SrcCol & 0xF800) - DestRed) * Alpha >> 8) + DestRed) & 0xF800;\
}

#define NEW_LOAD_AND_APPLY_ALPHA_GREEN()\
int Green;\
{\
  int DestGreen = DestCol & 0x7E0;\
  Green = ((((SrcCol & 0x7E0) - DestGreen) * Alpha >> 8) + DestGreen) & 0x7E0;\
}

#define NEW_LOAD_AND_APPLY_ALPHA_BLUE()\
int Blue;\
{\
  int DestBlue = DestCol & 0x1F;\
  Blue = (((SrcCol & 0x1F) - DestBlue) * Alpha >> 8) + DestBlue;\
}

bitmap::bitmap(const festring& FileName) : FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0)
{
  inputfile File(FileName.CStr(), 0, false);

  if(!File.IsOpen())
    ABORT("Bitmap %s not found!", FileName.CStr());

  uchar Palette[768];
  File.SeekPosEnd(-768);
  File.Read(reinterpret_cast<char*>(Palette), 768);
  File.SeekPosBegin(8);
  XSize  =  File.Get();
  XSize += (File.Get() << 8) + 1;
  YSize  =  File.Get();
  YSize += (File.Get() << 8) + 1;
  XSizeTimesYSize = XSize * YSize;
  File.SeekPosBegin(128);
  Alloc2D(Image, YSize, XSize);
  packedcolor16* Buffer = Image[0];

  for(int y = 0; y < YSize; ++y)
    for(int x = 0; x < XSize; ++x)
      {
	int Char1 = File.Get();

	if(Char1 > 192)
	  {
	    int Char2 = File.Get();
	    --x;

	    for(; Char1 > 192; --Char1)
	      {
		*Buffer++ = int(Palette[Char2 + (Char2 << 1)] >> 3) << 11 | int(Palette[Char2 + (Char2 << 1) + 1] >> 2) << 5 | int(Palette[Char2 + (Char2 << 1) + 2] >> 3);

		if(++x == XSize)
		  {
		    x = 0;
		    ++y;
		  }
	      }
	  }
	else
	  *Buffer++ = int(Palette[Char1 + (Char1 << 1)] >> 3) << 11 | int(Palette[Char1 + (Char1 << 1) + 1] >> 2) << 5 | int(Palette[Char1 + (Char1 << 1) + 2] >> 3);
      }
}

bitmap::bitmap(const bitmap* Bitmap, int Flags, bool CopyAlpha) : XSize(Bitmap->XSize), YSize(Bitmap->YSize), XSizeTimesYSize(Bitmap->XSizeTimesYSize), FastFlag(0), PriorityMap(0), RandMap(0)
{
  Alloc2D(Image, YSize, XSize);

  if(CopyAlpha && Bitmap->AlphaMap)
    {
      Alloc2D(AlphaMap, YSize, XSize);
      Bitmap->BlitAndCopyAlpha(this, Flags);
    }
  else
    {
      AlphaMap = 0;

      if(!Flags)
	Bitmap->FastBlit(this);
      else
	Bitmap->NormalBlit(this, Flags);
    }
}

bitmap::bitmap(int XSize, int YSize) : XSize(XSize), YSize(YSize), XSizeTimesYSize(XSize * YSize), FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0)
{
  Alloc2D(Image, YSize, XSize);
}

bitmap::bitmap(vector2d Size) : XSize(Size.X), YSize(Size.Y), XSizeTimesYSize(XSize * YSize), FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0)
{
  Alloc2D(Image, YSize, XSize);
}

bitmap::bitmap(int XSize, int YSize, color16 Color) : XSize(XSize), YSize(YSize), XSizeTimesYSize(XSize * YSize), FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0)
{
  Alloc2D(Image, YSize, XSize);
  ClearToColor(Color);
}

bitmap::bitmap(vector2d Size, color16 Color) : XSize(Size.X), YSize(Size.Y), XSizeTimesYSize(XSize * YSize), FastFlag(0), AlphaMap(0), PriorityMap(0), RandMap(0)
{
  Alloc2D(Image, YSize, XSize);
  ClearToColor(Color);
}

bitmap::~bitmap()
{
  delete [] Image;
  delete [] AlphaMap;
  delete [] PriorityMap;
  delete [] RandMap;
}

void bitmap::Save(outputfile& SaveFile) const
{
  SaveFile.Write(reinterpret_cast<char*>(Image[0]), XSizeTimesYSize * sizeof(packedcolor16));

  if(AlphaMap)
    {
      SaveFile << bool(true);
      SaveFile.Write(reinterpret_cast<char*>(AlphaMap[0]), XSizeTimesYSize * sizeof(packedalpha));
    }
  else
    SaveFile << bool(false);

  if(PriorityMap)
    {
      SaveFile << bool(true);
      SaveFile.Write(reinterpret_cast<char*>(PriorityMap[0]), XSizeTimesYSize * sizeof(packedpriority));
    }
  else
    SaveFile << bool(false);

  SaveFile << uchar(FastFlag);
}

void bitmap::Load(inputfile& SaveFile)
{
  SaveFile.Read(reinterpret_cast<char*>(Image[0]), XSizeTimesYSize * sizeof(packedcolor16));

  if(ReadType<bool>(SaveFile))
    {
      Alloc2D(AlphaMap, YSize, XSize);
      SaveFile.Read(reinterpret_cast<char*>(AlphaMap[0]), XSizeTimesYSize * sizeof(packedalpha));
    }

  if(ReadType<bool>(SaveFile))
    {
      Alloc2D(PriorityMap, YSize, XSize);
      SaveFile.Read(reinterpret_cast<char*>(PriorityMap[0]), XSizeTimesYSize * sizeof(packedpriority));
    }

  FastFlag = ReadType<uchar>(SaveFile);
}

void bitmap::Save(const festring& FileName) const
{
  static char BMPHeader[] =	{char(0x42), char(0x4D), char(0xB6), char(0x4F), char(0x12), char(0x00),
				 char(0x00), char(0x00), char(0x00), char(0x00), char(0x36), char(0x00),
				 char(0x00), char(0x00), char(0x28), char(0x00), char(0x00), char(0x00),
				 char(0x20), char(0x03), char(0x00), char(0x00), char(0xF4), char(0x01),
				 char(0x00), char(0x00), char(0x01), char(0x00), char(0x18), char(0x00),
				 char(0x00), char(0x00), char(0x00), char(0x00), char(0x80), char(0x4F),
				 char(0x12), char(0x00), char(0x33), char(0x0B), char(0x00), char(0x00),
				 char(0x33), char(0x0B), char(0x00), char(0x00), char(0x00), char(0x00),
				 char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00)};

  outputfile SaveFile(FileName);
  BMPHeader[0x12] =  XSize       & 0xFF;
  BMPHeader[0x13] = (XSize >> 8) & 0xFF;
  BMPHeader[0x16] =  YSize       & 0xFF;
  BMPHeader[0x17] = (YSize >> 8) & 0xFF;
  SaveFile.Write(BMPHeader, 0x36);

  for(int y = YSize - 1; y >= 0; --y)
    for(int x = 0; x < XSize; ++x)
      {
	color16 Pixel = GetPixel(x, y);
	SaveFile << char(Pixel << 3) << char((Pixel >> 5) << 2) << char((Pixel >> 11) << 3);
      }
}

void bitmap::Fill(vector2d TopLeft, int Width, int Height, color16 Color) { Fill(TopLeft.X, TopLeft.Y, Width, Height, Color); }
void bitmap::Fill(int X, int Y, vector2d FillSize, color16 Color) { Fill(X, Y, FillSize.X, FillSize.Y, Color); }
void bitmap::Fill(vector2d TopLeft, vector2d FillSize, color16 Color) { Fill(TopLeft.X, TopLeft.Y, FillSize.X, FillSize.Y, Color); }

void bitmap::Fill(int X, int Y, int Width, int Height, color16 Color)
{
  if(X >= XSize || Y >= YSize)
    return;

  if(X + Width > XSize)
    Width = XSize - X;

  if(Y + Height > YSize)
    Height = YSize - Y;

  if(Color >> 8 == (Color & 0xFF))
    {
      Width <<= 1;

      for(int y = 0; y < Height; ++y)
	memset(&Image[Y + y][X], Color, Width);
    }
  else
    for(int y = 0; y < Height; ++y)
      {
	packedcolor16* Ptr = &Image[Y + y][X];
	const packedcolor16*const EndPtr = Ptr + Width;

	while(Ptr != EndPtr)
	  *Ptr++ = Color;
      }
}

void bitmap::ClearToColor(color16 Color)
{
  packedcolor16* Ptr = Image[0];

  if(Color >> 8 == (Color & 0xFF))
    memset(Ptr, Color, XSizeTimesYSize * sizeof(packedcolor16));
  else
    {
      const packedcolor16*const EndPtr = Ptr + XSizeTimesYSize;

      while(Ptr != EndPtr)
	*Ptr++ = Color;
    }
}

void bitmap::NormalBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, int Flags) const { NormalBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, int Flags) const { NormalBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, int Flags) const { NormalBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, int Flags) const { NormalBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, int Flags) const { NormalBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, int Flags) const { NormalBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, int Flags) const  { NormalBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags); }
void bitmap::NormalBlit(bitmap* Bitmap, int Flags) const { NormalBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags); }

void bitmap::NormalBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Flags) const
{
  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap blit attempt detected!");

      if(Flags & ROTATE && Width != Height)
	ABORT("Blit error: FeLib supports only square rotating!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedcolor16** DestImage = Bitmap->Image;

  switch(Flags & 7)
    {
    case NONE:
      {
	if(XSize == Bitmap->XSize && YSize == Bitmap->YSize)
	  memcpy(DestImage[0], SrcImage[0], XSizeTimesYSize * sizeof(packedcolor16));
	else
	  {
	    int Bytes = Width * sizeof(packedcolor16);

	    for(int y = 0; y < Height; ++y)
	      memcpy(&DestImage[DestY + y][DestX], &SrcImage[SourceY + y][SourceX], Bytes);

	  }

	break;
      }

    case MIRROR:
      {
	DestX += Width - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case FLIP:
      {
	DestY += Height - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    const ulong* SrcPtr = reinterpret_cast<const ulong*>(&SrcImage[SourceY + y][SourceX]);
	    const ulong* EndPtr = SrcPtr + (Width >> 1);
	    ulong* DestPtr = reinterpret_cast<ulong*>(&DestImage[DestY - y][DestX]);

	    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
	      *DestPtr = *SrcPtr;

	    if(Width & 1)
	      *reinterpret_cast<packedcolor16*>(DestPtr) = *reinterpret_cast<const packedcolor16*>(SrcPtr);
	  }

	break;
      }

    case (MIRROR | FLIP):
      {
	DestX += Width - 1;
	DestY += Height - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[DestY - y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case ROTATE:
      {
	DestX += Width - 1;
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase - y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | ROTATE):
      {
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase + y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (FLIP | ROTATE):
      {
	DestX += Width - 1;
	DestY += Height - 1;
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase - y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	DestY += Height - 1;
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase + y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }
    }
}

void bitmap::FastBlit(bitmap* Bitmap) const
{
  if(!FastFlag && (XSize != Bitmap->XSize || YSize != Bitmap->YSize))
    ABORT("Fast blit attempt of noncongruent bitmaps detected!");

  memcpy(Bitmap->Image[0], Image[0], XSizeTimesYSize * sizeof(packedcolor16));
}

void bitmap::LuminanceBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, color24 Luminance) const { LuminanceBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, color24 Luminance) const { LuminanceBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, color24 Luminance) const { LuminanceBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, color24 Luminance) const { LuminanceBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, color24 Luminance) const { LuminanceBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, color24 Luminance) const { LuminanceBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, color24 Luminance) const  { LuminanceBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance); }
void bitmap::LuminanceBlit(bitmap* Bitmap, color24 Luminance) const { LuminanceBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance); }

void bitmap::LuminanceBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, color24 Luminance) const
{
  if(Luminance == NORMAL_LUMINANCE)
    {
      NormalBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height);
      return;
    }

  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedcolor16** DestImage = Bitmap->Image;
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < Height; ++y)
    {
      const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
      const packedcolor16* EndPtr = SrcPtr + Width;
      packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

      for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
	{
	  LOAD_SRC()
	  NEW_LUMINATE_RED()
	  NEW_LUMINATE_GREEN()
	  NEW_LUMINATE_BLUE()
	  STORE_COLOR()
	}
    }
}

void bitmap::NormalMaskedBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, int Flags, color16 MaskColor) const  { NormalMaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Flags, MaskColor); }
void bitmap::NormalMaskedBlit(bitmap* Bitmap, int Flags, color16 MaskColor) const { NormalMaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Flags, MaskColor); }

void bitmap::NormalMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Flags, color16 MaskColor) const
{
  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap masked blit attempt detected!");

      if(Flags & ROTATE && Width != Height)
	ABORT("MaskedBlit error: FeLib supports only square rotating!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedcolor16** DestImage = Bitmap->Image;
  packedcolor16 PackedMaskColor = MaskColor;

  switch(Flags & 7)
    {
    case NONE:
      {
	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case MIRROR:
      {
	DestX += Width - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case FLIP:
      {
	DestY += Height - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[DestY - y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP):
      {
	DestX += Width - 1;
	DestY += Height - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[DestY - y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case ROTATE:
      {
	DestX += Width - 1;
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase - y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | ROTATE):
      {
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase + y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += TrueDestXMove)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (FLIP | ROTATE):
      {
	DestX += Width - 1;
	DestY += Height - 1;
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase - y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	DestY += Height - 1;
	int TrueDestXMove = Bitmap->XSize;
	packedcolor16* DestBase = &DestImage[DestY][DestX];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = DestBase + y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= TrueDestXMove)
	      if(*SrcPtr != PackedMaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }
    }
}

void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const  { LuminanceMaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, color24 Luminance, color16 MaskColor) const { LuminanceMaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

void bitmap::LuminanceMaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const
{
  if(Luminance == NORMAL_LUMINANCE)
    {
      NormalMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, 0, MaskColor);
      return;
    }

  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap masked blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedcolor16** DestImage = Bitmap->Image;
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < Height; ++y)
    {
      const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
      const packedcolor16* EndPtr = SrcPtr + Width;
      packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

      for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
	{
	  LOAD_SRC()

	  if(SrcCol != MaskColor)
	    {
	      NEW_LUMINATE_RED()
	      NEW_LUMINATE_GREEN()
	      NEW_LUMINATE_BLUE()
	      STORE_COLOR()
	    }
	}
    }
}

void bitmap::SimpleAlphaBlit(bitmap* Bitmap, alpha Alpha, color16 MaskColor) const
{
  if(Alpha == 255)
    {
      NormalMaskedBlit(Bitmap, 0, MaskColor);
      return;
    }

  if(!FastFlag && (XSize != Bitmap->XSize || YSize != Bitmap->YSize))
    ABORT("Fast simple alpha blit attempt of noncongruent bitmaps detected!");

  const packedcolor16* SrcPtr = Image[0];
  const packedcolor16* EndPtr = SrcPtr + XSizeTimesYSize;
  packedcolor16* DestPtr = Bitmap->Image[0];

  for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr)
    {
      LOAD_SRC()

      if(SrcCol != MaskColor)
	{
	  LOAD_DEST()
	  NEW_LOAD_AND_APPLY_ALPHA_RED()
	  NEW_LOAD_AND_APPLY_ALPHA_GREEN()
	  NEW_LOAD_AND_APPLY_ALPHA_BLUE()
	  STORE_COLOR()
	}
    }
}

void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, color16 MaskColor) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, color16 MaskColor) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, color16 MaskColor) const { AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, color16 MaskColor) const { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, color16 MaskColor) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, color16 MaskColor) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, color16 MaskColor) const  { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, color16 MaskColor) const { AlphaBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, MaskColor); }

void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, color16 MaskColor) const
{
  if(!AlphaMap)
    {
      NormalMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, 0, MaskColor);
      return;
    }

  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap alpha blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedalpha** SrcAlphaMap = AlphaMap;
  packedcolor16** DestImage = Bitmap->Image;

  for(int y = 0; y < Height; ++y)
    {
      const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
      const packedalpha* AlphaPtr = &SrcAlphaMap[SourceY + y][SourceX];
      const packedcolor16* EndPtr = SrcPtr + Width;
      packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

      for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++AlphaPtr)
	{
	  LOAD_SRC()

	  if(SrcCol != MaskColor)
	    {
	      LOAD_DEST()
	      LOAD_ALPHA()
	      NEW_LOAD_AND_APPLY_ALPHA_RED()
	      NEW_LOAD_AND_APPLY_ALPHA_GREEN()
	      NEW_LOAD_AND_APPLY_ALPHA_BLUE()
	      STORE_COLOR()
	    }
	}
    }
}

void bitmap::DrawLine(vector2d From, int ToX, int ToY, color16 Color, bool Wide) { DrawLine(From.X, From.Y, ToX, ToY, Color, Wide); }
void bitmap::DrawLine(int FromX, int FromY, vector2d To, color16 Color, bool Wide) { DrawLine(FromX, FromY, To.X, To.Y, Color, Wide); }
void bitmap::DrawLine(vector2d From, vector2d To, color16 Color, bool Wide) { DrawLine(From.X, From.Y, To.X, To.Y, Color, Wide); }

void bitmap::DrawLine(int OrigFromX, int OrigFromY, int OrigToX, int OrigToY, color16 Color, bool Wide)
{
  if(OrigFromY == OrigToY)
    {
      DrawHorizontalLine(OrigFromX, OrigToX, OrigFromY, Color, Wide);
      return;
    }

  if(OrigFromX == OrigToX)
    {
      DrawVerticalLine(OrigFromX, OrigFromY, OrigToY, Color, Wide);
      return;
    }

  static const int PointX[] = { 0, 0, -1, 1, 0 };
  static const int PointY[] = { 0, -1, 0, 0, 1 };
  const int Times = Wide ? 5 : 1;

  for(int c = 0; c < Times; ++c)
    {
      const int X1 = OrigFromX + PointX[c];
      const int Y1 = OrigFromY + PointY[c];
      const int X2 = OrigToX + PointX[c];
      const int Y2 = OrigToY + PointY[c];
      const int DeltaX = abs(X2 - X1);
      const int DeltaY = abs(Y2 - Y1);
      int x, c;
      int XChange, PtrXChange, PtrYChange;
      int DoubleDeltaX, DoubleDeltaY, End;

      if(DeltaX >= DeltaY)
	{
	  x = X1;
	  c = DeltaX;
	  PtrXChange = XChange = X1 < X2 ? 1 : -1;
	  PtrYChange = Y1 < Y2 ? XSize : -XSize;
	  DoubleDeltaX = DeltaX << 1;
	  DoubleDeltaY = DeltaY << 1;
	  End = X2;
	}
      else
	{
	  x = Y1;
	  c = DeltaY;
	  XChange = Y1 < Y2 ? 1 : -1;
	  PtrXChange = Y1 < Y2 ? XSize : -XSize;
	  PtrYChange = X1 < X2 ? 1 : -1;
	  DoubleDeltaX = DeltaY << 1;
	  DoubleDeltaY = DeltaX << 1;
	  End = Y2;
	}

      packedcolor16* Ptr = &Image[Y1][X1];
      *Ptr = Color;

      while(x != End)
	{
	  x += XChange;
	  Ptr += PtrXChange;
	  c += DoubleDeltaY;

	  if(c >= DoubleDeltaX)
	    {
	      c -= DoubleDeltaX;
	      Ptr += PtrYChange;
	    }

	  *Ptr = Color;
	}
    }
}

void bitmap::DrawVerticalLine(int OrigX, int OrigFromY, int OrigToY, color16 Color, bool Wide)
{
  static const int PointX[] = { 0, -1, 1 };
  const int Times = Wide ? 3 : 1;

  for(int c = 0; c < Times; ++c)
    {
      int X = OrigX + PointX[c];
      int FromY = OrigFromY;
      int ToY = OrigToY;

      if(FromY > ToY)
	Swap(FromY, ToY);

      if(Wide && !c)
	{
	  --FromY;
	  ++ToY;
	}

      if(X < 0 || X >= XSize || ToY < 0 || FromY >= YSize)
	continue;

      FromY = Max(FromY, 0);
      ToY = Min(ToY, YSize-1);
      packedcolor16* Ptr = &Image[FromY][X];

      for(int y = FromY; y <= ToY; ++y, Ptr += XSize)
	*Ptr = Color;
    }
}

void bitmap::DrawHorizontalLine(int OrigFromX, int OrigToX, int OrigY, color16 Color, bool Wide)
{
  static const int PointY[] = { 0, -1, 1 };
  const int Times = Wide ? 3 : 1;

  for(int c = 0; c < Times; ++c)
    {
      int Y = OrigY + PointY[c];
      int FromX = OrigFromX;
      int ToX = OrigToX;

      if(FromX > ToX)
	Swap(FromX, ToX);

      if(Wide && !c)
	{
	  --FromX;
	  ++ToX;
	}

      if(Y < 0 || Y >= YSize || ToX < 0 || FromX >= XSize)
	continue;

      FromX = Max(FromX, 0);
      ToX = Min(ToX, XSize-1);
      packedcolor16* Ptr = &Image[Y][FromX];

      for(int x = FromX; x <= ToX; ++x, ++Ptr)
	*Ptr = Color;
    }
}

void bitmap::DrawPolygon(int CenterX, int CenterY, int Radius, int NumberOfSides, color16 Color, bool DrawSides, bool DrawDiameters, double Rotation)
{
  if(!DrawSides && !DrawDiameters)
    return;

  vector2d* Point = new vector2d[NumberOfSides];
  double AngleDelta = 2 * FPI / NumberOfSides;
  int c;

  for(c = 0; c < NumberOfSides; ++c)
    {
      Point[c].X = CenterX + int(sin(AngleDelta * c + Rotation) * Radius);
      Point[c].Y = CenterY + int(cos(AngleDelta * c + Rotation) * Radius);
    }

  if(DrawDiameters)
    {
      if(DrawSides)
	{
	  for(c = 0; c < NumberOfSides; ++c)
	    for(int a = 0; a < NumberOfSides; ++a)
	      if(c != a)
		DrawLine(Point[c].X, Point[c].Y, Point[a].X, Point[a].Y, Color, true);
	}
      else
	{
	  for(c = 0; c < NumberOfSides; ++c)
	    for(int a = 0; a < NumberOfSides; ++a)
	      if((int(c - a) > 1 || int(a - c) > 1) && (a || c != NumberOfSides - 1) && (c || a != NumberOfSides - 1))
		DrawLine(Point[c].X, Point[c].Y, Point[a].X, Point[a].Y, Color, true);
	}
    }
  else
    {
      for(c = 0; c < NumberOfSides - 1; ++c)
	DrawLine(Point[c].X, Point[c].Y, Point[c + 1].X, Point[c + 1].Y, Color, true);

      DrawLine(Point[NumberOfSides - 1].X, Point[NumberOfSides - 1].Y, Point[0].X, Point[0].Y, Color, true);
    }

  delete [] Point;
}

void bitmap::CreateAlphaMap(alpha InitialValue)
{
  if(AlphaMap)
    ABORT("Alpha leak detected!");

  Alloc2D(AlphaMap, YSize, XSize);
  memset(AlphaMap[0], InitialValue, XSizeTimesYSize);
}

bool bitmap::Fade(long& AlphaSum, packedalpha& AlphaAverage, int Amount)
{
  if(!AlphaMap)
    ABORT("No alpha map to fade.");

  bool Changes = false;
  long Alphas = 0;
  long NewAlphaSum = 0;
  long Size = XSizeTimesYSize;

  for(long c = 0; c < Size; ++c)
    {
      packedalpha* AlphaPtr = &AlphaMap[0][c];

      if(*AlphaPtr)
	if(*AlphaPtr > Amount)
	  {
	    *AlphaPtr -= Amount;
	    NewAlphaSum += *AlphaPtr;
	    ++Alphas;
	    Changes = true;
	  }
	else
	  {
	    *AlphaPtr = 0;
	    Changes = true;

	    if(RandMap)
	      UpdateRandMap(c, false);
	  }
    }

  AlphaSum = NewAlphaSum;
  AlphaAverage = Alphas ? NewAlphaSum / Alphas : 0;
  return Changes;
}

void bitmap::Outline(color16 Color, alpha Alpha, priority Priority)
{
  if(!AlphaMap)
    CreateAlphaMap(255);

  color16 LastColor, NextColor;
  int XMax = XSize;
  int YMax = YSize - 1;

  for(int x = 0; x < XMax; ++x)
    {
      packedcolor16* Buffer = &Image[0][x];
      LastColor = *Buffer;

      for(int y = 0; y < YMax; ++y)
	{
	  NextColor = *(Buffer + XMax);

	  if((LastColor == TRANSPARENT_COLOR || !y) && NextColor != TRANSPARENT_COLOR)
	    {
	      *Buffer = Color;
	      SetAlpha(x, y, Alpha);
	      SafeSetPriority(x, y, Priority);
	    }

	  Buffer += XMax;

	  if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || y == YMax - 1))
	    {
	      *Buffer = Color;
	      SetAlpha(x, y + 1, Alpha);
	      SafeSetPriority(x, y + 1, Priority);
	    }

	  LastColor = NextColor;
	}
    }

  --XMax;
  ++YMax;

  for(int y = 0; y < YMax; ++y)
    {
      packedcolor16* Buffer = Image[y];
      LastColor = *Buffer;

      for(int x = 0; x < XMax; ++x)
	{
	  NextColor = *(Buffer + 1);

	  if((LastColor == TRANSPARENT_COLOR || !x) && NextColor != TRANSPARENT_COLOR)
	    {
	      *Buffer = Color;
	      SetAlpha(x, y, Alpha);
	      SafeSetPriority(x, y, Priority);
	    }

	  ++Buffer;

	  if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || x == XMax - 1))
	    {
	      *Buffer = Color;
	      SetAlpha(x + 1, y, Alpha);
	      SafeSetPriority(x + 1, y, Priority);
	    }

	  LastColor = NextColor;
	}
    }
}

void bitmap::FadeToScreen(bitmapeditor BitmapEditor)
{
  bitmap Backup(DOUBLE_BUFFER);
  Backup.ActivateFastFlag();

  for(int c = 0; c <= 5; ++c)
    {
      clock_t StartTime = clock();
      int Element = 127 - c * 25;
      Backup.LuminanceMaskedBlit(DOUBLE_BUFFER, MakeRGB24(Element, Element, Element), 0);

      if(BitmapEditor)
	BitmapEditor(this);

      SimpleAlphaBlit(DOUBLE_BUFFER, c * 50, 0);
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.05 * CLOCKS_PER_SEC);
    }

  DOUBLE_BUFFER->ClearToColor(0);

  if(BitmapEditor)
    BitmapEditor(this);

  NormalMaskedBlit(DOUBLE_BUFFER, 0, 0);
  graphics::BlitDBToScreen();
}

void bitmap::StretchBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, int Stretch) const { StretchBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, int Stretch) const { StretchBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, int Stretch) const { StretchBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, int Stretch) const { StretchBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, int Stretch) const { StretchBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, int Stretch) const { StretchBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, int Stretch) const  { StretchBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Stretch); }
void bitmap::StretchBlit(bitmap* Bitmap, int Stretch) const { StretchBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Stretch); }

void bitmap::StretchBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Stretch) const
{
  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap stretch blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  if(Stretch > 1)
    {
      int tx = DestX;

      for(int x1 = SourceX; x1 < SourceX + Width; ++x1, tx += Stretch)
	{
	  int ty = DestY;

	  for(int y1 = SourceY; y1 < SourceY + Height; ++y1, ty += Stretch)
	    {
	      packedcolor16 Pixel = Image[y1][x1];

	      if(Pixel != TRANSPARENT_COLOR)
		for(int x2 = tx; x2 < tx + Stretch; ++x2)
		  for(int y2 = ty; y2 < ty + Stretch; ++y2)
		    Bitmap->Image[y2][x2] = Pixel;
	    }
	}

      return;
    }
  else if(Stretch < -1)
    {
      int tx = DestX;

      for(int x1 = SourceX; x1 < SourceX + Width; x1 -= Stretch, ++tx)
	{
	  int ty = DestY;

	  for(int y1 = SourceY; y1 < SourceY + Height; y1 -= Stretch, ++ty)
	    {
	      packedcolor16 Pixel = Image[y1][x1];

	      if(Pixel != TRANSPARENT_COLOR)
		Bitmap->Image[ty][tx] = Pixel;
	    }
	}

      return;
    }
  else
    {
      NormalMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height);
      return;
    }
}

outputfile& operator<<(outputfile& SaveFile, const bitmap* Bitmap)
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

inputfile& operator>>(inputfile& SaveFile, bitmap*& Bitmap)
{
  if(SaveFile.Get())
    {
      int XSize, YSize;
      SaveFile >> XSize >> YSize;
      Bitmap = new bitmap(XSize, YSize);
      Bitmap->Load(SaveFile);
    }

  return SaveFile;
}

void bitmap::DrawRectangle(vector2d TopLeft, int Right, int Bottom, color16 Color, bool Wide) { DrawRectangle(TopLeft.X, TopLeft.Y, Right, Bottom, Color, Wide); }
void bitmap::DrawRectangle(int Left, int Top, vector2d BottomRight, color16 Color, bool Wide) { DrawRectangle(Left, Top, BottomRight.X, BottomRight.Y, Color, Wide); }
void bitmap::DrawRectangle(vector2d TopLeft, vector2d BottomRight, color16 Color, bool Wide) { DrawRectangle(TopLeft.X, TopLeft.Y, BottomRight.X, BottomRight.Y, Color, Wide); }

void bitmap::DrawRectangle(int Left, int Top, int Right, int Bottom, color16 Color, bool Wide)
{
  DrawHorizontalLine(Left, Right, Top, Color, Wide);
  DrawHorizontalLine(Left, Right, Bottom, Color, Wide);
  DrawVerticalLine(Right, Top, Bottom, Color, Wide);
  DrawVerticalLine(Left, Top, Bottom, Color, Wide);
}

void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const  { AlphaBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaBlit(bitmap* Bitmap, color24 Luminance, color16 MaskColor) const { AlphaBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

void bitmap::AlphaBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const
{
  if(Luminance == NORMAL_LUMINANCE)
    {
      AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, MaskColor);
      return;
    }

  if(!AlphaMap)
    {
      LuminanceMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap alpha blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedalpha** SrcAlphaMap = AlphaMap;
  packedcolor16** DestImage = Bitmap->Image;
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < Height; ++y)
    {
      const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
      const packedalpha* AlphaPtr = &SrcAlphaMap[SourceY + y][SourceX];
      const packedcolor16* EndPtr = SrcPtr + Width;
      packedcolor16* DestPtr = &DestImage[DestY + y][DestX];

      for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++AlphaPtr)
	{
	  LOAD_SRC()

	  if(SrcCol != MaskColor)
	    {
	      LOAD_DEST()
	      LOAD_ALPHA()
	      NEW_LUMINATE_RED()
	      NEW_APPLY_ALPHA_RED()
	      NEW_LUMINATE_GREEN()
	      NEW_APPLY_ALPHA_GREEN()
	      NEW_LUMINATE_BLUE()
	      NEW_APPLY_ALPHA_BLUE()
	      STORE_COLOR()
	    }
	}
    }
}

void bitmap::CreateFlames(int Frame, color16 MaskColor)
{
  int* FlameLowestPoint = new int[XSize];
  int* FlameTop = new int[XSize];
  int* FlamePhase = new int[XSize];
  int x, y;
  femath::SaveSeed();
  femath::SetSeed(0x666);

  for(x = 0; x < XSize; ++x)
    {
      if(GetPixel(x, 0) == MaskColor && GetPixel(x, 1) == MaskColor)
	{
	  FlameLowestPoint[x] = NO_FLAME;

	  for(int y = 1; y < YSize - 1; ++y)
	    if(GetPixel(x, y) == MaskColor && GetPixel(x, y + 1) != MaskColor)
	      {
		FlameLowestPoint[x] = y;

		if(FlameLowestPoint[x] >= 4)
		  FlameTop[x] = (FlameLowestPoint[x] >> 1) - (RAND() % (FlameLowestPoint[x] >> 1)) - 1;
		else
		  FlameTop[x] = 0;

		FlamePhase[x] = RAND_16;
		break;
	      }
	}
      else
	{
	  FlameLowestPoint[x] = 1;
	  FlameTop[x] = 0;
	  FlamePhase[x] = RAND_16;
	}
    }

  for(x = 0; x < XSize; ++x)
    {
      if(FlameLowestPoint[x] != NO_FLAME)
	{
	  int Phase = (Frame + FlamePhase[x]) & 15;
	  int Length = FlameLowestPoint[x] - FlameTop[x];
	  int Top = FlameLowestPoint[x] - Length + Phase * (15 - Phase) * Length / 56;

	  for(y = Top; y <= FlameLowestPoint[x]; ++y)
	    {
	      int Pos = y - Top;
	      PowerPutPixel(x, y, MakeRGB16(255, 255 - (Pos << 7) / Length, 0), 127 + (Pos << 6) / Length, AVERAGE_PRIORITY);
	    }
	}
    }

  femath::LoadSeed();
  delete [] FlameLowestPoint;
  delete [] FlameTop;
  delete [] FlamePhase;
}

void bitmap::CreateSparkle(vector2d SparklePos, int Frame)
{
  if(Frame)
    {
      int Size = (Frame - 1) * (16 - Frame) / 10;
      PowerPutPixel(SparklePos.X, SparklePos.Y, WHITE, 255, SPARKLE_PRIORITY);

      for(int c = 1; c < Size; ++c)
	{
	  int Lightness = 191 + ((Size - c) << 6) / Size;
	  color16 RGB = MakeRGB16(Lightness, Lightness, Lightness);
	  PowerPutPixel(SparklePos.X + c, SparklePos.Y, RGB, 255, SPARKLE_PRIORITY);
	  PowerPutPixel(SparklePos.X - c, SparklePos.Y, RGB, 255, SPARKLE_PRIORITY);
	  PowerPutPixel(SparklePos.X, SparklePos.Y + c, RGB, 255, SPARKLE_PRIORITY);
	  PowerPutPixel(SparklePos.X, SparklePos.Y - c, RGB, 255, SPARKLE_PRIORITY);
	}
    }
}

void bitmap::CreateFlies(ulong Seed, int Frame, int FlyAmount)
{
  femath::SaveSeed();
  femath::SetSeed(Seed);

  for(int c = 0; c < FlyAmount; ++c)
    {
      double Constant = double(RAND() % 10000) / 10000 * FPI;
      vector2d StartPos = vector2d(5 + RAND() % 6, 5 + RAND() % 6);
      double Temp = (double(16 - Frame) * FPI) / 16;

      if(RAND() & 1)
	Temp = -Temp;

      vector2d Where;
      Where.X = int(StartPos.X + sin(Constant + Temp) * 3);
      Where.Y = int(StartPos.Y + sin(2*(Constant + Temp)) * 3);
      PowerPutPixel(Where.X, Where.Y, 0, 255, FLY_PRIORITY);
    }

  femath::LoadSeed();
}

void bitmap::CreateLightning(ulong Seed, color16 Color)
{
  femath::SaveSeed();
  femath::SetSeed(Seed);
  vector2d StartPos;
  vector2d Direction(0, 0);

  do
    {
      do
	{
	  if(RAND() & 1)
	    {
	      if(RAND() & 1)
		{
		  StartPos.X = 0;
		  Direction.X = 1;
		}
	      else
		{
		  StartPos.X = XSize - 1;
		  Direction.X = -1;
		}

	      StartPos.Y = RAND() % YSize;
	    }
	  else
	    {
	      if(RAND() & 1)
		{
		  StartPos.Y = 0;
		  Direction.Y = 1;
		}
	      else
		{
		  StartPos.Y = YSize - 1;
		  Direction.Y = -1;
		}

	      StartPos.X = RAND() % XSize;
	    }
	}
      while(GetPixel(StartPos) != TRANSPARENT_COLOR);
    }
  while(!CreateLightning(StartPos, Direction, NO_LIMIT, Color));

  femath::LoadSeed();
}

struct pixelvectorcontroller
{
  static bool Handler(int x, int y)
  {
    if(CurrentSprite->GetPixel(x, y) == TRANSPARENT_COLOR)
      {
	PixelVector.push_back(vector2d(x, y));
	return true;
      }
    else
      return false;
  }
  static std::vector<vector2d> PixelVector;
  static bitmap* CurrentSprite;
};

std::vector<vector2d> pixelvectorcontroller::PixelVector;
bitmap* pixelvectorcontroller::CurrentSprite;

bool bitmap::CreateLightning(vector2d StartPos, vector2d Direction, int MaxLength, color16 Color)
{
  pixelvectorcontroller::CurrentSprite = this;
  std::vector<vector2d>& PixelVector = pixelvectorcontroller::PixelVector;
  PixelVector.clear();
  vector2d LastMove(0, 0);
  int Counter = 0;

  for(;;)
    {
      vector2d Move(1 + (RAND() & 3), 1 + (RAND() & 3));

      if(Direction.X < 0 || (!Direction.X && RAND() & 1))
	Move.X = -Move.X;

      if(Direction.Y < 0 || (!Direction.Y && RAND() & 1))
	Move.Y = -Move.Y;

      Move.X = Limit(Move.X, -StartPos.X, XSize - StartPos.X - 1);
      Move.Y = Limit(Move.Y, -StartPos.Y, XSize - StartPos.Y - 1);

      if(Counter < 10 && ((!Move.Y && !LastMove.Y) || (Move.Y && LastMove.Y && (Move.X << 10) / Move.Y == (LastMove.X << 10) / LastMove.Y)))
	{
	  ++Counter;
	  continue;
	}

      Counter = 0;

      if(!mapmath<pixelvectorcontroller>::DoLine(StartPos.X, StartPos.Y, StartPos.X + Move.X, StartPos.Y + Move.Y)
      || ulong(MaxLength) <= PixelVector.size())
	{
	  int Limit = Min<int>(PixelVector.size(), MaxLength);

	  for(int c = 0; c < Limit; ++c)
	    {
	      PutPixel(PixelVector[c], Color);
	      SafeSetPriority(PixelVector[c], LIGHTNING_PRIORITY);
	    }

	  PixelVector.clear();
	  return true;
	}

      StartPos += Move;
      LastMove = Move;

      if((Direction.X && (!StartPos.X || StartPos.X == XSize - 1)) || (Direction.Y && (!StartPos.Y || StartPos.Y == XSize - 1)))
	{
	  PixelVector.clear();
	  return false;
	}
    }
}

void bitmap::BlitAndCopyAlpha(bitmap* Bitmap, int Flags) const
{
  if(!FastFlag)
    {
      if(!AlphaMap || !Bitmap->AlphaMap)
	ABORT("Attempt to blit and copy alpha without an alpha map detected!");

      if(Flags & ROTATE && XSize != YSize)
	ABORT("Blit and copy alpha error: FeLib supports only square rotating!");

      if(XSize != Bitmap->XSize || YSize != Bitmap->YSize)
	ABORT("Blit and copy alpha attempt of noncongruent bitmaps detected!");
    }

  packedcolor16** SrcImage = Image;
  packedalpha** SrcAlphaMap = AlphaMap;
  packedcolor16** DestImage = Bitmap->Image;
  packedalpha** DestAlphaMap = Bitmap->AlphaMap;

  switch(Flags & 7)
    {
    case NONE:
      {
	memcpy(DestImage[0], SrcImage[0], XSizeTimesYSize * sizeof(packedcolor16));
	memcpy(DestAlphaMap[0], SrcAlphaMap[0], XSizeTimesYSize * sizeof(packedalpha));
	break;
      }

    case MIRROR:
      {
	int Width = XSize;
	int Height = YSize;
	int DestX = Width - 1;
	const packedcolor16* SrcPtr = SrcImage[0];
	const packedalpha* SrcAlphaPtr = SrcAlphaMap[0];

	for(int y = 0; y < Height; ++y)
	  {
	    const packedcolor16* EndPtr = SrcPtr + Width;
	    packedcolor16* DestPtr = &DestImage[y][DestX];
	    packedalpha* DestAlphaPtr = &DestAlphaMap[y][DestX];

	    for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr, ++SrcAlphaPtr, --DestAlphaPtr)
	      {
		*DestPtr = *SrcPtr;
		*DestAlphaPtr = *SrcAlphaPtr;
	      }
	  }

	break;
      }

    case FLIP:
      {
	int Height = YSize;
	int Width = XSize;
	int DestY = Height - 1;

	for(int y = 0; y < Height; ++y)
	  {
	    memcpy(DestImage[DestY - y], SrcImage[y], Width * sizeof(packedcolor16));
	    memcpy(DestAlphaMap[DestY - y], SrcAlphaMap[y], Width * sizeof(packedalpha));
	  }

	break;
      }

    case (MIRROR | FLIP):
      {
	const packedcolor16* SrcPtr = SrcImage[0];
	const packedcolor16* EndPtr = SrcPtr + XSizeTimesYSize;
	const packedalpha* SrcAlphaPtr = SrcAlphaMap[0];
	packedcolor16* DestPtr = &DestImage[YSize - 1][XSize - 1];
	packedalpha* DestAlphaPtr = &DestAlphaMap[YSize - 1][XSize - 1];

	for(; SrcPtr != EndPtr; ++SrcPtr, --DestPtr, ++SrcAlphaPtr, --DestAlphaPtr)
	  {
	    *DestPtr = *SrcPtr;
	    *DestAlphaPtr = *SrcAlphaPtr;
	  }

	break;
      }

    case ROTATE:
      {
	const int Size = XSize;
	const packedcolor16* SrcPtr = SrcImage[0];
	const packedalpha* SrcAlphaPtr = SrcAlphaMap[0];
	packedcolor16* DestBase = &DestImage[0][Size - 1];
	packedalpha* DestAlphaBase = &DestAlphaMap[0][Size - 1];

	for(int y = 0; y < Size; ++y)
	  {
	    const packedcolor16* EndPtr = SrcPtr + Size;
	    packedcolor16* DestPtr = DestBase - y;
	    packedalpha* DestAlphaPtr = DestAlphaBase - y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += Size, ++SrcAlphaPtr, DestAlphaPtr += Size)
	      {
		*DestPtr = *SrcPtr;
		*DestAlphaPtr = *SrcAlphaPtr;
	      }
	  }

	break;
      }

    case (MIRROR | ROTATE):
      {
	const int Size = XSize;
	const packedcolor16* SrcPtr = SrcImage[0];
	const packedalpha* SrcAlphaPtr = SrcAlphaMap[0];
	packedcolor16* DestBase = DestImage[0];
	packedalpha* DestAlphaBase = DestAlphaMap[0];

	for(int y = 0; y < Size; ++y)
	  {
	    const packedcolor16* EndPtr = SrcPtr + Size;
	    packedcolor16* DestPtr = DestBase + y;
	    packedalpha* DestAlphaPtr = DestAlphaBase + y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr += Size, ++SrcAlphaPtr, DestAlphaPtr += Size)
	      {
		*DestPtr = *SrcPtr;
		*DestAlphaPtr = *SrcAlphaPtr;
	      }
	  }

	break;
      }

    case (FLIP | ROTATE):
      {
	const int Size = XSize;
	const packedcolor16* SrcPtr = SrcImage[0];
	const packedalpha* SrcAlphaPtr = SrcAlphaMap[0];
	packedcolor16* DestBase = &DestImage[Size - 1][Size - 1];
	packedalpha* DestAlphaBase = &DestAlphaMap[Size - 1][Size - 1];

	for(int y = 0; y < Size; ++y)
	  {
	    const packedcolor16* EndPtr = SrcPtr + Size;
	    packedcolor16* DestPtr = DestBase - y;
	    packedalpha* DestAlphaPtr = DestAlphaBase - y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= Size, ++SrcAlphaPtr, DestAlphaPtr -= Size)
	      {
		*DestPtr = *SrcPtr;
		*DestAlphaPtr = *SrcAlphaPtr;
	      }
	  }

	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	const int Size = XSize;
	const packedcolor16* SrcPtr = SrcImage[0];
	const packedalpha* SrcAlphaPtr = SrcAlphaMap[0];
	packedcolor16* DestBase = DestImage[Size - 1];
	packedalpha* DestAlphaBase = DestAlphaMap[Size - 1];

	for(int y = 0; y < Size; ++y)
	  {
	    const packedcolor16* EndPtr = SrcPtr + Size;
	    packedcolor16* DestPtr = DestBase + y;
	    packedalpha* DestAlphaPtr = DestAlphaBase + y;

	    for(; SrcPtr != EndPtr; ++SrcPtr, DestPtr -= Size, ++SrcAlphaPtr, DestAlphaPtr -= Size)
	      {
		*DestPtr = *SrcPtr;
		*DestAlphaPtr = *SrcAlphaPtr;
	      }
	  }

	break;
      }
    }
}

void bitmap::FillAlpha(alpha Alpha)
{
  memset(AlphaMap[0], Alpha, XSizeTimesYSize);
}

void bitmap::PowerPutPixel(int X, int Y, color16 Color, alpha Alpha, priority Priority)
{
  if(X >= 0 && Y >= 0 && X < XSize && Y < YSize)
    {
      Image[Y][X] = Color;

      if(AlphaMap)
	AlphaMap[Y][X] = Alpha;
      else if(Alpha != 255)
	{
	  CreateAlphaMap(255);
	  AlphaMap[Y][X] = Alpha;
	}

      if(PriorityMap)
	PriorityMap[Y][X] = Priority;
    }
}

void bitmap::MaskedPriorityBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const  { MaskedPriorityBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::MaskedPriorityBlit(bitmap* Bitmap, color24 Luminance, color16 MaskColor) const { MaskedPriorityBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

void bitmap::MaskedPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const
{
  if(!PriorityMap || !Bitmap->PriorityMap)
    {
      LuminanceMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap masked priority blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedpriority** SrcPriorityMap = PriorityMap;
  packedcolor16** DestImage = Bitmap->Image;
  packedpriority** DestPriorityMap = Bitmap->PriorityMap;
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < Height; ++y)
    {
      const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
      const packedpriority* SrcPriorityPtr = &SrcPriorityMap[SourceY + y][SourceX];
      const packedcolor16* EndPtr = SrcPtr + Width;
      packedcolor16* DestPtr = &DestImage[DestY + y][DestX];
      packedpriority* DestPriorityPtr = &DestPriorityMap[DestY + y][DestX];

      for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++SrcPriorityPtr, ++DestPriorityPtr)
	{
	  LOAD_SRC()

	  if(SrcCol != MaskColor)
	    {
	      priority SrcPriority = *SrcPriorityPtr;
	      priority DestPriority = *DestPriorityPtr;

	      if((SrcPriority & 0xF) >= (DestPriority & 0xF) || (SrcPriority & 0xF0) >= (DestPriority & 0xF0))
		{
		  NEW_LUMINATE_RED()
		  NEW_LUMINATE_GREEN()
		  NEW_LUMINATE_BLUE()
		  STORE_COLOR()
		  *DestPriorityPtr = SrcPriority;
		}
	    }
	}
    }
}

void bitmap::AlphaPriorityBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, color24 Luminance, color16 MaskColor) const  { AlphaPriorityBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Luminance, MaskColor); }
void bitmap::AlphaPriorityBlit(bitmap* Bitmap, color24 Luminance, color16 MaskColor) const { AlphaPriorityBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Luminance, MaskColor); }

void bitmap::AlphaPriorityBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, color24 Luminance, color16 MaskColor) const
{
  if(!AlphaMap)
    {
      MaskedPriorityBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!PriorityMap || !Bitmap->PriorityMap)
    {
      AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!FastFlag)
    {
      if(!Width || !Height)
	ABORT("Zero-sized bitmap alpha priority blit attempt detected!");

      if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
	return;
    }

  packedcolor16** SrcImage = Image;
  packedalpha** SrcAlphaMap = AlphaMap;
  packedpriority** SrcPriorityMap = PriorityMap;
  packedcolor16** DestImage = Bitmap->Image;
  packedpriority** DestPriorityMap = Bitmap->PriorityMap;
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;

  for(int y = 0; y < Height; ++y)
    {
      const packedcolor16* SrcPtr = &SrcImage[SourceY + y][SourceX];
      const packedalpha* AlphaPtr = &SrcAlphaMap[SourceY + y][SourceX];
      const packedpriority* SrcPriorityPtr = &SrcPriorityMap[SourceY + y][SourceX];
      const packedcolor16* EndPtr = SrcPtr + Width;
      packedcolor16* DestPtr = &DestImage[DestY + y][DestX];
      packedpriority* DestPriorityPtr = &DestPriorityMap[DestY + y][DestX];

      for(; SrcPtr != EndPtr; ++SrcPtr, ++DestPtr, ++AlphaPtr, ++SrcPriorityPtr, ++DestPriorityPtr)
	{
	  LOAD_SRC()

	  if(SrcCol != MaskColor)
	    {
	      priority SrcPriority = *SrcPriorityPtr;
	      priority DestPriority = *DestPriorityPtr;

	      if((SrcPriority & 0xF) >= (DestPriority & 0xF)
	      || (SrcPriority & 0xF0) >= (DestPriority & 0xF0))
		{
		  LOAD_DEST()
		  LOAD_ALPHA()
		  NEW_LUMINATE_RED()
		  NEW_APPLY_ALPHA_RED()
		  NEW_LUMINATE_GREEN()
		  NEW_APPLY_ALPHA_GREEN()
		  NEW_LUMINATE_BLUE()
		  NEW_APPLY_ALPHA_BLUE()
		  STORE_COLOR()
		  *DestPriorityPtr = SrcPriority;
		}
	    }
	}
    }
}

void bitmap::CreatePriorityMap(priority InitialValue)
{
  if(PriorityMap)
    ABORT("Priority leak detected!");

  Alloc2D(PriorityMap, YSize, XSize);
  memset(PriorityMap[0], InitialValue, XSizeTimesYSize);
}

void bitmap::FillPriority(priority Priority)
{
  memset(PriorityMap[0], Priority, XSizeTimesYSize);
}

void bitmap::FastBlitAndCopyAlpha(bitmap* Bitmap) const
{
  if(!FastFlag)
    {
      if(!AlphaMap || !Bitmap->AlphaMap)
	ABORT("Attempt to fast blit and copy alpha without an alpha map detected!");

      if(XSize != Bitmap->XSize || YSize != Bitmap->YSize)
	ABORT("Fast blit and copy alpha attempt of noncongruent bitmaps detected!");
    }

  memcpy(Bitmap->Image[0], Image[0], XSizeTimesYSize * sizeof(packedcolor16));
  memcpy(Bitmap->AlphaMap[0], AlphaMap[0], XSizeTimesYSize * sizeof(packedalpha));
}

void bitmap::UpdateRandMap(long Index, bool Value)
{
  long c1 = XSizeTimesYSize + Index;
  RandMap[c1] = Value;

  for(long c2 = c1 >> 1; c2; c1 = c2, c2 >>= 1)
    {
      Value |= RandMap[c1 ^ 1];

      if(RandMap[c2] != Value)
	RandMap[c2] = Value;
      else
	return;
    }
}

void bitmap::InitRandMap()
{
  if(!RandMap)
    RandMap = new bool[XSizeTimesYSize << 1];

  memset(RandMap, 0, (XSizeTimesYSize << 1) * sizeof(bool));
}

vector2d bitmap::RandomizePixel() const
{
  if(!RandMap[1])
    return ERROR_VECTOR;

  long Rand = RAND();
  ulong c, RandMask = 1;
  ulong Size = XSizeTimesYSize << 1;

  for(c = 2; c < Size; c <<= 1)
    if(RandMap[c + 1] && (!RandMap[c] || Rand & (RandMask <<= 1)))
      ++c;

  c = (c - Size) >> 1;
  return vector2d(c % XSize, c / XSize);
}

void bitmap::CalculateRandMap()
{
  if(!AlphaMap)
    ABORT("Alpha map needed to calculate random map.");

  ulong Size = XSizeTimesYSize;

  for(ulong c = 0; c < Size; ++c)
    UpdateRandMap(c, !!AlphaMap[0][c]);
}

void bitmap::AlphaPutPixel(int x, int y, color16 SrcCol, color24 Luminance, alpha Alpha)
{
  int DestCol = Image[y][x];
  int NewRedLuminance = (Luminance >> 7 & 0x1F800) - 0x10000;
  int NewGreenLuminance = (Luminance >> 4 & 0xFE0) - 0x800;
  int NewBlueLuminance = (Luminance >> 2 & 0x3F) - 0x20;
  NEW_LUMINATE_RED()
  NEW_APPLY_ALPHA_RED()
  NEW_LUMINATE_GREEN()
  NEW_APPLY_ALPHA_GREEN()
  NEW_LUMINATE_BLUE()
  NEW_APPLY_ALPHA_BLUE()
  Image[y][x] = Red|Green|Blue;

}

alpha bitmap::CalculateAlphaAverage() const
{
  if(!AlphaMap)
    ABORT("Alpha map needed to calculate alpha average!");

  long Alphas = 0;
  long AlphaSum = 0;
  ulong Size = XSizeTimesYSize;

  for(ulong c = 0; c < Size; ++c)
    {
      packedalpha* AlphaPtr = &AlphaMap[0][c];

      if(*AlphaPtr)
	{
	  AlphaSum += *AlphaPtr;
	  ++Alphas;
	}
    }

  return Alphas ? AlphaSum / Alphas : 0;
}

cachedfont::cachedfont(int XSize, int YSize) : bitmap(XSize, YSize)
{
  Alloc2D(MaskMap, XSize, YSize);
}

cachedfont::cachedfont(int XSize, int YSize, color16 Color) : bitmap(XSize, YSize, Color)
{
  Alloc2D(MaskMap, XSize, YSize);
}

void cachedfont::PrintCharacter(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY) const
{
  if(DestX < 0 || DestY < 0 || DestX + 10 >= Bitmap->XSize || DestY + 9 >= Bitmap->YSize)
    NormalMaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, 9, 9, 0, 0);

  packedcolor16** SrcLine = &Image[SourceY];
  packedcolor16** EndLine = SrcLine + 9;
  packedcolor16** SrcMaskLine = &MaskMap[SourceY];
  packedcolor16** DestLine = &Bitmap->Image[DestY];

  for(; SrcLine != EndLine; ++SrcLine, ++SrcMaskLine, ++DestLine)
    {
      const ulong* FontPtr = reinterpret_cast<const ulong*>(*SrcLine + SourceX);
      const ulong* EndPtr = FontPtr + 5;
      const ulong* MaskPtr = reinterpret_cast<const ulong*>(*SrcMaskLine + SourceX);
      ulong* DestPtr = reinterpret_cast<ulong*>(*DestLine + DestX);

      for(; FontPtr != EndPtr; ++DestPtr, ++MaskPtr, ++FontPtr)
	*DestPtr = *DestPtr & *MaskPtr | *FontPtr;
    }
}

void cachedfont::CreateMaskMap()
{
  packedcolor16* SrcPtr = Image[0];
  packedcolor16* EndPtr = SrcPtr + XSizeTimesYSize;
  packedcolor16* MaskPtr = MaskMap[0];

  for(; SrcPtr != EndPtr; ++SrcPtr, ++MaskPtr)
    if(*SrcPtr == TRANSPARENT_COLOR)
      {
	*SrcPtr = 0;
	*MaskPtr = 0xFFFF;
      }
    else
      *MaskPtr = 0;
}
