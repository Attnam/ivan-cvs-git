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

#ifndef __POOL_H__
#define __POOL_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

class entity;

class pool
{
 public:
  static void Add(entity*);
  static void Remove(entity*);
  static void AddToHell(entity*);
  static void BurnHell();
  static void Be();
 private:
  static entity* FirstEntity;
  static entity* LastEntity;
  static entity* FirstDoomed;
  static entity* LastDoomed;
  static entity* CurrentEntity;
};

#endif
