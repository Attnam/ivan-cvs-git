#ifndef __HELL_H__
#define __HELL_H__

#include <vector>

class entity;

class hell
{
 public:
  static void Add(entity* DoomedOne) { ToBeBurned.push_back(DoomedOne); }
  static void Burn();
 private:
  static std::vector<entity*> ToBeBurned;
};

#endif
