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

struct graphic_id
{
  graphic_id() { }
  bool operator<(const graphic_id& GI) const { return CompareBits(*this, GI); }
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
};

#ifdef VC
#pragma pack()
#endif

inline outputfile& operator<<(outputfile& SaveFile, const graphic_id& Value)
{
  SaveFile.Write(reinterpret_cast<const char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, graphic_id& Value)
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
  static colorizablebitmap* GetRawGraphic(ushort Index) { return RawGraphic[Index]; }
 private:
  static colorizablebitmap* RawGraphic[RAW_TYPES];
  static bitmap* Graphic[GRAPHIC_TYPES];
  static bitmap* TileBuffer;
  static std::string RawGraphicFileName[];
  static std::string GraphicFileName[];
  static tilemap TileMap;
  static bitmap* OutlineBuffer;
};

#endif
