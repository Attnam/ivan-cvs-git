#ifndef __ID_H__
#define __ID_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"
#include "ivandef.h"

#define CHARNAME(Case) GetName(Case).c_str()

#ifdef VC
typedef void YesThisIsNotAJoke;
#endif

class id
{
 public:
#ifdef VC
  virtual void ThisFunctionIsHereBecauseMicrosoftIsEvilAndVCGeneratesAnInternalCompilerErrorIfGetNameIsTheFirstVirtualFunctionHere(YesThisIsNotAJoke) { }
#endif
  virtual std::string GetName(uchar) const;
 protected:
  virtual std::string GetNameSingular() const = 0;
  virtual std::string GetNameSingular(bool) const;
  virtual std::string GetNamePlural() const { return GetNameSingular() + "s"; }
  virtual std::string GetArticle() const { return "a"; }
  virtual std::string GetAdjective(bool) const;
  virtual std::string GetAdjective() const { return ""; }
  virtual std::string GetAdjectiveArticle() const { return "a"; }
  virtual std::string GetMaterialDescription(bool) const = 0;
  virtual std::string GetPostFix() const { return ""; }
  virtual uchar GetArticleMode() const { return NORMALARTICLE; }
  virtual bool ShowMaterial() const { return false; }
  virtual std::string GetDivineMasterDescription(uchar) const;
  virtual std::string GetLockPostFix(uchar) const;
};

#endif
