#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"
#include "vector2d.h"

#include "igraph.h"
#include "type.h"
#include "unit.h"

class material;
class outputfile;
class inputfile;
class bitmap;
class lsquare;
class square;

class object : public type, public unit
{
 public:
  object(bool, bool);
  virtual ~object();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void InitMaterials(ushort, ...);
  virtual void InitMaterials(material*);
  virtual uchar GetGraphicsContainerIndex() const = 0;
  virtual void SetMaterial(uchar, material*);
  virtual void ChangeMaterial(uchar, material*);
  virtual void UpdatePicture();
  virtual ulong GetDefaultVolume(ushort Index) const = 0;
  virtual void ColorChangeSpeciality(uchar, bool) { }
 protected:
  virtual std::string NameArtifact(uchar, uchar) const;
  virtual std::string NameWithMaterial(uchar, uchar = 0) const;
  virtual std::string NameHandleDefaultMaterial(uchar, std::string, uchar) const;
  virtual std::string NameContainer(uchar) const;
  virtual std::string NameThingsThatAreLikeLumps(uchar, std::string) const;
  virtual std::string OwnerGodDescription(uchar) const;
  virtual vector2d GetBitmapPos() const = 0;
  graphic_id GraphicId;
};

#endif


