#include "igraph.h"
#include "felist.h"
#include "bitmap.h"
#include "graphics.h"
#include "iconf.h"
#include "colorbit.h"
#include "game.h"
#include "save.h"
#include "object.h"

colorizablebitmap* igraph::RawGraphic[RAW_TYPES];
bitmap* igraph::Graphic[GRAPHIC_TYPES + 1];
bitmap* igraph::TileBuffer;
bitmap* igraph::FlagBuffer;
const char* igraph::RawGraphicFileName[] = { "Graphics/GLTerra.pcx", "Graphics/OLTerra.pcx", "Graphics/Item.pcx", "Graphics/Char.pcx", "Graphics/Humanoid.pcx", "Graphics/Effect.pcx" };
const char* igraph::GraphicFileName[] = { "Graphics/WTerra.pcx", "Graphics/FOW.pcx", "Graphics/Cursor.pcx", "Graphics/Symbol.pcx", "Graphics/Menu.pcx" };
tilemap igraph::TileMap;
uchar igraph::RollBuffer[256];

void igraph::Init()
{
  static bool AlreadyInstalled = false;

  if(!AlreadyInstalled)
    {
      AlreadyInstalled = true;
      graphics::Init();
      graphics::SetMode("IVAN " IVAN_VERSION, festring(game::GetGameDir() + "Graphics/Icon.bmp").CStr(), 800, 600, ivanconfig::GetFullScreenMode());
      DOUBLE_BUFFER->ClearToColor(0);
      graphics::BlitDBToScreen();
      graphics::SetSwitchModeHandler(ivanconfig::SwitchModeHandler);
      graphics::LoadDefaultFont(game::GetGameDir() + "Graphics/Font.pcx");
      FONT->CreateFontCache(RED);
      FONT->CreateFontCache(BLUE);
      FONT->CreateFontCache(WHITE);
      FONT->CreateFontCache(LIGHT_GRAY);
      felist::CreateQuickDrawFontCaches(FONT, WHITE, 8);
      felist::CreateQuickDrawFontCaches(FONT, LIGHT_GRAY, 8);
      object::InitSparkleValidityArrays();

      ushort c;

      for(c = 0; c < RAW_TYPES; ++c)
	RawGraphic[c] = new colorizablebitmap(game::GetGameDir() + RawGraphicFileName[c]);

      for(c = 0; c < GRAPHIC_TYPES; ++c)
	Graphic[c] = new bitmap(game::GetGameDir() + GraphicFileName[c]);

      Graphic[GR_TRANSPARENT_COLOR_TILE] = new bitmap(16, 16, TRANSPARENT_COLOR);

      TileBuffer = new bitmap(16, 16);
      TileBuffer->CreatePriorityMap(0);
      FlagBuffer = new bitmap(16, 16);
      FlagBuffer->CreateAlphaMap(0);
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
  delete FlagBuffer;
}

void igraph::DrawCursor(vector2d Pos)
{
  igraph::GetCursorGraphic()->MaskedBlit(DOUBLE_BUFFER, 0, 0, Pos, 16, 16, ivanconfig::GetContrastLuminance());
}

tilemap::iterator igraph::AddUser(const graphicid& GI)
{
  tilemap::iterator Iterator = TileMap.find(GI);

  if(Iterator != TileMap.end())
    {
      tile& Tile = Iterator->second;
      ++Tile.Users;
      return Iterator;
    }
  else
    {
      bitmap* Bitmap = RawGraphic[GI.FileIndex]->Colorize(GI.BitmapPos, vector2d(16, 16), GI.Position, GI.Color, GI.BaseAlpha, GI.Alpha);
      const uchar SpecialFlags = GI.SpecialFlags;

      if((SpecialFlags & 0x38) == ST_RIGHT_ARM)
	Bitmap->Fill(8, 0, 8, 16, TRANSPARENT_COLOR);
      else if((SpecialFlags & 0x38) == ST_LEFT_ARM)
	Bitmap->Fill(0, 0, 8, 16, TRANSPARENT_COLOR);
      else if((SpecialFlags & 0x38) == ST_GROIN)
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
      else if((SpecialFlags & 0x38) == ST_RIGHT_LEG)
	{
	  /* Right leg from the character's, NOT the player's point of view */

	  Bitmap->Fill(8, 0, 8, 16, TRANSPARENT_COLOR);
	  Bitmap->Fill(0, 0, 8, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(5, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(6, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(6, 11, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 11, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(7, 12, TRANSPARENT_COLOR);
	}
      else if((SpecialFlags & 0x38) == ST_LEFT_LEG)
	{
	  /* Left leg from the character's, NOT the player's point of view */

	  Bitmap->Fill(0, 0, 8, 16, TRANSPARENT_COLOR);
	  Bitmap->Fill(8, 0, 8, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(8, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(9, 10, TRANSPARENT_COLOR);
	  Bitmap->PutPixel(8, 11, TRANSPARENT_COLOR);
	}

      if(GI.OutlineColor != TRANSPARENT_COLOR)
	Bitmap->Outline(GI.OutlineColor, GI.OutlineAlpha);

      vector2d SparklePos = GI.SparklePos;

      if(SpecialFlags & 0x7) /* Do we need rotating/flipping? */
	{
	  if(Bitmap->GetAlphaMap())
	    {
	      Bitmap->BlitAndCopyAlpha(FlagBuffer, SpecialFlags);
	      FlagBuffer->FastBlitAndCopyAlpha(Bitmap);
	    }
	  else
	    {
	      Bitmap->Blit(FlagBuffer, SpecialFlags);
	      FlagBuffer->FastBlit(Bitmap);
	    }

	  if(SparklePos != ERROR_VECTOR)
	    {
	      if(SpecialFlags & ROTATE)
		{
		  const short T = SparklePos.X;
		  SparklePos.X = 15 - SparklePos.Y;
		  SparklePos.Y = T;
		}

	      if(SpecialFlags & MIRROR)
		SparklePos.X = 15 - SparklePos.X;

	      if(SpecialFlags & FLIP)
		SparklePos.Y = 15 - SparklePos.Y;
	    }
	}

      if(SpecialFlags & 0x38)
	Bitmap->CreatePriorityMap(0);

      if(SparklePos != ERROR_VECTOR)
	Bitmap->CreateSparkle(SparklePos + GI.Position, GI.SparkleFrame);

      if(GI.FlyAmount)
	Bitmap->CreateFlies(GI.Seed, GI.Frame, GI.FlyAmount);

      if(SpecialFlags & ST_FLAME)
	Bitmap->CreateFlames(GI.Frame);

      if(SpecialFlags & ST_LIGHTNING && !((GI.Frame + 1) & 7))
	Bitmap->CreateLightning(GI.Seed + GI.Frame, WHITE);

      return TileMap.insert(std::pair<graphicid, tile>(GI, tile(Bitmap))).first;
    }
}

void igraph::RemoveUser(tilemap::iterator Iterator)
{
  tile& Tile = Iterator->second;

  if(!--Tile.Users)
    {
      delete Tile.Bitmap;
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
