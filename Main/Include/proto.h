#ifndef __PROTO_H__
#define __PROTO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "save.h"

class character;
class item;
class material;
template <class type> class database;

template <class type> class protocontainer
{
 public:
  friend class database<type>;
  typedef typename type::prototype prototype;
  static ushort Add(prototype*);
  static const prototype* GetProto(ushort Index) { return ProtoData[Index]; }
  static ushort GetProtoAmount() { return ProtoData.size(); }
  static ushort SearchCodeName(const std::string&);
  static const valuemap& GetCodeNameMap() { return CodeNameMap; }
  static const std::string& GetMainClassId() { return ProtoData[1]->GetClassId(); }
 private:
  static std::vector<prototype*> ProtoData;
  static valuemap CodeNameMap;
};

template <class type> inline ushort protocontainer<type>::Add(prototype* Proto)
{
  if(!ProtoData.size())
    ProtoData.push_back(0);

  ushort Index = ProtoData.size();
  ProtoData.push_back(Proto);
  CodeNameMap.insert(std::pair<std::string, long>(Proto->GetClassId(), Index));
  return Index;
}

template <class type> inline ushort protocontainer<type>::SearchCodeName(const std::string& Name)
{
  valuemap::iterator I = CodeNameMap.find(Name);
  return I != CodeNameMap.end() ? I->second : 0;
}

class protosystem
{
 public:
  static character* BalancedCreateMonster();
  static item* BalancedCreateItem(ulong = 0, ulong = 4294967295UL, ulong = 0, bool = false);
  static character* CreateMonster(ushort = 0, ushort = 10000, ushort = 0);
  static character* CreateMonster(const std::string&, ushort = 0, bool = true);
  static item* CreateItem(const std::string&, bool = true);
  static material* CreateMaterial(const std::string&, ulong = 0, bool = true);
  static void CreateEveryCharacter(std::vector<character*>&);
  static void CreateEveryItem(std::vector<item*>&);
};

template <class type> inline outputfile& operator<<(outputfile& SaveFile, const type* Class)
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
