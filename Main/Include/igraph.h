#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#pragma warning(disable : 4786)

#define GRAPHIC_TYPES	11

#define GLTERRAIN	0
#define GITEM		1
#define GCHARACTER	2
#define GHUMAN		3
#define GWTERRAIN	4
#define GFOW		5
#define GCURSOR		6
#define GFONTR		7
#define GFONTB		8
#define GFONTW		9
#define GSYMBOL		10

#define FONTR		igraph::GetFontRGraphic()
#define FONTB		igraph::GetFontBGraphic()
#define FONTW		igraph::GetFontWGraphic()

#include <windows.h>
#include <map>

#include "typedef.h"
#include "vector2d.h"

#include "save.h"

class bitmap;

struct graphic_id
{
	graphic_id() {}
	graphic_id(vector2d BitmapPos, ushort* Color, uchar FileIndex) : BitmapPos(BitmapPos), Color(Color), FileIndex(FileIndex) {}
	vector2d BitmapPos;
	ushort* Color;
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

	if(!GI1.Color || !GI2.Color)
		return false;

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
	static void Init(HINSTANCE, HWND*);
	static void DeInit();
	static bitmap* GetLevelTerrainGraphic()	{ return Graphic[GLTERRAIN]; }
	static bitmap* GetWorldMapTerrainGraphic()	{ return Graphic[GWTERRAIN]; }
	static bitmap* GetItemGraphic()		{ return Graphic[GITEM]; }
	static bitmap* GetCharacterGraphic()	{ return Graphic[GCHARACTER]; }
	static bitmap* GetFOWGraphic()		{ return Graphic[GFOW]; }
	static bitmap* GetCursorGraphic()		{ return Graphic[GCURSOR]; }
	static bitmap* GetHumanGraphic()		{ return Graphic[GHUMAN]; }
	static bitmap* GetFontRGraphic()		{ return Graphic[GFONTR]; }
	static bitmap* GetFontBGraphic()		{ return Graphic[GFONTB]; }
	static bitmap* GetFontWGraphic()		{ return Graphic[GFONTW]; }
	static bitmap* GetSymbolGraphic()		{ return Graphic[GSYMBOL]; }
	static bitmap* GetTileBuffer()		{ return TileBuffer; }
	static void BlitTileBuffer(vector2d, ushort = 256);
	static void DrawCursor(vector2d);
	static tile GetTile(graphic_id);
	static tile AddUser(graphic_id);
	static void RemoveUser(graphic_id);
private:
	static bitmap* Graphic[GRAPHIC_TYPES];
	static bitmap* TileBuffer;
	static char* GraphicFileName[];
	static tilemap TileMap;
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
