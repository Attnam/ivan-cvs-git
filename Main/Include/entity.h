#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <vector>
#include <list>

#include "typedef.h"
#include "pool.h"

class square;
class levelsquare;
class bitmap;
class material;
class outputfile;
class inputfile;

class entity
{
 public:
  entity(bool, bool);
  virtual ~entity();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual material* GetMaterial(ushort) const;
  virtual ushort GetEmitation() const;
  virtual ushort GetMaterials() const { return Material.size(); }
  virtual void Be() { }
  virtual std::list<entityinfo>::iterator GetPoolIterator() const { return PoolIterator; }
  virtual void SetPoolIterator(std::list<entityinfo>::iterator What) { PoolIterator = What; }
  virtual bool GetExists() const;
  virtual void SetExists(bool);
  virtual ulong GetID() const { return ID; }
  virtual void SetID(ulong What) { ID = What; }
  virtual void DrawToTileBuffer() const = 0;
  virtual bitmap* GetPicture() const { return Picture; }
  virtual void PreserveMaterial(ushort);
  virtual square* GetSquareUnder() const { return SquareUnder; }
  virtual void SetSquareUnder(square* What) { SquareUnder = What; }
  virtual levelsquare* GetLevelSquareUnder() const;
  virtual void SetLevelSquareUnder(levelsquare*);
 protected:
  std::vector<material*> Material;
  std::vector<bool> PreserveBit;
  std::list<entityinfo>::iterator PoolIterator;
  bool InPool;
  bitmap* Picture;
  ulong ID;
  square* SquareUnder;
};

#endif
