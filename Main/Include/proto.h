#ifndef __PROTO_H__
#define __PROTO_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>
#include <vector>

#include "festring.h"
#include "ivandef.h"
#include "save.h"

class character;
class item;
class material;
template <class type> class databasecreator;
struct itemdatabase;

typedef std::map<festring, long> valuemap;
typedef std::vector<item*> itemvector;
typedef std::vector<character*> charactervector;

template <class type> class protocontainer
{
 public:
  friend class protosystem;
  friend class databasecreator<type>;
  typedef typename type::prototype prototype;
  static int Add(prototype*);
  static const prototype* GetProto(int I) { return ProtoData[I]; }
  static int GetSize() { return Size; }
  static int SearchCodeName(const festring&);
  static const char* GetMainClassID() { return ProtoData[1]->GetClassID(); }
 private:
  static prototype** ProtoData;
  static valuemap CodeNameMap;
  static int Size;
};

template <class type> inline int protocontainer<type>::Add(prototype* Proto)
{
  if(!Size)
    (ProtoData = new prototype*[1024])[Size++] = 0;

  int Index = Size++;
  ProtoData[Index] = Proto;
  CodeNameMap.insert(std::pair<festring, long>(Proto->GetClassID(), Index));
  return Index;
}

template <class type> inline int protocontainer<type>::SearchCodeName(const festring& Name)
{
  valuemap::iterator I = CodeNameMap.find(Name);
  return I != CodeNameMap.end() ? I->second : 0;
}

class protosystem
{
 public:
  static character* BalancedCreateMonster();
  static item* BalancedCreateItem(long = 0, long = MAX_PRICE, long = ANY_CATEGORY, int = 0, int = 0, bool = false);
  static character* CreateMonster(int = 1, int = 9999, int = 0);
  static character* CreateMonster(const festring&, int = 0, bool = true);
  static item* CreateItem(const festring&, bool = true);
  static material* CreateMaterial(const festring&, long = 0, bool = true);
  static void CreateEveryNormalEnemy(charactervector&);
#ifdef WIZARD
  static void CreateEveryCharacter(charactervector&);
  static void CreateEveryItem(itemvector&);
  static void CreateEveryMaterial(std::vector<material*>&);
#endif
  static void Initialize();
 private:
  static itemdatabase** ItemConfigData;
  static int ItemConfigDataSize;
  static itemdatabase** ItemCategoryData[ITEM_CATEGORIES];
  static int ItemCategorySize[ITEM_CATEGORIES];
  static long ItemCategoryPossibility[ITEM_CATEGORIES];
  static long TotalItemPossibility;
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
  int Type = 0;
  SaveFile >> (ushort&)Type;
  Class = Type ? protocontainer<type>::GetProto(Type)->CloneAndLoad(SaveFile) : 0;
  return SaveFile;
}

#endif
