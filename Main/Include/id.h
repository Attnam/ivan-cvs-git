#ifndef __ID_H__
#define __ID_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

#define CHARNAME(Case) Name(Case).c_str()

class id
{
 public:
  virtual std::string NameSingular() const = 0;
  virtual std::string NameSingular(bool) const;
  virtual std::string NamePlural() const { return NameSingular() + "s"; }
  virtual std::string Name(uchar) const;
 protected:
  virtual std::string DivineMasterDescription(uchar) const;
  virtual std::string Article() const { return "a"; }
  virtual std::string Adjective(bool Articled) const { return Articled ? AdjectiveArticle() + " " + Adjective() : Adjective(); }
  virtual std::string Adjective() const { return ""; }
  virtual std::string AdjectiveArticle() const { return "a"; }
  virtual std::string MaterialDescription(bool) const = 0;
  virtual std::string PostFix() const { return ""; }
  virtual bool ForceDefiniteArticle() const { return false; }
  virtual bool ShowArticle() const { return true; }
  virtual bool ShowAdjective() const { return false; }
  virtual bool ShowMaterial() const { return false; }
  virtual bool ShowPostFix() const { return false; }
};

#endif
