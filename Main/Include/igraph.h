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
#include "vector.h"

class bitmap;

class igraph
{
public:
	static void Init(HINSTANCE, HWND*);
	static void DeInit(void);
	static bitmap* GetLevelTerrainGraphic(void)	{ return Graphic[GLTERRAIN]; }
	static bitmap* GetWorldMapTerrainGraphic(void)	{ return Graphic[GWTERRAIN]; }
	static bitmap* GetItemGraphic(void)		{ return Graphic[GITEM]; }
	static bitmap* GetCharacterGraphic(void)	{ return Graphic[GCHARACTER]; }
	static bitmap* GetFOWGraphic(void)		{ return Graphic[GFOWGRAPHIC]; }
	static bitmap* GetHumanGraphic(void)		{ return Graphic[GHUMAN]; }
	static bitmap* GetFontRGraphic(void)		{ return Graphic[GFONTR]; }
	static bitmap* GetFontBGraphic(void)		{ return Graphic[GFONTB]; }
	static bitmap* GetFontWGraphic(void)		{ return Graphic[GFONTW]; }
	static bitmap* GetSymbolGraphic(void)		{ return Graphic[GSYMBOL]; }
	static bitmap* GetTileBuffer(void)		{ return TileBuffer; }
	static void BlitTileBuffer(vector, ushort = 256, bool = false);
private:
	static bitmap* Graphic[GRAPHIC_TYPES];
	static bitmap* TileBuffer;
	static char* GraphicFileName[];
};

#endif
