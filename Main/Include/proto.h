#ifndef __PROTO_H__
#define __PROTO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"
#include "festring.h"

#include <vector>
#include <map>

class character;
class item;
class material;
class outputfile;
class inputfile;
class festring;
template <class type> class databasecreator;

typedef std::map<festring, long> valuemap;
typedef std::vector<item*> itemvector;

template <class type> class protocontainer
{
 public:
  friend class databasecreator<type>;
  typedef typename type::prototype prototype;
  static ushort Add(prototype*);
  static const prototype* GetProto(ushort Index) { return ProtoData[Index]; }
  static ushort GetProtoAmount() { return ProtoAmount; }
  static ushort SearchCodeName(const festring&);
  static const char* GetMainClassID() { return ProtoData[1]->GetClassID(); }
 private:
  static prototype** ProtoData;
  static valuemap CodeNameMap;
  static ushort ProtoAmount;
};

template <class type> inline ushort protocontainer<type>::Add(prototype* Proto)
{
  if(!ProtoAmount)
    (ProtoData = new prototype*[1024])[ProtoAmount++] = 0;

  ushort Index = ProtoAmount++;
  ProtoData[Index] = Proto;
  CodeNameMap.insert(std::pair<festring, long>(Proto->GetClassID(), Index));
  return Index;
}

template <class type> inline ushort protocontainer<type>::SearchCodeName(const festring& Name)
{
  valuemap::iterator I = CodeNameMap.find(Name);
  return I != CodeNameMap.end() ? I->second : 0;
}

class protosystem
{
 public:
  static character* BalancedCreateMonster();
  static item* BalancedCreateItem(ulong = 0, ulong = 4294967295UL, ulong = 0, ushort = 0, bool = false);
  static character* CreateMonster(ushort = 1, ushort = 9999, ushort = 0);
  static character* CreateMonster(const festring&, ushort = 0, bool = true);
  static item* CreateItem(const festring&, bool = true);
  static material* CreateMaterial(const festring&, ulong = 0, bool = true);
  static void CreateEveryNormalEnemy(std::vector<character*>&);
#ifdef WIZARD
  static void CreateEveryCharacter(std::vector<character*>&);
  static void CreateEveryItem(itemvector&);
  static void CreateEveryMaterial(std::vector<material*>&);
#endif
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

