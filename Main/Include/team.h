#ifndef __TEAM_H__
#define __TEAM_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <list>
#include <map>

#include "typedef.h"

class outputfile;
class inputfile;
class character;

class team
{
 public:
  team();
  team(ushort);
  void SetRelation(team*, uchar);
  uchar GetRelation(const team*) const;
  void Hostility(team*);
  ushort GetID() const { return ID; }
  void SetID(ushort What) { ID = What; }
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetLeader(character* What) { Leader = What; }
  character* GetLeader() const { return Leader; }
  std::list<character*>::iterator Add(character*);
  void Remove(std::list<character*>::iterator);
  const std::list<character*>& GetMember() const { return Member; }
  ushort GetKillEvilness() const { return KillEvilness; }
  void SetKillEvilness(ushort What) { KillEvilness = What; }
  bool HasEnemy() const;
  ushort GetMembers() const { return Member.size(); }
  ushort GetEnabledMembers() const;
 private:
  character* Leader;
  std::map<ulong, uchar> Relation;
  std::list<character*> Member;
  ushort ID;
  ushort KillEvilness;
};

outputfile& operator<<(outputfile&, const team*);
inputfile& operator>>(inputfile&, team*&);

#endif
