#ifndef __DATABASE_H__
#define __DATABASE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

class inputfile;

template <class type> class database
{
 public:
  static void ReadFrom(inputfile&);
  static void InstallDataBase(type*);
 private:
  static bool AnalyzeData(inputfile&, const std::string&, typename type::database&);
};

class databasesystem
{
 public:
  static void Initialize();
};

template <class type> void database<type>::InstallDataBase(type* Instance)
{
  const typename type::databasemap& Config = Instance->GetProtoType()->GetConfig();
  const typename type::databasemap::const_iterator i = Config.find(Instance->Config);

  if(i != Config.end())
    Instance->DataBase = &i->second;
  else
    ABORT("Undefined %s configuration #%d sought!", Instance->GetProtoType()->GetClassId().c_str(), Instance->Config);
}

#endif
