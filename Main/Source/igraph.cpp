#include "bitmap.h"
#include "graphics.h"
#include "igraph.h"
#include "game.h"
#include "error.h"
#include "colorbit.h"
#include "config.h"
#include "save.h"

#ifdef WIN32
#include "resource.h"
#endif

colorizablebitmap* igraph::RawGraphic[RAW_TYPES];
bitmap* igraph::Graphic[GRAPHIC_TYPES];
bitmap* igraph::TileBuffer;
bitmap* igraph::OutlineBuffer;
std::string igraph::RawGraphicFileName[] = { "Graphics/GLTerrain.pcx", "Graphics/OLTerrain.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/Humanoid.pcx" };
std::string igraph::GraphicFileName[] = { "Graphics/WTerrain.pcx", "Graphics/FOW.pcx", "Graphics/Cursor.pcx", "Graphics/Symbol.pcx", "Graphics/Menu.pcx"};
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
      graphics::SetMode(hInst, hWnd, Title.c_str(), vector2d(800, 600), 16, configuration::GetFullScreenMode(), MAKEINTRESOURCE(IDI_LOGO));
#endif
#ifdef USE_SDL
      graphics::SetMode(Title.c_str(), vector2d(800, 600), 16);
#endif
#ifdef __DJGPP__
      graphics::SetMode(0x114);
#endif
      DOUBLEBUFFER->Fill(0);
      graphics::BlitDBToScreen();
#ifdef WIN32
      graphics::SetSwitchModeHandler(configuration::SwitchModeHandler);
#endif
      graphics::LoadDefaultFont(GAME_DIR + "Graphics/Font.pcx");

      ushort c;

      for(c = 0; c < RAW_TYPES; ++c)
	RawGraphic[c] = new colorizablebitmap(GAME_DIR + RawGraphicFileName[c]);

      for(c = 0; c < GRAPHIC_TYPES; ++c)
	Graphic[c] = new bitmap(GAME_DIR + GraphicFileName[c]);

      TileBuffer = new bitmap(16, 16);
      OutlineBuffer = new bitmap(16, 16);
    }
}

void igraph::DeInit()
{
  ushort c;

  for(c = 0; c < RAW_TYPES; ++c)
    delete RawGraphic[c];

  for(c = 0; c < GRAPHIC_TYPES; ++c)
    delete Graphic[c];

  delete TileBuffer;
  delete OutlineBuffer;
}

void igraph::DrawCursor(vector2d Pos)
{
  igraph::GetCursorGraphic()->MaskedBlit(DOUBLEBUFFER, 0, 0, Pos, 16, 16, ushort((configuration::GetContrast() << 8) / 100));
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
      bitmap* Bitmap = RawGraphic[GI.FileIndex]->Colorize(GI.BitmapPos, vector2d(16, 16), GI.Color, GI.BaseAlpha, GI.Alpha);

      if(GI.SpecialFlags & 0x7)
	{
	  bitmap* Temp = new bitmap(Bitmap, GI.SpecialFlags);
	  delete Bitmap;
	  Bitmap = Temp;
	}

      if(GI.SpecialFlags == STRIGHTARM)
	{
	  Bitmap->Fill(8, 0, 8, 16, TRANSPARENTCOL);
	}

      if(GI.SpecialFlags == STLEFTARM)
	{
	  Bitmap->Fill(0, 0, 8, 16, TRANSPARENTCOL);
	}

      if(GI.SpecialFlags == STGROIN)
	{
	  ushort Pixel[9], y, i;

	  for(y = 10, i = 0; y < 13; ++y)
	    for(ushort x = y - 5; x < 20 - y; ++x)
	      Pixel[i++] = Bitmap->GetPixel(x, y);

	  Bitmap->Fill(0, 10, 16, 6, TRANSPARENTCOL);

	  for(y = 10, i = 0; y < 13; ++y)
	    for(ushort x = y - 5; x < 20 - y; ++x)
	      Bitmap->PutPixel(x, y, Pixel[i++]);
	}

      if(GI.SpecialFlags == STRIGHTLEG)
	{
	  /* Right leg from the character's, NOT the player's point of view */

	  Bitmap->Fill(0, 0, 7, 16, TRANSPARENTCOL);
	  Bitmap->PutPixel(7, 10, TRANSPARENTCOL);
	  Bitmap->PutPixel(8, 10, TRANSPARENTCOL);
	  Bitmap->PutPixel(9, 10, TRANSPARENTCOL);
	  Bitmap->PutPixel(7, 11, TRANSPARENTCOL);
	  Bitmap->PutPixel(8, 11, TRANSPARENTCOL);
	  Bitmap->PutPixel(7, 12, TRANSPARENTCOL);
	}

      if(GI.SpecialFlags == STLEFTLEG)
	{
	  /* Left leg from the character's, NOT the player's point of view */

	  Bitmap->Fill(8, 0, 8, 16, TRANSPARENTCOL);
	  Bitmap->PutPixel(5, 10, TRANSPARENTCOL);
	  Bitmap->PutPixel(6, 10, TRANSPARENTCOL);
	  Bitmap->PutPixel(7, 10, TRANSPARENTCOL);
	  Bitmap->PutPixel(6, 11, TRANSPARENTCOL);
	  Bitmap->PutPixel(7, 11, TRANSPARENTCOL);
	  Bitmap->PutPixel(7, 12, TRANSPARENTCOL);
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

outputfile& operator<<(outputfile& SaveFile, const graphic_id& GI)
{
  SaveFile << GI.BitmapPos << GI.FileIndex << GI.SpecialFlags << GI.BaseAlpha;
  SaveFile << GI.Color[0] << GI.Color[1] << GI.Color[2] << GI.Color[3];
  SaveFile << GI.Alpha[0] << GI.Alpha[1] << GI.Alpha[2] << GI.Alpha[3];
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, graphic_id& GI)
{
  SaveFile >> GI.BitmapPos >> GI.FileIndex >> GI.SpecialFlags >> GI.BaseAlpha;
  SaveFile >> GI.Color[0] >> GI.Color[1] >> GI.Color[2] >> GI.Color[3];
  SaveFile >> GI.Alpha[0] >> GI.Alpha[1] >> GI.Alpha[2] >> GI.Alpha[3];
  return SaveFile;
}

