#include "smoke.h"
#include "lsquare.h"
#include "bitmap.h"
#include "save.h"
#include "materias.h"
#include "femath.h"
#include "whandler.h"
#include "proto.h"
#include "game.h"
#include "igraph.h"
#include "colorbit.h"

smoke::smoke(gas* Gas, lsquare* LSquareUnder) : entity(HAS_BE), Gas(Gas), LSquareUnder(LSquareUnder)
{
  Gas->SetMotherEntity(this);
  Picture.resize(4);
  ushort Color = Gas->GetColor();
  for(ushort c = 0; c < 4; ++c)
    {
      Picture[c] = new bitmap(16,16,TRANSPARENT_COLOR);
      Picture[c]->CreateAlphaMap(Gas->GetAlpha());
      igraph::GetRawGraphic(GR_OLTERRAIN)->MaskedBlit(Picture[c], 16 + c * 16, 16, 0, 0, 16,16, &Color);
    }
}

smoke::smoke() : entity(HAS_BE)
{


}
smoke::~smoke()
{
  for(ushort c = 0; c < Picture.size(); ++c)
    delete Picture[c];
  delete Gas;
}

void smoke::Be()
{
  if(!(RAND() & 7))
    {
      for(ushort c = 0; c < 4; ++c)
	{
	  if(!Picture[c]->ChangeAlpha(-1))
	    {
	      GetLSquareUnder()->RemoveSmoke(this);
	      SendToHell();
	      break;
	    }
	}
      GetLSquareUnder()->SendNewDrawRequest();
      Gas->EditVolume(-Gas->GetVolume() / 50);
    }
}


square* smoke::GetSquareUnderEntity() const
{ 
  return LSquareUnder;
}

void smoke::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate) const
{
  Picture[!AllowAnimate ? 0 : (globalwindowhandler::GetTick() >> 1) & 3]->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

void smoke::Save(outputfile& SaveFile) const
{
  SaveFile << Picture << Gas;
}

void smoke::Load(inputfile& SaveFile)
{
  SaveFile >> Picture >> Gas;
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
}

outputfile& operator<<(outputfile& SaveFile, const smoke* Smoke)
{
  Smoke->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, smoke*& Smoke)
{
  Smoke = new smoke;
  Smoke->Load(SaveFile);
  return SaveFile;
}
