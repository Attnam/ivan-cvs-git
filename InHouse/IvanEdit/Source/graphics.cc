#include <allegro.h>
#include "graphics.h"
#include "rect.h"
#include "vector.h"
BITMAP* graphics::DoubleBuffer = 0;
BITMAP* graphics::ToolGraphics = 0;
BITMAP* graphics::Data = 0;
PALETTE graphics::Palette;
PALETTE graphics::DataPalette;

void graphics::Init(void)
{
 allegro_init();
 set_color_depth(16);
 set_gfx_mode(GFX_AUTODETECT, 800, 600, 0,0);
 DoubleBuffer = create_bitmap(800,600);
 ToolGraphics = load_pcx("tools.pcx", Palette);
 Data = load_pcx("item.pcx", Palette);
 MaterialBuffer = create_bitmap_ex(8, Data->w, Data->h);
 UpdateMaterial(makecol(255,0,0));
}

void graphics::DeInit(void)
{
 destroy_bitmap(DoubleBuffer);
}

void graphics::BlitDoubleBuffer(void)
{
 blit(DoubleBuffer, screen, 0,0,0,0, SCREEN_W, SCREEN_H);
}

void graphics::DrawFilledRectangle(rectangle Rect, unsigned short Color)
{
 rectfill(DoubleBuffer, Rect.Left, Rect.Top, Rect.Right, Rect.Bottom, Color);
}

void graphics::DrawRectangle(rectangle Rect, unsigned short Color)
{
 rect(DoubleBuffer, Rect.Left, Rect.Top, Rect.Right, Rect.Bottom, Color);
}

void graphics::DrawCursor(vector2d Pos)
{
// masked_blit(ToolGraphics, DoubleBuffer, 0,0, Pos.X, Pos.Y, 16,16);
 DrawTool(Pos, vector2d(0,0), vector2d(16,16));
}

void graphics::DrawTool(vector2d ToPos, vector2d FromPos, vector2d Size)
{
 masked_blit(ToolGraphics, DoubleBuffer, FromPos.X, FromPos.Y, ToPos.X, ToPos.Y, Size.X, Size.Y);
}

void graphics::Load(char* FileName)
{
 Data = load_pcx(FileName, DataPalette);
}

void graphics::DrawStretchData(rectangle From, rectangle To)
{
 stretch_blit(Data, DoubleBuffer,
 From.Left, From.Top, From.Right - From.Left, From.Bottom - From.Top,
 To.Left, To.Top, To.Right - To.Left, To.Bottom - To.Top);
}

void graphics::DrawText(std::string Text, vector2d Where)
{
 textout(DoubleBuffer, font, (char*)(Text.c_str()), Where.X, Where.Y, makecol(255,0,0));
}

void graphics::DrawStretched(rectangle To, BITMAP* From)
{
 stretch_blit(From, DoubleBuffer,
 0, 0, From->w, From->h,
 To.Left, To.Top, To.Right - To.Left, To.Bottom - To.Top);
}

void graphics::BlitData(rectangle From, rectangle To)
{
 blit(Data, DoubleBuffer, From.Left, From.Top, To.Left, To.Top, To.Right - To.Left, To.Bottom - To.Top);
}

void graphics::BlitToolTo(rectangle Pos, BITMAP* Where)
{
 blit(ToolGraphics, Where, Pos.Left, Pos.Top, 0, 0, Pos.Right - Pos.Left, Pos.Bottom - Pos.Top);
}

void graphics::DrawPixelOnData(vector2d What, unsigned short Color)
{
 putpixel(Data, What.X, What.Y, Color);
}

void graphics::UpdateMaterial(unsigned short Color1, unsigned short Color2, unsigned short Color3, unsigned short Color4)
{
 unsigned short Color = { Color1, Color2, Color3, Color4 };
 for(int x = 0; x < Data->w; x++)
  for(int y = 0; y < Data->h; y++)
  {
   if((unsigned short PixelColor = getpixel(Data, x,y)) < 128)
   {
    putpixel(MaterialBuffer, x,y, (PixelColor % 32) * Color[PixelColor / 32]);
   }


  }
}
