#ifndef __OBJECT_H__
#define __OBJECT_H__

#pragma warning(disable : 4786)

#define CNAME(Case) Name(Case).c_str()

#define	UNARTICLED		0	//0000000
#define	PLURAL			1	//0000001
#define	DEFINEBIT		2	//0000010
#define	DEFINITE		2	//0000010
#define	INDEFINEBIT		4	//0000100
#define	INDEFINITE		6	//0000110

#include <string>
#include <vector>
#include <list>

#include "typedef.h"
#include "vector2d.h"

#include "igraph.h"
#include "typeable.h"

class material;
class outputfile;
class inputfile;
class bitmap;
class levelsquare;
class square;

class object : public typeable
{
public:
	object(bool);
	virtual ~object();
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void InitMaterials(ushort, ...);
	virtual void InitMaterials(material*);
	virtual material* GetMaterial(ushort) const;
	virtual ushort GetEmitation() const;
	virtual void SetSize(ushort Value) { Size = Value; }
	virtual ushort GetSize() const { return Size; }
	virtual void EraseMaterials();
	virtual ushort GetMaterials() const { return Material.size(); }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual std::string GetNameSingular() const { return NameSingular(); }
	virtual std::string GetNamePlural() const { return NamePlural(); }
	virtual void Be() {}
	virtual std::list<object*>::iterator GetPoolIterator() { return PoolIterator; }
	virtual void SetPoolIterator(std::list<object*>::iterator What) { PoolIterator = What; }
	virtual bool GetExists() const { return Exists; }
	virtual void SetExists(bool What) { Exists = What; }
	virtual uchar GetGraphicsContainerIndex() const = 0;
	virtual void SetMaterial(uchar, material*);
	virtual void ChangeMaterial(uchar, material*);
	virtual void UpdatePicture();
	virtual ulong GetID() const { return ID; }
	virtual void SetID(ulong What) { ID = What; }
	virtual void DrawToTileBuffer() const = 0;
	virtual bitmap* GetPicture() const { return Picture; }
	virtual ulong GetDefaultVolume(ushort Index) const = 0;
	virtual void PreserveMaterial(ushort);
	virtual square* GetSquareUnder() const { return SquareUnder; }
	virtual void SetSquareUnder(square* What) { SquareUnder = What; }
	virtual levelsquare* GetLevelSquareUnder() const;
	virtual void SetLevelSquareUnder(levelsquare*);
	virtual void ColorChangeSpeciality(uchar, bool) {}
protected:
	virtual std::string NameSingular() const = 0;
	virtual std::string NamePlural() const { return NameSingular() + "s"; }
	virtual std::string NameNormal(uchar, std::string) const;
	virtual std::string NameProperNoun(uchar) const;
	virtual std::string NameArtifact(uchar, uchar) const;
	virtual std::string NameWithMaterial(uchar) const;
	virtual std::string NameHandleDefaultMaterial(uchar, std::string, uchar) const;
	virtual std::string NameContainer(uchar) const;
	virtual std::string NameSized(uchar, std::string, ushort, ushort) const;
	virtual std::string NameThingsThatAreLikeLumps(uchar, std::string) const;
	virtual std::string OwnerGodDescription(uchar) const;
	virtual vector2d GetBitmapPos() const = 0;
	std::vector<material*> Material;
	std::vector<bool> PreserveBit;
	std::list<object*>::iterator PoolIterator;
	ushort Size;
	bool InPool, Exists;
	bitmap* Picture;
	graphic_id GraphicId;
	ulong ID;
	square* SquareUnder;
};

#endif
