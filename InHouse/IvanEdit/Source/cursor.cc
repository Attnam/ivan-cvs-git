#include <allegro.h>
#include "cursor.h"
#include "graphics.h"
#include "vector.h"

void cursor::Draw(void)
{
 if(Show) graphics::DrawCursor(vector2d(mouse_x, mouse_y));
}