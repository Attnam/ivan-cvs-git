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
  ushort SpecialFlags NO_ALIGNMENT;
  vector2d SparklePos NO_ALIGNMENT;
  uchar SparkleFrame NO_ALIGNMENT;
  ushort OutlineColor NO_ALIGNMENT;
  uchar OutlineAlpha NO_ALIGNMENT;
  ushort Seed NO_ALIGNMENT;
  uchar FlyAmount NO_ALIGNMENT;
  vector2d Position NO_ALIGNMENT;
  uchar RustData[4] NO_ALIGNMENT;
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

struct graphicdata
{
  graphicdata() : AnimationFrames(0) { }
  ~graphicdata();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Retire();
  ushort AnimationFrames;
  bitmap** Picture;
  tilemap::iterator* GraphicIterator;
};

outputfile& operator<<(outputfile&, const graphicdata&);
inputfile& operator>>(inputfile&, graphicdata&);

class igraph
{
 public:
  static void Init();
  static void DeInit();
  static const bitmap* GetWTerrainGraphic() { return Graphic[GR_WTERRAIN]; }
  static const bitmap* GetFOWGraphic() { return Graphic[GR_FOW]; }
  static const bitmap* GetCursorGraphic() { return Graphic[GR_CURSOR]; }
  static const bitmap* GetSymbolGraphic() { return Graphic[GR_SYMBOL]; }
  static const bitmap* GetMenuGraphic() { return Graphic[GR_MENU]; }
  static const bitmap* GetTransparentTile() { return Graphic[GR_TRANSPARENT_COLOR_TILE]; }
  static bitmap* GetTileBuffer() { return TileBuffer; }
  static void DrawCursor(vector2d);
  static tilemap::iterator AddUser(const graphicid&);
  static void RemoveUser(tilemap::iterator);
  static const colorizablebitmap* GetHumanoidRawGraphic() { return RawGraphic[GR_HUMANOID]; }
  static const colorizablebitmap* GetCharacterRawGraphic() { return RawGraphic[GR_CHARACTER]; }
  static const colorizablebitmap* GetEffectRawGraphic() { return RawGraphic[GR_EFFECT]; }
  static const colorizablebitmap* GetRawGraphic(ushort Index) { return RawGraphic[Index]; }
  static bool** GetBodyBitmapValidityMap(ushort);
  static bitmap* GetFlagBuffer() { return FlagBuffer; }
 private:
  static void CreateBodyBitmapValidityMaps();
  static colorizablebitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES + 1];
  static bitmap* TileBuffer;
  static const char* RawGraphicFileName[];
  static const char* GraphicFileName[];
  static tilemap TileMap;
  static uchar RollBuffer[256];
  static bitmap* FlagBuffer;
  static bool*** BodyBitmapValidityMap;
};

#endif
