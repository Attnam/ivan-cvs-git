#ifndef __ROOMBA_H__
#define __ROOMBA_H__

#include <vector>

#include "vector2d.h"
#include "typeable.h"

class outputfile;
class inputfile;
class character;
class overlevelterrain;
class item;

class room : public typeable
{
public:
	room() : Master(0) {}
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void Enter(character*) {}
	virtual room* Clone() const = 0;
	virtual vector2d GetPos() const { return Pos; }
	virtual void SetPos(vector2d What) { Pos = What; }
	virtual vector2d GetSize() const { return Size; }
	virtual void SetSize(vector2d What) { Size = What; }
	virtual void HandleInstantiatedCharacter(character*) {}
	virtual void HandleInstantiatedOverLevelTerrain(overlevelterrain*);
	virtual void SetIndex(uchar What) { Index = What; }
	virtual uchar GetIndex() const { return Index; }
	virtual character* GetMaster() const { return Master; }
	virtual void SetMaster(character* What) { Master = What; }
	virtual bool PickupItem(character*, item*) { return true; }
	virtual bool DropItem(character*, item*) { return true; }
protected:
	std::vector<vector2d> Door;
	vector2d Pos, Size;
	character* Master;
	uchar Index;
};

#ifdef __FILE_OF_STATIC_PROTOTYPE_DECLARATIONS__

	#define ROOM_PROTOINSTALLER(name, base)\
	\
	class name##_protoinstaller\
	{\
	public:\
		name##_protoinstaller() : Index(protocontainer<room>::Add(new name)) { }\
		ushort GetIndex() const { return Index; }\
	private:\
		ushort Index;\
	} static name##_ProtoInstaller;\
	\
	ushort name::StaticType() { return name##_ProtoInstaller.GetIndex(); }\
	const room* const name::GetPrototype() { return protocontainer<room>::GetProto(StaticType()); }\
	ushort name::Type() const { return name##_ProtoInstaller.GetIndex(); }

#else

	#define ROOM_PROTOINSTALLER(name, base)

#endif

#define ROOM(name, base, data)\
\
name : public base\
{\
public:\
	virtual room* Clone() const { return new name; }\
	virtual typeable* CloneAndLoad(inputfile& SaveFile) const { room* Room = new name; Room->Load(SaveFile); return Room; }\
	static ushort StaticType();\
	static const room* const GetPrototype();\
	virtual std::string ClassName() const { return #name; }\
protected:\
	virtual ushort Type() const;\
	data\
}; ROOM_PROTOINSTALLER(name, base)

#endif
