#include "bitmap.h"
#include "graphics.h"
#include "igraph.h"
#include "game.h"
#include "error.h"
#include "colorbit.h"
#include "config.h"

#ifdef WIN32
#include "resource.h"
#endif

colorizablebitmap* igraph::RawGraphic[RAW_TYPES];
bitmap* igraph::Graphic[GRAPHIC_TYPES];
bitmap* igraph::TileBuffer;
bitmap* igraph::OutlineBuffer;
char* igraph::RawGraphicFileName[] = { "Graphics/LTerrain.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx" };
char* igraph::GraphicFileName[] = { "Graphics/Human.pcx", "Graphics/WTerrain.pcx", "Graphics/FOW.pcx", "Graphics/Cursor.pcx", "Graphics/Symbol.pcx"};
tilemap igraph::TileMap;

#ifdef WIN32
void igraph::Init(HINSTANCE hInst, HWND* hWnd)
#else
void igraph::Init()
#endif
{
	static bool AlreadyInstalled = false;

	if(!AlreadyInstalled)
	{
		AlreadyInstalled = true;
#ifdef WIN32
		graphics::SetMode(hInst, hWnd, "IVAN 0.302a", 800, 600, 16, configuration::GetFullScreenMode(), MAKEINTRESOURCE(IDI_LOGO));
#else
		graphics::SetMode("IVAN 0.302a", 800, 600, 16);
#endif
		DOUBLEBUFFER->Fill(0);
		graphics::BlitDBToScreen();
#ifdef WIN32
		graphics::SetSwitchModeHandler(configuration::SwitchModeHandler);
#endif
		graphics::LoadDefaultFont((GAME_DIR + "Graphics/Font.pcx").c_str());

		uchar c;

		for(c = 0; c < RAW_TYPES; ++c)
			RawGraphic[c] = new colorizablebitmap((GAME_DIR + std::string(RawGraphicFileName[c])).c_str());

		for(c = 0; c < GRAPHIC_TYPES; ++c)
			Graphic[c] = new bitmap((GAME_DIR + std::string(GraphicFileName[c])).c_str());

		TileBuffer = new bitmap(16, 16);
		OutlineBuffer = new bitmap(16, 16);

		atexit(igraph::DeInit);
	}
}

void igraph::DeInit()
{
	uchar c;

	for(c = 0; c < RAW_TYPES; ++c)
		delete RawGraphic[c];

	for(c = 0; c < GRAPHIC_TYPES; ++c)
		delete Graphic[c];

	delete TileBuffer;
	delete OutlineBuffer;
}

void igraph::DrawCursor(vector2d Pos)
{
	igraph::GetCursorGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, Pos.X, Pos.Y, 16, 16, ushort(256.0f * configuration::GetContrast() / 100));
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
		++Iterator->second.Users;
	else
	{
		bitmap* Bitmap = RawGraphic[GI.FileIndex]->Colorize(GI.BitmapPos, vector2d(16, 16), GI.Color);

		tile Tile(Bitmap, 1);

		TileMap[GI] = Tile;

		return Tile;
	}

	return Iterator->second;
}

void igraph::RemoveUser(graphic_id GI)
{
	tilemap::iterator Iterator = TileMap.find(GI);

	if(Iterator != TileMap.end())
		if(!--Iterator->second.Users)
		{
			delete Iterator->second.Bitmap;
			TileMap.erase(Iterator);
		}
}
