/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#ifndef __DATABASE_H__
#define __DATABASE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

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
  typedef typename type::prototype prototype;
  typedef std::map<festring, databasememberbase<database>*> databasemembermap;
  static void ReadFrom(inputfile&);
  static void FindDataBase(const database*&, const prototype*, int);
  static void InstallDataBase(type*, int);
  static void CreateDataBaseMemberMap();
  static int CreateDivineConfigurations(const prototype*, database**, int);
 private:
  static bool AnalyzeData(inputfile&, const festring&, database&);
  static void CheckDefaults(const festring&, database&);
  static void CreateLTerrainDataBaseMemberMap();
  static databasemembermap DataBaseMemberMap;
};

class databasesystem
{
 public:
  static void Initialize();
};

#endif
