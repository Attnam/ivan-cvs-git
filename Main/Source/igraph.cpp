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
char* igraph::RawGraphicFileName[] = { "Graphics/LTerrain.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/Humanoid.pcx" };
char* igraph::GraphicFileName[] = { "Graphics/WTerrain.pcx", "Graphics/FOW.pcx", "Graphics/Cursor.pcx", "Graphics/Symbol.pcx", "Graphics/Menu.pcx"};
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
      graphics::Init();

      std::string Title = std::string("IVAN ") + VERSION;

#ifdef WIN32
      graphics::SetMode(hInst, hWnd, Title.c_str(), 800, 600, 16, configuration::GetFullScreenMode(), MAKEINTRESOURCE(IDI_LOGO));
#endif
#ifdef USE_SDL
      graphics::SetMode(Title.c_str(), 800, 600, 16);
#endif
#ifdef __DJGPP__
      graphics::SetMode(0x114);
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
    {
      ++Iterator->second.Users;
      return Iterator->second;
    }
  else
    {
      bitmap* Bitmap = RawGraphic[GI.FileIndex]->Colorize(GI.BitmapPos, vector2d(16, 16), GI.Color);

      if(GI.SpecialType == STRIGHTARM)
	{
	  Bitmap->Fill(8, 0, 8, 16, 0xF81F);
	}

      if(GI.SpecialType == STLEFTARM)
	{
	  Bitmap->Fill(0, 0, 8, 16, 0xF81F);
	}

      if(GI.SpecialType == STGROIN)
	{
	  ushort Pixel[9], y, i;

	  for(y = 10, i = 0; y < 13; ++y)
	    for(ushort x = y - 5; x < 20 - y; ++x)
	      Pixel[i++] = Bitmap->GetPixel(x, y);

	  Bitmap->Fill(0, 10, 16, 6, 0xF81F);

	  for(y = 10, i = 0; y < 13; ++y)
	    for(ushort x = y - 5; x < 20 - y; ++x)
	      Bitmap->PutPixel(x, y, Pixel[i++]);
	}

      if(GI.SpecialType == STRIGHTLEG)
	{
	  Bitmap->Fill(8, 0, 8, 16, 0xF81F);
	  Bitmap->PutPixel(5, 10, 0xF81F);
	  Bitmap->PutPixel(6, 10, 0xF81F);
	  Bitmap->PutPixel(7, 10, 0xF81F);
	  Bitmap->PutPixel(6, 11, 0xF81F);
	  Bitmap->PutPixel(7, 11, 0xF81F);
	  Bitmap->PutPixel(7, 12, 0xF81F);
	}

      if(GI.SpecialType == STLEFTLEG)
	{
	  Bitmap->Fill(0, 0, 7, 16, 0xF81F);
	  Bitmap->PutPixel(7, 10, 0xF81F);
	  Bitmap->PutPixel(8, 10, 0xF81F);
	  Bitmap->PutPixel(9, 10, 0xF81F);
	  Bitmap->PutPixel(7, 11, 0xF81F);
	  Bitmap->PutPixel(8, 11, 0xF81F);
	  Bitmap->PutPixel(7, 12, 0xF81F);
	}

      tile Tile(Bitmap);
      TileMap[GI] = Tile;
      return Tile;
    }
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
