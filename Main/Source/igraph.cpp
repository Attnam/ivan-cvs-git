#include "igraph.h"
#include "felist.h"
#include "bitmap.h"
#include "graphics.h"
#include "save.h"
#include "config.h"
#include "colorbit.h"
#include "game.h"

colorizablebitmap* igraph::RawGraphic[RAW_TYPES];
bitmap* igraph::Graphic[GRAPHIC_TYPES + 1];
bitmap* igraph::TileBuffer;
bitmap* igraph::OutlineBuffer;
std::string igraph::RawGraphicFileName[] = { "Graphics/GLTerra.pcx", "Graphics/OLTerra.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/Humanoid.pcx" };
std::string igraph::GraphicFileName[] = { "Graphics/WTerra.pcx", "Graphics/FOW.pcx", "Graphics/Cursor.pcx", "Graphics/Symbol.pcx", "Graphics/Menu.pcx"};
tilemap igraph::TileMap;

void igraph::Init()
{
  static bool AlreadyInstalled = false;

  if(!AlreadyInstalled)
    {
      AlreadyInstalled = true;
      graphics::Init();
      graphics::SetMode("IVAN " IVAN_VERSION, (game::GetGameDir() + "Graphics/Icon.bmp").c_str(), vector2d(800, 600), configuration::GetFullScreenMode());
      DOUBLE_BUFFER->Fill(0);
      graphics::BlitDBToScreen();
      graphics::SetSwitchModeHandler(configuration::SwitchModeHandler);
      graphics::LoadDefaultFont(game::GetGameDir() + "Graphics/Font.pcx");
      FONT->CreateFontCache(RED);
      FONT->CreateFontCache(BLUE);
      FONT->CreateFontCache(WHITE);
      FONT->CreateFontCache(LIGHT_GRAY);
      felist::CreateQuickDrawFontCaches(FONT, WHITE, 8);
      felist::CreateQuickDrawFontCaches(FONT, LIGHT_GRAY, 8);

      ushort c;

      for(c = 0; c < RAW_TYPES; ++c)
	RawGraphic[c] = new colorizablebitmap(game::GetGameDir() + RawGraphicFileName[c]);

      for(c = 0; c < GRAPHIC_TYPES; ++c)
	Graphic[c] = new bitmap(game::GetGameDir() + GraphicFileName[c]);

      Graphic[GR_TRANSPARENT_COLOR_TILE] = new bitmap(16, 16, TRANSPARENT_COLOR);

      TileBuffer = new bitmap(16, 16);
      OutlineBuffer = new bitmap(16, 16);
    }
}

void igraph::DeInit()
{
  ushort c;

  for(c = 0; c < RAW_TYPES; ++c)
    delete RawGraphic[c];

  for(c = 0; c < GRAPHIC_TYPES + 1; ++c)
    delete Graphic[c];

  delete TileBuffer;
  delete OutlineBuffer;
}

void igraph::DrawCursor(vector2d Pos)
{
  igraph::GetCursorGraphic()->MaskedBlit(DOUBLE_BUFFER, 0, 0, Pos, 16, 16, configuration::GetContrastLuminance());
}

tile igraph::GetTile(graphicid GI)
{
  tilemap::iterator Iterator = TileMap.find(GI);

  if(Iterator == TileMap.end())
    ABORT("Art stolen!");

  return Iterator->second;
}

tile igraph::AddUser(graphicid GI)
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

      if((GI.SpecialFlags & 0x38) == ST_RIGHT_ARM)
	Bitmap->Fill(8, 0, 8, 16, TRANSPARENT_COLOR);

      if((GI.SpecialFlags & 0x38) == ST_LEFT_ARM)
	Bitmap->Fill(0, 0, 8, 16, TRANSPARENT_COLOR);

      if((GI.SpecialFlags & 0x38) == ST_GROIN)
	{
	  ushort Pixel[9], y, i;

	  for(y = 10, i = 0; y < 13; ++y)
	    for(ushort x = y - 5; x < 20 - y; ++x)
	      Pixel[i++] = Bitmap->GetPixel(x, y);

	  Bitmap->Fill(0, 10, 16, 6, TRANSPARENT_COLOR);

	  for(y = 10, i = 0; y < 13; ++y)
	    for(ushort x = y - 5; x < 20 - y; ++x)
	      Bitmap->PutPixel(x, y, Pixel[i++]);
	}

      if((GI.SpecialFlags & 0x38) == ST_RIGHT_LEG)
	{
	  /* Left leg from the character's, NOT the player's point of view */

	  Bitmap->Fill(8, 0, 8, 16, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(5, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(6, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(6, 11, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 11, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 12, TRANSPARENT_COLOR);
	}

      if((GI.SpecialFlags & 0x38) == ST_LEFT_LEG)
	{
	  /* Right leg from the character's, NOT the player's point of view */

	  Bitmap->Fill(0, 0, 7, 16, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(8, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(9, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 11, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(8, 11, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 12, TRANSPARENT_COLOR);
	}

      if(GI.OutlineColor != TRANSPARENT_COLOR)
	Bitmap->Outline(GI.OutlineColor);

      if(GI.SparklePos != ERROR_VECTOR)
	Bitmap->CreateSparkle(GI.SparklePos, GI.SparkleFrame);

      if(GI.FlyAmount)
	Bitmap->CreateFlies(GI.Seed, GI.Frame, GI.FlyAmount);

      if(GI.SpecialFlags & 0x7) /* Do we need rotating/flipping? */
	{
	  bitmap* Temp = new bitmap(Bitmap, GI.SpecialFlags);
	  delete Bitmap;
	  Bitmap = Temp;
	}

      if(GI.SpecialFlags & ST_FLAME)
	Bitmap->CreateFlames(GI.Frame);

      if(GI.SpecialFlags & ST_LIGHTNING && !((GI.Frame + 1) & 7))
	Bitmap->CreateLightning(GI.Seed + GI.Frame, WHITE);

      tile Tile(Bitmap);
      TileMap.insert(std::pair<graphicid, tile>(GI, Tile));
      return Tile;
    }
}

void igraph::RemoveUser(graphicid GI)
{
  tilemap::iterator Iterator = TileMap.find(GI);

  if(Iterator != TileMap.end())
    if(!--Iterator->second.Users)
      {
	delete Iterator->second.Bitmap;
	TileMap.erase(Iterator);
      }
}

outputfile& operator<<(outputfile& SaveFile, const graphicid& Value)
{
  SaveFile.Write(reinterpret_cast<const char*>(&Value), sizeof(Value));
  return SaveFile;
}


inputfile& operator>>(inputfile& SaveFile, graphicid& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}
