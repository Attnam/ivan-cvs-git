#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define RAW_TYPES	3

#define GLTERRAIN	0
#define GITEM		1
#define GCHARACTER	2

#define GRAPHIC_TYPES	5

#define GHUMAN		0
#define GWTERRAIN	1
#define GFOW		2
#define GCURSOR		3
#define GSYMBOL		4

#ifdef WIN32
#include <windows.h>
#endif

#include <map>

#include "typedef.h"
#include "vector2d.h"

#include "save.h"

class bitmap;
class colorizablebitmap;

struct graphic_id
{
  vector2d BitmapPos;
  ushort Color[4];
  uchar FileIndex;
};

inline bool operator < (const graphic_id& GI1, const graphic_id& GI2)
{
  if(GI1.FileIndex != GI2.FileIndex)
    return GI1.FileIndex < GI2.FileIndex;

  if(GI1.BitmapPos.X != GI2.BitmapPos.X)
    return GI1.BitmapPos.X < GI2.BitmapPos.X;

  if(GI1.BitmapPos.Y != GI2.BitmapPos.Y)
    return GI1.BitmapPos.Y < GI2.BitmapPos.Y;

  if(!GI1.Color || !GI2.Color)	// This shouldn't be possible, but if it is, it's better to behave oddly
    return false;		// than to crash horribly and undebuggablely

  if(GI1.Color[0] != GI2.Color[0])
    return GI1.Color[0] < GI2.Color[0];

  if(GI1.Color[1] != GI2.Color[1])
    return GI1.Color[1] < GI2.Color[1];

  if(GI1.Color[2] != GI2.Color[2])
    return GI1.Color[2] < GI2.Color[2];

  if(GI1.Color[3] != GI2.Color[3])
    return GI1.Color[3] < GI2.Color[3];

  return false;
}

struct tile
{
  tile() {}
  tile(bitmap* Bitmap, ulong Users) : Bitmap(Bitmap), Users(Users) {}
  bitmap* Bitmap;
  ulong Users;
};

typedef std::map<graphic_id, tile> tilemap;

class igraph
{
 public:
#ifdef WIN32
  static void Init(HINSTANCE, HWND*);
#else
  static void Init();
#endif
  static void DeInit();
  static bitmap* GetWorldMapTerrainGraphic() { return Graphic[GWTERRAIN]; }
  static bitmap* GetFOWGraphic() { return Graphic[GFOW]; }
  static bitmap* GetCursorGraphic() { return Graphic[GCURSOR]; }
  static bitmap* GetHumanGraphic() { return Graphic[GHUMAN]; }
  static bitmap* GetSymbolGraphic() { return Graphic[GSYMBOL]; }
  static bitmap* GetTileBuffer() { return TileBuffer; }
  static void DrawCursor(vector2d);
  static tile GetTile(graphic_id);
  static tile AddUser(graphic_id);
  static void RemoveUser(graphic_id);
  static bitmap* GetOutlineBuffer() { return OutlineBuffer; }
 private:
  static colorizablebitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES];
  static bitmap* TileBuffer;
  static char* RawGraphicFileName[];
  static char* GraphicFileName[];
  static tilemap TileMap;
  static bitmap* OutlineBuffer;
};

inline outputfile& operator<<(outputfile& SaveFile, graphic_id GI)
{
  SaveFile << GI.BitmapPos << GI.FileIndex;
  SaveFile << GI.Color[0] << GI.Color[1] << GI.Color[2] << GI.Color[3];
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, graphic_id& GI)
{
  SaveFile >> GI.BitmapPos >> GI.FileIndex;
  SaveFile >> GI.Color[0] >> GI.Color[1] >> GI.Color[2] >> GI.Color[3];
  return SaveFile;
}

#endif
