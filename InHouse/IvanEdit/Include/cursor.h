#include "vector2d2d.h"
class vector2d2d2d;
class cursor
{
 public:
  void Draw();
  void SetPos(vector2d2d2d What) { Pos = What; }
  void SetShow(bool What) { Show = What; }
 protected:
  vector2d2d2d Pos;
  bool Show;
};




