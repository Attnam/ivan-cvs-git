#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <fstream>
#include <cstdarg>
#include <vector>

#include "typedef.h"

#include "material.h"
#include "independ.h"

class material;

class object : public independency
{
public:
	object(void) {}
	~object(void);
	virtual void Save(std::ofstream*) const;
	virtual void Load(std::ifstream*);
	virtual void InitMaterials(ushort, ...);
	virtual void InitMaterials(material*);
	virtual material* CMaterial(ushort Index) const { return Material[Index]; }
	virtual ushort CEmitation(void) const;
	virtual void SSize(ushort Value) { Size = Value; }
	virtual ushort CSize(void) const { return Size; }
	virtual void EraseMaterials(void);
	virtual ushort CMaterials(void) const { return Material.size(); }
protected:
	virtual std::string NameArtifact(uchar, uchar) const;
	virtual std::string NameWithMaterial(uchar) const;
	virtual std::string NameHandleDefaultMaterial(uchar, std::string, uchar) const;
	virtual std::string NameContainer(uchar) const;
	virtual std::string NameSized(uchar, std::string, ushort, ushort) const;
	virtual std::string NameThingsThatAreLikeLumps(uchar, std::string) const;
	std::vector<material*> Material;
	ushort Size;
};

#endif
