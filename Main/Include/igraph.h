#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#define GRAPHIC_TYPES	10

#define GLTERRAIN	0
#define GWTERRAIN	1
#define GITEM		2
#define GCHARACTER	3
#define GFOWGRAPHIC	4
#define GHUMAN		5
#define GFONTR		6
#define GFONTB		7
#define GFONTW		8
#define GSYMBOL		9

#define FONTR		igraph::GetFontRGraphic()
#define FONTB		igraph::GetFontBGraphic()
#define FONTW		igraph::GetFontWGraphic()

#include <windows.h>

#include "typedef.h"
#include "vector2d.h"

class bitmap;

/*struct materialgraphics
{
	bitmap* CharacterGraphics;
};*/

class igraph
{
public:
	static void Init(HINSTANCE, HWND*);
	static void DeInit();
	static bitmap* GetLevelTerrainGraphic()	{ return Graphic[GLTERRAIN]; }
	static bitmap* GetWorldMapTerrainGraphic()	{ return Graphic[GWTERRAIN]; }
	static bitmap* GetItemGraphic()		{ return Graphic[GITEM]; }
	static bitmap* GetCharacterGraphic()	{ return Graphic[GCHARACTER]; }
	static bitmap* GetCharacterGraphic(ushort);// { return Graphic[GCHARACTER]; }
	static bitmap* GetFOWGraphic()		{ return Graphic[GFOWGRAPHIC]; }
	static bitmap* GetHumanGraphic()		{ return Graphic[GHUMAN]; }
	static bitmap* GetFontRGraphic()		{ return Graphic[GFONTR]; }
	static bitmap* GetFontBGraphic()		{ return Graphic[GFONTB]; }
	static bitmap* GetFontWGraphic()		{ return Graphic[GFONTW]; }
	static bitmap* GetSymbolGraphic()		{ return Graphic[GSYMBOL]; }
	static bitmap* GetTileBuffer()		{ return TileBuffer; }
	static void BlitTileBuffer(vector2d, ushort = 256);
	static void DrawCursor(vector2d);
	static void CreateGraphics(ushort) {}
	//static std::map<ushort, materialgraphics>& GetMaterialGraphics() const { return MaterialGraphics; }
private:
	static bitmap* Graphic[GRAPHIC_TYPES];
	static bitmap* TileBuffer;
	static char* GraphicFileName[];
	//static std::map<ushort, materialgraphics> MaterialGraphics;
};

#endif
