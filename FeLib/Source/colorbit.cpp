#include <cstdarg>

#include "colorbit.h"
#include "bitmap.h"
#include "save.h"
#include "femath.h"

void colorizablebitmap::MaskedBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, int Width, int Height, packedcolor16* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, int Width, int Height, packedcolor16* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, vector2d BlitSize, packedcolor16* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, int Width, int Height, packedcolor16* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, vector2d Source, int DestX, int DestY, vector2d BlitSize, packedcolor16* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, vector2d Dest, vector2d BlitSize, packedcolor16* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, packedcolor16* Color) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Color); }
void colorizablebitmap::MaskedBlit(bitmap* Bitmap, packedcolor16* Color) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Color); }

void colorizablebitmap::AlterGradient(vector2d Pos, int Width, int Height, int MColor, int Amount, bool Clip) { AlterGradient(Pos.X, Pos.Y, Width, Height, MColor, Amount, Clip); }
void colorizablebitmap::AlterGradient(int X, int Y, vector2d AlterSize, int MColor, int Amount, bool Clip) { AlterGradient(X, Y, AlterSize.X, AlterSize.Y, MColor, Amount, Clip); }
void colorizablebitmap::AlterGradient(vector2d Pos, vector2d AlterSize, int MColor, int Amount, bool Clip) { AlterGradient(Pos.X, Pos.Y, AlterSize.X, AlterSize.Y, MColor, Amount, Clip); }

void colorizablebitmap::SwapColors(vector2d Pos, int Width, int Height, int Color1, int Color2) { SwapColors(Pos.X, Pos.Y, Width, Height, Color1, Color2); }
void colorizablebitmap::SwapColors(int X, int Y, vector2d AlterSize, int Color1, int Color2) { SwapColors(X, Y, AlterSize.X, AlterSize.Y, Color1, Color2); }
void colorizablebitmap::SwapColors(vector2d Pos, vector2d AlterSize, int Color1, int Color2) { SwapColors(Pos.X, Pos.Y, AlterSize.X, AlterSize.Y, Color1, Color2); }

void colorizablebitmap::Roll(vector2d Pos, int Width, int Height, int XMove, int YMove, uchar* TempBuf) { Roll(Pos.X, Pos.Y, Width, Height, XMove, YMove, TempBuf); }
void colorizablebitmap::Roll(int X, int Y, vector2d BlitSize, int XMove, int YMove, uchar* TempBuf) { Roll(X, Y, BlitSize.X, BlitSize.Y, XMove, YMove, TempBuf); }
void colorizablebitmap::Roll(int X, int Y, int Width, int Height, vector2d Move, uchar* TempBuf) { Roll(X, Y, Width, Height, Move.X, Move.Y, TempBuf); }
void colorizablebitmap::Roll(vector2d Pos, vector2d BlitSize, int XMove, int YMove, uchar* TempBuf) { Roll(Pos.X, Pos.Y, BlitSize.X, BlitSize.Y, XMove, YMove, TempBuf); }
void colorizablebitmap::Roll(vector2d Pos, int Width, int Height, vector2d Move, uchar* TempBuf) { Roll(Pos.X, Pos.Y, Width, Height, Move.X, Move.Y, TempBuf); }
void colorizablebitmap::Roll(int X, int Y, vector2d BlitSize, vector2d Move, uchar* TempBuf) { Roll(X, Y, BlitSize.X, BlitSize.Y, Move.X, Move.Y, TempBuf); }
void colorizablebitmap::Roll(vector2d Pos, vector2d BlitSize, vector2d Move, uchar* TempBuf) { Roll(Pos.X, Pos.Y, BlitSize.X, BlitSize.Y, Move.X, Move.Y, TempBuf); }

colorizablebitmap::colorizablebitmap(const festring& FileName)
{
  inputfile File(FileName.CStr(), 0, false);

  if(!File.IsOpen())
    ABORT("Bitmap %s not found!", FileName.CStr());

  File.SeekPosEnd(-768);
  Palette = new uchar[768];
  File.Read(reinterpret_cast<char*>(Palette), 768);
  File.SeekPosBegin(8);
  XSize  =  File.Get();
  XSize += (File.Get() << 8) + 1;
  YSize  =  File.Get();
  YSize += (File.Get() << 8) + 1;
  File.SeekPosBegin(128);
  PaletteBuffer = new uchar[XSize * YSize];
  uchar* Buffer = PaletteBuffer;

  while(Buffer != PaletteBuffer + XSize * YSize)
    {
      int Char1 = File.Get();

      if(Char1 > 192)
	{
	  int Char2 = File.Get();

	  for(; Char1 > 192; Char1--)
	    *Buffer++ = Char2;
	}
      else
	*Buffer++ = Char1;
    }
}

colorizablebitmap::~colorizablebitmap()
{
  delete [] Palette;
  delete [] PaletteBuffer;

  for(fontcache::iterator i = FontCache.begin(); i != FontCache.end(); ++i)
    {
      delete i->second.first;
      delete i->second.second;
    }
}

/* A lousy bitmap saver that uses the pcx format but doesn't do any compression. */

void colorizablebitmap::Save(const festring& FileName)
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

  while(Buffer != PaletteBuffer + XSize * YSize)
    {
      uchar Char = *Buffer++;

      if(Char >= 192)
	SaveFile.Put(char(193));

      SaveFile.Put(Char);
    }

  SaveFile.Write(reinterpret_cast<char*>(Palette), 768);
}

void colorizablebitmap::MaskedBlit(bitmap* Bitmap, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, packedcolor16* Color) const
{
  if(!femath::Clip(SourceX, SourceY, DestX, DestY, Width, Height, XSize, YSize, Bitmap->GetXSize(), Bitmap->GetYSize()))
    return;

  uchar* Buffer = PaletteBuffer + SourceY * XSize + SourceX;
  packedcolor16* DestBuffer = &Bitmap->GetImage()[DestY][DestX];
  int BitmapXSize = Bitmap->GetXSize();
  uchar* Palette = this->Palette; // eliminate the efficiency cost of dereferencing

  for(int y = 0; y < Height; ++y)
    {
      for(int x = 0; x < Width; ++x)
	{
	  int PaletteElement = Buffer[x];

	  if(PaletteElement >= 192)
	    {
	      int ThisColor = Color[(PaletteElement - 192) >> 4];
	      int Index = PaletteElement & 15;
	      int Red = (ThisColor >> 8 & 0xF8) * Index;
	      int Green = (ThisColor >> 3 & 0xFC) * Index;
	      int Blue = (ThisColor << 3 & 0xF8) * Index;

	      if(Red > 0x7FF)
		Red = 0x7FF;

	      if(Green > 0x7FF)
		Green = 0x7FF;

	      if(Blue > 0x7FF)
		Blue = 0x7FF;

	      DestBuffer[x] = (Red << 5 & 0xF800) | (Green & 0x7E0) | (Blue >> 6 & 0x1F);
	    }
	  else
	    {
	      int PaletteIndex = PaletteElement + (PaletteElement << 1);
	      int ThisColor = ((Palette[PaletteIndex] & 0xFFF8) << 8)
			    | ((Palette[PaletteIndex + 1] & 0xFFFC) << 3)
			    |  (Palette[PaletteIndex + 2] >> 3);

	      if(ThisColor != TRANSPARENT_COLOR)
		DestBuffer[x] = ThisColor;
	    }
	}

      DestBuffer += BitmapXSize;
      Buffer += XSize;
    }
}

cachedfont* colorizablebitmap::Colorize(const packedcolor16* Color, alpha BaseAlpha, const packedalpha* Alpha) const
{
  cachedfont* Bitmap = new cachedfont(XSize, YSize);
  uchar* Buffer = PaletteBuffer;
  packedcolor16* DestBuffer = Bitmap->GetImage()[0];
  uchar* Palette = this->Palette; // eliminate the efficiency cost of dereferencing
  packedalpha* AlphaMap;
  bool UseAlpha;

  if(BaseAlpha != 255 || (Alpha && (Alpha[0] != 255 || Alpha[1] != 255 || Alpha[2] != 255 || Alpha[3] != 255)))
    {
      Bitmap->CreateAlphaMap(BaseAlpha);
      AlphaMap = Bitmap->GetAlphaMap()[0];
      UseAlpha = true;
    }
  else
    {
      AlphaMap = 0;
      UseAlpha = false;
    }

  int BitmapXSize = Bitmap->GetXSize();

  for(int y = 0; y < YSize; ++y)
    {
      for(int x = 0; x < XSize; ++x)
	{
	  int PaletteElement = Buffer[x];

	  if(PaletteElement >= 192)
	    {
	      int ColorIndex = (PaletteElement - 192) >> 4;
	      int ThisColor = Color[ColorIndex];

	      if(ThisColor != TRANSPARENT_COLOR)
		{
		  int Index = PaletteElement & 15;
		  int Red = (ThisColor >> 8 & 0xF8) * Index;
		  int Green = (ThisColor >> 3 & 0xFC) * Index;
		  int Blue = (ThisColor << 3 & 0xF8) * Index;

		  if(Red > 0x7FF)
		    Red = 0x7FF;

		  if(Green > 0x7FF)
		    Green = 0x7FF;

		  if(Blue > 0x7FF)
		    Blue = 0x7FF;

		  DestBuffer[x] = (Red << 5 & 0xF800) | (Green & 0x7E0) | (Blue >> 6 & 0x1F);

		  if(UseAlpha)
		    AlphaMap[x] = Alpha[ColorIndex];
		}
	      else
		DestBuffer[x] = TRANSPARENT_COLOR;
	    }
	  else
	    {
	      int PaletteIndex = PaletteElement + (PaletteElement << 1);
	      DestBuffer[x] = ((Palette[PaletteIndex] & 0xFFF8) << 8)
			    | ((Palette[PaletteIndex + 1] & 0xFFFC) << 3)
			    |  (Palette[PaletteIndex + 2] >> 3);
	    }
	}

      DestBuffer += BitmapXSize;
      AlphaMap += BitmapXSize;
      Buffer += XSize;
    }

  return Bitmap;
}

bitmap* colorizablebitmap::Colorize(vector2d Pos, vector2d Size, vector2d Move, const packedcolor16* Color, alpha BaseAlpha, const packedalpha* Alpha, const uchar* RustData, bool AllowReguralColors) const
{
  bitmap* Bitmap = new bitmap(Size.X, Size.Y);
  vector2d TargetPos(0, 0);

  if(Move.X || Move.Y)
    {
      Bitmap->ClearToColor(TRANSPARENT_COLOR);

      if(Move.X < 0)
	{
	  Pos.X -= Move.X;
	  Size.X += Move.X;
	}
      else if(Move.X > 0)
	{
	  TargetPos.X = Move.X;
	  Size.X -= Move.X;
	}

      if(Move.Y < 0)
	{
	  Pos.Y -= Move.Y;
	  Size.Y += Move.Y;
	}
      else if(Move.Y > 0)
	{
	  TargetPos.Y = Move.Y;
	  Size.Y -= Move.Y;
	}
    }

  uchar* Buffer = PaletteBuffer + Pos.Y * XSize + Pos.X;
  packedcolor16* DestBuffer = &Bitmap->GetImage()[TargetPos.Y][TargetPos.X];
  int BitmapXSize = Bitmap->GetXSize();
  uchar* Palette = this->Palette; // eliminate the efficiency cost of dereferencing
  packedalpha* AlphaMap;
  bool UseAlpha;

  if(BaseAlpha != 255 || (Alpha && (Alpha[0] != 255 || Alpha[1] != 255 || Alpha[2] != 255 || Alpha[3] != 255)))
    {
      Bitmap->CreateAlphaMap(BaseAlpha);
      AlphaMap = &Bitmap->GetAlphaMap()[TargetPos.Y][TargetPos.X];
      UseAlpha = true;
    }
  else
    {
      AlphaMap = 0;
      UseAlpha = false;
    }

  bool Rusted = RustData && (RustData[0] || RustData[1] || RustData[2] || RustData[3]);
  ulong RustSeed[4];

  if(Rusted)
    {
      RustSeed[0] = (RustData[0] & 0xFC) >> 2;
      RustSeed[1] = (RustData[1] & 0xFC) >> 2;
      RustSeed[2] = (RustData[2] & 0xFC) >> 2;
      RustSeed[3] = (RustData[3] & 0xFC) >> 2;
    }

  for(int y = 0; y < Size.Y; ++y)
    {
      for(int x = 0; x < Size.X; ++x)
	{
	  int PaletteElement = Buffer[x];

	  if(PaletteElement >= 192)
	    {
	      int ColorIndex = (PaletteElement - 192) >> 4;
	      int ThisColor = Color[ColorIndex];

	      if(ThisColor != TRANSPARENT_COLOR)
		{
		  int Index = PaletteElement & 15;
		  int Red = (ThisColor >> 8 & 0xF8) * Index;
		  int Green = (ThisColor >> 3 & 0xFC) * Index;
		  int Blue = (ThisColor << 3 & 0xF8) * Index;

		  if(Rusted && RustData[ColorIndex]
		  && (RustData[ColorIndex] & 3UL) > (RustSeed[ColorIndex] = RustSeed[ColorIndex] * 1103515245 + 12345) >> 30)
		    {
		      Green = ((Green << 1) + Green) >> 2;
		      Blue >>= 1;
		    }

		  if(Red > 0x7FF)
		    Red = 0x7FF;

		  if(Green > 0x7FF)
		    Green = 0x7FF;

		  if(Blue > 0x7FF)
		    Blue = 0x7FF;

		  DestBuffer[x] = (Red << 5 & 0xF800) | (Green & 0x7E0) | (Blue >> 6 & 0x1F);

		  if(UseAlpha)
		    AlphaMap[x] = Alpha[ColorIndex];
		}
	      else
		DestBuffer[x] = TRANSPARENT_COLOR;
	    }
	  else if(AllowReguralColors)
	    {
	      int PaletteIndex = PaletteElement + (PaletteElement << 1);
	      DestBuffer[x] = ((Palette[PaletteIndex] & 0xFFF8) << 8)
			    | ((Palette[PaletteIndex + 1] & 0xFFFC) << 3)
			    |  (Palette[PaletteIndex + 2] >> 3);
	    }
	  else
	    DestBuffer[x] = TRANSPARENT_COLOR;
	}

      DestBuffer += BitmapXSize;
      AlphaMap += BitmapXSize;
      Buffer += XSize;
    }

  return Bitmap;
}

void colorizablebitmap::Printf(bitmap* Bitmap, int X, int Y, packedcolor16 Color, const char* Format, ...) const
{
  char Buffer[256];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Buffer, Format, AP);
  va_end(AP);

  fontcache::const_iterator Iterator = FontCache.find(Color);
  int Size = strlen(Buffer);

  if(Iterator == FontCache.end())
    {
      packedcolor16 ShadeCol = MakeShadeColor(Color);

      for(int c = 0; c < Size; ++c)
	{
	  int FX = ((Buffer[c] - 0x20) & 0xF) << 4, FY = (Buffer[c] - 0x20) & 0xF0;
	  MaskedBlit(Bitmap, FX, FY, X + (c << 3) + 1, Y + 1, 8, 8, &ShadeCol);
	  MaskedBlit(Bitmap, FX, FY, X + (c << 3), Y, 8, 8, &Color);
	}
    }
  else
    {
      for(int c = 0; c < Size; ++c)
	Iterator->second.first->PrintCharacter(Bitmap, ((Buffer[c] - 0x20) & 0xF) << 4, (Buffer[c] - 0x20) & 0xF0, X + (c << 3), Y);
    }
}

void colorizablebitmap::PrintfUnshaded(bitmap* Bitmap, int X, int Y, packedcolor16 Color, const char* Format, ...) const
{
  char Buffer[256];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Buffer, Format, AP);
  va_end(AP);

  fontcache::const_iterator Iterator = FontCache.find(Color);
  int Size = strlen(Buffer);

  if(Iterator == FontCache.end())
    {
      for(int c = 0; c < Size; ++c)
	MaskedBlit(Bitmap, ((Buffer[c] - 0x20) & 0xF) << 4, (Buffer[c] - 0x20) & 0xF0, X + (c << 3), Y, 8, 8, &Color);
    }
  else
    {
      for(int c = 0; c < Size; ++c)
	Iterator->second.second->PrintCharacter(Bitmap, ((Buffer[c] - 0x20) & 0xF) << 4, (Buffer[c] - 0x20) & 0xF0, X + (c << 3), Y);
    }
}

void colorizablebitmap::AlterGradient(int X, int Y, int Width, int Height, int MColor, int Amount, bool Clip)
{
  int ColorMin = 192 + (MColor << 4);
  int ColorMax = 207 + (MColor << 4);

  if(Clip)
    {
      for(int x = X; x < X + Width; ++x)
	for(int y = Y; y < Y + Height; ++y)
	  {
	    int Pixel = AccessPaletteEntry(x, y);

	    if(Pixel >= ColorMin && Pixel <= ColorMax)
	      {
		int NewPixel = Pixel + Amount;

		if(NewPixel < ColorMin)
		  NewPixel = ColorMin;

		if(NewPixel > ColorMax)
		  NewPixel = ColorMax;

		AccessPaletteEntry(x, y) = NewPixel;
	      }
	  }
    }
  else
    {
      int x;

      for(x = X; x < X + Width; ++x)
	for(int y = Y; y < Y + Height; ++y)
	  {
	    int Pixel = AccessPaletteEntry(x, y);

	    if(Pixel >= ColorMin && Pixel <= ColorMax)
	      {
		int NewPixel = Pixel + Amount;

		if(NewPixel < ColorMin)
		  return;

		if(NewPixel > ColorMax)
		  return;
	      }
	  }

      for(x = X; x < X + Width; ++x)
	for(int y = Y; y < Y + Height; ++y)
	  {
	    int Pixel = AccessPaletteEntry(x, y);

	    if(Pixel >= ColorMin && Pixel <= ColorMax)
	      AccessPaletteEntry(x, y) = Pixel + Amount;
	  }
    }
}

void colorizablebitmap::SwapColors(int X, int Y, int Width, int Height, int Color1, int Color2)
{
  if(Color1 > 3 || Color2 > 3)
    ABORT("Illgal color swap!");

  for(int x = X; x < X + Width; ++x)
    for(int y = Y; y < Y + Height; ++y)
      {
	uchar& Pixel = AccessPaletteEntry(x, y);

	if(Pixel >= 192 + (Color1 << 4) && Pixel <= 207 + (Color1 << 4))
	  Pixel += (Color2 - Color1) << 4;
	else if(Pixel >= 192 + (Color2 << 4) && Pixel <= 207 + (Color2 << 4))
	  Pixel += (Color1 - Color2) << 4;
      }
}

/* TempBuffer must be an array of Width * Height uchars */

void colorizablebitmap::Roll(int X, int Y, int Width, int Height, int XMove, int YMove, uchar* TempBuffer)
{
  int x, y;

  for(x = X; x < X + Width; ++x)
    for(y = Y; y < Y + Height; ++y)
      {
	int XPos = x + XMove, YPos = y + YMove;

	if(XPos < X)
	  XPos += Width;

	if(YPos < Y)
	  YPos += Height;

	if(XPos >= X + Width)
	  XPos -= Width;

	if(YPos >= Y + Height)
	  YPos -= Height;

	TempBuffer[(YPos - Y) * Width + XPos - X] = AccessPaletteEntry(x, y);
      }

  for(x = X; x < X + Width; ++x)
    for(y = Y; y < Y + Height; ++y)
      AccessPaletteEntry(x, y) = TempBuffer[(y - Y) * Width + x - X];
}

void colorizablebitmap::CreateFontCache(packedcolor16 Color)
{
  if(FontCache.find(Color) != FontCache.end())
    return;

  packedcolor16 ShadeColor = MakeShadeColor(Color);
  cachedfont* Font = new cachedfont(XSize, YSize, TRANSPARENT_COLOR);
  MaskedBlit(Font, 0, 0, 1, 1, XSize - 1, YSize - 1, &ShadeColor);
  cachedfont* UnshadedFont = Colorize(&Color);
  UnshadedFont->NormalMaskedBlit(Font);
  Font->CreateMaskMap();
  UnshadedFont->CreateMaskMap();
  FontCache[Color] = std::pair<cachedfont*, cachedfont*>(Font, UnshadedFont);
}

/* returns ERROR_VECTOR if fails find Pos else returns pos */

vector2d colorizablebitmap::RandomizeSparklePos(const vector2d* ValidityArray, vector2d* PossibleBuffer, vector2d Pos, vector2d Size, int ValidityArraySize, bool* Sparkling) const
{
  bool SparklingReally = false;
  int c;

  for(c = 0; c < 4; ++c)
    if(Sparkling[c])
      SparklingReally = true;

  if(!SparklingReally)
    return ERROR_VECTOR;

  /* Don't use { } to initialize, or GCC optimizations will produce code that crashes! */

  vector2d* BadPossible[4];
  BadPossible[0] = PossibleBuffer;
  BadPossible[1] = BadPossible[0] + ((Size.X + Size.Y) << 1) -  4;
  BadPossible[2] = BadPossible[0] + ((Size.X + Size.Y) << 1) - 12;
  BadPossible[3] = BadPossible[0] + ((Size.X + Size.Y) << 1) - 20;
  vector2d* PreferredPossible = BadPossible[3] + ((Size.X + Size.Y) << 1) - 28;

  int Preferred = 0;
  int Bad[4] = { 0, 0, 0, 0 };
  int XMax = Pos.X + Size.X;
  int YMax = Pos.Y + Size.Y;

  for(c = 0; c < ValidityArraySize; ++c)
    {
      vector2d V = ValidityArray[c] + Pos;
      int Entry = AccessPaletteEntry(V.X, V.Y);

      if(IsMaterialColor(Entry) && Sparkling[GetMaterialColorIndex(Entry)])
	{
	  int MinDist = 0x7FFF;

	  if(V.X < Pos.X + 4)
	    MinDist = Min(V.X - Pos.X, MinDist);

	  if(V.X >= XMax - 4)
	    MinDist = Min(XMax - V.X - 1, MinDist);

	  if(V.Y < Pos.Y + 4)
	    MinDist = Min(V.Y - Pos.Y, MinDist);

	  if(V.Y >= YMax - 4)
	    MinDist = Min(YMax - V.Y - 1, MinDist);

	  if(MinDist >= 4)
	    PreferredPossible[Preferred++] = V;
	  else
	    BadPossible[MinDist][Bad[MinDist]++] = V;
	}
    }

  vector2d Return;

  if(Preferred)
    Return = PreferredPossible[RAND() % Preferred] - Pos;
  else if(Bad[3])
    Return = BadPossible[3][RAND() % Bad[3]] - Pos;
  else if(Bad[2])
    Return = BadPossible[2][RAND() % Bad[2]] - Pos;
  else if(Bad[1])
    Return = BadPossible[1][RAND() % Bad[1]] - Pos;
  else if(Bad[0])
    Return = BadPossible[0][RAND() % Bad[0]] - Pos;
  else
    Return = ERROR_VECTOR;

  return Return;
}

bool colorizablebitmap::IsTransparent(vector2d Pos) const
{
  return AccessPaletteEntry(Pos) == 191;
}

bool colorizablebitmap::IsMaterialColor1(vector2d Pos) const
{
  int P = AccessPaletteEntry(Pos);
  return P >= 192 && P < 208;
}
