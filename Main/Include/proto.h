#ifndef __PROTO_H__
#define __PROTO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>
#include <map>
#include <string>

#include "typedef.h"

template <class type> class protocontainer
{
 public:
  typedef typename type::prototype prototype;
  static ushort Add(prototype*);
  static const prototype* const GetProto(ushort Index) { return ProtoData[Index]; }
  static ushort GetProtoAmount() { return ProtoData.size() - 2; }
  static ushort SearchCodeName(std::string);
 private:
  static std::vector<prototype*> ProtoData;
  static std::map<std::string, ushort> CodeNameMap;
};

template <class type> ushort protocontainer<type>::Add(prototype* Proto)
{
  static bool Initialized = false;

  if(!Initialized)
    {
      ProtoData.resize(2, 0);
      Initialized = true;
    }

  ProtoData.insert(ProtoData.end() - 1, Proto);
  CodeNameMap[Proto->ClassName()] = ProtoData.size() - 2;
  return ProtoData.size() - 2;
}

template <class templatetype> ushort protocontainer<templatetype>::SearchCodeName(std::string Name)
{
  std::map<std::string, ushort>::iterator I = CodeNameMap.find(Name);

  if(I != CodeNameMap.end())
    return I->second;
  else
    return 0;
}

class character;
class item;
class material;
class outputfile;
class inputfile;

class protosystem
{
 public:
  static character*		BalancedCreateMonster(float = 1, bool = true);
  static item*			BalancedCreateItem(bool = false);
  static character*		CreateMonster(bool = true);
  static item*			CreateItem(std::string, bool = true);
  static material*		CreateRandomSolidMaterial(ulong);
  static material*		CreateMaterial(ushort, ulong);
  static material*		CreateMaterial(std::string, ulong, bool = true);
};

template <class type> inline outputfile& operator<<(outputfile& SaveFile, type* Class)
{
  if(Class)
    Class->Save(SaveFile);
  else
    SaveFile << ushort(0);

  return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, type*& Class)
{
  ushort Type;

  SaveFile >> Type;

  if(Type)
    Class = protocontainer<type>::GetProto(Type)->CloneAndLoad(SaveFile);
  else
    Class = 0;

  return SaveFile;
}

#endif

