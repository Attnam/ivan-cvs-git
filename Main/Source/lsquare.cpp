/* Compiled through levelset.cpp */

bool lsquare::IsDipDestination() const { return GLTerrain->IsDipDestination() || (OLTerrain && OLTerrain->IsDipDestination()); }

lsquare::lsquare(level* LevelUnder, vector2d Pos) : square(LevelUnder, Pos), GLTerrain(0), OLTerrain(0), Emitation(0), RoomIndex(0), TemporaryEmitation(0), Fluid(0), Memorized(0), MemorizedUpdateRequested(true), LastExplosionID(0), SmokeAlphaSum(0), Freezed(false)
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
  ushort c;

  for(c = 0; c < 4; ++c)
    delete SideStack[c];

  delete Fluid;
  delete Memorized;

  for(c = 0; c < Smoke.size(); ++c)
    delete Smoke[c];
}

void lsquare::SignalEmitationIncrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0 && !game::IsGenerating() && !Freezed)
    {
      CalculateEmitation();
      Emitate();
    }
}

void lsquare::SignalEmitationDecrease(ulong EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation && !game::IsGenerating() && !Freezed)
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

  game::CombineLights(Emitation, GLTerrain->GetEmitation());

  if(OLTerrain)
    game::CombineLights(Emitation, OLTerrain->GetEmitation());

  game::CombineLights(Emitation, TemporaryEmitation);
}

void lsquare::UpdateMemorized()
{
  if(MemorizedUpdateRequested)
    {
      if(!IsDark() || CanBeFeltByPlayer())
	{
	  DrawStaticContents(Memorized, vector2d(0, 0), NORMAL_LUMINANCE, false);
	  igraph::GetFOWGraphic()->MaskedBlit(Memorized, uchar(0), 0);
	}
      else
	igraph::GetFOWGraphic()->FastBlit(Memorized);

      MemorizedUpdateRequested = false;
    }
}

void lsquare::DrawStaticContents(bitmap* Bitmap, vector2d Pos, ulong Luminance, bool RealDraw) const
{
  GLTerrain->Draw(Bitmap, Pos, Luminance, RealDraw);

  if(Fluid)
    Fluid->Draw(Bitmap, Pos, Luminance, RealDraw);

  if(OLTerrain)	
    OLTerrain->Draw(Bitmap, Pos, Luminance, RealDraw);

  if(IsTransparent())
    {
      Stack->Draw(PLAYER, Bitmap, Pos, Luminance, RealDraw, RealDraw);

      for(ushort c = 0; c < 4; ++c)
	{
	  stack* Stack = GetSideStackOfAdjacentSquare(c);

	  if(Stack)
	    Stack->Draw(PLAYER, Bitmap, Pos, Luminance, RealDraw, RealDraw);
	}
    }
}

void lsquare::Draw()
{
  if(NewDrawRequested || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(!IsDark() || game::GetSeeWholeMapCheatMode())
	{
	  ulong RealLuminance;

	  if(game::GetSeeWholeMapCheatMode() == SHOW_MAP_IN_UNIFORM_LIGHT || (game::GetSeeWholeMapCheatMode() && !IsTransparent()))
	    RealLuminance = configuration::GetContrastLuminance();
	  else
	    RealLuminance = configuration::ApplyContrastTo(Luminance);

	  DrawStaticContents(DOUBLE_BUFFER, BitPos, RealLuminance, true);

	  if(Character && (Character->CanBeSeenByPlayer() || game::GetSeeWholeMapCheatMode()))
	    {
	      ushort Index = Character->GetSquareIndex(Pos);

	      if(Character->GetMoveType() & FLY)
		{
		  for(ushort c = 0; c < Smoke.size(); ++c)
		    Smoke[c]->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);

		  Character->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, Index, true);
		}
	      else
		{
		  Character->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, Index, true);

		  for(ushort c = 0; c < Smoke.size(); ++c)
		    Smoke[c]->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);
		}
	    }
	  else
	    for(ushort c = 0; c < Smoke.size(); ++c)
	      Smoke[c]->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, true);
	}
      else if(CanBeFeltByPlayer())
	{
	  ulong RealLuminance = Luminance;
	  game::CombineLights(RealLuminance, DIM_LUMINANCE);
	  DrawStaticContents(DOUBLE_BUFFER, BitPos, configuration::ApplyContrastTo(RealLuminance), true);
	}
      else
	{
	  DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

	  if(Character && Character->CanBeSeenByPlayer())
	    Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);
	}

      NewDrawRequested = false;
    }
}

void lsquare::Emitate()
{
  if(game::IsDark(Emitation))
    return;

  ushort Radius = GetLevel()->CalculateMinimumEmitationRadius(Emitation);

  if(!Radius)
    return;

  ulong RadiusSquare = Radius * Radius;
  game::InstallCurrentEmitter(Pos, Emitation);

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
  game::InstallCurrentEmitter(Pos, Emitation);

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
  game::InstallCurrentNoxifier(Pos);

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
  /* DO NOT remove the iterator, or ForceEmitterEmitation() will not work properly! */

  for(std::vector<emitter>::iterator i = Emitter.begin(); i != Emitter.end(); ++i)
    if(i->Pos == Dir)
      {
	if(!Luminance || game::CompareLights(i->DilatedEmitation, Luminance) < 0)
	  i->DilatedEmitation = 0;
	else if(!IsTransparent())
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

  #define IW(X, Y) GetNearLSquare(Pos + vector2d(X, Y))->IsTransparent()

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
		if(!IsTransparent())
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
	    else if(!IsTransparent())
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
	  if(!IsTransparent())
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
  return GetStack()->Open(Opener) || (OLTerrain && OLTerrain->Open(Opener)); 
}

bool lsquare::Close(character* Closer)
{
  if(!GetStack()->GetItems() && !Character)
    return OLTerrain && OLTerrain->Close(Closer);
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

  SaveFile << Emitter << Fluid << Emitation << Engraved << RoomIndex << Luminance << Smoke << SmokeAlphaSum;

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

  SaveFile >> Emitter >> Fluid >> Emitation >> Engraved >> RoomIndex >> Luminance >> Smoke >> SmokeAlphaSum;

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

  if(IsTransparent())
    {
      for(ushort c = 0; c < Emitter.size(); ++c)
	game::CombineLights(Luminance, Emitter[c].DilatedEmitation);
    }
  else
    {
      for(ushort c = 0; c < Emitter.size(); ++c)
	if(CalculateBitMask(Emitter[c].Pos) & CalculateBitMask(PLAYER->GetPos()))
	  game::CombineLights(Luminance, Emitter[c].DilatedEmitation);
    }
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
  SignalEmitationIncrease(Guy->GetEmitation());
  NewDrawRequested = true;
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
      DecAnimatedEntities();
      character* Backup = Character;
      Character = 0;
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
	  MemorizedDescription.Empty();

	  if(!OLTerrain || OLTerrain->IsTransparent())
	    {
	      bool Anything = false;

	      if(OLTerrain && OLTerrain->GetNameSingular().GetSize())
		{
		  OLTerrain->AddName(MemorizedDescription, INDEFINITE);
		  Anything = true;
		}

	      if(IsTransparent())
		{
		  std::vector<itemvector> PileVector;
		  GetStack()->Pile(PileVector, PLAYER);

		  if(PileVector.size())
		    {
		      if(Anything)
			MemorizedDescription << " and ";

		      if(PileVector.size() == 1)
			PileVector[0][0]->AddName(MemorizedDescription, INDEFINITE, PileVector[0].size());
		      else
			MemorizedDescription << "many items";

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
			Items += Stack->GetItems(PLAYER, Cheat);
		    }

		  if(Items > 1)
		    MemorizedDescription << " and many items on the wall";
		  else if(Items == 1)
		    {
		      MemorizedDescription << " and ";

		      for(ushort c = 0; c < 4; ++c)
			{
			  stack* Stack = GetSideStackOfAdjacentSquare(c);

			  if(Stack && Stack->GetItems(PLAYER, Cheat))
			    Stack->GetBottomItem(PLAYER, Cheat)->AddName(MemorizedDescription, INDEFINITE);
			}

		      MemorizedDescription << " on the wall";
		    }
		}
	      else
		{
		  if(Anything)
		    MemorizedDescription << " on ";

		  GLTerrain->AddName(MemorizedDescription, INDEFINITE);
		}
	    }
	  else
	    OLTerrain->AddName(MemorizedDescription, INDEFINITE);

	  if(Cheat)
	    MemorizedDescription << " (pos " << Pos.X << ':' << Pos.Y << ")";
	}
      else if(CanBeFeltByPlayer())
	{
	  MemorizedDescription.Empty();
	  OLTerrain->AddName(MemorizedDescription, INDEFINITE);
	}
      else
	MemorizedDescription = CONST_S("darkness");

      DescriptionChanged = false;
    }
}

bool lsquare::BeKicked(character* Kicker, item* Boot, float KickDamage, float KickToHitValue, short Success, uchar Direction, bool Critical, bool ForceHit)
{
  if(RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->KickSquare(Kicker, this);

  GetStack()->BeKicked(Kicker, ushort(KickDamage), Direction);

  stack* SideStack = GetFirstSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->BeKicked(Kicker, ushort(KickDamage), Direction);

  SideStack = GetSecondSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->BeKicked(Kicker, ushort(KickDamage), Direction);

  if(GetOLTerrain())
    GetOLTerrain()->BeKicked(Kicker, ushort(KickDamage * (100 + Success) / 100), Direction);

  if(GetCharacter())
    {
      GetCharacter()->BeKicked(Kicker, Boot, Pos, KickDamage, KickToHitValue, Success, Direction, Critical, ForceHit);
      return true;
    }
  else
    return false;
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
  if(GLTerrain->IsAnimated())
    DecAnimatedEntities();

  delete GLTerrain;
  GLTerrain = NewGround;
  NewGround->SetLSquareUnder(this);
  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;
  GetLevel()->SetWalkability(Pos, GetWalkability());

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::ChangeOLTerrain(olterrain* NewOver)
{
  if(OLTerrain && OLTerrain->IsAnimated())
    DecAnimatedEntities();

  delete OLTerrain;
  OLTerrain = NewOver;
  NewDrawRequested = true;
  MemorizedUpdateRequested = true;
  DescriptionChanged = true;
  GetLevel()->SetWalkability(Pos, GetWalkability());

  if(NewOver)
    {
      NewOver->SetLSquareUnder(this);

      if(NewOver->IsAnimated())
	IncAnimatedEntities();
    }
}

void lsquare::SetLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  SetGLTerrain(NewGround);
  SetOLTerrain(NewOver);
  GetLevel()->SetWalkability(Pos, GetWalkability());
}

void lsquare::SetGLTerrain(glterrain* NewGround) // NOTICE WALKABILITY CHANGE!!
{
  GLTerrain = NewGround;
  NewGround->SetLSquareUnder(this);

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::SetOLTerrain(olterrain* NewOver) // NOTICE WALKABILITY CHANGE!!
{
  OLTerrain = NewOver;

  if(NewOver)
    {
      NewOver->SetLSquareUnder(this);

      if(NewOver->IsAnimated())
	IncAnimatedEntities();
    }
}

void lsquare::ApplyScript(const squarescript* SquareScript, room* Room)
{
  const bool* AttachRequired = SquareScript->AttachRequired();

  if(AttachRequired && *AttachRequired)
    GetLevel()->AddToAttachQueue(Pos);

  const uchar* EntryIndex = SquareScript->GetEntryIndex();

  if(EntryIndex)
    GetLevel()->SetEntryPos(*EntryIndex, Pos);

  const contentscript<character>* CharacterScript = SquareScript->GetCharacter();

  if(CharacterScript)
    {
      character* Char = CharacterScript->Instantiate();

      if(!Char->GetTeam())
	Char->SetTeam(game::GetTeam(*GetLevel()->GetLevelScript()->GetTeamDefault()));

      Char->PutTo(Pos);
      Char->CreateHomeData();

      if(Room)
	{
	  const bool* IsMaster = CharacterScript->IsMaster();

	  if(IsMaster && *IsMaster)
	    Room->SetMasterID(Char->GetID());
	}
    }

  const std::list<contentscript<item> >* Items = SquareScript->GetItems();

  if(Items)
    for(std::list<contentscript<item> >::const_iterator i = Items->begin(); i != Items->end(); ++i)
      {
	stack* Stack;
	item* Item = i->Instantiate();

	if(Item)
	  {
	    const uchar* SideStackIndex = i->GetSideStackIndex();

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

  const contentscript<glterrain>* GLTerrainScript = SquareScript->GetGTerrain();

  if(GLTerrainScript)
    ChangeGLTerrain(GLTerrainScript->Instantiate());

  const contentscript<olterrain>* OLTerrainScript = SquareScript->GetOTerrain();

  if(OLTerrainScript)
    ChangeOLTerrain(OLTerrainScript->Instantiate());
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

void lsquare::StepOn(character* Stepper, lsquare** ComingFrom)
{
  if(RoomIndex)
    {
      bool WasInRoom = false;

      for(ushort c = 0; c < Stepper->GetSquaresUnder(); ++c)
	if(ComingFrom[c]->GetRoomIndex() == RoomIndex)
	  {
	    WasInRoom = true;
	    break;
	  }

      if(!WasInRoom)
	GetLevel()->GetRoom(RoomIndex)->Enter(Stepper);
    }

  GLTerrain->StepOn(Stepper);

  if(OLTerrain)
    OLTerrain->StepOn(Stepper);

  if(!(Stepper->GetMoveType() & FLY))
    GetStack()->CheckForStepOnEffect(Stepper);
}

void lsquare::ReceiveVomit(character* Who, ushort Amount)
{
  SpillFluid(Amount, MakeRGB16(10, 230, 10), 5, 60);

  if(GetOLTerrain())
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
  bool WasTransParent = IsTransparent();

  if(WasTransParent && NewTerrain && !NewTerrain->IsTransparent())
    ForceEmitterNoxify();

  ulong OldEmit = OLTerrain ? OLTerrain->GetEmitation() : 0;
  ChangeOLTerrain(NewTerrain);

  if(NewTerrain)
    SignalEmitationIncrease(NewTerrain->GetEmitation());

  SignalEmitationDecrease(OldEmit);

  for(ushort c = 0; c < 4; ++c)
    {
      for(stackiterator i = GetSideStack(c)->GetBottom(); i.HasItem(); ++i)
	i->SignalSquarePositionChange(CENTER);

      GetSideStack(c)->MoveItemsTo(GetStack());
    }

  if(WasTransParent != IsTransparent())
    {
      ForceEmitterEmitation();
      CalculateLuminance();

      if(LastSeen == game::GetLOSTurns())
	game::SendLOSUpdateRequest();
    }

  if(!IsFlyable() && !Smoke.empty())
    {
      DecAnimatedEntities();

      for(ushort c = 0; c < Smoke.size(); ++c)
	Smoke[c]->SendToHell();

      Smoke.clear();
      SmokeAlphaSum = 0;
    }
}

void lsquare::DrawParticles(ulong Color, bool DrawHere)
{
  if(GetPos().X < game::GetCamera().X
  || GetPos().Y < game::GetCamera().Y
  || GetPos().X >= game::GetCamera().X + game::GetScreenXSize()
  || GetPos().Y >= game::GetCamera().Y + game::GetScreenYSize()
  || !CanBeSeenByPlayer(true)
  || Color == TRANSPARENT_COLOR)
    return;

  clock_t StartTime = clock();

  if(DrawHere)
    game::DrawEverythingNoBlit();

  if(Color & RANDOM_COLOR)
    Color = MakeRGB16(60 + RAND() % 190, 60 + RAND() % 190, 60 + RAND() % 190);

  vector2d Pos = game::CalculateScreenCoordinates(GetPos());

  for(ushort c = 0; c < 10; ++c)
    DOUBLE_BUFFER->PutPixel(Pos + vector2d(1 + RAND() % 14, 1 + RAND() % 14), Color);

  NewDrawRequested = true; // Clean the pixels from the screen afterwards

  if(DrawHere)
    {
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
    }
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
  if(IsDipDestination())
    {
      room* Room = GetRoom();

      if(Room && Room->HasDipHandler() && !Room->Dip(Dipper))
	return false;

      return (GLTerrain->IsDipDestination() && GLTerrain->DipInto(Thingy, Dipper)) || (OLTerrain && OLTerrain->IsDipDestination() && OLTerrain->DipInto(Thingy, Dipper));
    }
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
  if(GetOLTerrain() && GetOLTerrain()->TryKey(Key, Applier))
    return true;

  if((!GetOLTerrain() || !GetOLTerrain()->HasKeyHole()) && !GetStack()->TryKey(Key, Applier))
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

  if(!IsTransparent())
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
      short XDist = Pos.X - PLAYER->GetPos().X;

      if(XDist < -1 || XDist > 1)
	return;

      short YDist = Pos.Y - PLAYER->GetPos().Y;

      if(YDist < -1 || YDist > 1)
	return;
    }

  if(!LastSeen)
    Memorized = new bitmap(16, 16);

  LastSeen = What;
  UpdateMemorized();
  UpdateMemorizedDescription();
}

void lsquare::DrawMemorized()
{
  NewDrawRequested = false;
  vector2d BitPos = game::CalculateScreenCoordinates(Pos);

  if(LastSeen)
    Memorized->Blit(DOUBLE_BUFFER, 0, 0, BitPos, 16, 16, configuration::GetContrastLuminance());
  else
    DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

  if(Character && Character->CanBeSeenByPlayer())
    Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);
}

void lsquare::DrawMemorizedCharacter()
{
  if(Character && Character->CanBeSeenByPlayer())
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(LastSeen)
	Memorized->Blit(DOUBLE_BUFFER, 0, 0, BitPos, 16, 16, configuration::GetContrastLuminance());
      else
	DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

      Character->Draw(DOUBLE_BUFFER, BitPos, configuration::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);
      NewDrawRequested = false;
    }
}

bool lsquare::IsDangerousForAIToStepOn(const character* Who) const
{
  return (!(Who->GetMoveType() & FLY) && Stack->IsDangerousForAIToStepOn(Who)) || IsDangerousForAIToBreathe(Who);
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
  if(!Freezed)
    {
      MemorizedUpdateRequested = true;
      DescriptionChanged = true;

      if(CanBeSeenByPlayer())
	{
	  UpdateMemorized();
	  UpdateMemorizedDescription();
	}
    }
}

void lsquare::KickAnyoneStandingHereAway()
{
  if(Character)
    {
      character* Backup = Character;
      Backup->Remove();
      Backup->PutNear(Pos);
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

  if(GetCharacter() && HitPercent > RAND() % 100)
    {
      if(Spiller)
	Spiller->Hostility(GetCharacter());

      GetCharacter()->ReceiveFluidSpill(Liquid, (HitPercent >> 1) + RAND() % ((HitPercent >> 1) + 1));
    }

  GetStack()->ReceiveFluidSpill(Liquid);
  delete Liquid;
}

void lsquare::AddItem(item* Item)
{
  Stack->AddItem(Item);
}

vector2d lsquare::DrawLightning(vector2d StartPos, ulong Color, uchar Direction, bool DrawHere)
{
  if(GetPos().X < game::GetCamera().X
  || GetPos().Y < game::GetCamera().Y
  || GetPos().X >= game::GetCamera().X + game::GetScreenXSize()
  || GetPos().Y >= game::GetCamera().Y + game::GetScreenYSize()
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
    {
      static vector2d Dir[4] = { vector2d(0, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(0, 1) };

      for(ushort d = 0; d < 4; ++d)
	while(!Empty.CreateLightning(StartPos + Dir[d], vector2d(0, 0), 10, Color));
    }

  if(DrawHere)
    game::DrawEverythingNoBlit();

  Empty.MaskedBlit(DOUBLE_BUFFER, 0, 0, game::CalculateScreenCoordinates(GetPos()), 16, 16);
  NewDrawRequested = true;

  if(DrawHere)
    {
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.02f * CLOCKS_PER_SEC);
    }

  return StartPos;
}

bool lsquare::Polymorph(character* Zapper, const festring&, uchar)
{
  GetStack()->Polymorph(Zapper);

  if(GetOLTerrain())
    GetOLTerrain()->Polymorph(Zapper);

  character* Character = GetCharacter();

  if(Character)
    {
      if(Zapper && Character->GetTeam() != Zapper->GetTeam())
	Zapper->Hostility(Character);

      Character->PolymorphRandomly(1, 9999, 5000 + RAND() % 5000);
    }

  return false;
}

bool lsquare::Strike(character* Zapper, const festring& DeathMsg, uchar Direction) 
{
  ushort Damage = 50 + RAND() % 21 - RAND() % 21;
  GetStack()->ReceiveDamage(Zapper, Damage, ENERGY);
  stack* SideStack = GetFirstSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->ReceiveDamage(Zapper, Damage, ENERGY);

  SideStack = GetSecondSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->ReceiveDamage(Zapper, Damage, ENERGY);

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("You are hit by a burst of energy!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is hit by a burst of energy!", Char->CHAR_NAME(DEFINITE));

      if(Zapper)
	Zapper->Hostility(Char);

      Char->ReceiveDamage(Zapper, Damage, ENERGY, ALL);
      Char->CheckDeath(DeathMsg, Zapper);
    }

  if(GetOLTerrain())
    GetOLTerrain()->ReceiveDamage(Zapper, Damage, ENERGY);

  return false;
}

bool lsquare::FireBall(character* Who, const festring& DeathMsg, uchar) 
{ 
  if(!IsFlyable() || GetCharacter())
    {
      GetLevel()->Explosion(Who, DeathMsg, Pos, 70 + RAND() % 21 - RAND() % 21);
      return true;
    }

  return false;
}

bool lsquare::Teleport(character* Teleporter, const festring&, uchar) 
{ 
  if(GetCharacter())
    {
      if(Teleporter && Character->GetTeam() != Teleporter->GetTeam())
	Teleporter->Hostility(GetCharacter());

      GetCharacter()->TeleportRandomly();
    }

  if(RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->TeleportSquare(Teleporter, this);

  GetStack()->TeleportRandomly();
  return false;
}

bool lsquare::Haste(character*, const festring&, uchar)
{
  character* Dude = GetCharacter();

  if(Dude)
    Dude->BeginTemporaryState(HASTE, 500 + RAND() % 1000);

  return false;
}

bool lsquare::Slow(character* Slower, const festring&, uchar)
{
  character* Dude = GetCharacter();

  if(Dude)
    {
      if(Slower)
	Slower->Hostility(Dude);

      Dude->BeginTemporaryState(SLOW, 500 + RAND() % 1000);
    }

  return false;
}

bool lsquare::Resurrect(character* Summoner, const festring&, uchar)
{
  if(GetCharacter())
    return GetCharacter()->RaiseTheDead(Summoner);
  else if(GetStack()->RaiseTheDead(Summoner))
    return true;
  else
    return false;
}

bool lsquare::Invisibility(character*, const festring&, uchar) 
{
  if(GetCharacter())
    GetCharacter()->BeginTemporaryState(INVISIBLE, 1000 + RAND() % 1001);

  return false;
}

bool lsquare::Clone(character* Zapper, const festring&, uchar)
{
  bool ClonedSomething = false;
  character* Character = GetCharacter();

  if(Character)
    ClonedSomething = Character->CloneToNearestSquare(Zapper) != 0;

  if(GetStack()->Clone(ClonedSomething ? 4 : 5))
    ClonedSomething = true;

  return ClonedSomething;
}

bool lsquare::Lightning(character* Zapper, const festring& DeathMsg, uchar Direction)
{
  ushort Damage = 20 + RAND() % 6 - RAND() % 6;
  GetStack()->ReceiveDamage(Zapper, Damage, ELECTRICITY);
  stack* SideStack = GetFirstSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->ReceiveDamage(Zapper, Damage, ELECTRICITY);

  SideStack = GetSecondSideStackUnderAttack(Direction);

  if(SideStack)
    SideStack->ReceiveDamage(Zapper, Damage, ELECTRICITY);

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("A massive burst of electricity runs through your body!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("A massive burst of electricity runs through %s!", Char->CHAR_NAME(DEFINITE));

      if(Zapper)
	Zapper->Hostility(Char);

      Char->ReceiveDamage(Zapper, Damage, ELECTRICITY, ALL);
      Char->CheckDeath(DeathMsg, Zapper);
    }

  if(GetOLTerrain())
    GetOLTerrain()->ReceiveDamage(Zapper, Damage, ELECTRICITY);

  return false;
}

bool lsquare::DoorCreation(character* Creator, const festring&, uchar)
{
  if((!GetOLTerrain() || GetOLTerrain()->IsSafeToCreateDoor()) && !GetCharacter())
    {
      if(Creator && GetRoom())
	GetRoom()->HostileAction(Creator);

      door* Door = new door(0, NO_MATERIALS);
      Door->InitMaterials(MAKE_MATERIAL(STEEL));

      if(RAND() % 10)
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
    default: return 0; /* YOURSELF */
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

bool (lsquare::*lsquare::GetBeamEffect(ushort Index))(character*, const festring&, uchar)
{
  static bool (lsquare::*BeamEffect[BEAM_EFFECTS])(character*, const festring&, uchar) = { &lsquare::Polymorph, &lsquare::Strike, &lsquare::FireBall, &lsquare::Teleport, &lsquare::Haste, &lsquare::Slow, &lsquare::Resurrect, &lsquare::Invisibility, &lsquare::Clone, &lsquare::Lightning, &lsquare::DoorCreation };
  return BeamEffect[Index];
}

bool lsquare::CheckKick(const character* Kicker) const
{
  if(RoomIndex && !GetLevel()->GetRoom(RoomIndex)->CheckKickSquare(Kicker, this))
    return false;

  return true;
}

void lsquare::GetHitByExplosion(const explosion* Explosion)
{
  if(Explosion->ID == LastExplosionID)
    return;

  LastExplosionID = Explosion->ID;
  ushort DistanceSquare = (Pos - Explosion->Pos).GetLengthSquare();

  if(DistanceSquare > Explosion->RadiusSquare)
    return;

  ushort Damage = Explosion->Strength / (DistanceSquare + 1);

  if(Character && (Explosion->HurtNeutrals || (Explosion->Terrorist && Character->GetRelation(Explosion->Terrorist) == HOSTILE)))
    if(Character->IsPlayer())
      game::SetPlayerWasHurtByExplosion(true);
    else
      Character->GetHitByExplosion(Explosion, Damage);

  GetStack()->ReceiveDamage(Explosion->Terrorist, Damage >> 1, FIRE);
  GetStack()->ReceiveDamage(Explosion->Terrorist, Damage >> 1, PHYSICAL_DAMAGE);

  if(GetOLTerrain())
    GetOLTerrain()->ReceiveDamage(Explosion->Terrorist, Damage >> 1, FIRE);

  if(GetOLTerrain())
    GetOLTerrain()->ReceiveDamage(Explosion->Terrorist, Damage >> 1, PHYSICAL_DAMAGE);
}

ushort lsquare::GetSpoiledItems() const
{
  return GetStack()->GetSpoiledItems();
}

bool lsquare::LowerEnchantment(character* Zapper, const festring&, uchar)
{
  character* Char = GetCharacter();
  itemvector AllItems;
  SortAllItems(AllItems, Zapper, &item::EnchantableSorter);
  item* RandomItem;

  if(!AllItems.empty())
    RandomItem = AllItems[RAND() % AllItems.size()];
  else
    return false;

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("%s glows blue for a moment!", RandomItem->CHAR_NAME(DEFINITE));

      Zapper->Hostility(Char);
    }

  if(RandomItem->GetEnchantment() > -5)
    RandomItem->EditEnchantment(-1);

  return true;
}

void lsquare::SortAllItems(itemvector& AllItems, const character* Character, bool (*Sorter)(const item*, const character*))
{
  if(GetCharacter())
    GetCharacter()->SortAllItems(AllItems, Character, Sorter);

  GetStack()->SortAllItems(AllItems, Character, Sorter);
}

void lsquare::RemoveSmoke(smoke* ToBeRemoved)
{
  Smoke.erase(std::find(Smoke.begin(), Smoke.end(), ToBeRemoved));

  if(Smoke.empty())
    DecAnimatedEntities();
}

void lsquare::AddSmoke(gas* ToBeAdded)
{
  SendNewDrawRequest();

  if(Smoke.empty())
    IncAnimatedEntities();
  else
    {
      for(ushort c = 0; c < Smoke.size(); ++c)
	if(ToBeAdded->IsSameAs(Smoke[c]->GetGas()))
	  {
	    Smoke[c]->Merge(ToBeAdded);
	    return;
	  }
    }

  Smoke.push_back(new smoke(ToBeAdded, this));
}

void lsquare::ShowSmokeMessage() const
{
  for(ushort c = 0; c < Smoke.size(); ++c)
    Smoke[c]->AddBreatheMessage();
}

void lsquare::SignalSmokeAlphaChange(short What)
{
  if(IsTransparent())
    {
      if(SmokeAlphaSum + What >= 175)
	{
	  ForceEmitterNoxify();
	  SmokeAlphaSum += What;
	  ForceEmitterEmitation();
	  CalculateLuminance();
	  DescriptionChanged = true;

	  if(LastSeen == game::GetLOSTurns())
	    game::SendLOSUpdateRequest();
	}
      else
	SmokeAlphaSum += What;
    }
  else
    {
      SmokeAlphaSum += What;

      if(IsTransparent())
	{
	  ForceEmitterEmitation();
	  CalculateLuminance();
	  DescriptionChanged = true;

	  if(LastSeen == game::GetLOSTurns())
	    game::SendLOSUpdateRequest();
	}
    }
}

uchar lsquare::GetDivineMaster() const
{
  return RoomIndex ? GetLevel()->GetRoom(RoomIndex)->GetDivineMaster() : 0;
}

void lsquare::DisplaySmokeInfo(festring& Msg) const
{
  if(Smoke.size() == 1)
    Msg << " A cloud of " << Smoke[0]->GetGas()->GetName(false, false) << " surrounds the square.";
  else if(Smoke.size())
    Msg << " A lot of gases hover over the square.";
}

void lsquare::ReceiveEarthQuakeDamage()
{
  GetStack()->ReceiveDamage(0, 5 + RAND() % 10, PHYSICAL_DAMAGE);

  for(ushort c = 0; c < 4; ++c)
    GetSideStack(c)->ReceiveDamage(0, 5 + RAND() % 10, PHYSICAL_DAMAGE);

  /* Gum solution */

  if(GetOLTerrain() && GetOLTerrain()->IsDoor())
    GetOLTerrain()->ReceiveDamage(0, 5 + RAND() % 10, PHYSICAL_DAMAGE);
}

bool lsquare::IsDangerous(character* ToWhom) const
{
  return GetStack()->IsDangerous(ToWhom) || !Smoke.empty();
}

bool lsquare::CanBeFeltByPlayer() const
{
  return OLTerrain && !PLAYER->CanMoveOn(this) && Pos.IsAdjacent(PLAYER->GetPos());
}

void lsquare::PreProcessForBone()
{
  delete Memorized;
  Memorized = 0;
  LastSeen = 0;

  if(!Smoke.empty())
    {
      DecAnimatedEntities();

      for(ushort c = 0; c < Smoke.size(); ++c)
	Smoke[c]->SendToHell();

      Smoke.clear();
      SmokeAlphaSum = 0;
    }

  if(Character && !Character->PreProcessForBone())
    {
      Character->SendToHell();
      Character->Remove();
    }

  GetStack()->PreProcessForBone();
}

void lsquare::PostProcessForBone(float& DangerSum, ushort& Enemies)
{
  if(Character && !Character->PostProcessForBone(DangerSum, Enemies))
    {
      Character->SendToHell();
      Character->Remove();
    }

  GetStack()->PostProcessForBone();
}

void lsquare::FinalProcessForBone()
{
  if(Character)
    Character->FinalProcessForBone();

  GetStack()->FinalProcessForBone();
}

bool lsquare::EngravingsCanBeReadByPlayer()
{
  return PLAYER->CanRead(); // Might be a good idea to improve sometime in the distant future.
}

void lsquare::DisplayEngravedInfo(festring& Buffer) const
{
  Buffer << " There is a message engraved here: \"" << Engraved << '\"';
}

bool lsquare::IsDangerousForAIToBreathe(const character* Who) const
{
  for(ushort c = 0; c < Smoke.size(); ++c)
    if(Smoke[c]->IsDangerousForAIToBreathe(Who))
      return true;

  return false;
}


