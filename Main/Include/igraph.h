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

#include "felibdef.h"
#include "typedef.h"
#include "vector2d.h"
#include "ivandef.h"
#include "save.h"

class bitmap;
class colorizablebitmap;

/* CompareBits doesn't like alignment of structure members */

#ifdef VC
#pragma pack(1)
#endif

struct graphicid
{
  graphicid() { }
  bool operator<(const graphicid& GI) const { return CompareBits(*this, GI); }
  vector2d BitmapPos PACKED;
  ushort Color[4] PACKED;
  ushort Frame PACKED;
  uchar FileIndex PACKED;
  uchar BaseAlpha PACKED;
  uchar Alpha[4] PACKED;
  uchar SpecialFlags PACKED;
  vector2d SparklePos PACKED;
  uchar SparkleTime PACKED;
  ushort OutlineColor PACKED;
  ulong FlySeed PACKED;
  uchar FlyAmount PACKED;
};

#ifdef VC
#pragma pack()
#endif

inline outputfile& operator<<(outputfile& SaveFile, const graphicid& Value)
{
  SaveFile.Write(reinterpret_cast<const char*>(&Value), sizeof(Value));
  return SaveFile;
}


inline inputfile& operator>>(inputfile& SaveFile, graphicid& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

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
#ifdef WIN32
  static void Init(HINSTANCE, HWND*);
#else
  static void Init();
#endif
  static void DeInit();
  static bitmap* GetWTerrainGraphic() { return Graphic[GR_WTERRAIN]; }
  static bitmap* GetFOWGraphic() { return Graphic[GR_FOW]; }
  static bitmap* GetCursorGraphic() { return Graphic[GR_CURSOR]; }
  static bitmap* GetSymbolGraphic() { return Graphic[GR_SYMBOL]; }
  static bitmap* GetMenuGraphic() { return Graphic[GR_MENU]; }
  static bitmap* GetTransparentTile() { return Graphic[GR_TRANSPARENT_TILE]; }
  static bitmap* GetTileBuffer() { return TileBuffer; }
  static void DrawCursor(vector2d);
  static tile GetTile(graphicid);
  static tile AddUser(graphicid);
  static void RemoveUser(graphicid);
  static bitmap* GetOutlineBuffer() { return OutlineBuffer; }
  static colorizablebitmap* GetCharacterRawGraphic() { return RawGraphic[GR_CHARACTER]; }
  static colorizablebitmap* GetRawGraphic(ushort Index) { return RawGraphic[Index]; }
 private:
  static colorizablebitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES + 1];
  static bitmap* TileBuffer;
  static std::string RawGraphicFileName[];
  static std::string GraphicFileName[];
  static tilemap TileMap;
  static bitmap* OutlineBuffer;
};

#endif
