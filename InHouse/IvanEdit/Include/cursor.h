#include "vector.h"
class vector2d;
class cursor
{
 public:
  void Draw(void);
  void SetPos(vector2d What) { Pos = What; }
  void SetShow(bool What) { Show = What; }
 protected:
  vector2d Pos;
  bool Show;
};
