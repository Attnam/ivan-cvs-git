#include "id.h"
#include "game.h"
#include "god.h"
#include "festring.h"
#include "ivandef.h"

void id::AddNameSingular(festring& String, bool Articled) const
{
  if(Articled)
    String << GetArticle() << ' ';

  String << GetNameSingular();
}

void id::AddName(festring& Name, uchar Case) const
{
  bool Articled;

  if((Case & ARTICLE_BIT) && (GetArticleMode() == DEFINITE_ARTICLE || (GetArticleMode() == NORMAL_ARTICLE && !(Case & INDEFINE_BIT))))
    {
      Name << "the ";
      Articled = false;
    }
  else
    Articled = !(Case & PLURAL) && (Case & ARTICLE_BIT) && (Case & INDEFINE_BIT) && GetArticleMode() != NO_ARTICLE;

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

festring id::GetName(uchar Case) const
{
  static festring Name;
  Name.Empty();
  AddName(Name, Case);
  return Name;
}

void id::AddName(festring& Name, uchar Case, ushort Amount) const
{
  if(Amount == 1)
    AddName(Name, Case&~PLURAL);
  else
    {
      if((Case & ARTICLE_BIT) && (GetArticleMode() == DEFINITE_ARTICLE || (GetArticleMode() == NORMAL_ARTICLE && !(Case & INDEFINE_BIT))))
	Name << "the ";

      Name << Amount << ' ';
      AddName(Name, Case&~ARTICLE_BIT|PLURAL);
    }
}

festring id::GetName(uchar Case, ushort Amount) const
{
  static festring Name;
  Name.Empty();
  AddName(Name, Case, Amount);
  return Name;
}

bool id::AddAdjective(festring& String, bool Articled) const
{
  if(GetAdjective().GetSize())
    {
      if(Articled)
	String << GetAdjectiveArticle() << ' ';

      String << GetAdjective() << ' ';
      return true;
    }
  else
    return false;
}

void id::AddPostFix(festring& String) const
{
  if(GetPostFix().GetSize())
    String << ' ' << GetPostFix();
}

uchar id::GetArticleMode() const
{
  return NORMAL_ARTICLE;
}

bool id::AddActiveAdjective(festring& String, bool Articled) const
{
  String << (Articled ? "an active " : "active ");
  return true;
}
