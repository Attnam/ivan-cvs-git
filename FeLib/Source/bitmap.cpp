#include <string>
#include <cmath>
#include <ctime>

#include "bitmap.h"
#include "graphics.h"
#include "error.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"
#include "blit.h"

bitmap::bitmap(std::string FileName) : IsIndependent(true)
{
  SetAlphaMap(0);

  inputfile File(FileName.c_str(), false);

  if(!File.IsOpen())
    ABORT("Bitmap %s not found!", FileName.c_str());

  uchar Palette[768];
  File.SeekPosEnd(-768);
  File.Read((char*)Palette, 768);
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

bitmap::bitmap(bitmap* Bitmap) : XSize(Bitmap->XSize), YSize(Bitmap->YSize), IsIndependent(true)
{
  SetImage(Alloc2D<ushort>(YSize, XSize));
  SetAlphaMap(0);
  Bitmap->Blit(this, 0, 0, 0, 0, XSize, YSize);
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

  SaveFile.Write((char*)GetImage()[0], (XSize * YSize) << 1);

  if(GetAlphaMap())
    {
      SaveFile << uchar(1);
      SaveFile.Write((char*)GetAlphaMap()[0], XSize * YSize);
    }
  else
    SaveFile << uchar(0);
}

void bitmap::Load(inputfile& SaveFile)
{
  SaveFile.Read((char*)GetImage()[0], (XSize * YSize) << 1);

  uchar Alpha;
  SaveFile >> Alpha;

  if(Alpha)
    {
      SetAlphaMap(Alloc2D<uchar>(YSize, XSize));
      SaveFile.Read((char*)GetAlphaMap()[0], XSize * YSize);
    }
}

void bitmap::Save(std::string FileName) const
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

    case ROTATE_90:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	BlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }

    case (MIRROR | ROTATE_90):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	BlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }

    case (FLIP | ROTATE_90):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	BlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }

    case (MIRROR | FLIP | ROTATE_90):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	BlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
	break;
      }
    }
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
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

  if(Luminance == 256)
    {
      Blit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

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

    case ROTATE_90:
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	MaskedBlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | ROTATE_90):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	MaskedBlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (FLIP | ROTATE_90):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX + Width - 1]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
	MaskedBlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }

    case (MIRROR | FLIP | ROTATE_90):
      {
	ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY + Height - 1][DestX]);
	ulong TrueDestXMove = Bitmap->XSize << 1;
	ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
	MaskedBlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
	break;
      }
    }
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance, ushort MaskColor) const
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

  if(Luminance == 256)
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, uchar(0), MaskColor);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;
  ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

  MaskedBlitLuminated(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Luminance, MaskColor);
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Alpha, ushort MaskColor) const
{
  if(!IsIndependent)
    {
      GetMotherBitmap()->AlphaBlit(Bitmap, GetXPos() + SourceX, GetYPos() + SourceY, DestX, DestY, Width, Height, Alpha, MaskColor);
      return;
    }

  if(!Bitmap->IsIndependent)
    {
      AlphaBlit(Bitmap->GetMotherBitmap(), SourceX, SourceY, Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, Width, Height, Alpha, MaskColor);
      return;
    }

  if(Alpha == 255)
    {
      MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, uchar(0), MaskColor);
      return;
    }

  if(!Width || !Height)
    ABORT("Zero-sized bitmap blit attempt detected!");

  ulong TrueSourceOffset = ulong(&GetImage()[SourceY][SourceX]);
  ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
  ulong TrueSourceXMove = (XSize - Width) << 1;
  ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

  ::AlphaBlit(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Alpha, MaskColor);
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort DestX, ushort DestY, ushort MaskColor) const
{
  if(!IsIndependent)
    ABORT("Illegal alphamapped subbitmap blit request detected!");

  if(!Bitmap->IsIndependent)
    {
      AlphaBlit(Bitmap->GetMotherBitmap(), Bitmap->GetXPos() + DestX, Bitmap->GetYPos() + DestY, MaskColor);
      return;
    }

  if(!GetAlphaMap())
    ABORT("AlphaMap not available!");

  ulong TrueSourceOffset = ulong(GetImage()[0]);
  ulong TrueDestOffset = ulong(&Bitmap->GetImage()[DestY][DestX]);
  ulong TrueDestXMove = (Bitmap->XSize - XSize) << 1;
  ulong AlphaMapOffset = ulong(GetAlphaMap()[0]);

  ushort Width = XSize, Height = YSize;

  ::AlphaBlit(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueDestXMove, Width, Height, MaskColor);
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
	if(abs(int(c) - a) > 1 && !((a == 0) && c == Points.size() - 1) && !((c == 0) && a == Points.size() - 1))
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

      LastColor = *(ushort*)Buffer;

      for(ushort y = 0; y < YSize - 1; ++y)
	{
	  NextColor = *(ushort*)(Buffer + (XSize << 1));

	  if((LastColor == DEFAULT_TRANSPARENT || !y) && NextColor != DEFAULT_TRANSPARENT)
	    *(ushort*)Buffer = Color;

	  Buffer += XSize << 1;

	  if(LastColor != DEFAULT_TRANSPARENT && (NextColor == DEFAULT_TRANSPARENT || y == YSize - 2))
	    *(ushort*)Buffer = Color;

	  LastColor = NextColor;
	}
    }

  for(ushort y = 0; y < YSize; ++y)
    {
      Buffer = ulong(GetImage()[y]);

      LastColor = *(ushort*)Buffer;

      for(ushort x = 0; x < XSize - 1; ++x)
	{
	  NextColor = *(ushort*)(Buffer + 2);

	  if((LastColor == DEFAULT_TRANSPARENT || !x) && NextColor != DEFAULT_TRANSPARENT)
	    *(ushort*)Buffer = Color;

	  Buffer += 2;

	  if(LastColor != DEFAULT_TRANSPARENT && (NextColor == DEFAULT_TRANSPARENT || x == XSize - 2))
	    *(ushort*)Buffer = Color;

	  LastColor = NextColor;
	}
    }
}

void bitmap::CreateOutlineBitmap(bitmap* Bitmap, ushort Color)
{
  if(!IsIndependent)
    ABORT("Subbitmap outline bitmap creation request detected!");

  Bitmap->Fill(DEFAULT_TRANSPARENT);

  for(ushort x = 0; x < XSize; ++x)
    {
      ulong SrcBuffer = ulong(&GetImage()[0][x]);
      ulong DestBuffer = ulong(&Bitmap->GetImage()[0][x]);

      ushort LastColor = *(ushort*)SrcBuffer;

      for(ushort y = 0; y < YSize - 1; ++y)
	{
	  ushort NextColor = *(ushort*)(SrcBuffer + (XSize << 1));

	  if((LastColor == DEFAULT_TRANSPARENT || !y) && NextColor != DEFAULT_TRANSPARENT)
	    *(ushort*)DestBuffer = Color;

	  SrcBuffer += XSize << 1;
	  DestBuffer += Bitmap->XSize << 1;

	  if(LastColor != DEFAULT_TRANSPARENT && (NextColor == DEFAULT_TRANSPARENT || y == YSize - 2))
	    *(ushort*)DestBuffer = Color;

	  LastColor = NextColor;
	}
    }

  for(ushort y = 0; y < YSize; ++y)
    {
      ulong SrcBuffer = ulong(GetImage()[y]);
      ulong DestBuffer = ulong(Bitmap->GetImage()[y]);

      ushort LastSrcColor = *(ushort*)SrcBuffer;
      ushort LastDestColor = *(ushort*)DestBuffer;

      for(ushort x = 0; x < XSize - 1; ++x)
	{
	  ushort NextSrcColor = *(ushort*)(SrcBuffer + 2);
	  ushort NextDestColor = *(ushort*)(DestBuffer + 2);

	  if((LastSrcColor == DEFAULT_TRANSPARENT || !x) && (NextSrcColor != DEFAULT_TRANSPARENT || NextDestColor != DEFAULT_TRANSPARENT))
	    *(ushort*)DestBuffer = Color;

	  SrcBuffer += 2;
	  DestBuffer += 2;

	  if((LastSrcColor != DEFAULT_TRANSPARENT || LastDestColor != DEFAULT_TRANSPARENT) && (NextSrcColor == DEFAULT_TRANSPARENT || x == XSize - 2))
	    *(ushort*)DestBuffer = Color;

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
      Backup.MaskedBlit(DOUBLEBUFFER, ushort(255 - c * 50), 0);

      if(BitmapEditor)
	BitmapEditor(this);

      AlphaBlit(DOUBLEBUFFER, uchar(c * 50), 0);
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

	PutPixel(X + x, Y + y, MAKE_RGB(ushort(GET_RED(Color1) * (1.0f - Multiplier) + GET_RED(Color2) * Multiplier),
					ushort(GET_GREEN(Color1) * (1.0f - Multiplier) + GET_GREEN(Color2) * Multiplier),
					ushort(GET_BLUE(Color1) * (1.0f - Multiplier) + GET_BLUE(Color2) * Multiplier)));
      }
}
