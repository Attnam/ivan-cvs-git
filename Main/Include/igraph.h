#ifndef __IGRAPH_H__
#define __IGRAPH_H__

#define GRAPHIC_TYPES	9

#define GTERRAIN	0
#define GITEM		1
#define GCHARACTER	2
#define GFOWGRAPHIC	3
#define GHUMAN		4
#define GFONTR		5
#define GFONTB		6
#define GFONTW		7
#define GSYMBOL		8

#define FONTR		igraph::CFontRGraphic()
#define FONTB		igraph::CFontBGraphic()
#define FONTW		igraph::CFontWGraphic()

#include <windows.h>

#include "typedef.h"
#include "vector.h"

#undef RGB
#define RGB(Red, Green, Blue) ((Red) << 8 & 0xF800) | ((Green) << 3 & 0x7E0) | ((Blue) >> 3 & 0x1F)

#define GET_RED(Color)		((Color >> 8) & 0xF8)
#define GET_GREEN(Color)	((Color >> 3) & 0xFC)
#define GET_BLUE(Color)		((Color << 3) & 0xF8)

#define PINK 0xF81F
class bitmap;

class igraph
{
public:
	static void Init(HINSTANCE, HWND*);
	static void DeInit(void);
	static bitmap* CTerrainGraphic(void)	{return Graphic[GTERRAIN];}
	static bitmap* CItemGraphic(void)	{return Graphic[GITEM];}
	static bitmap* CCharacterGraphic(void)	{return Graphic[GCHARACTER];}
	static bitmap* CFOWGraphic(void)	{return Graphic[GFOWGRAPHIC];}
	static bitmap* CHumanGraphic(void)	{return Graphic[GHUMAN];}
	static bitmap* CFontRGraphic(void)	{return Graphic[GFONTR];}
	static bitmap* CFontBGraphic(void)	{return Graphic[GFONTB];}
	static bitmap* CFontWGraphic(void)	{return Graphic[GFONTW];}
	static bitmap* CSymbolGraphic(void)	{return Graphic[GSYMBOL];}
	static bitmap* CTileBuffer(void)	{ return TileBuffer; }
	static void BlitTileBuffer(vector, ushort = 256);
private:
	static bitmap* Graphic[GRAPHIC_TYPES];
	static bitmap* TileBuffer;
	static char* GraphicFileName[];
};

#endif
