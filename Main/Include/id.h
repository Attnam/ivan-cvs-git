#ifndef __ID_H__
#define __ID_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"
#include "ivandef.h"

#define CHARNAME(Case) GetName(Case).c_str()
#define CHARDESCRIPTION(Case) GetDescription(Case).c_str()

class id
{
 public:
  virtual void AddName(std::string&, uchar) const;
  virtual std::string GetName(uchar) const;
 protected:
  virtual const std::string& GetNameSingular() const = 0;
  virtual void AddNameSingular(std::string&, bool) const;
  virtual const std::string& GetNamePlural() const = 0;
  virtual const std::string& GetArticle() const = 0;
  virtual bool AddAdjective(std::string&, bool) const;
  virtual const std::string& GetAdjective() const = 0;
  virtual const std::string& GetAdjectiveArticle() const = 0;
  virtual bool AddMaterialDescription(std::string&, bool) const = 0;
  virtual const std::string& GetPostFix() const = 0;
  virtual void AddPostFix(std::string&) const;
  virtual uchar GetArticleMode() const { return NORMALARTICLE; }
  virtual bool ShowMaterial() const { return false; }
  virtual void AddDivineMasterDescription(std::string&, uchar) const;
  virtual void AddLockPostFix(std::string&, uchar) const;
};

#endif

