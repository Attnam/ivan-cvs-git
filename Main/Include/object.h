#ifndef __OBJECT_H__
#define __OBJECT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define CNAME(Case) Name(Case).c_str()

#define	UNARTICLED		0	//0000000
#define	PLURAL			1	//0000001
#define	DEFINEBIT		2	//0000010
#define	DEFINITE		2	//0000010
#define	INDEFINEBIT		4	//0000100
#define	INDEFINITE		6	//0000110

#include <string>

#include "typedef.h"
#include "vector2d.h"

#include "igraph.h"
#include "typeable.h"
#include "entity.h"

class material;
class outputfile;
class inputfile;
class bitmap;
class levelsquare;
class square;

class object : public typeable, public entity
{
 public:
  object(bool, bool);
  virtual ~object();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void InitMaterials(ushort, ...);
  virtual void InitMaterials(material*);
  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
  virtual std::string GetNameSingular() const { return NameSingular(); }
  virtual std::string GetNamePlural() const { return NamePlural(); }
  virtual uchar GetGraphicsContainerIndex() const = 0;
  virtual void SetMaterial(uchar, material*);
  virtual void ChangeMaterial(uchar, material*);
  virtual void UpdatePicture();
  virtual ulong GetDefaultVolume(ushort Index) const = 0;
  virtual void ColorChangeSpeciality(uchar, bool) { }
 protected:
  virtual std::string NameSingular() const = 0;
  virtual std::string NamePlural() const { return NameSingular() + "s"; }
  virtual std::string NameNormal(uchar, std::string, std::string = "") const;
  virtual std::string NameProperNoun(uchar) const;
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
