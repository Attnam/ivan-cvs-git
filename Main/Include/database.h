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
 private:
  static void AnalyzeData(inputfile&, const std::string&, typename type::database*);
};

class databasesystem
{
 public:
  static void Initialize();
};

#endif
