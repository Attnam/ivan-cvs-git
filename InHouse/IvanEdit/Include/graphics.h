#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__
#include <allegro.h>
#include <string>
class rectangle;
class vector2d;
class graphics
{
 public:
  static void Init(void);
  static void DeInit(void);
  static void Load(char*);
  static void BlitDoubleBuffer(void);
  static void DrawFilledRectangle(rectangle, unsigned short);
  static void DrawRectangle(rectangle, unsigned short);
  static void Clear(void) { clear(DoubleBuffer); }
  static void DrawCursor(vector2d);
  static void DrawTool(vector2d, vector2d, vector2d);
  static void DrawStretchData(rectangle, rectangle);
  static void DrawText(std::string, vector2d);
 private:
  static BITMAP* DoubleBuffer;
  static BITMAP* ToolGraphics;
  static BITMAP*  Data;
  static PALETTE Palette;
  static PALETTE DataPalette;
};

#endif
