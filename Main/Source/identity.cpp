#include "identity.h"
#include "game.h"
#include "godba.h"

std::string identity::NameSingular(bool Articled) const
{
  if(Articled)
    return Article() + " " + NameSingular();
  else
    return NameSingular();
}

std::string identity::OwnerGodDescription(uchar OwnerGod) const
{
  return std::string("of ") + game::GetGod(OwnerGod)->Name();
}

std::string identity::Name(uchar Case) const
{
  std::string Name;
  bool Articled = !(Case & PLURAL) && (Case & ARTICLEBIT) && (Case & INDEFINEBIT) && ShowArticle();

  if((Case & ARTICLEBIT) && (ForceDefiniteArticle() || !(Case & INDEFINEBIT)) && ShowArticle())
    {
      Name += std::string("the ");
      Articled = false;
    }

  if(ShowAdjective())
    {
      std::string AF = Adjective(Articled);

      if(AF != "")
	{
	  Name += AF + " ";
	  Articled = false;
	}
    }

  if(ShowMaterial())
    {
      std::string MD = MaterialDescription(Articled);

      if(MD != "")
	{
	  Name += MD + " ";
	  Articled = false;
	}
    }

  Name += Case & PLURAL ? NamePlural() : NameSingular(Articled);

  if(ShowPostFix())
    {
      std::string PoF = PostFix();

      if(PoF != "")
	Name += std::string(" ") + PoF;
    }

  return Name;
}
