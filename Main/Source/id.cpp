#include "id.h"
#include "game.h"
#include "godba.h"
#include "festring.h"

void id::AddNameSingular(std::string& String, bool Articled) const
{
  if(Articled)
    String << GetArticle() << " ";

  String << GetNameSingular();
}

void id::AddName(std::string& Name, uchar Case) const
{
  bool Articled;

  if((Case & ARTICLEBIT) && (GetArticleMode() == DEFINITEARTICLE || (GetArticleMode() == NORMALARTICLE && !(Case & INDEFINEBIT))))
    {
      Name << "the ";
      Articled = false;
    }
  else
    Articled = !(Case & PLURAL) && (Case & ARTICLEBIT) && (Case & INDEFINEBIT) && GetArticleMode() != NOARTICLE;

  if(AddAdjective(Name, Articled))
    Articled = false;

  if(ShowMaterial() && AddMaterialDescription(Name, Articled))
    Articled = false;

  if(Case & PLURAL)
    Name << GetNamePlural();
  else
    AddNameSingular(Name, Articled);

  AddPostFix(Name);
}

std::string id::GetName(uchar Case) const
{
  std::string Name;
  AddName(Name, Case);
  return Name;
}

void id::AddName(std::string& Name, uchar Case, ushort Amount) const
{
  if(Amount == 1)
    AddName(Name, Case&~PLURAL);
  else
    {
      if((Case & ARTICLEBIT) && (GetArticleMode() == DEFINITEARTICLE || (GetArticleMode() == NORMALARTICLE && !(Case & INDEFINEBIT))))
	Name << "the ";

      Name << Amount << ' ';
      AddName(Name, Case&~ARTICLEBIT|PLURAL);
    }
}

std::string id::GetName(uchar Case, ushort Amount) const
{
  std::string Name;
  AddName(Name, Case, Amount);
  return Name;
}

bool id::AddAdjective(std::string& String, bool Articled) const
{
  if(GetAdjective().length())
    {
      if(Articled)
	String << GetAdjectiveArticle() << " ";

      String << GetAdjective() << " ";
      return true;
    }
  else
    return false;
}

void id::AddPostFix(std::string& String) const
{
  if(GetPostFix().length())
    String << " " << GetPostFix();
}

void id::AddLockPostFix(std::string& String, uchar LockType) const
{
  /* doesn't yet support other locktype articles than "a" */

  String << " with a " << game::GetLockDescription(LockType) << " lock";
}

void id::AddDivineMasterDescription(std::string& String, uchar DivineMaster) const
{
  String << " of " << game::GetGod(DivineMaster)->Name();
}

