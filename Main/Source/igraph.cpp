#include "bitmap.h"
#include "graphics.h"
#include "igraph.h"
#include "game.h"
#include "error.h"

bitmap* igraph::Graphic[GRAPHIC_TYPES];
bitmap* igraph::TileBuffer;
char* igraph::GraphicFileName[] = {"Graphics/LTerrain.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/Human.pcx", "Graphics/WTerrain.pcx", "Graphics/FOW.pcx", "Graphics/Cursor.pcx", "Graphics/FontR.pcx", "Graphics/FontB.pcx", "Graphics/FontW.pcx", "Graphics/Symbol.pcx"};
tilemap igraph::TileMap;

void igraph::Init(HINSTANCE hInst, HWND* hWnd)
{
	static bool AlreadyInstalled = false;

	if(!AlreadyInstalled)
	{
		AlreadyInstalled = true;

		graphics::SetMode(hInst, hWnd, "IVAN 0.240a", 800, 600, 16, false);

		uchar c;

		for(c = 0; c < 3; ++c)
			Graphic[c] = new bitmap(GraphicFileName[c], false);

		for(c = 3; c < GRAPHIC_TYPES; ++c)
			Graphic[c] = new bitmap(GraphicFileName[c]);

		TileBuffer = new bitmap(16, 16);

		atexit(igraph::DeInit);
	}
}

void igraph::DeInit()
{
	for(uchar c = 0; c < GRAPHIC_TYPES; ++c)
		delete Graphic[c];
}

void igraph::BlitTileBuffer(vector2d Pos, ushort Luminance)
{
	Luminance = ushort(Luminance * game::GetSoftGamma());

	if(Luminance == 256)
		TileBuffer->Blit(DOUBLEBUFFER, 0, 0, Pos.X, Pos.Y, 16, 16);
	else
		TileBuffer->Blit(DOUBLEBUFFER, 0, 0, Pos.X, Pos.Y, 16, 16, Luminance);
}

void igraph::DrawCursor(vector2d Pos)
{
	ushort Luminance = ushort(256 * game::GetSoftGamma());

	igraph::GetCursorGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, Pos.X, Pos.Y, 16, 16, Luminance);
}

tile igraph::GetTile(graphic_id GI)
{
	tilemap::iterator Iterator = TileMap.find(GI);

	if(Iterator == TileMap.end())
		ABORT("Art stolen!");

	return Iterator->second;
}

tile igraph::AddUser(graphic_id GI)
{
	tilemap::iterator Iterator = TileMap.find(GI);

	if(Iterator != TileMap.end())
		++(Iterator->second.Users);
	else
	{
		bitmap* Bitmap = Graphic[GI.FileIndex]->ColorizeTo16Bit(GI.BitmapPos, vector2d(16, 16), GI.Color);

		tile Tile(Bitmap, 1);

		graphic_id NewGI(GI.BitmapPos, new ushort[4], GI.FileIndex);

		for(uchar c = 0; c < 4; ++c)
			NewGI.Color[c] = GI.Color[c];

		TileMap[NewGI] = Tile;

		return Tile;
	}

	return Iterator->second;
}

void igraph::RemoveUser(graphic_id GI)
{
	tilemap::iterator Iterator = TileMap.find(GI);

	if(Iterator != TileMap.end())
		if(!(--Iterator->second.Users))
		{
			delete Iterator->second.Bitmap;
			delete [] Iterator->first.Color;
			TileMap.erase(Iterator);
		}
}
