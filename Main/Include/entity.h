#ifndef __ENTITY_H__
#define __ENTITY_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define CNAME(Case) Name(Case).c_str()

#define	UNARTICLED		0	//0000000
#define	PLURAL			1	//0000001
#define	DEFINEBIT		2	//0000010
#define	DEFINITE		2	//0000010
#define	INDEFINEBIT		4	//0000100
#define	INDEFINITE		6	//0000110

#include <list>
#include <string>

#include "typedef.h"
#include "pool.h"

class square;
class lsquare;
class outputfile;
class inputfile;

class entity
{
 public:
  entity(bool, bool);
  virtual ~entity();
  virtual void Save(outputfile&) const;
  virtual void Load(inputfile&);
  virtual void Be() { }
  virtual std::list<entityinfo>::iterator GetPoolIterator() const { return PoolIterator; }
  virtual void SetPoolIterator(std::list<entityinfo>::iterator What) { PoolIterator = What; }
  virtual bool GetExists() const;
  virtual void SetExists(bool);
  virtual void DrawToTileBuffer() const = 0;
  virtual square* GetSquareUnder() const { return SquareUnder; }
  virtual void SetSquareUnder(square* What) { SquareUnder = What; }
  virtual lsquare* GetLSquareUnder() const;
  virtual void SetLSquareUnder(lsquare*);

  /* Temporary */

  virtual std::string Name(uchar Case) const { return NameNormal(Case, "a", ""); } //?
  virtual std::string GetNameSingular() const { return NameSingular(); }
  virtual std::string GetNamePlural() const { return NamePlural(); }

 protected:

  /* Temporary */

  virtual std::string NameSingular() const { return ""; }
  virtual std::string NamePlural() const { return NameSingular() + "s"; }
  virtual std::string NameNormal(uchar, std::string, std::string = "") const;
  virtual std::string NameProperNoun(uchar) const;

  std::list<entityinfo>::iterator PoolIterator;
  bool InPool;
  square* SquareUnder;
};

#endif

