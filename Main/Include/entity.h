#ifndef __ENTITY_H__
#define __ENTITY_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>
#include <string>

#include "typedef.h"
#include "pool.h"

class square;
class lsquare;
class outputfile;
class inputfile;

class entity
{
 public:
  entity();
  virtual ~entity();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Be() { }
  virtual std::list<entityinfo>::iterator GetPoolIterator() const { return PoolIterator; }
  virtual void SetPoolIterator(std::list<entityinfo>::iterator What) { PoolIterator = What; }
  virtual bool GetExists() const;
  virtual void SetExists(bool);
  virtual void DrawToTileBuffer() const = 0;
  virtual square* GetSquareUnder() const { return SquareUnder; }
  virtual void SetSquareUnder(square* What) { SquareUnder = What; }
  virtual lsquare* GetLSquareUnder() const;
  virtual void SetLSquareUnder(lsquare*);
  virtual bool GetHasBe() const;
  virtual void SetHasBe(bool);
 protected:
  std::list<entityinfo>::iterator PoolIterator;
  bool InPool;
  square* SquareUnder;
};

#endif

