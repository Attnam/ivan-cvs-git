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
  static bool AnalyzeData(inputfile&, const std::string&, typename type::database*);
};

class databasesystem
{
 public:
  static void Initialize();
};

template <class type> void database<type>::InstallDataBase(type* Instance)
{
  if(!Instance->Config)
    Instance->DataBase = Instance->GetProtoType()->GetDataBase();
  else
    {
      const typename type::databasemap& Configs = Instance->GetProtoType()->GetConfig();
      typename type::databasemap::const_iterator i = Configs.find(Instance->Config);

      if(i != Configs.end())
	Instance->DataBase = &i->second;
      else
	ABORT("Undefined %s configuration #%d sought!", Instance->GetProtoType()->GetClassId().c_str(), Instance->Config);
    }
}

#endif
