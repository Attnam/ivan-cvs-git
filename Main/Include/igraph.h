#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <map>
#include <string>

#include "typedef.h"
#include "vector2d.h"
#include "ivandef.h"

class bitmap;
class colorizablebitmap;
class inputfile;
class outputfile;

struct graphic_id
{
  vector2d BitmapPos;
  ushort Color[4];
  uchar Alpha[4];
  uchar FileIndex;
  uchar SpecialType;
};

inline bool operator < (const graphic_id& GI1, const graphic_id& GI2)
{
  if(GI1.FileIndex != GI2.FileIndex)
    return GI1.FileIndex < GI2.FileIndex;

  if(GI1.BitmapPos.X != GI2.BitmapPos.X)
    return GI1.BitmapPos.X < GI2.BitmapPos.X;

  if(GI1.BitmapPos.Y != GI2.BitmapPos.Y)
    return GI1.BitmapPos.Y < GI2.BitmapPos.Y;

  if(GI1.SpecialType != GI2.SpecialType)
    return GI1.SpecialType < GI2.SpecialType;

  if(GI1.Color[0] != GI2.Color[0])
    return GI1.Color[0] < GI2.Color[0];

  if(GI1.Color[1] != GI2.Color[1])
    return GI1.Color[1] < GI2.Color[1];

  if(GI1.Color[2] != GI2.Color[2])
    return GI1.Color[2] < GI2.Color[2];

  if(GI1.Color[3] != GI2.Color[3])
    return GI1.Color[3] < GI2.Color[3];

  if(GI1.Alpha[0] != GI2.Alpha[0])
    return GI1.Alpha[0] < GI2.Alpha[0];

  if(GI1.Alpha[1] != GI2.Alpha[1])
    return GI1.Alpha[1] < GI2.Alpha[1];

  if(GI1.Alpha[2] != GI2.Alpha[2])
    return GI1.Alpha[2] < GI2.Alpha[2];

  if(GI1.Alpha[3] != GI2.Alpha[3])
    return GI1.Alpha[3] < GI2.Alpha[3];

  return false;
}

struct tile
{
  tile() { }
  tile(bitmap* Bitmap, ulong Users = 1) : Bitmap(Bitmap), Users(Users) { }
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
  static bitmap* GetWTerrainGraphic() { return Graphic[GRWTERRAIN]; }
  static bitmap* GetFOWGraphic() { return Graphic[GRFOW]; }
  static bitmap* GetCursorGraphic() { return Graphic[GRCURSOR]; }
  static bitmap* GetSymbolGraphic() { return Graphic[GRSYMBOL]; }
  static bitmap* GetMenuGraphic() { return Graphic[GRMENU]; }
  static bitmap* GetTileBuffer() { return TileBuffer; }
  static void DrawCursor(vector2d);
  static tile GetTile(graphic_id);
  static tile AddUser(graphic_id);
  static void RemoveUser(graphic_id);
  static bitmap* GetOutlineBuffer() { return OutlineBuffer; }
  static colorizablebitmap* GetCharacterRawGraphic() { return RawGraphic[GRCHARACTER]; }
 private:
  static colorizablebitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES];
  static bitmap* TileBuffer;
  static std::string RawGraphicFileName[];
  static std::string GraphicFileName[];
  static tilemap TileMap;
  static bitmap* OutlineBuffer;
};

outputfile& operator<<(outputfile&, const graphic_id&);
inputfile& operator>>(inputfile&, graphic_id&);

#endif


