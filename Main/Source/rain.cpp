/* Compiled through materset.cpp */

rain::rain(liquid* Liquid, lsquare* LSquareUnder, vector2d Speed, bool OwnLiquid) : entity(HAS_BE), Liquid(Liquid), LSquareUnder(LSquareUnder), Speed(Speed), SpeedAbs(sqrt(Speed.GetLengthSquare())), OwnLiquid(OwnLiquid), Drops(0)
{
  Emitation = Liquid->GetEmitation();
}

rain::~rain()
{
  if(OwnLiquid)
    delete Liquid;
}

void rain::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance) const
{
  ulong Volume = Liquid->GetVolume();

  if(!Volume && !Drops)
    return;

  ushort DropMax = Volume ? Limit<ushort>(Volume / 50, 1, MAX_RAIN_DROPS) : 0, c;

  for(c = 0; c < Drops && c < DropMax; ++c)
    if(!MaxDropAge[c])
      RandomizeDropPos(c);

  if(Drops < DropMax)
    {
      for(; Drops < DropMax; ++Drops)
	RandomizeDropPos(Drops);
    }
  else if(Drops > DropMax)
    {
      for(; Drops > DropMax && !MaxDropAge[Drops - 1]; --Drops);

      if(!Drops)
	return;
    }

  ushort Color = Liquid->GetColor();

  for(c = 0; c < Drops; ++c)
    if(MaxDropAge[c])
      {
	ulong Age = GET_TICK() - DropStartTick[c];

	if(Age > MaxDropAge[c])
	  {
	    MaxDropAge[c] = 0;
	    continue;
	  }

	vector2d DropPos = DropStartPos[c] + (Speed * short(Age) >> 8);

	if(DropPos.X < 0 || DropPos.Y < 0 || DropPos.X >= 16 || DropPos.Y >= 16)
	  {
	    MaxDropAge[c] = 0;
	    continue;
	  }

	Bitmap->AlphaPutPixel(Pos + DropPos, Color, Luminance, 255);
      }
}

void rain::RandomizeDropPos(ushort Index) const
{
  DropStartTick[Index] = GET_TICK() - (RAND() & 3);
  vector2d Pos;

  if(Speed.X && (!Speed.Y || RAND() & 1))
    {
      Pos.X = Speed.X > 0 ? 0 : 15;
      Pos.Y = RAND() & 15;
    }
  else
    {
      Pos.X = RAND() & 15;
      Pos.Y = Speed.Y > 0 ? 0 : 15;
    }

  DropStartPos[Index] = Pos;
  ulong AgeModifier = 5000 / SpeedAbs;
  MaxDropAge[Index] = AgeModifier > 1 ? 1 + RAND() % AgeModifier : 1;
}

void rain::Be()
{
  ulong Volume = Liquid->GetVolume();

  if(Volume)
    {
      ulong Rand = 250000000 / (Liquid->GetVolume() * SpeedAbs);

      if(Rand < 1 || !(RAND() % Rand))
	{
	  ulong DropVolume = Min(Volume, 50UL);
	  LSquareUnder->SpillFluid(0, Liquid->CloneLiquid(DropVolume), false);

	  if(OwnLiquid)
	    if(Volume == DropVolume)
	      {
		LSquareUnder->RemoveRain(this);
		SendToHell();
	      }
	    else
	      Liquid->EditVolume(DropVolume);
	}
    }
}

void rain::Save(outputfile& SaveFile) const
{
  SaveFile << Liquid << Speed;
}

void rain::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  Liquid = static_cast<liquid*>(ReadType<material*>(SaveFile));
  Liquid->SetMotherEntity(this);
  Emitation = Liquid->GetEmitation();
  SaveFile >> Speed;
  SpeedAbs = sqrt(Speed.GetLengthSquare());
}

outputfile& operator<<(outputfile& SaveFile, const rain* Rain)
{
  if(Rain->HasOwnLiquid())
    {
      SaveFile << bool(true);
      Rain->Save(SaveFile);
    }
  else
    SaveFile << bool(false);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, rain*& Rain)
{
  if(ReadType<bool>(SaveFile))
    {
      Rain = new rain;
      Rain->Load(SaveFile);
    }
  else
    Rain = game::ConstructGlobalRain();

  return SaveFile;
}
