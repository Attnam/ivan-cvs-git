#include <cmath>
#include <ctime>

#include "bitmap.h"
#include "graphics.h"
#include "error.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"
#include "blit.h"

bitmap::bitmap(const std::string& FileName) : IsIndependent(true)
{
  SetAlphaMap(0);
  inputfile File(FileName.c_str(), false);

  if(!File.IsOpen())
    ABORT("Bitmap %s not found!", FileName.c_str());

  uchar Palette[768];
  File.SeekPosEnd(-768);
  File.Read(reinterpret_cast<char*>(Palette), 768);
  File.SeekPosBeg(8);
  XSize  =  File.Get();
  XSize += (File.Get() << 8) + 1;
  YSize  =  File.Get();
  YSize += (File.Get() << 8) + 1;
  File.SeekPosBeg(128);
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

  ulong TrueOffset = ulong(&GetImage()[Y][X]);
  ulong TrueXMove = (XSize - Width) << 1;

  ::Fill(TrueOffset, TrueXMove, Width, Height, Color);
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
  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;

  switch(Flags)
    {
    case NONE:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
	BlitNoFlags(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
	break;
      }

    case MIRROR:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
	BlitMirror(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
	break;
      }

    case FLIP:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
	BlitFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
	break;
      }

    case (MIRROR | FLIP):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
	BlitMirrorFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
	break;
      }

    case ROTATE:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	BlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }

    case (MIRROR | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	BlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }

    case (FLIP | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	BlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	BlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
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

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;
  ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
  BlitLuminated(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Luminance);
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

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;

  switch(Flags)
    {
    case NONE:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
	MaskedBlitNoFlags(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case MIRROR:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
	MaskedBlitMirror(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case FLIP:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
	MaskedBlitFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | FLIP):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
	MaskedBlitMirrorFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case ROTATE:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	MaskedBlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	MaskedBlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (FLIP | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	MaskedBlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	MaskedBlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
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

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;
  ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
  MaskedBlitLuminated(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Luminance, MaskColor);
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

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;
  ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
  ::SimpleAlphaBlit(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Alpha, MaskColor);
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

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;
  ulong AlphaMapOffset = ulong(&GetAlphaMap()[SourceY][SourceX]);

  switch(Flags)
    {
    case NONE:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
	AlphaBlitNoFlags(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case MIRROR:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
	AlphaBlitMirror(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case FLIP:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
	AlphaBlitFlip(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | FLIP):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
	AlphaBlitMirrorFlip(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
	break;
      }

    case ROTATE:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	AlphaBlitRotate90(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	AlphaBlitMirrorRotate90(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (FLIP | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	AlphaBlitFlipRotate90(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | FLIP | ROTATE):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	AlphaBlitMirrorFlipRotate90(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
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

  ushort ThisXSize = XSize, ThisYSize = YSize;
  ulong Pitch = XSize << 1, Surface = ulong(GetImage()[0]);
  static vector2d Point[] = {vector2d(0, 0), vector2d(0, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(0, 1)};

  for(ushort c = 0; c < (Wide ? 5 : 1); ++c)
    {
      ushort FromX = OrigFromX + Point[c].X;
      ushort FromY = OrigFromY + Point[c].Y;
      ushort ToX = OrigToX + Point[c].X;
      ushort ToY = OrigToY + Point[c].Y;
      ::DrawLine(Surface, Pitch, FromX, FromY, ToX, ToY, ThisXSize, ThisYSize, Color);
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
      float PosX = sin((2 * PI / NumberOfSides) * c + Rotation) * Radius;
      float PosY = cos((2 * PI / NumberOfSides) * c + Rotation) * Radius;
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

  ulong Buffer;
  ushort LastColor, NextColor;

  for(ushort x = 0; x < XSize; ++x)
    {
      Buffer = ulong(&GetImage()[0][x]);

      LastColor = *reinterpret_cast<ushort*>(Buffer);

      for(ushort y = 0; y < YSize - 1; ++y)
	{
	  NextColor = *reinterpret_cast<ushort*>(Buffer + (XSize << 1));

	  if((LastColor == DEFAULTTRANSPARENT || !y) && NextColor != DEFAULTTRANSPARENT)
	    *reinterpret_cast<ushort*>(Buffer) = Color;

	  Buffer += XSize << 1;

	  if(LastColor != DEFAULTTRANSPARENT && (NextColor == DEFAULTTRANSPARENT || y == YSize - 2))
	    *reinterpret_cast<ushort*>(Buffer) = Color;

	  LastColor = NextColor;
	}
    }

  for(ushort y = 0; y < YSize; ++y)
    {
      Buffer = ulong(GetImage()[y]);
      LastColor = *reinterpret_cast<ushort*>(Buffer);

      for(ushort x = 0; x < XSize - 1; ++x)
	{
	  NextColor = *reinterpret_cast<ushort*>(Buffer + 2);

	  if((LastColor == DEFAULTTRANSPARENT || !x) && NextColor != DEFAULTTRANSPARENT)
	    *reinterpret_cast<ushort*>(Buffer) = Color;

	  Buffer += 2;

	  if(LastColor != DEFAULTTRANSPARENT && (NextColor == DEFAULTTRANSPARENT || x == XSize - 2))
	    *reinterpret_cast<ushort*>(Buffer) = Color;

	  LastColor = NextColor;
	}
    }
}

void bitmap::CreateOutlineBitmap(bitmap* Bitmap, ushort Color)
{
  if(!IsIndependent)
    ABORT("Subbitmap outline bitmap creation request detected!");

  Bitmap->Fill(DEFAULTTRANSPARENT);

  for(ushort x = 0; x < XSize; ++x)
    {
      ulong SrcBuffer = ulong(&GetImage()[0][x]);
      ulong DestBuffer = ulong(&Bitmap->GetImage()[0][x]);
      ushort LastColor = *reinterpret_cast<ushort*>(SrcBuffer);

      for(ushort y = 0; y < YSize - 1; ++y)
	{
	  ushort NextColor = *reinterpret_cast<ushort*>(SrcBuffer + (XSize << 1));

	  if((LastColor == DEFAULTTRANSPARENT || !y) && NextColor != DEFAULTTRANSPARENT)
	    *reinterpret_cast<ushort*>(DestBuffer) = Color;

	  SrcBuffer += XSize << 1;
	  DestBuffer += Bitmap->XSize << 1;

	  if(LastColor != DEFAULTTRANSPARENT && (NextColor == DEFAULTTRANSPARENT || y == YSize - 2))
	    *reinterpret_cast<ushort*>(DestBuffer) = Color;

	  LastColor = NextColor;
	}
    }

  for(ushort y = 0; y < YSize; ++y)
    {
      ulong SrcBuffer = ulong(GetImage()[y]);
      ulong DestBuffer = ulong(Bitmap->GetImage()[y]);
      ushort LastSrcColor = *reinterpret_cast<ushort*>(SrcBuffer);
      ushort LastDestColor = *reinterpret_cast<ushort*>(DestBuffer);

      for(ushort x = 0; x < XSize - 1; ++x)
	{
	  ushort NextSrcColor = *reinterpret_cast<ushort*>(SrcBuffer + 2);
	  ushort NextDestColor = *reinterpret_cast<ushort*>(DestBuffer + 2);

	  if((LastSrcColor == DEFAULTTRANSPARENT || !x) && (NextSrcColor != DEFAULTTRANSPARENT || NextDestColor != DEFAULTTRANSPARENT))
	    *reinterpret_cast<ushort*>(DestBuffer) = Color;

	  SrcBuffer += 2;
	  DestBuffer += 2;

	  if((LastSrcColor != DEFAULTTRANSPARENT || LastDestColor != DEFAULTTRANSPARENT) && (NextSrcColor == DEFAULTTRANSPARENT || x == XSize - 2))
	    *reinterpret_cast<ushort*>(DestBuffer) = Color;

	  LastSrcColor = NextSrcColor;
	  LastDestColor = NextDestColor;
	}
    }
}

void bitmap::FadeToScreen(void (*BitmapEditor)(bitmap*))
{
  bitmap Backup(RES);
  DOUBLEBUFFER->Blit(&Backup);

  for(ushort c = 0; c <= 5; ++c)
    {
      clock_t StartTime = clock();
      ushort Element = 127 - c * 25;
      Backup.MaskedBlit(DOUBLEBUFFER, MakeRGB24(Element, Element, Element), 0);

      if(BitmapEditor)
	BitmapEditor(this);

      SimpleAlphaBlit(DOUBLEBUFFER, c * 50, 0);
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.01f * CLOCKS_PER_SEC);
    }

  DOUBLEBUFFER->Fill(0);

  if(BitmapEditor)
    BitmapEditor(this);

  MaskedBlit(DOUBLEBUFFER, uchar(0), 0);
  graphics::BlitDBToScreen();
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

void bitmap::FillWithGradient(ushort X, ushort Y, ushort Width, ushort Height, ushort Color1, ushort Color2)
{
  for(ushort x = 0; x < Width; ++x)
    for(ushort y = 0; y < Height; ++y)
      {
	float Multiplier = (float(x) / Width + float(y) / Height) / 2;

	PutPixel(X + x, Y + y, MakeRGB16(ushort(GetRed16(Color1) * (1.0f - Multiplier) + GetRed16(Color2) * Multiplier),
					ushort(GetGreen16(Color1) * (1.0f - Multiplier) + GetGreen16(Color2) * Multiplier),
					ushort(GetBlue16(Color1) * (1.0f - Multiplier) + GetBlue16(Color2) * Multiplier)));
      }
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

	      if(Pixel != DEFAULTTRANSPARENT)
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

	      if(Pixel != DEFAULTTRANSPARENT)
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
  DrawLine(Left, Top, Right, Top, Color, Wide);
  DrawLine(Right, Top, Right, Bottom, Color, Wide);
  DrawLine(Right, Bottom, Left, Bottom, Color, Wide);
  DrawLine(Left, Bottom, Left, Top, Color, Wide);
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
	  {
	    ushort Red = GetRed16(*SrcPtr) + RedLuminance;

	    if(Red & 0x8000)
	      Red = 0;

	    if(Red > 0xFF)
	      Red = 0xFF;

	    ushort Green = GetGreen16(*SrcPtr) + GreenLuminance;

	    if(Green & 0x8000)
	      Green = 0;

	    if(Green > 0xFF)
	      Green = 0xFF;

	    ushort Blue = GetBlue16(*SrcPtr) + BlueLuminance;

	    if(Blue & 0x8000)
	      Blue = 0;

	    if(Blue > 0xFF)
	      Blue = 0xFF;

	    *DestPtr = ((Red * (*AlphaPtr) + GetRed16(*DestPtr) * (255 - (*AlphaPtr))) & 0xF800)
		     | ((Green * (*AlphaPtr) + GetGreen16(*DestPtr) * (255 - (*AlphaPtr))) >> 5 & 0x7E0)
		     | ((Blue * (*AlphaPtr) + GetBlue16(*DestPtr) * (255 - (*AlphaPtr))) >> 11);
	  }
    }
}

void bitmap::DrawFlames(ushort Frame, ushort MaskColor)
{
  ushort* FlameLowestPoint = new ushort[XSize];
  ushort x,y, Top, MaxDist, RelPos;
  long NewSeed = RAND(); 
  femath::SetSeed((Frame & 15) + 1); /* We want flame animation loops to be same in every session */

  for(x = 0; x < XSize; ++x)
    {
      FlameLowestPoint[x] = NOFLAME;
      if(GetPixel(x, 0) != MaskColor)
	FlameLowestPoint[x] = 0;
      else
	{
	  for(ushort y = 1; y < YSize; ++y)
	    {
	      if(GetPixel(x,y - 1) == MaskColor && GetPixel(x,y) != MaskColor && (FlameLowestPoint[x] == NOFLAME && FlameLowestPoint[x] > y))
		FlameLowestPoint[x] = y;
	    }
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
		  PutPixel(x,y, MakeRGB16((RelPos * 128) / MaxDist, 255 - ((RelPos * 128) / MaxDist), 0));
		}
	    }
	  else if(RAND() & 1)
	    PutPixel(x,0, MakeRGB16(0,255,0));
	}
    }

  femath::SetSeed(NewSeed);
}

void bitmap::CreateSparkle(vector2d SparklePos, ushort Frame)
{
  if(!Frame)
    return;

  ushort Size = (Frame - 1) * (16 - Frame) / 10;
  SafePutPixel(SparklePos.X, SparklePos.Y, WHITE);

  for(ushort c = 1; c < Size; ++c)
    {
      uchar Lightness = 191 + (Size - c) * 64 / Size;
      ushort RGB = MakeRGB16(Lightness, Lightness, Lightness);
      SafePutPixel(SparklePos.X + c, SparklePos.Y, RGB);
      SafePutPixel(SparklePos.X - c, SparklePos.Y, RGB);
      SafePutPixel(SparklePos.X, SparklePos.Y + c, RGB);
      SafePutPixel(SparklePos.X, SparklePos.Y - c, RGB);
    }
}

void bitmap::CreateFlies(ushort FlyAmount, ulong Seed, ushort Frame)
{
  long NewSeed = RAND();
  femath::SetSeed(Seed);
  for(ushort c = 0; c < FlyAmount; ++c)
    {
      double Constant = double(RAND() % 10000) / 10000 * PI;
      vector2d StartPos = vector2d(5 + RAND() % 6, 5 + RAND() % 6);
      double Temp = (double(16 - Frame) * PI) / 16;
      if(RAND() & 1)
	Temp = -Temp;
      vector2d Where;
      Where.X = short(StartPos.X + sin(Constant + Temp) * 3);
      Where.Y = short(StartPos.Y + sin(2*(Constant + Temp))  * 3);

      SafePutPixel(Where.X, Where.Y, MakeRGB16(100,100,100));
    }
  femath::SetSeed(NewSeed);
}
