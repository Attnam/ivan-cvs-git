/* Compiled through materset.cpp */

/* Used to determine how pixels are distributed when fluid over bodyarmors
   is shown */

const ulong fluid::BodyArmorPartPixels[] = { 34, 7, 7, 8, 6, 6 };

fluid::fluid() : entity(HAS_BE), MotherItem(0), GearImage(0) { }

fluid::fluid(liquid* Liquid, lsquare* LSquareUnder) : entity(HAS_BE), Liquid(Liquid), LSquareUnder(LSquareUnder), MotherItem(0), GearImage(0)
{
  Emitation = Liquid->GetEmitation();
  AddLiquid(Liquid->GetVolume());
}

fluid::fluid(liquid* Liquid, item* MotherItem) : entity(HAS_BE), Liquid(Liquid), LSquareUnder(0), MotherItem(MotherItem), GearImage(0)
{
  Emitation = Liquid->GetEmitation();
  Image.Picture->InitRandMap();
  Image.ShadowPos = MotherItem->GetBitmapPos(0);
  Image.SpecialFlags = MotherItem->GetSpecialFlags();
  AddLiquid(Liquid->GetVolume());
}

fluid::~fluid()
{
  delete Liquid;
  delete [] GearImage;
}

void fluid::AddLiquid(ulong Volume)
{
  ulong Pixels = Volume >> 2;

  if(!Pixels)
    return;

  ushort Col = Liquid->GetColor();

  if(MotherItem)
    {
      pixelpredicate Pred = MotherItem->GetFluidPixelAllowedPredicate();
      Image.AddLiquidToPicture(MotherItem->GetRawPicture(), Pixels, 225, Col, Pred);

      if(GearImage)
	if(HasBodyArmorPictures)
	  for(ushort c = 0; c < BODY_ARMOR_PARTS; ++c)
	    GearImage[c].AddLiquidToPicture(igraph::GetHumanoidRawGraphic(), Pixels * BodyArmorPartPixels[c] / HUMAN_BODY_ARMOR_PIXELS, Image.AlphaAverage, Col, Pred);
	else
	  GearImage->AddLiquidToPicture(igraph::GetHumanoidRawGraphic(), Pixels, Image.AlphaAverage, Col, Pred);
    }
  else
    Image.AddLiquidToPicture(0, Pixels, 225, Col, 0);

  /* Image.AlphaSum < Pixels << 8 so the volume can only decrease */

  Liquid->SetVolumeNoSignals(Image.AlphaSum >> 6);
}

void fluid::Be()
{
  if(MotherItem ? !(RAND() & 7) && MotherItem->AllowFluidBe() : !(RAND() & 15))
    {
      FadePictures();
      Liquid->SetVolumeNoSignals(Image.AlphaSum >> 6);

      if(!MotherItem)
	{
	  LSquareUnder->SendNewDrawRequest();
	  LSquareUnder->SendMemorizedUpdateRequest();
	}

      if(!Liquid->GetVolume())
	Destroy();
      else if(MotherItem)
	{
	  if(MotherItem->Exists())
	    Liquid->TouchEffect(MotherItem);
	}
      else
	{
	  Liquid->TouchEffect(LSquareUnder->GetGLTerrain());

	  if(LSquareUnder->GetOLTerrain())
	    Liquid->TouchEffect(LSquareUnder->GetOLTerrain());

	  if(LSquareUnder->GetCharacter())
	    LSquareUnder->GetCharacter()->StayOn(Liquid);
	}
    }
}

void fluid::Save(outputfile& SaveFile) const
{
  SaveFile << Liquid;
  Image.Save(SaveFile);

  if(GearImage)
    {
      SaveFile << bool(true) << HasBodyArmorPictures;
      ushort Images = HasBodyArmorPictures ? BODY_ARMOR_PARTS : 1;

      for(ushort c = 0; c < Images; ++c)
	GearImage[c].Save(SaveFile);
    }
  else
    SaveFile << bool(false);
}

void fluid::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
  Liquid = static_cast<liquid*>(ReadType<material*>(SaveFile));
  Liquid->SetMotherEntity(this);
  Emitation = Liquid->GetEmitation();
  Image.Load(SaveFile);

  if(ReadType<bool>(SaveFile))
    {
      SaveFile >> HasBodyArmorPictures;
      ushort Images = HasBodyArmorPictures ? BODY_ARMOR_PARTS : 1;
      GearImage = new imagedata[Images];

      for(ushort c = 0; c < Images; ++c)
	{
	  GearImage[c].Load(SaveFile);
	  GearImage[c].Picture->InitRandMap();
	  GearImage[c].Picture->CalculateRandMap();
	}
    }
}

void fluid::Draw(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool AllowAnimate) const
{
  bitmap* Picture = Image.Picture;
  ushort SpecialFlags = MotherItem ? MotherItem->GetSpecialFlags() : 0;

  if(SpecialFlags & 0x7)
    {
      Picture->BlitAndCopyAlpha(igraph::GetFlagBuffer(), SpecialFlags);
      igraph::GetFlagBuffer()->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
  else
    Picture->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);

  if(MotherItem && AllowAnimate)
    Image.Animate(Bitmap, Pos, Luminance, SpecialFlags);
}

outputfile& operator<<(outputfile& SaveFile, const fluid* Fluid)
{
  Fluid->Save(SaveFile);
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, fluid*& Fluid)
{
  Fluid = new fluid;
  Fluid->Load(SaveFile);
  return SaveFile;
}

/* If fluid has decreased, fade, otherwise add new pixels */

void fluid::SignalVolumeAndWeightChange()
{
  ulong Volume = Liquid->GetVolume();

  if(Volume < Image.AlphaSum >> 6)
    {
      while(FadePictures() && Volume < Image.AlphaSum >> 6);
      Liquid->SetVolumeNoSignals(Image.AlphaSum >> 6);
    }
  else
    AddLiquid(Volume - (Image.AlphaSum >> 6));
}

bool fluid::FadePictures()
{
  bool Change = Image.Fade();

  if(GearImage)
    {
      ushort Images = HasBodyArmorPictures ? BODY_ARMOR_PARTS : 1;

      for(ushort c = 0; c < Images; ++c)
	GearImage[c].Fade();
    }

  return Change;
}

/* Used by lookmode, item descriptions etc. If there are 1-3 members in
   Fluid, list them all, otherwise say "a lot of liquids". If there are
   several types of blood in the list, they are counted as one. */

void fluid::AddFluidInfo(const fluidlist& Fluid, festring& String)
{
  liquid* LiquidStack[4];
  liquid** Show = LiquidStack + 1;
  ushort Index = 0;
  bool Blood = false, OneBlood = true;

  for(fluidlist::const_iterator i = Fluid.begin(); i != Fluid.end(); ++i)
    {
      liquid* Liquid = (*i)->GetLiquid();
      bool LiquidBlood = Liquid->IsBlood();

      if(!LiquidBlood || !Blood)
	{
	  if(Index < 3)
	    {
	      if(LiquidBlood)
		{
		  --Show;
		  Show[0] = Liquid;
		  ++Index;
		}
	      else
		Show[Index++] = Liquid;
	    }
	  else
	    {
	      ++Index;
	      break;
	    }
	}

      if(LiquidBlood)
	{
	  if(Blood)
	    OneBlood = false;
	  else
	    Blood = true;
	}
    }

  if(Index <= 3)
    {
      if(!Blood || OneBlood)
	String << Show[0]->GetName(false, false);
      else
	String << "blood";

      if(Index == 2)
	String << " and " << Show[1]->GetName(false, false);
      else if(Index == 3)
	String << ", " << Show[1]->GetName(false, false) << " and " << Show[2]->GetName(false, false);
    }
  else
    String << "a lot of liquids";
}

/* Used only when loading fluids. Correcting RandMap here is somewhat a gum
   solution. */

void fluid::SetMotherItem(item* What)
{
  MotherItem = What;
  Image.Picture->InitRandMap();
  Image.Picture->CalculateRandMap();
}

/* Ensures the gear pictures are correct after this. ShadowPos is the armor's
   or weapon's BitmapPos in humanoid.pcx. SpecialFlags should include drawing
   information of the bodypart in question (ST_RIGHT_ARM etc). BodyArmor should
   be true iff the picture is part of a body armor, for instance armor covering
   one's shoulder. */

void fluid::CheckGearPicture(vector2d ShadowPos, ushort SpecialFlags, bool BodyArmor)
{
  HasBodyArmorPictures = BodyArmor;
  imagedata* ImagePtr;
  ulong Pixels;

  if(BodyArmor)
    {
      ushort Index = (SpecialFlags & 0x38) >> 3;

      if(Index >= BODY_ARMOR_PARTS)
	Index = 0;

      if(GearImage)
	if(GearImage[Index].ShadowPos != ShadowPos)
	  GearImage[Index].Clear(false);
	else
	  return; // the picture already exists and is correct
      else
	GearImage = new imagedata[BODY_ARMOR_PARTS];

      ImagePtr = &GearImage[Index];
      Pixels = (Image.AlphaSum * BodyArmorPartPixels[Index] / HUMAN_BODY_ARMOR_PIXELS) >> 10;
    }
  else
    {
      if(GearImage)
	if(GearImage->ShadowPos != ShadowPos)
	  GearImage->Clear(false);
	else
	  return; // the picture already exists and is correct
      else
	GearImage = new imagedata[1];

      ImagePtr = GearImage;
      ulong Pixels = Image.AlphaSum >> 10;
    }

  ImagePtr->ShadowPos = ShadowPos;
  ImagePtr->SpecialFlags = SpecialFlags;
  ImagePtr->Picture->InitRandMap();

  if(Pixels)
    ImagePtr->AddLiquidToPicture(igraph::GetHumanoidRawGraphic(),
				 Pixels,
				 Image.AlphaAverage,
				 Liquid->GetColor(),
				 MotherItem->GetFluidPixelAllowedPredicate());
}

void fluid::DrawGearPicture(bitmap* Bitmap, vector2d Pos, ulong Luminance, ushort SpecialFlags, bool AllowAnimate) const
{
  bitmap* Picture = GearImage->Picture;

  if(SpecialFlags & 0x7)
    {
      Picture->BlitAndCopyAlpha(igraph::GetFlagBuffer(), SpecialFlags);
      igraph::GetFlagBuffer()->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
    }
  else
    GearImage->Picture->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);

  if(AllowAnimate)
    GearImage->Animate(Bitmap, Pos, Luminance, SpecialFlags);
}

void fluid::DrawBodyArmorPicture(bitmap* Bitmap, vector2d Pos, ulong Luminance, ushort SpecialFlags, bool AllowAnimate) const
{
  /* We suppose body armor pictures are never rotated */

  ushort Index = (SpecialFlags & 0x38) >> 3;

  if(Index >= BODY_ARMOR_PARTS)
    Index = 0;

  GearImage[Index].Picture->AlphaBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);

  if(AllowAnimate)
    GearImage[Index].Animate(Bitmap, Pos, Luminance, 0);
}

bool fluid::imagedata::Fade()
{
  return Picture->Fade(AlphaSum, AlphaAverage, 1);
}

/* Let two pixels fall every now and then over the picture. CurrentFlags
   should include rotation info. */

void fluid::imagedata::Animate(bitmap* Bitmap, vector2d Pos, ulong Luminance, ushort CurrentFlags) const
{
  return;

  if(!AlphaSum)
    return;

  if(!DripTimer)
    {
      DripPos = Picture->RandomizePixel();

      /* DripPos != ERROR_VECTOR since AlphaSum != 0 */

      if(DripPos.Y <= 12)
	{
	  DripColor = Picture->GetPixel(DripPos);
	  DripAlpha = Picture->GetAlpha(DripPos);
	}
      else
	++DripTimer;
    }

  if(DripTimer <= 0)
    {
      vector2d TrueDripPos = DripPos;
      Rotate(TrueDripPos, 16, CurrentFlags);
      TrueDripPos.Y -= DripTimer;

      if(TrueDripPos.Y < 16)
	Bitmap->AlphaPutPixel(Pos + TrueDripPos, DripColor, Luminance, DripAlpha);

      if(TrueDripPos.Y < 15)
	{
	  ++TrueDripPos.Y;
	  Bitmap->AlphaPutPixel(Pos + TrueDripPos, DripColor, Luminance, DripAlpha);
	}
      else
	DripTimer = Min<long>(RAND() % (500000 / AlphaSum), 25000);
    }

  --DripTimer;
}

fluid::imagedata::imagedata() : Picture(new bitmap(16, 16, TRANSPARENT_COLOR)), DripTimer(0), AlphaSum(0)
{
  Picture->CreateAlphaMap(0);
}

fluid::imagedata::~imagedata()
{
  delete Picture;
}

void fluid::imagedata::Save(outputfile& SaveFile) const
{
  SaveFile << Picture << AlphaSum << ShadowPos << SpecialFlags;
}

void fluid::imagedata::Load(inputfile& SaveFile)
{
  SaveFile >> Picture >> AlphaSum >> ShadowPos >> SpecialFlags;
}

/* Shadow and this->ShadowPos specify the location of the raw image of
   MotherItem of which shape the pixels are bound. If the image is meant
   to be drawn on the ground, Shadow should be zero. The alphas of the
   pixels will be on average AlphaSuggestion. PixelPredicate is used
   to determine whether pixels of the Shadow are allowed to be covered
   by the fluid. It is not used if Shadow == 0. */

void fluid::imagedata::AddLiquidToPicture(const colorizablebitmap* Shadow, ulong Pixels, ulong AlphaSuggestion, ushort Color, pixelpredicate PixelPredicate)
{
  DripTimer = 0;
  ulong Lumps = Pixels - (Pixels << 3) / 9; // ceil[Pixels/9]
  ulong RoomForPixels = (Lumps << 3) + Lumps;
  ushort Red = GetRed16(Color);
  ushort Green = GetGreen16(Color);
  ushort Blue = GetBlue16(Color);
  bool** ValidityMap = igraph::GetBodyBitmapValidityMap(SpecialFlags);

  for(ulong c = 0; c < Lumps; ++c)
    {
      /* Note! Don't ever give an item a fully transparent picture to make it
	 invisible or something, or this will crash. */

      vector2d Cords(1 + RAND() % 14, 1 + RAND() % 14);
      ulong Counter = 0;

      if(Shadow)
	while((Shadow->*PixelPredicate)(ShadowPos + Cords) || !ValidityMap[Cords.X][Cords.Y])
	  {
	    Cords.X = 1 + RAND() % 14;
	    Cords.Y = 1 + RAND() % 14;

	    if(++Counter == 256)
	      {
		AlphaAverage = Picture->CalculateAlphaAverage();
		return;
	      }
	  }

      if(AlphaSuggestion < 25) /// check!
	int eskoo = 2;

      Picture->PutPixel(Cords, Color);
      long Alpha = Limit<long>(AlphaSuggestion - 25 + RAND() % 50, 0, 0xFF);
      AlphaSum += Alpha - Picture->GetAlpha(Cords);
      Picture->SetAlpha(Cords, Alpha);
      Picture->SafeUpdateRandMap(Cords, true);
      --Pixels;
      --RoomForPixels;

      for(ushort d = 0; d < 8; ++d)
	{
	  if(Pixels > RAND() % RoomForPixels)
	    {
	      vector2d Pos = Cords + game::GetMoveVector(d);

	      if(!Shadow || (!(Shadow->*PixelPredicate)(ShadowPos + Pos) && ValidityMap[Pos.X][Pos.Y]))
		{
		  --Pixels;
		  Picture->PutPixel(Pos, MakeRGB16(Limit<short>(Red - 25 + RAND() % 51, 0, 0xFF),
						   Limit<short>(Green - 25 + RAND() % 51, 0, 0xFF),
						   Limit<short>(Blue - 25 + RAND() % 51, 0, 0xFF)));

		  long Alpha = Limit<long>(AlphaSuggestion - 25 + RAND() % 50, 0, 0xFF);
		  AlphaSum += Alpha - Picture->GetAlpha(Pos);
		  Picture->SetAlpha(Pos, Alpha);
		  Picture->SafeUpdateRandMap(Pos, true);

		  if(!Pixels) // implies c + 1 == Lumps
		    break;
		}
	    }

	  --RoomForPixels;
	}
    }

  AlphaAverage = Picture->CalculateAlphaAverage();
}

/* Remakes all images. Usually decreases, and never increases, the liquid's
   volume */

void fluid::Redistribute()
{
  bool InitRandMap = !!MotherItem;
  Image.Clear(InitRandMap);

  if(GearImage)
    if(HasBodyArmorPictures)
      for(ushort c = 0; c < BODY_ARMOR_PARTS; ++c)
	GearImage[c].Clear(InitRandMap);
    else
      GearImage->Clear(InitRandMap);

  AddLiquid(Liquid->GetVolume());
}

void fluid::imagedata::Clear(bool InitRandMap)
{
  Picture->ClearToColor(TRANSPARENT_COLOR);
  Picture->FillAlpha(0);
  AlphaSum = 0;

  if(InitRandMap)
    Picture->InitRandMap();
}

material* fluid::RemoveMaterial(material*)
{
  Destroy();
  return 0;
}

void fluid::Destroy()
{
  if(MotherItem)
    {
      if(!MotherItem->Exists())
	return;

      MotherItem->RemoveFluid(this);
    }
  else
    LSquareUnder->RemoveFluid(this);

  SendToHell();
}
