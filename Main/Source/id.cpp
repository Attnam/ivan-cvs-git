#include "id.h"
#include "game.h"
#include "godba.h"

std::string id::GetNameSingular(bool Articled) const
{
  if(Articled)
    return GetArticle() + " " + GetNameSingular();
  else
    return GetNameSingular();
}

std::string id::GetDivineMasterDescription(uchar DivineMaster) const
{
  return DivineMaster ? "of " + game::GetGod(DivineMaster)->Name() : "";
}

std::string id::GetName(uchar Case) const
{
  std::string Name;
  bool Articled;

  if((Case & ARTICLEBIT) && (GetArticleMode() == DEFINITEARTICLE || (GetArticleMode() == NORMALARTICLE && !(Case & INDEFINEBIT))))
    {
      Name += "the ";
      Articled = false;
    }
  else
    Articled = !(Case & PLURAL) && (Case & ARTICLEBIT) && (Case & INDEFINEBIT) && GetArticleMode() != NOARTICLE;

  std::string AF = GetAdjective(Articled);

  if(AF != "")
    {
      Name += AF + " ";
      Articled = false;
    }

  if(ShowMaterial())
    {
      std::string MD = GetMaterialDescription(Articled);

      if(MD != "")
	{
	  Name += MD + " ";
	  Articled = false;
	}
    }

  Name += Case & PLURAL ? GetNamePlural() : GetNameSingular(Articled);

  std::string PoF = GetPostFix();

  if(PoF != "")
    Name += " " + PoF;

  return Name;
}

std::string id::GetAdjective(bool Articled) const
{
  if(GetAdjective() != "")
    {
      if(Articled)
	return GetAdjectiveArticle() + " " + GetAdjective();
      else
	return GetAdjective();
    }
  else
    return "";
}

std::string id::GetLockPostFix(uchar LockType) const
{
  /* doesn't yet support other locktype articles than "a" */

  return "with a " + game::GetLockDescription(LockType) + " lock";
}
