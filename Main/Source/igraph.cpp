#include <windows.h>
#include <conio.h>
#include <cstdlib>

#include "bitmap.h"
#include "graphics.h"
#include "igraph.h"
#include "whandler.h"
#include "game.h"

bitmap* igraph::Graphic[GRAPHIC_TYPES];
bitmap* igraph::TileBuffer;
char* igraph::GraphicFileName[] = {"Graphics/LTerrain.pcx", "Graphics/WTerrain.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/FOW.pcx", "Graphics/Human.pcx", "Graphics/FontR.pcx", "Graphics/FontB.pcx", "Graphics/FontW.pcx", "Graphics/Symbol.pcx"};

void igraph::Init(HINSTANCE hInst, HWND* hWnd)
{
	static bool AlreadyInstalled = false;

	if(!AlreadyInstalled)
	{
		AlreadyInstalled = true;

		graphics::SetMode(hInst, hWnd, "IVAN 0.240a", 800, 600, 16, false);

		for(uchar c = 0; c < GRAPHIC_TYPES; c++)
			Graphic[c] = new bitmap(GraphicFileName[c]);

		TileBuffer = new bitmap(16, 16);

		atexit(igraph::DeInit);
	}
}

void igraph::DeInit(void)
{
	for(uchar c = 0; c < GRAPHIC_TYPES; c++)
		delete Graphic[c];
}

void igraph::BlitTileBuffer(vector Pos, ushort Luminance)
{
	Luminance = ushort(Luminance * game::CSoftGamma());

	if(Luminance == 256)
		TileBuffer->BlitToDB(0, 0, Pos.X, Pos.Y, 16, 16);
	else
		TileBuffer->BlitToDB(0, 0, Pos.X, Pos.Y, 16, 16, Luminance);
}


