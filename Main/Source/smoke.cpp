/* Compiled through materset.cpp */

smoke::smoke() : entity(HAS_BE), Next(0) { }

smoke::smoke(gas* Gas, lsquare* LSquareUnder) : entity(HAS_BE), Next(0), Gas(Gas), LSquareUnder(LSquareUnder), Alpha(Gas->GetAlpha())
{
  Gas->SetMotherEntity(this);
  Picture.resize(16);
  packedcolor16 Color = Gas->GetColor();
  bitmap Temp(16, 16, TRANSPARENT_COLOR);
  Temp.ActivateFastFlag();
  int Frame[16];
  int Flags[16];

  for(int c = 0; c < 16; ++c)
    {
      Picture[c] = new bitmap(16, 16, TRANSPARENT_COLOR);
      Picture[c]->ActivateFastFlag();
      Picture[c]->CreateAlphaMap(Alpha);
      bool Correct = false;

      while(!Correct)
	{
	  Frame[c] = RAND() & 3;
	  Flags[c] = RAND() & 7;
	  Correct = true;

	  for(int i = 0; i < c; ++i)
	    if(Frame[c] == Frame[i] && Flags[c] == Flags[i])
	      {
		Correct = false;
		break;
	      }
	}

      igraph::GetRawGraphic(GR_EFFECT)->MaskedBlit(&Temp, Frame[c] << 4, 32, 0, 0, 16, 16, &Color);
      Temp.NormalBlit(Picture[c], Flags[c]);
    }

  LSquareUnder->SignalSmokeAlphaChange(Alpha);
}

smoke::~smoke()
{
  for(uint c = 0; c < Picture.size(); ++c)
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

      for(int c = 0; c < 16; ++c)
	Picture[c]->FillAlpha(Alpha);

      Gas->SetVolume(Gas->GetVolume() - Gas->GetVolume() / 50);
    }

  character* Char = LSquareUnder->GetCharacter();

  if(Char && !Char->StateIsActivated(GAS_IMMUNITY))
    Gas->BreatheEffect(Char);
}

square* smoke::GetSquareUnderEntity(int) const
{ 
  return LSquareUnder;
}

void smoke::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance) const
{
  Picture[(GET_TICK() >> 1) & 3]->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

void smoke::Save(outputfile& SaveFile) const
{
  SaveFile << Picture << Gas << Alpha;
}

void smoke::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  SaveFile >> Picture >> Gas >> Alpha;
  Gas->SetMotherEntity(this);
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
  if(Gas->GetBreatheMessage().GetSize())
    ADD_MESSAGE("%s", Gas->GetBreatheMessage().CStr());
}

void smoke::Merge(gas* OtherGas)
{
  Gas->EditVolume(OtherGas->GetVolume());
  LSquareUnder->SignalSmokeAlphaChange(OtherGas->GetAlpha() - Alpha);
  Alpha = OtherGas->GetAlpha();

  for(int c = 0; c < 16; ++c)
    Picture[c]->FillAlpha(Alpha);

  delete OtherGas;
}

bool smoke::IsDangerousForAIToBreathe(const character* Who) const
{
  return !Who->StateIsActivated(GAS_IMMUNITY)
      && Who->GetAttribute(WISDOM) >= Gas->GetBreatheWisdomLimit();
}
