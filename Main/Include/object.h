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
	object(std::ifstream*);
	~object(void);
	virtual void Save(std::ofstream*);
	virtual void InitMaterials(ushort, ...);
	virtual void InitMaterials(material*);
	virtual material* CMaterial(ushort Index) { return Material[Index]; }
	virtual ushort CEmitation(void);
	virtual ushort CSize(void) { return Size; }
	virtual void EraseMaterials(void);
	virtual ushort CMaterials(void) { return Material.size(); }
protected:
	virtual std::string NameArtifact(uchar, uchar);
	virtual std::string NameWithMaterial(uchar);
	virtual std::string NameHandleDefaultMaterial(uchar, std::string, uchar);
	virtual std::string NameContainer(uchar);
	virtual std::string NameSized(uchar, std::string, ushort, ushort);
	virtual std::string NameThingsThatAreLikeLumps(uchar, std::string);
	std::vector<material*> Material;
	ushort Size;
};

#endif
