#include <cmath>
#include <ctime>

#include "bitmap.h"
#include "graphics.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"

bitmap* CurrentSprite;
std::vector<vector2d> CurrentPixelVector;

bitmap::bitmap(const std::string& FileName) : IsIndependent(true)
{
  SetAlphaMap(0);
  inputfile File(FileName.c_str(), 0, false);

  if(!File.IsOpen())
    ABORT("Bitmap %s not found!", FileName.c_str());

  uchar Palette[768];
  File.SeekPosEnd(-768);
  File.Read(reinterpret_cast<char*>(Palette), 768);
  File.SeekPosBegin(8);
  XSize  =  File.Get();
  XSize += (File.Get() << 8) + 1;
  YSize  =  File.Get();
  YSize += (File.Get() << 8) + 1;
  File.SeekPosBegin(128);
  SetImage(Alloc2D<ushort>(YSize, XSize));
  ushort* Buffer = GetImage()[0];

  for(ushort y = 0; y < YSize; ++y)
    for(ushort x = 0; x < XSize; ++x)
      {
	int Char1 = File.Get();

	if(Char1 > 192)
	  {
	    int Char2 = File.Get();
	    --x;

	    for(; Char1 > 192; --Char1)
	      {
		*(Buffer++) = ushort(Palette[Char2 + (Char2 << 1)] >> 3) << 11 | ushort(Palette[Char2 + (Char2 << 1) + 1] >> 2) << 5 | ushort(Palette[Char2 + (Char2 << 1) + 2] >> 3);

		if(++x == XSize)
		  {
		    x = 0;
		    ++y;
		  }
	      }
	  }
	else
	  *(Buffer++) = ushort(Palette[Char1 + (Char1 << 1)] >> 3) << 11 | ushort(Palette[Char1 + (Char1 << 1) + 1] >> 2) << 5 | ushort(Palette[Char1 + (Char1 << 1) + 2] >> 3);
      }
}

bitmap::bitmap(bitmap* Bitmap, uchar Flags) : XSize(Bitmap->XSize), YSize(Bitmap->YSize), IsIndependent(true)
{
  SetImage(Alloc2D<ushort>(YSize, XSize));
  SetAlphaMap(0);
  Bitmap->Blit(this, 0, 0, 0, 0, XSize, YSize, Flags);
}

bitmap::bitmap(ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), IsIndependent(true)
{
  SetImage(Alloc2D<ushort>(YSize, XSize));
  SetAlphaMap(0);
}

bitmap::bitmap(vector2d Size) : XSize(Size.X), YSize(Size.Y), IsIndependent(true)
{
  SetImage(Alloc2D<ushort>(YSize, XSize));
  SetAlphaMap(0);
}

bitmap::bitmap(ushort XSize, ushort YSize, ushort Color) : XSize(XSize), YSize(YSize), IsIndependent(true)
{
  SetImage(Alloc2D<ushort>(YSize, XSize));
  SetAlphaMap(0);
  Fill(Color);
}

bitmap::bitmap(vector2d Size, ushort Color) : XSize(Size.X), YSize(Size.Y), IsIndependent(true)
{
  SetImage(Alloc2D<ushort>(YSize, XSize));
  SetAlphaMap(0);
  Fill(Color);
}

bitmap::bitmap(bitmap* MotherBitmap, ushort XPos, ushort YPos, ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), IsIndependent(false)
{
  SetMotherBitmap(MotherBitmap);
  SetXPos(XPos);
  SetYPos(YPos);
}

bitmap::bitmap(bitmap* MotherBitmap, vector2d Pos, vector2d Size) : XSize(Size.X), YSize(Size.Y), IsIndependent(false)
{
  SetMotherBitmap(MotherBitmap);
  SetXPos(Pos.X);
  SetYPos(Pos.Y);
}

bitmap::~bitmap()
{
  if(IsIndependent)
    {
      delete [] GetImage();
      delete [] GetAlphaMap();
    }
}

void bitmap::Save(outputfile& SaveFile) const
{
  if(!IsIndependent)
    ABORT("Subbitmap save request detected!");

  SaveFile.Write(reinterpret_cast<char*>(GetImage()[0]), (XSize * YSize) << 1);

  if(GetAlphaMap())
    {
      SaveFile << uchar(1);
      SaveFile.Write(reinterpret_cast<char*>(GetAlphaMap()[0]), XSize * YSize);
    }
  else
    SaveFile << uchar(0);
}

void bitmap::Load(inputfile& SaveFile)
{
  SaveFile.Read(reinterpret_cast<char*>(GetImage()[0]), (XSize * YSize) << 1);

  uchar Alpha;
  SaveFile >> Alpha;

  if(Alpha)
    {
      SetAlphaMap(Alloc2D<uchar>(YSize, XSize));
      SaveFile.Read(reinterpret_cast<char*>(GetAlphaMap()[0]), XSize * YSize);
    }
}

void bitmap::Save(const std::string& FileName) const
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

  for(long y = YSize - 1; y >= 0; --y)
    for(ushort x = 0; x < XSize; ++x)
      {
	ushort Pixel = GetPixel(x, y);
	SaveFile << char(Pixel << 3) << char((Pixel >> 5) << 2) << char((Pixel >> 11) << 3);
      }
}

void bitmap::Fill(ushort X, ushort Y, ushort Width, ushort Height, ushort Color)
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->Fill(GetXPos() + X, GetYPos() + Y, Width, Height, Color);
      return;
    }

  if(X > XSize || Y > YSize)
    return;

  if(X + Width > XSize)
    Width = XSize - X;

  if(Y + Height > YSize)
    Height = YSize - Y;

  ushort** Image = GetImage();

  for(ushort y = 0; y < Height; ++y)
    for(ushort* Ptr = &Image[Y + y][X], x = 0; x < Width; ++x, ++Ptr)
      *Ptr = Color;
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->Blit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Flags);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      Blit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Flags);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  if(Flags & ROTATE && Width != Height)
    ABORT("Blit error: FeLib supports only square rotating!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  Flags &= 0x7;
  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();

  switch(Flags)
    {
    case NONE:
      {
	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY + y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case MIRROR:
      {
	DestX += Width - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY + y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, --DestPtr)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case FLIP:
      {
	DestY += Height - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY - y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP):
      {
	DestX += Width - 1;
	DestY += Height - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY - y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, --DestPtr)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case ROTATE:
      {
	DestX += Width - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX - y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr += TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | ROTATE):
      {
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX + y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr += TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (FLIP | ROTATE):
      {
	DestX += Width - 1;
	DestY += Height - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX - y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr -= TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	DestY += Height - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX + y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr -= TrueDestXMove)
	      *DestPtr = *SrcPtr;
	  }

	break;
      }
    }
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ulong Luminance) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->Blit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Luminance);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      Blit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Luminance);
      return;
    }

  if(Luminance == MakeRGB24(128, 128, 128))
    {
      Blit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();

  ushort RedLuminance = (Luminance >> 15 & 0x1FE) - 256;
  ushort GreenLuminance = (Luminance >> 7 & 0x1FE) - 256;
  ushort BlueLuminance = (Luminance << 1 & 0x1FE) - 256;

  for(ushort y = 0; y < Height; ++y)
    {
      ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
      ushort* DestPtr = &DestImage[DestY + y][DestX];

      for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	*DestPtr = MakeRGB16(Limit<short>(GetRed16(*SrcPtr) + RedLuminance, 0, 0xFF),
			     Limit<short>(GetGreen16(*SrcPtr) + GreenLuminance, 0, 0xFF),
			     Limit<short>(GetBlue16(*SrcPtr) + BlueLuminance, 0, 0xFF));
    }
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags, ushort MaskColor) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->MaskedBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Flags, MaskColor);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      MaskedBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Flags, MaskColor);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  if(Flags & ROTATE && Width != Height)
    ABORT("MaskedBlit error: FeLib supports only square rotating!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  Flags &= 0x7;
  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();

  switch(Flags)
    {
    case NONE:
      {
	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY + y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case MIRROR:
      {
	DestX += Width - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY + y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, --DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case FLIP:
      {
	DestY += Height - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY - y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP):
      {
	DestX += Width - 1;
	DestY += Height - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY - y][DestX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, --DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case ROTATE:
      {
	DestX += Width - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX - y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr += TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | ROTATE):
      {
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX + y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr += TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (FLIP | ROTATE):
      {
	DestX += Width - 1;
	DestY += Height - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX - y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr -= TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	DestY += Height - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX + y];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr -= TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = *SrcPtr;
	  }

	break;
      }
    }
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ulong Luminance, ushort MaskColor) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->MaskedBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      MaskedBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(Luminance == MakeRGB24(128, 128, 128))
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, uchar(0), MaskColor);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();

  ushort RedLuminance = (Luminance >> 15 & 0x1FE) - 256;
  ushort GreenLuminance = (Luminance >> 7 & 0x1FE) - 256;
  ushort BlueLuminance = (Luminance << 1 & 0x1FE) - 256;

  for(ushort y = 0; y < Height; ++y)
    {
      ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
      ushort* DestPtr = &DestImage[DestY + y][DestX];

      for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	if(*SrcPtr != MaskColor)
	  *DestPtr = MakeRGB16(Limit<short>(GetRed16(*SrcPtr) + RedLuminance, 0, 0xFF),
			       Limit<short>(GetGreen16(*SrcPtr) + GreenLuminance, 0, 0xFF),
			       Limit<short>(GetBlue16(*SrcPtr) + BlueLuminance, 0, 0xFF));
    }
}

void bitmap::SimpleAlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Alpha, ushort MaskColor) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->SimpleAlphaBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Alpha, MaskColor);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      SimpleAlphaBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Alpha, MaskColor);
      return;
    }

  if(Alpha == 255)
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, uchar(0), MaskColor);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();
  uchar NegAlpha = 0xFF - Alpha;

  for(ushort y = 0; y < Height; ++y)
    {
      ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
      ushort* DestPtr = &DestImage[DestY + y][DestX];

      for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	if(*SrcPtr != MaskColor)
	  *DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * Alpha + GetRed16(*DestPtr) * NegAlpha,
					     GetGreen16(*SrcPtr) * Alpha + GetGreen16(*DestPtr) * NegAlpha,
					     GetBlue16(*SrcPtr) * Alpha + GetBlue16(*DestPtr) * NegAlpha);
    }
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags, ushort MaskColor) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->AlphaBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Flags, MaskColor);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      AlphaBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Flags, MaskColor);
      return;
    }

  if(!GetAlphaMap())
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, Flags, MaskColor);
      return;
    }

  if(Flags & ROTATE && Width != Height)
    ABORT("AlphaBlit error: FeLib supports only square rotating!");

  if(!Width || !Height)
    ABORT("Zero-sized bitmap alpha blit attempt detected!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  Flags &= 0x7;
  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();
  uchar** AlphaMap = GetAlphaMap();

  switch(Flags)
    {
    case NONE:
      {
	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY + y][DestX];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case MIRROR:
      {
	DestX += Width - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY + y][DestX];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, --DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case FLIP:
      {
	DestY += Height - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY - y][DestX];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case (MIRROR | FLIP):
      {
	DestX += Width - 1;
	DestY += Height - 1;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY - y][DestX];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, --DestPtr)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case ROTATE:
      {
	DestX += Width - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX - y];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr += TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case (MIRROR | ROTATE):
      {
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX + y];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr += TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case (FLIP | ROTATE):
      {
	DestX += Width - 1;
	DestY += Height - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX - y];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr -= TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	DestY += Height - 1;
	ulong TrueDestXMove = Bitmap->XSize;

	for(ushort y = 0; y < Height; ++y)
	  {
	    ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
	    ushort* DestPtr = &DestImage[DestY][DestX + y];
	    uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

	    for(ushort x = 0; x < Width; ++x, ++SrcPtr, DestPtr -= TrueDestXMove)
	      if(*SrcPtr != MaskColor)
		*DestPtr = RightShift8AndMakeRGB16(GetRed16(*SrcPtr) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetGreen16(*SrcPtr) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
						   GetBlue16(*SrcPtr) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
	  }

	break;
      }
    }
}

void bitmap::DrawLine(ushort OrigFromX, ushort OrigFromY, ushort OrigToX, ushort OrigToY, ushort Color, bool Wide)
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->DrawLine(GetXPos() + OrigFromX, GetYPos() + OrigFromY, GetXPos() + OrigToX, GetYPos() + OrigToY, Color, Wide);
      return;
    }

  if(OrigFromY == OrigToY)
    DrawHorizontalLine(OrigFromX, OrigToX, OrigFromY, Color, Wide);

  if(OrigFromX == OrigToX)
    DrawVerticalLine(OrigFromX, OrigFromY, OrigToY, Color, Wide);

  static short PointX[] = { 0, 0, -1, 1, 0 };
  static short PointY[] = { 0, -1, 0, 0, 1 };
  ushort Times = Wide ? 5 : 1;

  for(ushort c = 0; c < Times; ++c)
    {
      short FromX = OrigFromX + PointX[c];
      short FromY = OrigFromY + PointY[c];
      short ToX = OrigToX + PointX[c];
      short ToY = OrigToY + PointY[c];
      short DistaX = ToX - FromX;
      short DistaY = ToY - FromY;

      if(DistaX < 0)
	DistaX = -DistaX;

      if(DistaY < 0)
	DistaY = -DistaY;

      if(DistaX >= DistaY)
	{
	  if(FromX > ToX)
	    {
	      Swap(FromX, ToX);
	      Swap(FromY, ToY);
	    }

	  /* FromY may be negative, so using &GetImage()[FromY][FromX] is a bad idea */

	  ushort* Ptr = &GetImage()[0][0] + FromY * XSize + FromX;
	  short UFO = (DistaX >> 1) + 1;
	  short YMove, PtrYMove;

	  if(FromY <= ToY)
	    {
	      YMove = 1;
	      PtrYMove = XSize;
	    }
	  else
	    {
	      YMove = -1;
	      PtrYMove = -XSize;
	    }

	  for(short x = FromX, y = FromY, c = 0; x <= ToX; ++x, ++Ptr, c += DistaY)
	    {
	      if(c >= UFO)
		{
		  c -= DistaX;
		  y += YMove;
		  Ptr += PtrYMove;
		}

	      if(IsValidPos(x, y))
		*Ptr = Color;
	    }
	}
      else
	{
	  if(FromY > ToY)
	    {
	      Swap(FromX, ToX);
	      Swap(FromY, ToY);
	    }

	  /* FromY may be negative, so using &GetImage()[FromY][FromX] is a bad idea */

	  ushort* Ptr = &GetImage()[0][0] + FromY * XSize + FromX;
	  short UFO = (DistaY >> 1) + 1;
	  short XMove = FromX <= ToX ? 1 : -1;

	  for(short x = FromX, y = FromY, c = 0; y <= ToY; ++y, Ptr += XSize, c += DistaX)
	    {
	      if(c >= UFO)
		{
		  c -= DistaY;
		  x += XMove;
		  Ptr += XMove;
		}

	      if(IsValidPos(x, y))
		*Ptr = Color;
	    }
	}
    }
}

void bitmap::DrawVerticalLine(ushort OrigX, ushort OrigFromY, ushort OrigToY, ushort Color, bool Wide)
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->DrawVerticalLine(GetXPos() + OrigX, GetYPos() + OrigFromY, GetYPos() + OrigToY, Color, Wide);
      return;
    }

  static short PointX[] = { 0, -1, 1 };
  ushort Times = Wide ? 3 : 1;

  for(ushort c = 0; c < Times; ++c)
    {
      short X = OrigX + PointX[c];
      short FromY = OrigFromY;
      short ToY = OrigToY;

      if(FromY > ToY)
	Swap(FromY, ToY);

      if(Wide && !c)
	{
	  --FromY;
	  ++ToY;
	}

      if(X < 0 || X >= XSize || ToY < 0 || FromY >= YSize)
	continue;

      FromY = Max<short>(FromY, 0);
      ToY = Min<short>(ToY, YSize);
      ushort* Ptr = &GetImage()[FromY][X];

      for(short y = FromY; y <= ToY; ++y, Ptr += XSize)
	*Ptr = Color;
    }
}

void bitmap::DrawHorizontalLine(ushort OrigFromX, ushort OrigToX, ushort OrigY, ushort Color, bool Wide)
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->DrawVerticalLine(GetXPos() + OrigFromX, GetXPos() + OrigToX, GetYPos() + OrigY, Color, Wide);
      return;
    }

  static short PointY[] = { 0, -1, 1 };
  ushort Times = Wide ? 3 : 1;

  for(ushort c = 0; c < Times; ++c)
    {
      short Y = OrigY + PointY[c];
      short FromX = OrigFromX;
      short ToX = OrigToX;

      if(FromX > ToX)
	Swap(FromX, ToX);

      if(Wide && !c)
	{
	  --FromX;
	  ++ToX;
	}

      if(Y < 0 || Y >= YSize || ToX < 0 || FromX >= XSize)
	continue;

      FromX = Max<short>(FromX, 0);
      ToX = Min<short>(ToX, XSize);
      ushort* Ptr = &GetImage()[Y][FromX];

      for(short x = FromX; x <= ToX; ++x, ++Ptr)
	*Ptr = Color;
    }
}

void bitmap::DrawPolygon(vector2d Center, ushort Radius, ushort NumberOfSides, ushort Color, bool DrawSides, bool DrawDiameters, double Rotation)
{
  if(!IsIndependent)
    {
      /* Note: this doesn't care about subbitmap borders! */

      GetMotherBitmap()->DrawPolygon(vector2d(GetXPos(), GetYPos()) + Center, Radius, NumberOfSides, Color, DrawSides, DrawDiameters, Rotation);
      return;
    }

  if(!DrawSides && !DrawDiameters)
    return;

  std::vector<vector2d> Points;
  ushort c;

  for(c = 0; c < NumberOfSides; ++c)
    {
      float PosX = sin((2 * FPI / NumberOfSides) * c + Rotation) * Radius;
      float PosY = cos((2 * FPI / NumberOfSides) * c + Rotation) * Radius;
      Points.push_back(vector2d(short(PosX), short(PosY)) + Center);
    }

  if(DrawDiameters && DrawSides)
    for(c = 0; c < Points.size(); ++c)
      for(ushort a = 0; a < Points.size(); ++a)
	if(c != a)
	  DrawLine(Points[c].X, Points[c].Y, Points[a].X, Points[a].Y, Color, true);

  if(DrawDiameters && !DrawSides)
    for(c = 0; c < Points.size(); ++c)
      for(ushort a = 0; a < Points.size(); ++a)
	if(abs(int(c - a)) > 1 && !((a == 0) && c == Points.size() - 1) && !((c == 0) && a == Points.size() - 1))
	  DrawLine(Points[c].X, Points[c].Y, Points[a].X, Points[a].Y, Color, true);

  if(!DrawDiameters)
    for(c = 0; c < NumberOfSides; ++c)
      DrawLine(Points[c].X, Points[c].Y, Points[(c + 1) % Points.size()].X, Points[(c + 1) % Points.size()].Y, Color, true);
}

void bitmap::CreateAlphaMap(uchar InitialValue)
{
  if(!IsIndependent)
    ABORT("Subbitmap AlphaMap creation request detected!");

  if(GetAlphaMap())
    ABORT("Alpha leak detected!");

  SetAlphaMap(Alloc2D<uchar>(YSize, XSize, InitialValue));
}

bool bitmap::ChangeAlpha(char Amount)
{
  if(!IsIndependent)
    ABORT("Subbitmap alpha change request detected!");

  if(!Amount)
    return false;

  bool Changes = false;

  if(!GetAlphaMap())
    ABORT("No alpha map to fade.");

  if(Amount > 0)
    {
      for(ulong c = 0; c < ulong(XSize * YSize); ++c)
	if(GetAlphaMap()[0][c] < 255 - Amount)
	  {
	    GetAlphaMap()[0][c] += Amount;
	    Changes = true;
	  }
	else
	  if(GetAlphaMap()[0][c] != 255)
	    {
	      GetAlphaMap()[0][c] = 255;
	      Changes = true;
	    }
    }
  else
    for(ulong c = 0; c < ulong(XSize * YSize); ++c)
      if(GetAlphaMap()[0][c] > -Amount)
	{
	  GetAlphaMap()[0][c] += Amount;
	  Changes = true;
	}
      else
	if(GetAlphaMap()[0][c])
	  {
	    GetAlphaMap()[0][c] = 0;
	    Changes = true;
	  }

  return Changes;
}

void bitmap::Outline(ushort Color)
{
  if(!IsIndependent)
    ABORT("Subbitmap outline request detected!");

  ushort LastColor, NextColor;

  for(ushort x = 0; x < XSize; ++x)
    {
      ushort* Buffer = &GetImage()[0][x];
      LastColor = *Buffer;

      for(ushort y = 0; y < YSize - 1; ++y)
	{
	  NextColor = *(Buffer + XSize);

	  if((LastColor == TRANSPARENT_COLOR || !y) && NextColor != TRANSPARENT_COLOR)
	    *Buffer = Color;

	  Buffer += XSize;

	  if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || y == YSize - 2))
	    *Buffer = Color;

	  LastColor = NextColor;
	}
    }

  for(ushort y = 0; y < YSize; ++y)
    {
      ushort* Buffer = GetImage()[y];
      LastColor = *Buffer;

      for(ushort x = 0; x < XSize - 1; ++x)
	{
	  NextColor = *(Buffer + 1);

	  if((LastColor == TRANSPARENT_COLOR || !x) && NextColor != TRANSPARENT_COLOR)
	    *Buffer = Color;

	  ++Buffer;

	  if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || x == XSize - 2))
	    *Buffer = Color;

	  LastColor = NextColor;
	}
    }
}

void bitmap::CreateOutlineBitmap(bitmap* Bitmap, ushort Color)
{
  if(!IsIndependent)
    ABORT("Subbitmap outline bitmap creation request detected!");

  Bitmap->Fill(TRANSPARENT_COLOR);

  for(ushort x = 0; x < XSize; ++x)
    {
      ushort* SrcBuffer = &GetImage()[0][x];
      ushort* DestBuffer = &Bitmap->GetImage()[0][x];
      ushort LastColor = *SrcBuffer;

      for(ushort y = 0; y < YSize - 1; ++y)
	{
	  ushort NextColor = *(SrcBuffer + XSize);

	  if((LastColor == TRANSPARENT_COLOR || !y) && NextColor != TRANSPARENT_COLOR)
	    *DestBuffer = Color;

	  SrcBuffer += XSize;
	  DestBuffer += Bitmap->XSize;

	  if(LastColor != TRANSPARENT_COLOR && (NextColor == TRANSPARENT_COLOR || y == YSize - 2))
	    *DestBuffer = Color;

	  LastColor = NextColor;
	}
    }

  for(ushort y = 0; y < YSize; ++y)
    {
      ushort* SrcBuffer = GetImage()[y];
      ushort* DestBuffer = Bitmap->GetImage()[y];
      ushort LastSrcColor = *SrcBuffer;
      ushort LastDestColor = *DestBuffer;

      for(ushort x = 0; x < XSize - 1; ++x)
	{
	  ushort NextSrcColor = *(SrcBuffer + 1);
	  ushort NextDestColor = *(DestBuffer + 1);

	  if((LastSrcColor == TRANSPARENT_COLOR || !x) && (NextSrcColor != TRANSPARENT_COLOR || NextDestColor != TRANSPARENT_COLOR))
	    *DestBuffer = Color;

	  ++SrcBuffer;
	  ++DestBuffer;

	  if((LastSrcColor != TRANSPARENT_COLOR || LastDestColor != TRANSPARENT_COLOR) && (NextSrcColor == TRANSPARENT_COLOR || x == XSize - 2))
	    *DestBuffer = Color;

	  LastSrcColor = NextSrcColor;
	  LastDestColor = NextDestColor;
	}
    }
}

void bitmap::ResetAlpha(ushort X, ushort Y)
{
  if(IsIndependent)
    {
      uchar** AlphaMap = GetAlphaMap();

      if(AlphaMap)
	AlphaMap[Y][X] = 255;
    }
  else
    GetMotherBitmap()->ResetAlpha(X + GetXPos(), Y + GetYPos());
}

void bitmap::PutPixel(ushort X, ushort Y, ushort Color)
{
  if(IsIndependent)
    GetImage()[Y][X] = Color;
  else
    GetMotherBitmap()->PutPixel(GetXPos() + X, GetYPos() + Y, Color);
}

ushort bitmap::GetPixel(ushort X, ushort Y) const
{
  if(IsIndependent)
    return GetImage()[Y][X];
  else
    return GetMotherBitmap()->GetPixel(GetXPos() + X, GetYPos() + Y);
}

void bitmap::SetAlpha(ushort X, ushort Y, uchar Alpha)
{
  if(IsIndependent)
    GetAlphaMap()[Y][X] = Alpha;
  else
    GetMotherBitmap()->SetAlpha(GetXPos() + X, GetYPos() + Y, Alpha);
}

uchar bitmap::GetAlpha(ushort X, ushort Y) const
{
  if(IsIndependent)
    return GetAlphaMap()[Y][X];
  else
    return GetMotherBitmap()->GetAlpha(GetXPos() + X, GetYPos() + Y);
}

void bitmap::FadeToScreen(void (*BitmapEditor)(bitmap*))
{
  bitmap Backup(RES_X, RES_Y);
  DOUBLE_BUFFER->Blit(&Backup);

  for(ushort c = 0; c <= 5; ++c)
    {
      clock_t StartTime = clock();
      ushort Element = 127 - c * 25;
      Backup.MaskedBlit(DOUBLE_BUFFER, MakeRGB24(Element, Element, Element), 0);

      if(BitmapEditor)
	BitmapEditor(this);

      SimpleAlphaBlit(DOUBLE_BUFFER, c * 50, 0);
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.01f * CLOCKS_PER_SEC);
    }

  DOUBLE_BUFFER->Fill(0);

  if(BitmapEditor)
    BitmapEditor(this);

  MaskedBlit(DOUBLE_BUFFER, uchar(0), 0);
  graphics::BlitDBToScreen();
}

void bitmap::StretchBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, char Stretch) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->StretchBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Stretch);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      StretchBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Stretch);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap stretch blit attempt detected!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  if(Stretch > 1)
    {
      ushort tx = DestX;

      for(ushort x1 = SourceX; x1 < SourceX + Width; ++x1, tx += Stretch)
	{
	  ushort ty = DestY;

	  for(ushort y1 = SourceY; y1 < SourceY + Height; ++y1, ty += Stretch)
	    {
	      ushort Pixel = GetImage()[y1][x1];

	      if(Pixel != TRANSPARENT_COLOR)
		for(ushort x2 = tx; x2 < tx + Stretch; ++x2)
		  for(ushort y2 = ty; y2 < ty + Stretch; ++y2)
		    Bitmap->GetImage()[y2][x2] = Pixel;
	    }
	}

      return;
    }
  else if(Stretch < -1)
    {
      ushort tx = DestX;

      for(ushort x1 = SourceX; x1 < SourceX + Width; x1 -= Stretch, ++tx)
	{
	  ushort ty = DestY;

	  for(ushort y1 = SourceY; y1 < SourceY + Height; y1 -= Stretch, ++ty)
	    {
	      ushort Pixel = GetImage()[y1][x1];

	      if(Pixel != TRANSPARENT_COLOR)
		Bitmap->GetImage()[ty][tx] = Pixel;
	    }
	}

      return;
    }
  else
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height);
      return;
    }
}

outputfile& operator<<(outputfile& SaveFile, bitmap* Bitmap)
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
      ushort XSize, YSize;
      SaveFile >> XSize >> YSize;
      Bitmap = new bitmap(XSize, YSize);
      Bitmap->Load(SaveFile);
    }

  return SaveFile;
}

void bitmap::DrawRectangle(ushort Left, ushort Top, ushort Right, ushort Bottom, ushort Color, bool Wide)
{
  DrawHorizontalLine(Left, Right, Top, Color, Wide);
  DrawHorizontalLine(Left, Right, Bottom, Color, Wide);
  DrawVerticalLine(Right, Top, Bottom, Color, Wide);
  DrawVerticalLine(Left, Top, Bottom, Color, Wide);
}

void bitmap::PowerBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ulong Luminance, ushort MaskColor) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->PowerBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      PowerBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(Luminance == MakeRGB24(128, 128, 128))
    {
      AlphaBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, 0, MaskColor);
      return;
    }

  if(!GetAlphaMap())
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, Luminance, MaskColor);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap power blit attempt detected!");

  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->XSize, Bitmap->YSize))
    return;

  ushort** SrcImage = GetImage();
  ushort** DestImage = Bitmap->GetImage();
  uchar** AlphaMap = GetAlphaMap();

  ushort RedLuminance = (Luminance >> 15 & 0x1FE) - 256;
  ushort GreenLuminance = (Luminance >> 7 & 0x1FE) - 256;
  ushort BlueLuminance = (Luminance << 1 & 0x1FE) - 256;

  for(ushort y = 0; y < Height; ++y)
    {
      ushort* SrcPtr = &SrcImage[SourceY + y][SourceX];
      ushort* DestPtr = &DestImage[DestY + y][DestX];
      uchar* AlphaPtr = &AlphaMap[SourceY + y][SourceX];

      for(ushort x = 0; x < Width; ++x, ++SrcPtr, ++DestPtr, ++AlphaPtr)
	if(*SrcPtr != MaskColor)
	  *DestPtr = RightShift8AndMakeRGB16(Limit<short>(GetRed16(*SrcPtr) + RedLuminance, 0, 0xFF) * (*AlphaPtr) + GetRed16(*DestPtr) * (0xFF - (*AlphaPtr)),
					     Limit<short>(GetGreen16(*SrcPtr) + GreenLuminance, 0, 0xFF) * (*AlphaPtr) + GetGreen16(*DestPtr) * (0xFF - (*AlphaPtr)),
					     Limit<short>(GetBlue16(*SrcPtr) + BlueLuminance, 0, 0xFF) * (*AlphaPtr) + GetBlue16(*DestPtr) * (0xFF - (*AlphaPtr)));
    }
}

void bitmap::CreateFlames(ushort Frame, ushort MaskColor)
{
  ushort* FlameLowestPoint = new ushort[XSize];
  ushort x,y, Top, MaxDist, RelPos;
  ulonglong OldSeed = femath::GetSeed();
  femath::SetSeed((Frame & 15) + 1); /* We want flame animation loops to be same in every session */

  for(x = 0; x < XSize; ++x)
    {
      FlameLowestPoint[x] = NOFLAME;

      if(GetPixel(x, 0) != MaskColor)
	FlameLowestPoint[x] = 0;
      else
	{
	  for(ushort y = 1; y < YSize; ++y)
	    if(GetPixel(x,y - 1) == MaskColor && GetPixel(x,y) != MaskColor && (FlameLowestPoint[x] == NOFLAME && FlameLowestPoint[x] > y))
	      FlameLowestPoint[x] = y;
	}
    }
  
  for(x = 0; x < 16; ++x)
    {
      if(FlameLowestPoint[x] != NOFLAME)
	{
	  if(FlameLowestPoint[x] != 0)
	    {
	      Top = RAND() % FlameLowestPoint[x];

	      for(y = Top; y <= FlameLowestPoint[x]; ++y)
		{
		  MaxDist = FlameLowestPoint[x] - Top;
		  RelPos = y - Top;
		  SafePutPixelAndResetAlpha(x,y, MakeRGB16((RelPos << 7) / MaxDist, 255 - ((RelPos << 7) / MaxDist), 0));
		}
	    }
	  else if(RAND() & 1)
	    SafePutPixelAndResetAlpha(x,0, MakeRGB16(0,255,0));
	}
    }

  femath::SetSeed(OldSeed);
}

void bitmap::CreateSparkle(vector2d SparklePos, ushort Frame)
{
  if(!Frame)
    return;

  ushort Size = (Frame - 1) * (16 - Frame) / 10;
  SafePutPixelAndResetAlpha(SparklePos.X, SparklePos.Y, WHITE);

  for(ushort c = 1; c < Size; ++c)
    {
      uchar Lightness = 191 + ((Size - c) << 6) / Size;
      ushort RGB = MakeRGB16(Lightness, Lightness, Lightness);
      SafePutPixelAndResetAlpha(SparklePos.X + c, SparklePos.Y, RGB);
      SafePutPixelAndResetAlpha(SparklePos.X - c, SparklePos.Y, RGB);
      SafePutPixelAndResetAlpha(SparklePos.X, SparklePos.Y + c, RGB);
      SafePutPixelAndResetAlpha(SparklePos.X, SparklePos.Y - c, RGB);
    }
}

void bitmap::CreateFlies(ulonglong Seed, ushort Frame, uchar FlyAmount)
{
  ulonglong OldSeed = femath::GetSeed();
  femath::SetSeed(Seed);

  for(uchar c = 0; c < FlyAmount; ++c)
    {
      double Constant = double(RAND() % 10000) / 10000 * FPI;
      vector2d StartPos = vector2d(5 + RAND() % 6, 5 + RAND() % 6);
      double Temp = (double(16 - Frame) * FPI) / 16;

      if(RAND() & 1)
	Temp = -Temp;

      vector2d Where;
      Where.X = short(StartPos.X + sin(Constant + Temp) * 3);
      Where.Y = short(StartPos.Y + sin(2*(Constant + Temp)) * 3);
      SafePutPixelAndResetAlpha(Where.X, Where.Y, MakeRGB16(0, 0, 0));
    }

  femath::SetSeed(OldSeed);
}

void bitmap::CreateLightning(ulonglong Seed, ushort Color)
{
  ulonglong OldSeed = femath::GetSeed();
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

  femath::SetSeed(OldSeed);
}

bool bitmap::CreateLightning(vector2d StartPos, vector2d Direction, ushort MaxLength, ushort Color)
{
  CurrentSprite = this;
  CurrentPixelVector.clear();
  vector2d LastMove(0, 0);
  ushort Counter = 0;

  while(true)
    {
      vector2d Move(1 + (RAND() & 3), 1 + (RAND() & 3));

      if(Direction.X < 0 || (!Direction.X && RAND() & 1))
	Move.X = -Move.X;

      if(Direction.Y < 0 || (!Direction.Y && RAND() & 1))
	Move.Y = -Move.Y;

      Move.X = Limit<short>(Move.X, -StartPos.X, XSize - StartPos.X - 1);
      Move.Y = Limit<short>(Move.Y, -StartPos.Y, XSize - StartPos.Y - 1);

      if(Counter < 10 && ((!Move.Y && !LastMove.Y) || (Move.Y && LastMove.Y && (Move.X << 10) / Move.Y == (LastMove.X << 10) / LastMove.Y)))
	{
	  ++Counter;
	  continue;
	}

      Counter = 0;

      if(!femath::DoLine(StartPos.X, StartPos.Y, StartPos.X + Move.X, StartPos.Y + Move.Y, PixelVectorHandler) || MaxLength <= CurrentPixelVector.size())
	{
	  ushort Limit = Min<ushort>(CurrentPixelVector.size(), MaxLength);

	  for(ushort c = 0; c < Limit; ++c)
	    PutPixel(CurrentPixelVector[c], Color);

	  CurrentPixelVector.clear();
	  return true;
	}

      StartPos += Move;
      LastMove = Move;

      if((Direction.X && (!StartPos.X || StartPos.X == XSize - 1)) || (Direction.Y && (!StartPos.Y || StartPos.Y == XSize - 1)))
	return false;
    }
}

bool bitmap::PixelVectorHandler(long X, long Y)
{
  if(CurrentSprite->GetPixel(X, Y) == TRANSPARENT_COLOR)
    {
      CurrentPixelVector.push_back(vector2d(X, Y));
      return true;
    }
  else
    return false;
}
