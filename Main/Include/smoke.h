#ifndef __SMOKE_H__
#define __SMOKE_H__

#include <vector>

#include "entity.h"
#include "vector2d.h"

class gas;
class lsquare;
class bitmap;
class inputfile;
class outputfile;

class smoke : public entity
{
 public:
  smoke(gas*, lsquare*);
  smoke();
  virtual ~smoke();
  virtual void Be();
  virtual void Draw(bitmap*, vector2d, ulong, bool) const;
  virtual square* GetSquareUnderEntity() const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  void Save(outputfile&) const;
  void Load(inputfile&);
  virtual bool IsOnGround() const { return true; }
  void AddBreatheMessage() const;
  void Merge(gas*);
  const material* GetGas() const { return Gas; }
 protected:
  material* Gas;
  std::vector<bitmap*> Picture;
  lsquare* LSquareUnder;
};

outputfile& operator<<(outputfile&, const smoke*);
inputfile& operator>>(inputfile&, smoke*&);

#endif
