#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
#include <allegro.h>
#include <string>
class rectangle;
class vector2d2d2d;
class graphics
{
 public:
  static void Init();
  static void DeInit();
  static void Load(char*);
  static void BlitDoubleBuffer();
  static void DrawFilledRectangle(rectangle, unsigned short);
  static void DrawRectangle(rectangle, unsigned short);
  static void Clear() { clear(DoubleBuffer); }
  static void DrawCursor(vector2d2d2d);
  static void DrawTool(vector2d2d2d, vector2d2d2d, vector2d2d2d);
  static void DrawStretchData(rectangle, rectangle);
  static void DrawText(std::string, vector2d2d2d);
 private:
  static BITMAP* DoubleBuffer;
  static BITMAP* ToolGraphics;
  static BITMAP*  Data;
  static PALETTE Palette;
  static PALETTE DataPalette;
};

#endif

