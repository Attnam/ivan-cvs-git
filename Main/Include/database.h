#ifndef __DATABASE_H__
#define __DATABASE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <map>

#include "error.h"

class inputfile;
class festring;

template <class database> struct databasememberbase
{
  virtual void ReadData(database&, inputfile&) = 0;
};

template <class type> class databasecreator
{
 public:
  typedef typename type::database database;
  typedef std::map<festring, databasememberbase<database>*> databasemembermap;
  static void ReadFrom(inputfile&);
  static void InstallDataBase(type*);
  static void CreateDataBaseMemberMap();
 private:
  static bool AnalyzeData(inputfile&, const festring&, database&);
  static void CheckDefaults(const festring&, database&);
  static databasemembermap DataBaseMemberMap;
};

class databasesystem
{
 public:
  static void Initialize();
  static void CreateDataBaseMemberMaps();
};

template <class type> inline void databasecreator<type>::InstallDataBase(type* Instance)
{
  const typename type::databasemap& Config = Instance->GetProtoType()->GetConfig();
  const typename type::databasemap::const_iterator i = Config.find(Instance->Config);

  if(i != Config.end())
    Instance->DataBase = &i->second;
  else
    ABORT("Undefined %s configuration #%d sought!", Instance->GetProtoType()->GetClassId(), Instance->Config);
}

#endif
