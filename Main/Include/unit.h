#ifndef __UNIT_H__
#define __UNIT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "entity.h"

class bitmap;
class material;

class unit : public entity
{
 public:
  unit(bool, bool);
  virtual ~unit();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual material* GetMaterial(ushort) const;
  virtual ushort GetEmitation() const;
  virtual ushort GetMaterials() const;
  virtual bitmap* GetPicture() const { return Picture; }
  virtual void PreserveMaterial(ushort);
 protected:
  std::vector<material*> Material;
  std::vector<bool> PreserveBit;
  bitmap* Picture;
};

#endif
