#ifndef __PROTO_H__
#define __PROTO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "typedef.h"

template <class type> class protocontainer
{
 public:
  typedef typename type::prototype prototype;
  static ushort Add(prototype*);
  static const prototype* const GetProto(ushort Index) { return ProtoData[Index]; }
  static ushort GetProtoAmount() { return ProtoData.size(); }
  static ushort SearchCodeName(const std::string&);
  static const valuemap& GetCodeNameMap() { return CodeNameMap; }
  static void GenerateCodeNameMap();
 private:
  static std::vector<prototype*> ProtoData;
  static valuemap CodeNameMap;
};

template <class type> inline ushort protocontainer<type>::Add(prototype* Proto)
{
  if(!ProtoData.size())
    ProtoData.push_back(0);

  ProtoData.push_back(Proto);
  return ProtoData.size() - 1;
}

template <class type> inline ushort protocontainer<type>::SearchCodeName(const std::string& Name)
{
  valuemap::iterator I = CodeNameMap.find(Name);
  return I != CodeNameMap.end() ? I->second : 0;
}

class character;
class item;
class material;
class outputfile;
class inputfile;

class protosystem
{
 public:
  static character* BalancedCreateMonster(float = 1.0f, bool = true);
  static item* BalancedCreateItem(bool = false);
  static character* CreateMonster(bool = true);
  static item* CreateItem(const std::string&, bool = true);
  static material* CreateRandomSolidMaterial(ulong);
  static material* CreateMaterial(const std::string&, ulong, bool = true);
  static void GenerateCodeNameMaps();
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
  Class = Type ? protocontainer<type>::GetProto(Type)->CloneAndLoad(SaveFile) : 0;
  return SaveFile;
}

#endif

