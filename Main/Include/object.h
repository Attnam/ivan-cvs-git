#ifndef __OBJECT_H__
#define __OBJECT_H__

#define CNAME(Case) Name(Case).c_str()

#define	UNARTICLED		0	//0000000
#define	PLURAL			1	//0000001
#define	DEFINEBIT		2	//0000010
#define	DEFINITE		2	//0000010
#define	INDEFINEBIT		4	//0000100
#define	INDEFINITE		6	//0000110

#include <string>
#include <fstream>
#include <vector>
#include <list>

#include "typedef.h"

#include "typeable.h"
#include "drawable.h"

class material;
class vector;

class object : virtual public typeable, virtual public drawable
{
public:
	object(bool);
	virtual ~object(void);
	virtual void Save(std::ofstream&) const;
	virtual void Load(std::ifstream&);
	virtual void InitMaterials(ushort, ...);
	virtual void InitMaterials(material*);
	virtual material* GetMaterial(ushort Index) const { return Material[Index]; }
	virtual ushort GetEmitation(void) const;
	virtual void SetSize(ushort Value) { Size = Value; }
	virtual ushort GetSize(void) const { return Size; }
	virtual void EraseMaterials(void);
	virtual ushort GetMaterials(void) const { return Material.size(); }
	virtual std::string Name(uchar Case) const { return NameNormal(Case, "a"); }
	virtual std::string GetNameSingular(void) const { return NameSingular(); }
	virtual std::string GetNamePlural(void) const { return NamePlural(); }
	virtual void Be(void) {}
	virtual std::list<object*>::iterator GetPoolIterator(void) { return PoolIterator; }
	virtual void SetPoolIterator(std::list<object*>::iterator What) { PoolIterator = What; }
protected:
	virtual std::string NameSingular(void) const = 0;
	virtual std::string NamePlural(void) const = 0;
	virtual std::string NameNormal(uchar, std::string) const;
	virtual std::string NameProperNoun(uchar) const;
	virtual std::string NameArtifact(uchar, uchar) const;
	virtual std::string NameWithMaterial(uchar) const;
	virtual std::string NameHandleDefaultMaterial(uchar, std::string, uchar) const;
	virtual std::string NameContainer(uchar) const;
	virtual std::string NameSized(uchar, std::string, ushort, ushort) const;
	virtual std::string NameThingsThatAreLikeLumps(uchar, std::string) const;
	std::vector<material*> Material;
	std::list<object*>::iterator PoolIterator;
	ushort Size;
	bool InPool;
};

#endif

