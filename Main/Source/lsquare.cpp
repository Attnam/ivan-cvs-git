/* Compiled through levelset.cpp */

lsquare::lsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), GLTerrain(0), OLTerrain(0), Emitation(0), DivineMaster(0), RoomIndex(0), TemporaryEmitation(0), Fluid(0), Memorized(0), MemorizedUpdateRequested(true)
{
  Stack = new stack(this, 0, CENTER, false);
  SideStack[DOWN] = new stack(this, 0, DOWN, false);
  SideStack[LEFT] = new stack(this, 0, LEFT, false);
  SideStack[UP] = new stack(this, 0, UP, false);
  SideStack[RIGHT] = new stack(this, 0, RIGHT, false);
}

lsquare::~lsquare()
{
  delete GLTerrain;
  delete OLTerrain;
  delete Stack;

  for(ushort c = 0; c < 4; ++c)
    delete SideStack[c];

  delete Fluid;
  delete Memorized;
}

void lsquare::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0 && !game::IsGenerating())
    {
      CalculateEmitation();
      Emitate();
    }
}

void lsquare::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation && !game::IsGenerating())
    {
      ulong Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	ReEmitate(EmitationUpdate);
    }
}

void lsquare::CalculateEmitation()
{
  Emitation = Stack->GetEmitation();

  for(ushort c = 0; c < 4; ++c)
    {
      stack* Stack = GetSideStackOfAdjacentSquare(c);

      if(Stack)
	game::CombineLights(Emitation, Stack->GetEmitation());
    }

  if(Character)
    game::CombineLights(Emitation, Character->GetEmitation());

  if(GLTerrain)
    game::CombineLights(Emitation, GLTerrain->GetEmitation());

  if(OLTerrain)
    game::CombineLights(Emitation, OLTerrain->GetEmitation());

  game::CombineLights(Emitation, TemporaryEmitation);
}

void lsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      if(!IsDark())
	{
	  DrawStaticContents(Memorized, vector2d(0, 0), MakeRGB24(128, 128, 128), false);
	  igraph::GetFOWGraphic()->MaskedBlit(Memorized, 0, 0, 0, 0, 16, 16, uchar(0), 0);
	}
      else
	igraph::GetFOWGraphic()->Blit(Memorized);

      MemorizedUpdateRequested = false;
    }
}

void lsquare::DrawStaticContents(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool RealDraw) const
{
  GLTerrain->Draw(Bitmap, Pos, Luminance, false, RealDraw);

  if(Fluid)
    Fluid->Draw(Bitmap, Pos, Luminance, true, RealDraw);
	
  OLTerrain->Draw(Bitmap, Pos, Luminance, true, RealDraw);

  if(OLTerrain->IsWalkable())
    {
      Stack->Draw(game::GetPlayer(), Bitmap, Pos, Luminance, true, RealDraw, RealDraw);

      for(ushort c = 0; c < 4; ++c)
	{
	  stack* Stack = GetSideStackOfAdjacentSquare(c);

	  if(Stack)
	    Stack->Draw(game::GetPlayer(), Bitmap, Pos, Luminance, true, RealDraw, RealDraw);
	}
    }
}

void lsquare::Draw()
{
  if(NewDrawRequested || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(!IsDark() || game::SeeWholeMapCheatIsActive())
	{
	  ulong RealLuminance = game::SeeWholeMapCheatIsActive() ? configuration::GetContrastLuminance() : configuration::ApplyContrastTo(Luminance);
	  DrawStaticContents(DOUBLE_BUFFER, BitPos, RealLuminance, true);

	  if(Character && (Character->CanBeSeenByPlayer() || game::SeeWholeMapCheatIsActive()))
	    Character->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true, true);
	}
      else
	{
	  DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

	  if(Character && Character->CanBeSeenByPlayer())
	    Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), false, true);
	}

      NewDrawRequested = false;
    }
}

void lsquare::Emitate()
{
  if(game::IsDark(GetEmitation()))
    return;

  ushort Radius = GetLevel()->CalculateMinimumEmitationRadius(GetEmitation());

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterEmitation(GetEmitation());
  game::SetCurrentEmitterPos(GetPos());

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, game::EmitationHandler);
}

void lsquare::ReEmitate(ulong OldEmitation)
{
  if(game::IsDark(OldEmitation))
    return;

  ushort Radius = GetLevel()->CalculateMinimumEmitationRadius(OldEmitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterEmitation(GetEmitation());
  game::SetCurrentEmitterPos(GetPos());

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, game::EmitationHandler);
}

void lsquare::Noxify()
{
  ushort Radius = GetLevel()->CalculateMinimumEmitationRadius(GetEmitation());

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::SetCurrentEmitterPos(GetPos());

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, GetLevel()->GetBorder(), Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, game::NoxifyHandler);
}

void lsquare::ForceEmitterNoxify()
{
  for(ushort c = 0; c < Emitter.size(); ++c)
    GetNearLSquare(Emitter[c].Pos)->Noxify();
}

void lsquare::ForceEmitterEmitation()
{
  for(ushort c = 0; c < Emitter.size(); ++c)
    GetNearLSquare(Emitter[c].Pos)->Emitate();
}

void lsquare::NoxifyEmitter(vector2d Dir)
{
  /*
   * DO NOT remove the iterator, or ForceEmitterEmitation() will not work properly!
   * (had to add add this comment when I came here for the nth time and thought not to do it is really stupid...)
   */

  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(!Luminance || game::CompareLights(i->DilatedEmitation, Luminance) < 0)
	  i->DilatedEmitation = 0;
	else if(!OLTerrain->IsWalkable())
	  {
	    i->DilatedEmitation = 0;
	    NewDrawRequested = true;

	    if(LastSeen == game::GetLOSTurns())
	      game::SendLOSUpdateRequest();
	  }
	else
	  {
	    ulong OldLuminance = Luminance;
	    i->DilatedEmitation = 0;
	    CalculateLuminance();

	    if(OldLuminance == Luminance)
	      return;

	    NewDrawRequested = true;

	    if(!Luminance)
	      {
		MemorizedUpdateRequested = true;
		DescriptionChanged = true;

		if(LastSeen == game::GetLOSTurns())
		  game::SendLOSUpdateRequest();
	      }
	  }

	return;
      }
}

uchar lsquare::CalculateBitMask(vector2d Dir) const
{
  uchar BitMask = 0;

  #define IW(X, Y) GetNearLSquare(Pos + vector2d(X, Y))->GetOLTerrain()->IsWalkable()

  if(Dir.X < Pos.X)
    {
      if(Dir.Y < Pos.Y)
	{
	  BitMask |= 1;
	  if(IW(-1,  0)) BitMask |= 4;
	  if(IW( 0, -1)) BitMask |= 2;
	}

      if(Dir.Y == Pos.Y)
	BitMask |= 5;

      if(Dir.Y > Pos.Y)
	{
	  BitMask |= 4;
	  if(IW(-1, 0)) BitMask |= 1;
	  if(IW( 0, 1)) BitMask |= 8;
	}
    }

  if(Dir.X == Pos.X)
    {
      if(Dir.Y < Pos.Y)
	BitMask |= 3;

      if(Dir.Y == Pos.Y)
	BitMask |= 15;

      if(Dir.Y > Pos.Y)
	BitMask |= 12;
    }

  if(Dir.X > Pos.X)
    {
      if(Dir.Y < Pos.Y)
	{
	  BitMask |= 2;
	  if(IW(1,  0)) BitMask |= 8;
	  if(IW(0, -1)) BitMask |= 1;
	}

      if(Dir.Y == Pos.Y)
	BitMask |= 10;

      if(Dir.Y > Pos.Y)
	{
	  BitMask |= 8;
	  if(IW(1, 0)) BitMask |= 2;
	  if(IW(0, 1)) BitMask |= 4;
	}
    }

  return BitMask;
}

void lsquare::AlterLuminance(vector2d Dir, ulong NewLuminance)
{
  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(i->DilatedEmitation == NewLuminance)
	  return;

	if(!game::IsDark(NewLuminance))
	  {
	    if(NewLuminance != Luminance)
	      {
		if(!OLTerrain->IsWalkable())
		  {
		    i->DilatedEmitation = NewLuminance;
		    NewDrawRequested = true;

		    if(LastSeen == game::GetLOSTurns())
		      game::SendLOSUpdateRequest();

		    return;
		  }

		ulong OldLuminance = Luminance;
		i->DilatedEmitation = NewLuminance;
  		CalculateLuminance();

		if(OldLuminance == Luminance)
		  return;

		NewDrawRequested = true;

		if(!OldLuminance)
		  {
		    MemorizedUpdateRequested = true;
		    DescriptionChanged = true;

		    if(LastSeen == game::GetLOSTurns())
		      game::SendLOSUpdateRequest();
		  }
	      }
	    else
	      i->DilatedEmitation = NewLuminance;
	  }
	else
	  {
	    if(!Luminance || game::CompareLights(i->DilatedEmitation, Luminance) < 0)
	      Emitter.erase(i);
	    else if(!OLTerrain->IsWalkable())
	      {
		Emitter.erase(i);
		NewDrawRequested = true;

		if(LastSeen == game::GetLOSTurns())
		  game::SendLOSUpdateRequest();
	      }
	    else
	      {
		ulong OldLuminance = Luminance;
		Emitter.erase(i);
		CalculateLuminance();

		if(OldLuminance == Luminance)
		  return;

		NewDrawRequested = true;

		if(!Luminance)
		  {
		    MemorizedUpdateRequested = true;
		    DescriptionChanged = true;

		    if(LastSeen == game::GetLOSTurns())
		      game::SendLOSUpdateRequest();
		  }
	      }
	  }

	return;
      }

  /* Emitter was not found, add it. */

  if(!game::IsDark(NewLuminance))
    {
      if(game::CompareLights(NewLuminance, Luminance) > 0)
	{
	  if(!OLTerrain->IsWalkable())
	    {
	      Emitter.push_back(emitter(Dir, NewLuminance));
	      NewDrawRequested = true;

	      if(LastSeen == game::GetLOSTurns())
		game::SendLOSUpdateRequest();

	      return;
	    }

	  ulong OldLuminance = Luminance;
	  Emitter.push_back(emitter(Dir, NewLuminance));
  	  CalculateLuminance();

	  if(OldLuminance == Luminance)
	    return;

	  NewDrawRequested = true;

	  if(!OldLuminance)
	    {
	      DescriptionChanged = true;
	      MemorizedUpdateRequested = true;

	      if(LastSeen == game::GetLOSTurns())
		game::SendLOSUpdateRequest();
	    }
	}
      else
	Emitter.push_back(emitter(Dir, NewLuminance));
    }
}

bool lsquare::Open(character* Opener)
{
  return GetStack()->Open(Opener) || GetOLTerrain()->Open(Opener); 
}

bool lsquare::Close(character* Closer)
{
  if(!GetStack()->GetItems() && !Character)
    return GetOLTerrain()->Close(Closer);
  else
    {
      ADD_MESSAGE("There's something in the way...");
      return false;
    }
}

void lsquare::Save(outputfile& SaveFile) const
{
  Stack->Save(SaveFile); // This must be before square::Save! (Note: This comment is years old. It's probably obsolete)
  square::Save(SaveFile);
  SaveFile << GLTerrain << OLTerrain;

  for(ushort c = 0; c < 4; ++c)
    SideStack[c]->Save(SaveFile);

  SaveFile << Emitter << Fluid << Emitation << DivineMaster << Engraved << RoomIndex << Luminance;

  if(LastSeen)
    Memorized->Save(SaveFile);
}

void lsquare::Load(inputfile& SaveFile)
{
  Stack->Load(SaveFile); // This must be before square::Load! (Note: This comment is years old. It's probably obsolete)
  Stack->SetMotherSquare(this);
  square::Load(SaveFile);
  SaveFile >> GLTerrain >> OLTerrain;

  for(ushort c = 0; c < 4; ++c)
    {
      SideStack[c]->Load(SaveFile);
      SideStack[c]->SetMotherSquare(this);
    }

  SaveFile >> Emitter >> Fluid >> Emitation >> DivineMaster >> Engraved >> RoomIndex >> Luminance;

  if(LastSeen)
    {
      Memorized = new bitmap(16, 16);
      Memorized->Load(SaveFile);
    }
}

void lsquare::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation) // ho ho ho /me is very funny. - Anonymous
{
  if(!Amount)
    return;
	
  if(!GetFluid())
    SetFluid(new fluid(this));

  GetFluid()->SpillFluid(Amount, Color, Lumpiness, Variation);
  SendMemorizedUpdateRequest();
  SendNewDrawRequest();
}

void lsquare::CalculateLuminance()
{
  Luminance = *GetLevel()->GetLevelScript()->GetAmbientLight();

  if(OLTerrain->IsWalkable())
    {
      for(ushort c = 0; c < Emitter.size(); ++c)
	game::CombineLights(Luminance, Emitter[c].DilatedEmitation);
    }
  else
    for(ushort c = 0; c < Emitter.size(); ++c)
      if(CalculateBitMask(Emitter[c].Pos) & CalculateBitMask(game::GetPlayer()->GetPos()))
	game::CombineLights(Luminance, Emitter[c].DilatedEmitation);
}

ulong lsquare::GetRawLuminance() const
{
  ulong Luminance = *GetLevel()->GetLevelScript()->GetAmbientLight();

  for(ushort c = 0; c < Emitter.size(); ++c)
    game::CombineLights(Luminance, Emitter[c].DilatedEmitation);

  return Luminance;
}

void lsquare::AddCharacter(character* Guy)
{
  if(Character)
    ABORT("Overgrowth of square population detected!");

  Character = Guy;
  Guy->SetSquareUnder(this);
  SignalEmitationIncrease(Guy->GetEmitation());
  NewDrawRequested = true;

  if(Guy->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::Clean()
{
  GetStack()->Clean();

  for(ushort c = 0; c < 4; ++c)
    GetSideStack(c)->Clean();
}

void lsquare::RemoveCharacter()
{
  if(Character)
    {
      if(Character->IsAnimated())
	DecAnimatedEntities();

      character* Backup = Character;
      SetCharacter(0);
      SignalEmitationDecrease(Backup->GetEmitation());
      NewDrawRequested = true;
    }
}

void lsquare::UpdateMemorizedDescription(bool Cheat)
{
  if(DescriptionChanged || Cheat)
    {
      if(!IsDark() || Cheat)
	{
	  MemorizedDescription.resize(0);

	  if(GetOTerrain()->IsWalkable())
	    {
	      bool Anything = false;

	      if(OLTerrain->GetNameSingular().length())
		{
		  OLTerrain->AddName(MemorizedDescription, INDEFINITE);
		  Anything = true;
		}

	      ushort VisibleItems = GetStack()->GetItems(game::GetPlayer(), Cheat);

	      if(VisibleItems)
		{
		  if(Anything)
		    MemorizedDescription << " and ";

		  if(VisibleItems == 1)
		    GetStack()->GetBottomItem(game::GetPlayer(), Cheat)->AddName(MemorizedDescription, INDEFINITE);
		  else
		    MemorizedDescription = "many items";

		  MemorizedDescription << " on ";
		  Anything = true;
		}
	      else if(Anything)
		MemorizedDescription << " on ";

	      GLTerrain->AddName(MemorizedDescription, INDEFINITE);
	      ushort Items = 0;

	      for(ushort c = 0; c < 4; ++c)
		{
		  stack* Stack = GetSideStackOfAdjacentSquare(c);

		  if(Stack)
		    Items += Stack->GetItems(game::GetPlayer(), Cheat);
		}

	      if(Items > 1)
		MemorizedDescription << " and many items on the wall";
	      else if(Items == 1)
		{
		  MemorizedDescription << " and ";

		  for(ushort c = 0; c < 4; ++c)
		    {
		      stack* Stack = GetSideStackOfAdjacentSquare(c);

		      if(Stack && Stack->GetItems(game::GetPlayer(), Cheat))
			Stack->GetBottomItem(game::GetPlayer(), Cheat)->AddName(MemorizedDescription, INDEFINITE);
		    }

		  MemorizedDescription << " on the wall";
		}
	    }
	  else
	    OLTerrain->AddName(MemorizedDescription, INDEFINITE);

	  if(Cheat)
	    MemorizedDescription << " (pos " << Pos.X << ":" << Pos.Y << ")";
	}
      else
	MemorizedDescription = "darkness";

      DescriptionChanged = false;
    }
}

void lsquare::BeKicked(character* Kicker, item* Boot, float KickDamage, float KickToHitValue, short Success, bool Critical, bool ForceHit)
{
  if(RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->KickSquare(Kicker, this);

  GetStack()->BeKicked(Kicker, ushort(KickDamage));

  if(GetCharacter())
    GetCharacter()->BeKicked(Kicker, Boot, KickDamage, KickToHitValue, Success, Critical, ForceHit);

  GetOLTerrain()->BeKicked(Kicker, ushort(KickDamage));
}

bool lsquare::CanBeDug() const
{
  if((GetPos().X == 0 || GetPos().Y == 0 || GetPos().X == GetLevel()->GetXSize() - 1 || GetPos().Y == GetLevel()->GetYSize() - 1) && !*GetLevel()->GetLevelScript()->IsOnGround())
    {
      ADD_MESSAGE("Somehow you feel that by digging this square you would collapse the whole dungeon.");
      return false;
    }
  else
    return true;
}

void lsquare::ChangeLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  ChangeGLTerrain(NewGround);
  ChangeOLTerrain(NewOver);
}

void lsquare::ChangeGLTerrain(glterrain* NewGround)
{
  if(GetGLTerrain()->IsAnimated())
    DecAnimatedEntities();

  delete GetGLTerrain();
  GLTerrain = NewGround;
  NewGround->SetLSquareUnder(this);
  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::ChangeOLTerrain(olterrain* NewOver)
{
  if(GetOLTerrain()->IsAnimated())
    DecAnimatedEntities();

  delete GetOLTerrain();
  OLTerrain = NewOver;
  NewOver->SetLSquareUnder(this);
  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;

  if(NewOver->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::SetLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  SetGLTerrain(NewGround);
  SetOLTerrain(NewOver);
}

void lsquare::SetGLTerrain(glterrain* NewGround)
{
  GLTerrain = NewGround;
  NewGround->SetLSquareUnder(this);

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::SetOLTerrain(olterrain* NewOver)
{
  OLTerrain = NewOver;
  NewOver->SetLSquareUnder(this);

  if(NewOver->IsAnimated())
    IncAnimatedEntities();
}

gterrain* lsquare::GetGTerrain() const
{
  return GLTerrain;
}

oterrain* lsquare::GetOTerrain() const
{
  return OLTerrain;
}

void lsquare::ApplyScript(squarescript* SquareScript, room* Room)
{
  bool* AttachRequired = SquareScript->AttachRequired();

  if(AttachRequired && *AttachRequired)
    GetLevel()->AttachPos(Pos);

  uchar* EntryIndex = SquareScript->GetEntryIndex();

  if(EntryIndex)
    GetLevel()->SetEntryPos(*EntryIndex, Pos);

  contentscript<character>* CharacterScript = SquareScript->GetCharacter();

  if(CharacterScript)
    {
      character* Char = CharacterScript->Instantiate();

      if(!Char->GetTeam())
	Char->SetTeam(game::GetTeam(*GetLevel()->GetLevelScript()->GetTeamDefault()));

      AddCharacter(Char);
      Char->CreateHomeData();

      if(Room)
	{
	  bool* IsMaster = CharacterScript->IsMaster();

	  if(IsMaster && *IsMaster)
	    Room->SetMasterID(Char->GetID());
	}
    }

  std::vector<contentscript<item> >* Items = SquareScript->GetItems();

  if(Items)
    for(ushort c = 0; c < Items->size(); ++c)
      {
	stack* Stack;
	item* Item = (*Items)[c].Instantiate();

	if(Item)
	  {
	    uchar* SideStackIndex = (*Items)[c].GetSideStackIndex();

	    if(!SideStackIndex)
	      Stack = GetStack();
	    else
	      {
		Item->SignalSquarePositionChange(*SideStackIndex);
		Stack = GetSideStack(*SideStackIndex);
	      }

	    Stack->AddItem(Item);
	    Item->SpecialGenerationHandler();
	  }
      }

  contentscript<glterrain>* GLTerrainScript = SquareScript->GetGTerrain();

  if(GLTerrainScript)
    ChangeGLTerrain(GLTerrainScript->Instantiate());

  contentscript<olterrain>* OLTerrainScript = SquareScript->GetOTerrain();

  if(OLTerrainScript)
    {
      olterrain* Terrain = OLTerrainScript->Instantiate();
      ChangeOLTerrain(Terrain);
    }
}

bool lsquare::CanBeSeenByPlayer(bool IgnoreDarkness) const
{
  return (IgnoreDarkness || !IsDark()) && LastSeen == game::GetLOSTurns();
}

bool lsquare::CanBeSeenFrom(vector2d FromPos, ulong MaxDistance, bool IgnoreDarkness) const
{
  return (GetPos() - FromPos).GetLengthSquare() <= MaxDistance && (IgnoreDarkness || !IsDark())
      && ((Character && Character->IsPlayer() && GetNearSquare(FromPos)->CanBeSeenByPlayer(true))
      || femath::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, game::EyeHandler));
}

void lsquare::MoveCharacter(lsquare* To)
{
  if(Character)
    {
      if(To->Character)
	ABORT("Overgrowth of square population detected!");

      character* Movee = Character;
      ulong Emit = Movee->GetEmitation();
      SetCharacter(0);
      To->Character = Movee;
      Movee->SetSquareUnder(To);
      To->SignalEmitationIncrease(Emit);
      SignalEmitationDecrease(Emit);
      NewDrawRequested = true;
      To->NewDrawRequested = true;

      if(Movee->IsAnimated())
	{
	  DecAnimatedEntities();
	  To->IncAnimatedEntities();
	}

      To->StepOn(Movee, this);
    }
}

void lsquare::StepOn(character* Stepper, square* ComingFrom)
{
  if(RoomIndex && ((lsquare*)ComingFrom)->GetRoomIndex() != RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->Enter(Stepper);

  GetGLTerrain()->StepOn(Stepper);
  GetOLTerrain()->StepOn(Stepper);

  if(!Stepper->CanFly())
    GetStack()->CheckForStepOnEffect(Stepper);
}

void lsquare::SwapCharacter(lsquare* With)
{
  if(Character)
    if(!With->Character)
      MoveCharacter(With);
    else
      {
	character* MoveeOne = Character, * MoveeTwo = With->Character;;
	ulong EmitOne = MoveeOne->GetEmitation();
	ulong EmitTwo = MoveeTwo->GetEmitation();
	SetCharacter(MoveeTwo);
	With->SetCharacter(MoveeOne);
	MoveeTwo->SetSquareUnder(this);
	MoveeOne->SetSquareUnder(With);
	SignalEmitationIncrease(EmitTwo);
	With->SignalEmitationIncrease(EmitOne);
	SignalEmitationDecrease(EmitOne);
	With->SignalEmitationDecrease(EmitTwo);
	NewDrawRequested = true;
	With->NewDrawRequested = true;

	if(MoveeOne->IsAnimated())
	  {
	    DecAnimatedEntities();
	    With->IncAnimatedEntities();
	  }

	if(MoveeTwo->IsAnimated())
	  {
	    IncAnimatedEntities();
	    With->DecAnimatedEntities();
	  }

	With->StepOn(MoveeOne, this);
	StepOn(MoveeTwo, With);
      }
  else
    if(With->Character)
      With->MoveCharacter(this);
}

void lsquare::ReceiveVomit(character* Who, ushort Amount)
{
  SpillFluid(Amount, MakeRGB16(10, 230, 10), 5, 60);
  GetOLTerrain()->ReceiveVomit(Who);
}

void lsquare::SetTemporaryEmitation(ulong What)
{
  ulong Old = TemporaryEmitation;
  TemporaryEmitation = 0;
  SignalEmitationDecrease(Old);
  TemporaryEmitation = What;
  SignalEmitationIncrease(What);
}

void lsquare::ChangeOLTerrainAndUpdateLights(olterrain* NewTerrain)
{
  bool WasWalkable = GetOLTerrain()->IsWalkable();

  if(WasWalkable && !NewTerrain->IsWalkable())
    ForceEmitterNoxify();

  ulong OldEmit = GetOLTerrain()->GetEmitation();
  ChangeOLTerrain(NewTerrain);
  SignalEmitationIncrease(NewTerrain->GetEmitation());
  SignalEmitationDecrease(OldEmit);

  for(ushort c = 0; c < 4; ++c)
    {
      for(stackiterator i = GetSideStack(c)->GetBottom(); i.HasItem(); ++i)
	i->SignalSquarePositionChange(CENTER);

      GetSideStack(c)->MoveItemsTo(GetStack());
    }

  if(WasWalkable != GetOLTerrain()->IsWalkable())
    {
      ForceEmitterEmitation();
      CalculateLuminance();

      if(LastSeen == game::GetLOSTurns())
	game::SendLOSUpdateRequest();
    }
}

void lsquare::DrawParticles(ulong Color)
{
  if(GetPos().X < game::GetCamera().X
  || GetPos().Y < game::GetCamera().Y
  || GetPos().X >= game::GetCamera().X + game::GetScreenSize().X
  || GetPos().Y >= game::GetCamera().Y + game::GetScreenSize().Y
  || !CanBeSeenByPlayer(true)
  || Color == TRANSPARENT_COLOR)
    return;

  clock_t StartTime = clock();
  game::DrawEverythingNoBlit();

  if(Color & RANDOM_COLOR)
    Color = MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190);

  vector2d Pos = game::CalculateScreenCoordinates(GetPos());

  for(ushort c = 0; c < 10; ++c)
    DOUBLE_BUFFER->PutPixel(Pos + vector2d(1 + RAND() % 14, 1 + RAND() % 14), Color);

  graphics::BlitDBToScreen();
  NewDrawRequested = true; // Clean the pixels from the screen afterwards
  while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
}

void lsquare::RemoveFluid()
{
  if(GetFluid())
    {
      fluid* Backup = GetFluid();
      SetFluid(0);
      SignalEmitationDecrease(Backup->GetEmitation());
    }
}

bool lsquare::DipInto(item* Thingy, character* Dipper)
{
  if(GetGLTerrain()->IsDipDestination() || GetOLTerrain()->IsDipDestination())
    {
      room* Room = GetRoom();

      if(Room && Room->HasDipHandler() && !Room->Dip(Dipper))
	return false;
    }
  
  if(GetGLTerrain()->DipInto(Thingy, Dipper) || GetOLTerrain()->DipInto(Thingy, Dipper))
    return true;
  else
    {
      if(Dipper->IsPlayer())
	ADD_MESSAGE("You cannot dip %s on that square!", Thingy->CHAR_NAME(DEFINITE));

      return false;
    }
}

// return true if key fits someplace

bool lsquare::TryKey(item* Key, character* Applier)
{
  if(GetOLTerrain()->TryKey(Key, Applier))
    return true;

  if(!GetOLTerrain()->HasKeyHole() && !GetStack()->TryKey(Key,Applier))
    {
      ADD_MESSAGE("There's no place here to put the key in!");
      return false;
    }
  return true;
}

void lsquare::SetLastSeen(ulong What)
{
  if(LastSeen == What)
    return;

  if(LastSeen < What - 1)
    NewDrawRequested = true;

  if(!OLTerrain->IsWalkable())
    {
      ulong OldLuminance = Luminance;
      CalculateLuminance();

      if(OldLuminance != Luminance)
	{
	  NewDrawRequested = true;

	  if(IsDark() != game::IsDark(OldLuminance))
	    {
	      MemorizedUpdateRequested = true;
	      DescriptionChanged = true;
	    }
	}
    }

  if(IsDark())
    {
      short XDist = Pos.X - game::GetPlayer()->GetPos().X;

      if(XDist < -1 || XDist > 1)
	return;

      short YDist = Pos.Y - game::GetPlayer()->GetPos().Y;

      if(YDist < -1 || YDist > 1)
	return;
    }

  if(!LastSeen)
    Memorized = new bitmap(16, 16);

  LastSeen = What;
  UpdateMemorized();
  UpdateMemorizedDescription();
}

#include "char.h"

void lsquare::DrawMemorized()
{
  if(NewDrawRequested || LastSeen == game::GetLOSTurns() - 1)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(LastSeen)
	Memorized->Blit(DOUBLE_BUFFER, 0, 0, BitPos, 16, 16, configuration::GetContrastLuminance());
      else
	DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

      if(Character && Character->CanBeSeenByPlayer())
	Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), LastSeen != 0, true);

      NewDrawRequested = false;
    }
}

bool lsquare::IsDangerousForAIToStepOn(const character* Who) const
{
  return !Who->CanFly() && GetStack()->IsDangerousForAIToStepOn(Who); 
}

stack* lsquare::GetSideStackOfAdjacentSquare(ushort Index) const
{
  switch(Index)
    {
    case LEFT: return Pos.X ? GetNearLSquare(Pos + vector2d(-1, 0))->GetSideStack(RIGHT) : 0;
    case RIGHT: return Pos.X < GetLevel()->GetXSize() - 1 ? GetNearLSquare(Pos + vector2d(1, 0))->GetSideStack(LEFT) : 0;
    case UP: return Pos.Y ? GetNearLSquare(Pos + vector2d(0, -1))->GetSideStack(DOWN) : 0;
    case DOWN: return Pos.Y < GetLevel()->GetYSize() - 1 ? GetNearLSquare(Pos + vector2d(0, 1))->GetSideStack(UP) : 0;
    default: return 0;
    }
}

void lsquare::SendMemorizedUpdateRequest()
{
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;

  if(CanBeSeenByPlayer())
    {
      UpdateMemorized();
      UpdateMemorizedDescription();
    }
}

void lsquare::KickAnyoneStandingHereAway()
{
  if(Character)
    {
      vector2d Pos = GetLevel()->GetNearestFreeSquare(Character, GetPos());

      if(Pos == ERROR_VECTOR)
	Pos = GetLevel()->GetRandomSquare(Character);

      GetLevel()->AddCharacter(Pos, Character);
      RemoveCharacter();
    }
}

outputfile& operator<<(outputfile& SaveFile, const emitter& Emitter)
{
  SaveFile << Emitter.Pos << Emitter.DilatedEmitation;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, emitter& Emitter)
{
  SaveFile >> Emitter.Pos >> Emitter.DilatedEmitation;
  return SaveFile;
}

void lsquare::SpillFluid(character* Spiller, material* Liquid, ushort HitPercent)
{
  SpillFluid(1, Liquid->GetColor(), 5, 60);

  if(GetCharacter() && HitPercent + RAND() % 100 > 99)
    {
      if(Spiller)
	Spiller->Hostility(GetCharacter());

      GetCharacter()->ReceiveFluidSpill(Liquid, HitPercent);
    }

  GetStack()->ReceiveFluidSpill(Liquid);
  delete Liquid;
}

bool lsquare::IsDark() const
{
  return !Luminance || (GetRed24(Luminance) < LIGHT_BORDER && GetGreen24(Luminance) < LIGHT_BORDER && GetBlue24(Luminance) < LIGHT_BORDER);
}

void lsquare::AddItem(item* Item)
{
  Stack->AddItem(Item);
}

vector2d lsquare::DrawLightning(vector2d StartPos, ulong Color, uchar Direction)
{
  if(GetPos().X < game::GetCamera().X
  || GetPos().Y < game::GetCamera().Y
  || GetPos().X >= game::GetCamera().X + game::GetScreenSize().X
  || GetPos().Y >= game::GetCamera().Y + game::GetScreenSize().Y
  || !CanBeSeenByPlayer(true))
    switch(Direction)
      {
      case 1: return vector2d(RAND() & 15, 15);
      case 3: return vector2d(15, RAND() & 15);
      case 4: return vector2d(0, RAND() & 15);
      case 6: return vector2d(RAND() & 15, 0);
      default: return StartPos;
      }

  clock_t StartTime = clock();
  bitmap Empty(16, 16, TRANSPARENT_COLOR);

  if(Color & RANDOM_COLOR)
    Color = MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190);

  if(Direction != YOURSELF)
    {
      while(!Empty.CreateLightning(StartPos, game::GetMoveVector(Direction), 16, Color));
      vector2d EndPos(0, 0);

      switch(Direction)
	{
	case 0: EndPos = vector2d(0, 0); break;
	case 1: EndPos = vector2d(RAND() & 15, 0); StartPos = vector2d(EndPos.X, 15); break;
	case 2: EndPos = vector2d(15, 0); break;
	case 3: EndPos = vector2d(0, RAND() & 15); StartPos = vector2d(15, EndPos.Y); break;
	case 4: EndPos = vector2d(15, RAND() & 15); StartPos = vector2d(0, EndPos.Y); break;
	case 5: EndPos = vector2d(0, 15); break;
	case 6: EndPos = vector2d(RAND() & 15, 15); StartPos = vector2d(EndPos.X, 0); break;
	case 7: EndPos = vector2d(15, 15); break;
	}

      while(!Empty.CreateLightning(EndPos, -game::GetMoveVector(Direction), NO_LIMIT, Color));
    }
  else
    while(!Empty.CreateLightning(StartPos, vector2d(0, 0), 10, Color));

  game::DrawEverythingNoBlit();
  Empty.MaskedBlit(DOUBLE_BUFFER, 0, 0, game::CalculateScreenCoordinates(GetPos()), 16, 16);
  graphics::BlitDBToScreen();
  NewDrawRequested = true;
  while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
  return StartPos;
}

bool lsquare::Polymorph(character* Zapper, const std::string&, uchar)
{
  GetStack()->Polymorph();
  GetOLTerrain()->Polymorph(Zapper);
  character* Character = GetCharacter();

  if(Character)
    {
      if(Character != Zapper && Character->GetTeam() != Zapper->GetTeam())
	Zapper->Hostility(Character);

      Character->PolymorphRandomly(0, 10000, 5000 + RAND() % 5000);
    }

  return false;
}

bool lsquare::Strike(character* Zapper, const std::string& DeathMsg, uchar Direction) 
{
  ushort Damage = 50 + RAND() % 21 - RAND() % 21;
  GetStack()->ReceiveDamage(Zapper, Damage, ENERGY);
  GetFirstSideStackUnderAttack(Direction)->ReceiveDamage(Zapper, Damage, ENERGY);
  stack* SideStack = GetSecondSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->ReceiveDamage(Zapper, Damage, ENERGY);

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("You are hit by a burst of energy!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is hit by a burst of energy!", Char->CHAR_NAME(DEFINITE));

      Char->ReceiveDamage(Zapper, Damage, ENERGY, ALL);
      Zapper->Hostility(Char);
      Char->CheckDeath(DeathMsg, Zapper);
    }

  GetOLTerrain()->ReceiveDamage(Zapper, Damage, ENERGY);
  return false;
}

bool lsquare::FireBall(character* Who, const std::string& DeathMsg, uchar) 
{ 
  if(!GetOTerrain()->IsWalkable() || GetCharacter())
    {
      GetLevel()->Explosion(Who, DeathMsg, GetPos(), 60 + RAND() % 11 - RAND() % 11);
      return true;
    }

  return false;
}

bool lsquare::Teleport(character* Teleporter, const std::string&, uchar) 
{ 
  if(GetCharacter())
    {
      Teleporter->Hostility(GetCharacter());
      GetCharacter()->TeleportRandomly();
    }

  if(RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->TeleportSquare(Teleporter, this);

  GetStack()->TeleportRandomly();
  return false;
}

bool lsquare::Haste(character*, const std::string&, uchar)
{
  character* Dude = GetCharacter();

  if(Dude)
    Dude->BeginTemporaryState(HASTE, 500 + RAND() % 1000);

  return false;
}

bool lsquare::Slow(character*, const std::string&, uchar)
{
  character* Dude = GetCharacter();

  if(Dude)
    Dude->BeginTemporaryState(SLOW, 500 + RAND() % 1000);

  return false;
}

bool lsquare::Resurrect(character* Summoner, const std::string&, uchar)
{
  if(GetCharacter())
    return GetCharacter()->RaiseTheDead(Summoner);
  else if(GetStack()->RaiseTheDead(Summoner))
    return true;
  else
    return false;
}

bool lsquare::Invisibility(character*, const std::string&, uchar) 
{
  if(GetCharacter())
    GetCharacter()->BeginTemporaryState(INVISIBLE, 1000 + RAND() % 1001);

  return false;
}

bool lsquare::Clone(character* Zapper, const std::string&, uchar)
{
  bool ClonedSomething = false;
  character* Character = GetCharacter();

  if(Character)
    ClonedSomething = Character->CloneToNearestSquare(Zapper) != 0;

  if(GetStack()->Clone(5))
    ClonedSomething = true;

  return ClonedSomething;
}

bool lsquare::Lightning(character* Zapper, const std::string& DeathMsg, uchar Direction)
{
  ushort Damage = 30 + RAND() % 11 - RAND() % 11;
  GetStack()->ReceiveDamage(Zapper, Damage, ELECTRICITY);
  GetFirstSideStackUnderAttack(Direction)->ReceiveDamage(Zapper, Damage, ELECTRICITY);
  stack* SideStack = GetSecondSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->ReceiveDamage(Zapper, Damage, ELECTRICITY);

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("A massive burst of electricity runs through your body!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("A massive burst of electricity runs through %s!", Char->CHAR_NAME(DEFINITE));

      Char->ReceiveDamage(Zapper, Damage, ELECTRICITY, ALL);
      Zapper->Hostility(Char);
      Char->CheckDeath(DeathMsg, Zapper);
    }

  GetOLTerrain()->ReceiveDamage(Zapper, Damage, ELECTRICITY);
  return false;
}

bool lsquare::DoorCreation(character*, const std::string&, uchar)
{
  if(GetOLTerrain()->IsSafeToCreateDoor() && !GetCharacter())
    {
      door* Door = new door(0, NO_MATERIALS);
      Door->InitMaterials(MAKE_MATERIAL(IRON));
      Door->Lock();
      ChangeOLTerrainAndUpdateLights(Door);
      return true;
    }

  return false;
}

stack* lsquare::GetFirstSideStackUnderAttack(uchar Direction) const
{
  switch(Direction)
    {
    case 0: return GetSideStack(DOWN);
    case 1: return GetSideStack(DOWN);
    case 2: return GetSideStack(DOWN);
    case 3: return GetSideStack(RIGHT);
    case 4: return GetSideStack(LEFT);
    case 5: return GetSideStack(UP);
    case 6: return GetSideStack(UP);
    case 7: return GetSideStack(UP);
    default: return 0; /* Not possible */
    }
}

stack* lsquare::GetSecondSideStackUnderAttack(uchar Direction) const
{
  switch(Direction)
    {
    case 0: return GetSideStack(RIGHT);
    case 2: return GetSideStack(LEFT);
    case 5: return GetSideStack(RIGHT);
    case 7: return GetSideStack(LEFT);
    default: return 0;
    }
}

bool (lsquare::*lsquare::GetBeamEffect(ushort Index))(character*, const std::string&, uchar)
{
  static bool (lsquare::*BeamEffect[BEAM_EFFECTS])(character*, const std::string&, uchar) = { &lsquare::Polymorph, &lsquare::Strike, &lsquare::FireBall, &lsquare::Teleport, &lsquare::Haste, &lsquare::Slow, &lsquare::Resurrect, &lsquare::Invisibility, &lsquare::Clone, &lsquare::Lightning, &lsquare::DoorCreation };
  return BeamEffect[Index];
}
