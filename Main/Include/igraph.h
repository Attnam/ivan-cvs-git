#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>

#include "ivandef.h"
#include "femath.h"

class bitmap;
class colorizablebitmap;
class outputfile;
class inputfile;
class festring;

/* memcmp doesn't like alignment of structure members */

#ifdef VC
#pragma pack(1)
#endif

struct graphicid
{
  graphicid() { }
  bool operator<(const graphicid& GI) const { return memcmp(this, &GI, sizeof(graphicid)) < 0; }
  vector2d BitmapPos NO_ALIGNMENT;
  ushort Color[4] NO_ALIGNMENT;
  uchar Frame NO_ALIGNMENT;
  uchar FileIndex NO_ALIGNMENT;
  uchar BaseAlpha NO_ALIGNMENT;
  uchar Alpha[4] NO_ALIGNMENT;
  uchar SpecialFlags NO_ALIGNMENT;
  vector2d SparklePos NO_ALIGNMENT;
  uchar SparkleFrame NO_ALIGNMENT;
  ushort OutlineColor NO_ALIGNMENT;
  ushort Seed NO_ALIGNMENT;
  uchar FlyAmount NO_ALIGNMENT;
};

#ifdef VC
#pragma pack()
#endif

outputfile& operator<<(outputfile&, const graphicid&);
inputfile& operator>>(inputfile&, graphicid&);

struct tile
{
  tile() { }
  tile(bitmap* Bitmap) : Bitmap(Bitmap), Users(1) { }
  bitmap* Bitmap;
  ulong Users;
};

typedef std::map<graphicid, tile> tilemap;

class igraph
{
 public:
  static void Init();
  static void DeInit();
  static bitmap* GetWTerrainGraphic() { return Graphic[GR_WTERRAIN]; }
  static bitmap* GetFOWGraphic() { return Graphic[GR_FOW]; }
  static bitmap* GetCursorGraphic() { return Graphic[GR_CURSOR]; }
  static bitmap* GetSymbolGraphic() { return Graphic[GR_SYMBOL]; }
  static bitmap* GetMenuGraphic() { return Graphic[GR_MENU]; }
  static bitmap* GetTransparentTile() { return Graphic[GR_TRANSPARENT_COLOR_TILE]; }
  static bitmap* GetTileBuffer() { return TileBuffer; }
  static void DrawCursor(vector2d);
  static bitmap* AddUser(const graphicid&);
  static void RemoveUser(const graphicid&);
  static bitmap* GetOutlineBuffer() { return OutlineBuffer; }
  static colorizablebitmap* GetHumanoidRawGraphic() { return RawGraphic[GR_HUMANOID]; }
  static colorizablebitmap* GetCharacterRawGraphic() { return RawGraphic[GR_CHARACTER]; }
  static colorizablebitmap* GetEffectRawGraphic() { return RawGraphic[GR_EFFECT]; }
  static colorizablebitmap* GetRawGraphic(ushort Index) { return RawGraphic[Index]; }
 private:
  static colorizablebitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES + 1];
  static bitmap* TileBuffer;
  static const char* RawGraphicFileName[];
  static const char* GraphicFileName[];
  static tilemap TileMap;
  static bitmap* OutlineBuffer;
};

#endif
