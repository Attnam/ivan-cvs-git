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
	static bitmap* GetTerrainGraphic(void)	{return Graphic[GTERRAIN];}
	static bitmap* GetItemGraphic(void)	{return Graphic[GITEM];}
	static bitmap* GetCharacterGraphic(void)	{return Graphic[GCHARACTER];}
	static bitmap* GetFOWGraphic(void)	{return Graphic[GFOWGRAPHIC];}
	static bitmap* GetHumanGraphic(void)	{return Graphic[GHUMAN];}
	static bitmap* GetFontRGraphic(void)	{return Graphic[GFONTR];}
	static bitmap* GetFontBGraphic(void)	{return Graphic[GFONTB];}
	static bitmap* GetFontWGraphic(void)	{return Graphic[GFONTW];}
	static bitmap* GetSymbolGraphic(void)	{return Graphic[GSYMBOL];}
	static bitmap* GetTileBuffer(void)	{ return TileBuffer; }
	static void BlitTileBuffer(vector, ushort = 256);
private:
	static bitmap* Graphic[GRAPHIC_TYPES];
	static bitmap* TileBuffer;
	static char* GraphicFileName[];
};

#endif
