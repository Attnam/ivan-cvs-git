#ifndef __TEAM_H__
#define __TEAM_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define HOSTILE 0
#define NEUTRAL 1
#define FRIEND 2

#include <map>
#include <list>

#include "typedef.h"

class outputfile;
class inputfile;
class character;

class team
{
 public:
  team() : Leader(0) {}
  team(ushort ID) : Leader(0), ID(ID) {}
  void SetRelation(team*, uchar);
  uchar GetRelation(team*);
  void Hostility(team*);
  ushort GetID() const { return ID; }
  void SetID(ushort What) { ID = What; }
  void Save(outputfile&) const;
  void Load(inputfile&);
  void SetLeader(character* What) { Leader = What; }
  character* GetLeader() const { return Leader; }
  std::list<character*>::iterator Add(character* Char) { return Member.insert(Member.end(), Char); }
  void Remove(std::list<character*>::iterator Iterator) { Member.erase(Iterator); }
  std::list<character*>& GetMember() { return Member; }
 private:
  character* Leader;
  std::map<ulong, uchar> Relation;
  std::list<character*> Member;
  ushort ID;
};

inline outputfile& operator<<(outputfile& SaveFile, team* Team)
{
  Team->Save(SaveFile);

  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, team*& Team)
{
  Team = new team;
  Team->Load(SaveFile);

  return SaveFile;
}

#endif



