#include <cstdio>
#include <cstdarg>
#include <fstream>
#include <memory.h>

#include "colorbit.h"
#include "error.h"
#include "bitmap.h"
#include "graphics.h"
#include "save.h"

colorizablebitmap::colorizablebitmap(const std::string& FileName)
{
  inputfile File(FileName.c_str(), false);

  if(!File.IsOpen())
    ABORT("Bitmap %s not found!", FileName.c_str());

  File.SeekPosEnd(-768);
  Palette = new uchar[768];
  File.Read((char*)Palette, 768);
  File.SeekPosBeg(8);
  XSize  =  File.Get();
  XSize += (File.Get() << 8) + 1;
  YSize  =  File.Get();
  YSize += (File.Get() << 8) + 1;
  File.SeekPosBeg(128);
  PaletteBuffer = new uchar[XSize * YSize];
  uchar* Buffer = PaletteBuffer;

  while(ulong(Buffer) != ulong(PaletteBuffer) + XSize * YSize)
    {
      int Char1 = File.Get();

      if(Char1 > 192)
	{
	  int Char2 = File.Get();

	  for(; Char1 > 192; Char1--)
	    *(Buffer++) = Char2;
	}
      else
	*(Buffer++) = Char1;
    }
}

colorizablebitmap::~colorizablebitmap()
{
  delete [] Palette;
  delete [] PaletteBuffer;
}

/*
 * A lousy bitmap saver that uses the pcx format but doesn't do any compression.
 */

void colorizablebitmap::Save(const std::string& FileName)
{
  char PCXHeader[128];
  memset(PCXHeader, 0, 128);
  *((ulong*)PCXHeader) = 0x0801050A;
  PCXHeader[65] = 0x01;
  PCXHeader[66] = XSize & 0xFF;
  PCXHeader[67] = (XSize >> 8) & 0xFF;
  PCXHeader[0x08] = (XSize - 1) & 0xFF;
  PCXHeader[0x09] = ((XSize - 1) >> 8) & 0xFF;
  PCXHeader[0x0A] = (YSize - 1) & 0xFF;
  PCXHeader[0x0B] = ((YSize - 1) >> 8) & 0xFF;
  outputfile SaveFile(FileName);
  SaveFile.Write(PCXHeader, 128);
  uchar* Buffer = PaletteBuffer;

  while(ulong(Buffer) != ulong(PaletteBuffer) + XSize * YSize)
    {
      uchar Char = *(Buffer++);

      if(Char >= 192)
	SaveFile.Put(uchar(193));

      SaveFile.Put(Char);
    }

  SaveFile.Write((char*)Palette, 768);
}

void colorizablebitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort* Color) const
{
  uchar* Buffer = (uchar*)(ulong(PaletteBuffer) + ulong(SourceY) * XSize);
  ulong DestBuffer = ulong(Bitmap->GetImage()[DestY]);

  for(ushort y = 0; y < Height; ++y)
    {
      for(ushort x = 0; x < Width; ++x)
	{
	  uchar PaletteElement = Buffer[SourceX + x];

	  if(PaletteElement >= 192)
	    {
	      ushort ThisColor = Color[(PaletteElement - 192) / 16];
	      float Gradient = float(PaletteElement % 16) / 8;
	      ushort Red = ushort(GET_RED(ThisColor) * Gradient), Blue = ushort(GET_BLUE(ThisColor) * Gradient), Green = ushort(GET_GREEN(ThisColor) * Gradient);
	      ((ushort*)DestBuffer)[DestX + x] = MAKE_RGB(Red < 256 ? Red : 255, Green < 256 ? Green : 255, Blue < 256 ? Blue : 255);
	    }
	  else
	    {
	      ushort ThisColor = ((Palette[PaletteElement + (PaletteElement << 1)] >> 3) << 11) | ((Palette[PaletteElement + (PaletteElement << 1) + 1] >> 2) << 5) | (Palette[PaletteElement + (PaletteElement << 1) + 2] >> 3);

	      if(ThisColor != DEFAULT_TRANSPARENT)
		((ushort*)DestBuffer)[DestX + x] = ThisColor;
	    }
	}

      DestBuffer += (Bitmap->XSize << 1);
      Buffer = (uchar*)(ulong(Buffer) + XSize);
    }
}

bitmap* colorizablebitmap::Colorize(ushort* Color) const
{
  bitmap* Bitmap = new bitmap(XSize, YSize);
  uchar* Buffer = PaletteBuffer;
  ulong DestBuffer = ulong(Bitmap->GetImage()[0]);

  for(ushort y = 0; y < YSize; ++y)
    {
      for(ushort x = 0; x < XSize; ++x)
	{
	  if(Buffer[x] >= 192)
	    {
	      ushort ThisColor = Color[(Buffer[x] - 192) / 16];
	      float Gradient = float(Buffer[x] % 16) / 8 - 1.0f;
	      ((ushort*)DestBuffer)[x] = MAKE_RGB(uchar(GET_RED(ThisColor) * Gradient), uchar(GET_GREEN(ThisColor) * Gradient), uchar(GET_BLUE(ThisColor) * Gradient));
	    }
	  else
	    ((ushort*)DestBuffer)[x] = ((Palette[Buffer[x] + (Buffer[x] << 1)] >> 3) << 11) | ((Palette[Buffer[x] + (Buffer[x] << 1) + 1] >> 2) << 5) | (Palette[Buffer[x] + (Buffer[x] << 1) + 2] >> 3);
	}

      DestBuffer += (Bitmap->XSize << 1);
      Buffer = (uchar*)(ulong(Buffer) + XSize);
    }

  return Bitmap;
}

bitmap* colorizablebitmap::Colorize(vector2d Pos, vector2d Size, ushort* Color) const
{
  bitmap* Bitmap = new bitmap(Size.X, Size.Y);
  uchar* Buffer = (uchar*)(ulong(PaletteBuffer) + ulong(Pos.Y) * XSize);
  ulong DestBuffer = ulong(Bitmap->GetImage()[0]);

  for(ushort y = 0; y < Size.Y; ++y)
    {
      for(ushort x = 0; x < Size.X; ++x)
	{
	  uchar PaletteElement = Buffer[Pos.X + x];

	  if(PaletteElement >= 192)
	    {
	      ushort ThisColor = Color[(PaletteElement - 192) / 16];
	      float Gradient = float(PaletteElement % 16) / 8;
	      ushort Red = ushort(GET_RED(ThisColor) * Gradient), Blue = ushort(GET_BLUE(ThisColor) * Gradient), Green = ushort(GET_GREEN(ThisColor) * Gradient);
	      ((ushort*)DestBuffer)[x] = MAKE_RGB(Red < 256 ? Red : 255, Green < 256 ? Green : 255, Blue < 256 ? Blue : 255);
	    }
	  else
	    ((ushort*)DestBuffer)[x] = ((Palette[PaletteElement + (PaletteElement << 1)] >> 3) << 11) | ((Palette[PaletteElement + (PaletteElement << 1) + 1] >> 2) << 5) | (Palette[PaletteElement + (PaletteElement << 1) + 2] >> 3);
	}

      DestBuffer += (Bitmap->XSize << 1);
      Buffer = (uchar*)(ulong(Buffer) + XSize);
    }

  return Bitmap;
}

ushort colorizablebitmap::Printf(bitmap* Bitmap, ushort X, ushort Y, ushort Color, const char* Format, ...) const
{
  char Buffer[256];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Buffer, Format, AP);
  va_end(AP);

  ushort ShadeCol = MAKE_SHADE_COL(Color);

  for(ushort c = 0; c < strlen(Buffer); ++c)
    {
      ushort FX = ((Buffer[c] - 0x20) & 0xF) << 4, FY = (Buffer[c] - 0x20) & 0xF0;
      MaskedBlit(Bitmap, FX, FY, X + (c << 3) + 1, Y + 1, 8, 8, &ShadeCol);
      MaskedBlit(Bitmap, FX, FY, X + (c << 3), Y, 8, 8, &Color);
    }

  return strlen(Buffer);
}

ushort colorizablebitmap::PrintfUnshaded(bitmap* Bitmap, ushort X, ushort Y, ushort Color, const char* Format, ...) const
{
  char Buffer[256];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Buffer, Format, AP);
  va_end(AP);

  for(ushort c = 0; c < strlen(Buffer); ++c)
    {
      ushort FX = ((Buffer[c] - 0x20) & 0xF) << 4, FY = (Buffer[c] - 0x20) & 0xF0;
      MaskedBlit(Bitmap, FX, FY, X + (c << 3), Y, 8, 8, &Color);
    }

  return strlen(Buffer);
}

void colorizablebitmap::AlterGradient(ushort X, ushort Y, ushort Width, ushort Height, uchar MColor, char Amount, bool Clip)
{
  uchar ColorMin = 192 + MColor * 16;
  uchar ColorMax = 207 + MColor * 16;

  if(Clip)
    {
      for(ushort x = X; x < X + Width; ++x)
	for(ushort y = Y; y < Y + Height; ++y)
	  {
	    uchar Pixel = PaletteBuffer[y * XSize + x];

	    if(Pixel >= ColorMin && Pixel <= ColorMax)
	      {
		ushort NewPixel = ushort(Pixel) + Amount;

		if(NewPixel < ColorMin)
		  NewPixel = ColorMin;

		if(NewPixel > ColorMax)
		  NewPixel = ColorMax;

		PaletteBuffer[y * XSize + x] = NewPixel;
	      }
	  }
    }
  else
    {
      ushort x;

      for(x = X; x < X + Width; ++x)
	for(ushort y = Y; y < Y + Height; ++y)
	  {
	    uchar Pixel = PaletteBuffer[y * XSize + x];

	    if(Pixel >= ColorMin && Pixel <= ColorMax)
	      {
		ushort NewPixel = ushort(Pixel) + Amount;

		if(NewPixel < ColorMin)
		  return;

		if(NewPixel > ColorMax)
		  return;
	      }
	  }

      for(x = X; x < X + Width; ++x)
	for(ushort y = Y; y < Y + Height; ++y)
	  {
	    uchar Pixel = PaletteBuffer[y * XSize + x];

	    if(Pixel >= ColorMin && Pixel <= ColorMax)
	      PaletteBuffer[y * XSize + x] = Pixel + Amount;
	  }
    }
}

void colorizablebitmap::SwapColors(ushort X, ushort Y, ushort Width, ushort Height, uchar Color1, uchar Color2)
{
  if(Color1 > 3 || Color2 > 3)
    ABORT("Illgal color swap!");

  for(ushort x = X; x < X + Width; ++x)
    for(ushort y = Y; y < Y + Height; ++y)
      {
	uchar& Pixel = PaletteBuffer[y * XSize + x];

	if(Pixel >= 192 + Color1 * 16 && Pixel <= 207 + Color1 * 16)
	  Pixel += (Color2 - Color1) * 16;
	else if(Pixel >= 192 + Color2 * 16 && Pixel <= 207 + Color2 * 16)
	  Pixel += (Color1 - Color2) * 16;
      }
}
