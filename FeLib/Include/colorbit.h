#ifndef __COLORBIT_H__
#define __COLORBIT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>
#include <map>

#include "typedef.h"
#include "vector2d.h"

class outputfile;
class inputfile;
class bitmap;

typedef std::map<ushort, std::pair<bitmap*, bitmap*> > fontcache;

class colorizablebitmap
{
 public:
  colorizablebitmap(const std::string&);
  ~colorizablebitmap();
  void colorizablebitmap::Save(const std::string&);

  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort*) const;
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, Width, Height, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, ushort Width, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, Width, Height, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, vector2d BlitSize, ushort* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, ushort Width, ushort Height, ushort* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, Width, Height, Color); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, ushort DestX, ushort DestY, vector2d BlitSize, ushort* Color) const { MaskedBlit(Bitmap, Source.X, Source.Y, DestX, DestY, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, vector2d Dest, vector2d BlitSize, ushort* Color) const { MaskedBlit(Bitmap, SourceX, SourceY, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, vector2d Source, vector2d Dest, vector2d BlitSize, ushort* Color) const  { MaskedBlit(Bitmap, Source.X, Source.Y, Dest.X, Dest.Y, BlitSize.X, BlitSize.Y, Color); }
  void MaskedBlit(bitmap* Bitmap, ushort* Color) const { MaskedBlit(Bitmap, 0, 0, 0, 0, XSize, YSize, Color); }

  void Printf(bitmap*, ushort, ushort, ushort, const char*, ...) const;
  void PrintfShade(bitmap*, ushort, ushort, ushort, const char*, ...) const;
  bitmap* Colorize(ushort*, uchar = 255, uchar* = 0) const;
  bitmap* Colorize(vector2d, vector2d, ushort*, uchar = 255, uchar* = 0) const;
  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  vector2d GetSize() const { return vector2d(XSize, YSize); }

  void AlterGradient(ushort, ushort, ushort, ushort, uchar, char, bool);
  void AlterGradient(vector2d Pos, ushort Width, ushort Height, uchar MColor, char Amount, bool Clip) { AlterGradient(Pos.X, Pos.Y, Width, Height, MColor, Amount, Clip); }
  void AlterGradient(ushort X, ushort Y, vector2d AlterSize, uchar MColor, char Amount, bool Clip) { AlterGradient(X, Y, AlterSize.X, AlterSize.Y, MColor, Amount, Clip); }
  void AlterGradient(vector2d Pos, vector2d AlterSize, uchar MColor, char Amount, bool Clip) { AlterGradient(Pos.X, Pos.Y, AlterSize.X, AlterSize.Y, MColor, Amount, Clip); }

  void SwapColors(ushort, ushort, ushort, ushort, uchar, uchar);
  void SwapColors(vector2d Pos, ushort Width, ushort Height, uchar Color1, uchar Color2) { SwapColors(Pos.X, Pos.Y, Width, Height, Color1, Color2); }
  void SwapColors(ushort X, ushort Y, vector2d AlterSize, uchar Color1, uchar Color2) { SwapColors(X, Y, AlterSize.X, AlterSize.Y, Color1, Color2); }
  void SwapColors(vector2d Pos, vector2d AlterSize, uchar Color1, uchar Color2) { SwapColors(Pos.X, Pos.Y, AlterSize.X, AlterSize.Y, Color1, Color2); }

  void Roll(ushort, ushort, ushort, ushort, short, short);
  void Roll(vector2d Pos, ushort Width, ushort Height, short XMove, short YMove) { Roll(Pos.X, Pos.Y, Width, Height, XMove, YMove); }
  void Roll(ushort X, ushort Y, vector2d BlitSize, short XMove, short YMove) { Roll(X, Y, BlitSize.X, BlitSize.Y, XMove, YMove); }
  void Roll(ushort X, ushort Y, ushort Width, ushort Height, vector2d Move) { Roll(X, Y, Width, Height, Move.X, Move.Y); }
  void Roll(vector2d Pos, vector2d BlitSize, short XMove, short YMove) { Roll(Pos.X, Pos.Y, BlitSize.X, BlitSize.Y, XMove, YMove); }
  void Roll(vector2d Pos, ushort Width, ushort Height, vector2d Move) { Roll(Pos.X, Pos.Y, Width, Height, Move.X, Move.Y); }
  void Roll(ushort X, ushort Y, vector2d BlitSize, vector2d Move) { Roll(X, Y, BlitSize.X, BlitSize.Y, Move.X, Move.Y); }
  void Roll(vector2d Pos, vector2d BlitSize, vector2d Move) { Roll(Pos.X, Pos.Y, BlitSize.X, BlitSize.Y, Move.X, Move.Y); }

  void CreateFontCache(ushort);
 protected:
  ushort XSize, YSize;
  uchar* Palette;
  uchar* PaletteBuffer;
  fontcache FontCache;
};

#endif

