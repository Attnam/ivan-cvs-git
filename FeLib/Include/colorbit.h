#ifndef __COLORBIT_H__
#define __COLORBIT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"
#include "vector2d.h"

class outputfile;
class inputfile;
class bitmap;

class colorizablebitmap
{
 public:
  colorizablebitmap(std::string);
  ~colorizablebitmap();

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort*) const;
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ushort* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Color); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ushort* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ushort* Color) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort* Color) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Color); }

  ushort Printf(bitmap*, ushort, ushort, ushort, const char*, ...) const;
  ushort PrintfUnshaded(bitmap*, ushort, ushort, ushort, const char*, ...) const;
  bitmap* Colorize(ushort*) const;
  bitmap* Colorize(vector2d, vector2d, ushort*) const;
 protected:
  ushort XSize, YSize;
  uchar* Palette;
  uchar* PaletteBuffer;
};

#endif
