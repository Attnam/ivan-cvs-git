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
