#ifndef __SMOKE_H__
#define __SMOKE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

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
  /* Come To The Dark Side */
  smoke* Next;
 public:
  smoke();
  smoke(gas*, lsquare*);
  virtual ~smoke();
  virtual void Be();
  virtual void Draw(bitmap*, vector2d, color24) const;
  virtual square* GetSquareUnderEntity(int = 0) const;
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  void Save(outputfile&) const;
  void Load(inputfile&);
  virtual bool IsOnGround() const { return true; }
  void AddBreatheMessage() const;
  void Merge(gas*);
  const material* GetGas() const { return Gas; }
  bool IsDangerousForAIToBreathe(const character*) const;
 protected:
  material* Gas;
  std::vector<bitmap*> Picture;
  lsquare* LSquareUnder;
  alpha Alpha;
};

outputfile& operator<<(outputfile&, const smoke*);
inputfile& operator>>(inputfile&, smoke*&);

#endif
