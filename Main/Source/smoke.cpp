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
#include "message.h"
#include "char.h"

smoke::smoke(gas* Gas, lsquare* LSquareUnder) : entity(HAS_BE), Gas(Gas), LSquareUnder(LSquareUnder), Alpha(Gas->GetAlpha())
{
  Gas->SetMotherEntity(this);
  Picture.resize(4);
  ushort Color = Gas->GetColor();

  for(ushort c = 0; c < 4; ++c)
    {
      Picture[c] = new bitmap(16, 16, TRANSPARENT_COLOR);
      Picture[c]->CreateAlphaMap(Alpha);
      igraph::GetRawGraphic(GR_OLTERRAIN)->MaskedBlit(Picture[c], 16 + (c << 4), 16, 0, 0, 16,16, &Color);
    }

  LSquareUnder->SignalSmokeAlphaChange(Alpha);
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
      LSquareUnder->SendNewDrawRequest();
      LSquareUnder->SignalSmokeAlphaChange(-1);

      if(!--Alpha)
	{
	  LSquareUnder->RemoveSmoke(this);
	  SendToHell();
	  return;
	}

      for(ushort c = 0; c < 4; ++c)
	Picture[c]->FillAlpha(Alpha);

      Gas->SetVolume(Gas->GetVolume() - Gas->GetVolume() / 50);
    }

  character* Char = LSquareUnder->GetCharacter();

  if(Char && !Char->StateIsActivated(GAS_IMMUNITY))
    Gas->BreatheEffect(Char);
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
  SaveFile << Picture << Gas << Alpha;
}

void smoke::Load(inputfile& SaveFile)
{
  SaveFile >> Picture >> Gas >> Alpha;
  Gas->SetMotherEntity(this);
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

void smoke::AddBreatheMessage() const
{
  if(Gas->GetBreatheMessage().length())
    ADD_MESSAGE("%s", Gas->GetBreatheMessage().c_str());
}

void smoke::Merge(gas* OtherGas)
{
  Gas->EditVolume(OtherGas->GetVolume());
  LSquareUnder->SignalSmokeAlphaChange(OtherGas->GetAlpha() - Alpha);
  Alpha = OtherGas->GetAlpha();

  for(ushort c = 0; c < 4; ++c)
    Picture[c]->FillAlpha(Alpha);

  delete OtherGas;
}
