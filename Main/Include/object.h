#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define CHARNAME(Case) Name(Case).c_str()

#include <string>

#include "typedef.h"
#include "vector2d.h"

#include "igraph.h"
#include "type.h"
#include "unit.h"
#include "id.h"

class material;
class outputfile;
class inputfile;
class bitmap;
class lsquare;
class square;

class object : public type, public unit, public id
{
 public:
  object(bool, bool);
  virtual ~object();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void InitMaterials(ushort, ...);
  virtual void InitMaterials(material*);
  virtual void SetMaterial(uchar, material*);
  virtual void ChangeMaterial(uchar, material*);
  virtual void UpdatePicture(bool = true);
  virtual ulong GetDefaultVolume(ushort Index) const = 0;
  virtual uchar GetMainMaterialIndex() const { return 0; }
  virtual material* GetMainMaterial() const { return GetMaterial(GetMainMaterialIndex()); }
  virtual uchar GetContainedMaterialIndex() const { return 1; }
  virtual material* GetContainedMaterial() const { return GetMaterial(GetContainedMaterialIndex()); }
  virtual uchar GetConsumeMaterialIndex() const { return 0; }
  virtual material* GetConsumeMaterial() const { return GetMaterial(GetConsumeMaterialIndex()); }
 protected:
  virtual uchar GetSpecialType() const { return STNORMAL; }
  virtual uchar GetGraphicsContainerIndex() const = 0;
  virtual ushort GetMaterialColor0() const;
  virtual ushort GetMaterialColor1() const;
  virtual ushort GetMaterialColor2() const;
  virtual ushort GetMaterialColor3() const;
  virtual std::string MaterialDescription(bool) const;
  virtual std::string ContainerPostFix() const;
  virtual std::string LumpyPostFix() const;
  virtual vector2d GetBitmapPos() const = 0;
  graphic_id GraphicId;
};

#endif


