/* Compiled through levelset.cpp */

lsquare*** eyecontroller::Map;

lsquare*** stackcontroller::Map;
lsquare** stackcontroller::Stack;
long stackcontroller::StackIndex;
int stackcontroller::LevelXSize, stackcontroller::LevelYSize;
vector2d stackcontroller::Center;

ulong tickcontroller::Tick;
ulong tickcontroller::ShiftedTick[4];
ulong tickcontroller::ShiftedQuadriTick[4];

void tickcontroller::PrepareShiftedTick()
{
  for(int c = 0; c < 4; ++c)
    {
      ShiftedTick[c] = Tick << (c << 3);
      ShiftedQuadriTick[c] = (Tick + 1) << (c << 3);
    }
}

bool lsquare::IsDipDestination() const { return GLTerrain->IsDipDestination() || (OLTerrain && OLTerrain->IsDipDestination()); }

lsquare::lsquare(level* LevelUnder, vector2d Pos)
: square(LevelUnder, Pos),
  Fluid(0), Smoke(0), Rain(0),
  GLTerrain(0), OLTerrain(0),
  Memorized(0), FowMemorized(0),
  Engraved(0),
  SquarePartEmitationTick(0),
  SquarePartLastSeen(0),
  Emitation(0),
  SmokeAlphaSum(0),
  AmbientLuminance(0),
  SunLightLuminance(0),
  TemporaryEmitation(0),
  SecondarySunLightEmitation(0),
  LastExplosionID(0),
  RoomIndex(0)
{
  Stack = new stack(this, 0);
}

lsquare::~lsquare()
{
  delete GLTerrain;
  delete OLTerrain;
  delete Stack;
  delete [] Engraved;

  for(fluid* F = Fluid; F;)
    {
      fluid* ToDel = F;
      F = F->Next;
      delete ToDel;
    }

  delete Memorized;
  delete FowMemorized;

  for(smoke* S = Smoke; S;)
    {
      smoke* ToDel = S;
      S = S->Next;
      delete ToDel;
    }

  for(rain* R = Rain; R;)
    {
      rain* ToDel = R;
      R = R->Next;
      delete ToDel;
    }
}

void lsquare::SignalEmitationIncrease(color24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) > 0 && !game::IsGenerating() && !(Flags & FREEZED))
    {
      CalculateEmitation(); // could this be optimized?
      Emitate();
    }
}

void lsquare::SignalEmitationDecrease(color24 EmitationUpdate)
{
  if(game::CompareLights(EmitationUpdate, Emitation) >= 0 && Emitation && !game::IsGenerating() && !(Flags & FREEZED))
    {
      color24 Backup = Emitation;
      CalculateEmitation();

      if(Backup != Emitation)
	{
	  Noxify(Backup);
	  Emitate(Emitation, FORCE_ADD);
	}
    }
}

void lsquare::CalculateEmitation()
{
  Emitation = Stack->GetEmitation();
  int c;

  for(c = 0; c < 4; ++c)
    {
      stack* Stack = GetStackOfAdjacentSquare(c);

      if(Stack)
	game::CombineLights(Emitation, Stack->GetSideEmitation(3 - c));
    }

  if(Character)
    game::CombineLights(Emitation, Character->GetEmitation());

  game::CombineLights(Emitation, GLTerrain->GetEmitation());

  if(OLTerrain)
    game::CombineLights(Emitation, OLTerrain->GetEmitation());

  game::CombineLights(Emitation, TemporaryEmitation);

  for(const fluid* F = Fluid; F; F = F->Next)
    game::CombineLights(Emitation, F->GetEmitation());

  for(const rain* R = Rain; R; R = R->Next)
    game::CombineLights(Emitation, R->GetEmitation());
}

void lsquare::UpdateMemorized()
{
  if(Flags & MEMORIZED_UPDATE_REQUEST)
    {
      if(!IsDark() || CanBeFeltByPlayer())
	{
	  DrawStaticContents(Memorized, vector2d(0, 0), NORMAL_LUMINANCE, false);
	  Memorized->FastBlit(FowMemorized);
	  igraph::GetFOWGraphic()->NormalMaskedBlit(FowMemorized, 0, 0);
	}
      else
	{
	  Memorized->ClearToColor(0);
	  igraph::GetFOWGraphic()->FastBlit(FowMemorized);
	}

      Flags &= ~MEMORIZED_UPDATE_REQUEST;
    }
}

void lsquare::DrawStaticContents(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool RealDraw) const
{
  if(RealDraw && !AnimatedEntities && Memorized && !game::GetSeeWholeMapCheatMode())
    {
      Memorized->LuminanceBlit(Bitmap, 0, 0, Pos.X, Pos.Y, 16, 16, Luminance);
      return;
    }

  if(!OLTerrain || OLTerrain->ShowThingsUnder())
    GLTerrain->Draw(Bitmap, Pos, Luminance, RealDraw);

  bool StackDrawn = false;

  if(OLTerrain && !IsFlyable())
    {
      if(OLTerrain->IsTransparent() && OLTerrain->ShowThingsUnder())
	{
	  StackDrawn = true;
	  DrawStacks(Bitmap, Pos, Luminance, RealDraw);
	}

      OLTerrain->Draw(Bitmap, Pos, Luminance, 0, RealDraw);
    }

  for(const fluid* F = Fluid; F; F = F->Next)
    F->Draw(Bitmap, Pos, Luminance, RealDraw);

  if(OLTerrain && IsFlyable())
    OLTerrain->Draw(Bitmap, Pos, Luminance, 0, RealDraw);

  if(!StackDrawn && Flags & IS_TRANSPARENT)
    DrawStacks(Bitmap, Pos, Luminance, RealDraw);

  int Partners = BorderPartnerInfo >> 24;

  for(int c = 0; c < Partners; ++c)
    BorderPartnerTerrain[c]->Draw(Bitmap, Pos, Luminance, 8 - (BorderPartnerInfo >> ((c << 1) + c) & 7), RealDraw);
}

void lsquare::Draw()
{
  if(Flags & NEW_DRAW_REQUEST || AnimatedEntities)
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(!IsDark() || game::GetSeeWholeMapCheatMode())
	{
	  long RealLuminance;

	  if(game::GetSeeWholeMapCheatMode() == SHOW_MAP_IN_UNIFORM_LIGHT
	  || (game::GetSeeWholeMapCheatMode()
	  && !(Flags & IS_TRANSPARENT)))
	    RealLuminance = ivanconfig::GetContrastLuminance();
	  else
	    RealLuminance = ivanconfig::ApplyContrastTo(Luminance);

	  DrawStaticContents(DOUBLE_BUFFER, BitPos, RealLuminance, true);

	  if(Character && (Character->CanBeSeenByPlayer() || game::GetSeeWholeMapCheatMode()))
	    {
	      int Index = Character->GetSquareIndex(Pos);

	      if(Character->GetMoveType() & FLY)
		{
		  for(const smoke* S = Smoke; S; S = S->Next)
		    S->Draw(DOUBLE_BUFFER, BitPos, RealLuminance);

		  Character->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, Index, true);
		}
	      else
		{
		  Character->Draw(DOUBLE_BUFFER, BitPos, RealLuminance, Index, true);

		  for(const smoke* S = Smoke; S; S = S->Next)
		    S->Draw(DOUBLE_BUFFER, BitPos, RealLuminance);
		}
	    }
	  else
	    for(const smoke* S = Smoke; S; S = S->Next)
	      S->Draw(DOUBLE_BUFFER, BitPos, RealLuminance);

	  for(const rain* R = Rain; R; R = R->Next)
	    if(R->IsEnabled())
	      R->Draw(DOUBLE_BUFFER, BitPos, RealLuminance);
	}
      else if(CanBeFeltByPlayer())
	{
	  color24 RealLuminance = Luminance;
	  game::CombineLights(RealLuminance, DIM_LUMINANCE);
	  DrawStaticContents(DOUBLE_BUFFER, BitPos, ivanconfig::ApplyContrastTo(RealLuminance), true);

	  for(const rain* R = Rain; R; R = R->Next)
	    if(R->IsEnabled())
	      R->Draw(DOUBLE_BUFFER, BitPos, RealLuminance);
	}
      else
	{
	  DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

	  if(Character && Character->CanBeSeenByPlayer())
	    Character->Draw(DOUBLE_BUFFER, BitPos, ivanconfig::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);
	}

      Flags &= ~STRONG_NEW_DRAW_REQUEST;
    }
}

struct emitationcontroller : public tickcontroller, public stackcontroller
{
  static int Handler(int x, int y)
  {
    lsquare* Square = Map[x >> 1][y >> 1];
    const ulong SquareFlags = Square->Flags;

    if(SquareFlags & PERFECTLY_QUADRI_HANDLED)
      return SquareFlags & ALLOW_EMITATION_CONTINUE;

    if(SquareFlags & IS_TRANSPARENT)
      return ProcessSquare(x >> 1, y >> 1, Square);

    if(!(SquareFlags & IN_SQUARE_STACK))
      {
	Square->Flags |= IN_SQUARE_STACK;
	Stack[StackIndex++] = Square;
      }

    const int SquarePartIndex = (x & 1) + ((y & 1) << 1);
    Square->SquarePartEmitationTick = Square->SquarePartEmitationTick
				    & ~SquarePartTickMask[SquarePartIndex]
				    | ShiftedTick[SquarePartIndex];

    return false;
  }
  static int ProcessSquare(int X, int Y, lsquare* Square)
  {
    Stack[StackIndex++] = Square;
    const ulong SquareFlags = Square->Flags;
    const int MaxE = MaxLuxTable[X - EmitterPosXMinus16][Y - EmitterPosYMinus16];

    if(MaxE >= LIGHT_BORDER
    && (SquareFlags & INSIDE
     || (!(ID & SECONDARY_SUN_LIGHT)
      && MaxE > MinNightAmbientLuminanceElement)))
      {
	Square->Flags |= ALLOW_EMITATION_CONTINUE | PERFECTLY_QUADRI_HANDLED;
	return true;
      }
    else
      {
	Square->Flags = SquareFlags & ~ALLOW_EMITATION_CONTINUE | PERFECTLY_QUADRI_HANDLED;
	return false;
      }
  }
  static ulong& GetTickReference(int X, int Y)
  {
    return Map[X][Y]->SquarePartEmitationTick;
  }
  static void ProcessStack()
  {
    for(long c1 = 0; c1 < StackIndex; ++c1)
      {
	lsquare* Square = Stack[c1];
	const ulong SquareTick = Square->SquarePartEmitationTick;
	ulong TempID = ID;

	for(int c2 = 0; c2 < 4; ++c2)
	  if((SquareTick & SquarePartTickMask[c2]) == ShiftedTick[c2])
	    TempID |= 1 << EMITTER_SQUARE_PART_SHIFT << c2;

	Square->Flags &= ~(IN_SQUARE_STACK|PERFECTLY_QUADRI_HANDLED);
	vector2d Pos = Square->Pos;
	int XVal = Pos.X - EmitterPosXMinus16;
	int YVal = Pos.Y - EmitterPosYMinus16;

	if(MaxLuxTable[XVal][YVal] >= LIGHT_BORDER)
	  Square->AlterLuminance(TempID, MakeRGB24(RedLuxTable[XVal][YVal],
						   GreenLuxTable[XVal][YVal],
						   BlueLuxTable[XVal][YVal]));
      }
  }
  static ulong ID;
  static int MinNightAmbientLuminanceElement;
  static int EmitterPosXMinus16;
  static int EmitterPosYMinus16;
  static uchar** MaxLuxTable;
  static uchar** RedLuxTable;
  static uchar** GreenLuxTable;
  static uchar** BlueLuxTable;
};

ulong emitationcontroller::ID;
int emitationcontroller::MinNightAmbientLuminanceElement;
int emitationcontroller::EmitterPosXMinus16;
int emitationcontroller::EmitterPosYMinus16;
uchar** emitationcontroller::MaxLuxTable;
uchar** emitationcontroller::RedLuxTable;
uchar** emitationcontroller::GreenLuxTable;
uchar** emitationcontroller::BlueLuxTable;

void lsquare::Emitate(color24 Emitation, ulong IDFlags)
{
  if(game::IsDark(Emitation))
    return;

  int Radius = game::CalculateMinimumEmitationRadius(Emitation);

  if(!Radius)
    return;

  stackcontroller::Map = GetLevel()->GetMap();
  stackcontroller::Stack = GetLevel()->GetSquareStack();
  stackcontroller::StackIndex = 0;
  tickcontroller::Tick = game::IncreaseSquarePartEmitationTicks();
  tickcontroller::PrepareShiftedTick();
  emitationcontroller::ID = MakeEmitterID(Pos) | IDFlags;
  emitationcontroller::MinNightAmbientLuminanceElement = GetMinColor24(GetLevel()->GetNightAmbientLuminance());
  emitationcontroller::EmitterPosXMinus16 = Pos.X - 16;
  emitationcontroller::EmitterPosYMinus16 = Pos.Y - 16;
  emitationcontroller::MaxLuxTable = game::GetLuxTable()[GetMaxColor24(Emitation)];
  emitationcontroller::RedLuxTable = game::GetLuxTable()[GetRed24(Emitation)];
  emitationcontroller::GreenLuxTable = game::GetLuxTable()[GetGreen24(Emitation)];
  emitationcontroller::BlueLuxTable = game::GetLuxTable()[GetBlue24(Emitation)];
  mapmath<emitationcontroller>::DoQuadriArea(Pos.X, Pos.Y,
					     Radius * Radius,
					     GetLevel()->GetXSize(),
					     GetLevel()->GetYSize());
  emitationcontroller::ProcessStack();
}

struct noxifycontroller : public stackcontroller
{
  static bool Handler(int x, int y)
  {
    if(x >= 0 && y >= 0 && x < LevelXSize && y < LevelYSize)
      {
	lsquare* Square = Map[x][y];

	if(Square->SquarePartEmitationTick != Tick)
	  {
	    Square->SquarePartEmitationTick = Tick;
	    return Square->NoxifyEmitter(ID);
	  }
      }

    return false;
  }
  static int GetStartX(int) { return Center.X; }
  static int GetStartY(int) { return Center.Y; }
  static ulong ID;
  static ulong Tick;
};

ulong noxifycontroller::ID;
ulong noxifycontroller::Tick;

void lsquare::Noxify(color24 Emitation, ulong IDFlags)
{
  if(game::IsDark(Emitation))
    return;

  int Radius = game::CalculateMinimumEmitationRadius(Emitation);

  if(!Radius)
    return;

  stackcontroller::Map = GetLevel()->GetMap();
  stackcontroller::LevelXSize = GetLevel()->GetXSize();
  stackcontroller::LevelYSize = GetLevel()->GetYSize();
  stackcontroller::Center = Pos;
  noxifycontroller::ID = MakeEmitterID(Pos) | IDFlags;
  noxifycontroller::Tick = game::IncreaseSquarePartEmitationTicks();
  NoxifyEmitter(noxifycontroller::ID);
  mapmath<noxifycontroller>::DoArea();
}

bool lsquare::NoxifyEmitter(ulong ID)
{
  emittervector::iterator i, End = Emitter.end();

  for(i = Emitter.begin(); i != End; ++i)
    if(!((i->ID ^ ID) & (EMITTER_IDENTIFIER_BITS|SECONDARY_SUN_LIGHT)))
      {
	RemoveLuminance(i->Emitation);
	Swap(*i, Emitter.back());
	Emitter.pop_back();
	return true;
      }

  return false;
}

void lsquare::AlterLuminance(ulong ID, color24 NewLuminance)
{
  emittervector::iterator i, End = Emitter.end();

  if(!(ID & FORCE_ADD))
    for(i = Emitter.begin(); i != End; ++i)
      if(!((i->ID ^ ID) & (EMITTER_IDENTIFIER_BITS|SECONDARY_SUN_LIGHT)))
	{
	  i->ID |= ID;

	  if(i->Emitation != NewLuminance)
	    ChangeLuminance(i->Emitation, NewLuminance);

	  return;
	}

  Emitter.push_back(emitter(ID, 0));
  ChangeLuminance(Emitter.back().Emitation, NewLuminance);
}

void lsquare::AddSunLightEmitter(ulong ID)
{
  sunemittervector::iterator i, End = SunEmitter.end();

  for(i = SunEmitter.begin(); i != End; ++i)
    if(!((*i ^ ID) & EMITTER_IDENTIFIER_BITS))
      {
	if(ID & ~*i & RE_SUN_EMITATED)
	  *i &= ~EMITTER_SHADOW_BITS;

	*i |= ID;
	Swap(*i, SunEmitter.front());
	return;
      }

  SunEmitter.push_back(ID);
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
  SaveFile << Emitter << SunEmitter;
  SaveFile << Emitation << Engraved << Luminance;
  SaveFile << SmokeAlphaSum << (uchar)Flags << Memorized;
  SaveFile << SecondarySunLightEmitation;
  SaveFile << (uchar)RoomIndex;
  SaveFile << SunLightLuminance;
  SaveLinkedList(SaveFile, Fluid);
  SaveLinkedList(SaveFile, Smoke);
  SaveLinkedList(SaveFile, Rain);
}

void lsquare::Load(inputfile& SaveFile)
{
  Stack->Load(SaveFile); // This must be before square::Load! (Note: This comment is years old. It's probably obsolete)
  Stack->SetMotherSquare(this);
  square::Load(SaveFile);

  if(vector2d(24, 10) == Pos)
    int esko = 2;

  SaveFile >> GLTerrain >> OLTerrain;
  SaveFile >> Emitter >> SunEmitter;
  SaveFile >> Emitation >> Engraved >> Luminance;
  SaveFile >> SmokeAlphaSum >> (uchar&)Flags >> Memorized;
  Flags &= INSIDE|DESCRIPTION_CHANGE; //only these flags are loaded
  Flags |= MEMORIZED_UPDATE_REQUEST;
  SecondarySunLightEmitation = ReadType<color24>(SaveFile);
  RoomIndex = ReadType<uchar>(SaveFile);
  SunLightLuminance = ReadType<color24>(SaveFile);
  LoadLinkedList(SaveFile, Fluid);
  LoadLinkedList(SaveFile, Smoke);
  LoadLinkedList(SaveFile, Rain);
  CalculateIsTransparent();

  if(Memorized)
    {
      FowMemorized = new bitmap(16, 16);
      FowMemorized->ActivateFastFlag();
      Memorized->FastBlit(FowMemorized);
      igraph::GetFOWGraphic()->NormalMaskedBlit(FowMemorized, 0, 0);
    }
}

void lsquare::CalculateLuminance()
{
  Luminance = AmbientLuminance;
  emittervector::const_iterator i, End = Emitter.end();

  if(Flags & IS_TRANSPARENT)
    {
      game::CombineLights(Luminance, SunLightLuminance);

      for(i = Emitter.begin(); i != End; ++i)
	game::CombineLights(Luminance, i->Emitation);
    }
  else
    {
      ulong BitMask = 0, LOSTick = game::GetLOSTick();

      for(int c = 0; c < 4; ++c)
	if((SquarePartLastSeen >> (c << 3) & 0xFF) == LOSTick)
	  BitMask |= 1 << EMITTER_SQUARE_PART_SHIFT << c;

      CalculateSunLightLuminance(BitMask);
      game::CombineLights(Luminance, SunLightLuminance);

      for(i = Emitter.begin(); i != End; ++i)
	if(BitMask & i->ID)
	  game::CombineLights(Luminance, i->Emitation);
    }
}

void lsquare::AddCharacter(character* Guy)
{
  if(Character)
    ABORT("Overgrowth of square population detected!");

  Character = Guy;
  SignalEmitationIncrease(Guy->GetEmitation());
  Flags |= STRONG_NEW_DRAW_REQUEST;
  IncAnimatedEntities();

  if(Guy->IsPlayer()
  || (Guy->CanBeSeenByPlayer(true) && CanBeSeenByPlayer()))
    Guy->SignalSeen();
  //Guy->CheckIfSeen();
}

void lsquare::Clean()
{
  GetStack()->Clean();
}

void lsquare::RemoveCharacter()
{
  if(Character)
    {
      DecAnimatedEntities();
      character* Backup = Character;
      Character = 0;
      SignalEmitationDecrease(Backup->GetEmitation());
      Flags |= STRONG_NEW_DRAW_REQUEST;
    }
}

void lsquare::UpdateMemorizedDescription(bool Cheat)
{
  if(Flags & DESCRIPTION_CHANGE || Cheat)
    {
      if(!IsDark() || Cheat)
	{
	  MemorizedDescription.Empty();

	  if(!OLTerrain || (OLTerrain->IsTransparent() && OLTerrain->ShowThingsUnder()))
	    {
	      bool Anything = false;

	      if(OLTerrain && OLTerrain->GetNameSingular().GetSize())
		{
		  OLTerrain->AddName(MemorizedDescription, INDEFINITE);
		  Anything = true;
		}

	      if(Flags & IS_TRANSPARENT)
		{
		  std::vector<itemvector> PileVector;
		  GetStack()->Pile(PileVector, PLAYER, CENTER);

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
		  festring SideItems;
		  GetSideItemDescription(SideItems, Cheat);

		  if(!SideItems.IsEmpty())
		    MemorizedDescription << " and " << SideItems;
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

      Flags &= ~DESCRIPTION_CHANGE;
    }
}

void lsquare::GetSideItemDescription(festring& String, bool Cheat) const
{
  int Items = 0;

  for(int c = 0; c < 4; ++c)
    {
      stack* Stack = GetStackOfAdjacentSquare(c);

      if(Stack)
	Items += Cheat
	       ? Stack->GetSideItems(3 - c)
	       : Stack->GetVisibleSideItems(PLAYER, 3 - c);
    }

  if(Items > 1)
    String << "many items on the wall";
  else if(Items == 1)
    {
      for(int c = 0; c < 4; ++c)
	{
	  stack* Stack = GetStackOfAdjacentSquare(c);

	  if(Stack
	  && ((Cheat && Stack->GetSideItems(3 - c))
	   || (!Cheat && Stack->GetVisibleSideItems(PLAYER, 3 - c))))
	    Stack->GetBottomSideItem(PLAYER, 3 - c, Cheat)->AddName(String, INDEFINITE);
	}

      String << " on the wall";
    }
}

bool lsquare::BeKicked(character* Kicker, item* Boot, bodypart* Leg, double KickDamage, double KickToHitValue, int Success, int Direction, bool Critical, bool ForceHit)
{
  if(RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->KickSquare(Kicker, this);

  GetStack()->BeKicked(Kicker, int(KickDamage), Direction);

  if(GetOLTerrain())
    GetOLTerrain()->BeKicked(Kicker, int(KickDamage * (100 + Success) / 100), Direction);

  if(GetCharacter())
    {
      GetCharacter()->BeKicked(Kicker, Boot, Leg, Pos, KickDamage, KickToHitValue, Success, Direction, Critical, ForceHit);
      return true;
    }
  else
    return false;
}

bool lsquare::CanBeDug() const
{
  if((!GetPos().X || !GetPos().Y || GetPos().X == GetLevel()->GetXSize() - 1 || GetPos().Y == GetLevel()->GetYSize() - 1) && !*GetLevel()->GetLevelScript()->IsOnGround())
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
  Flags |= NEW_DRAW_REQUEST;
  SendMemorizedUpdateRequest();
  GetLevel()->SetWalkability(Pos, GetTheoreticalWalkability());

  if(NewGround->IsAnimated())
    IncAnimatedEntities();
}

void lsquare::ChangeOLTerrain(olterrain* NewOver)
{
  if(OLTerrain && OLTerrain->IsAnimated())
    DecAnimatedEntities();

  bool WasUsingBorderTiles = OLTerrain && OLTerrain->UseBorderTiles();
  delete OLTerrain;
  OLTerrain = NewOver;
  Flags |= NEW_DRAW_REQUEST;
  GetLevel()->SetWalkability(Pos, GetTheoreticalWalkability());
  CalculateBorderPartners();
  CalculateIsTransparent();
  SendMemorizedUpdateRequest();

  if(WasUsingBorderTiles || (NewOver && NewOver->UseBorderTiles()))
    RequestForBorderPartnerUpdates();

  if(NewOver)
    {
      NewOver->SetLSquareUnder(this);

      if(NewOver->IsAnimated())
	IncAnimatedEntities();
    }
}

void lsquare::SetLTerrain(glterrain* NewGround, olterrain* NewOver)
{
  GLTerrain = NewGround;
  NewGround->SetLSquareUnder(this);

  if(NewGround->IsAnimated())
    IncAnimatedEntities();

  OLTerrain = NewOver;

  if(NewOver)
    {
      NewOver->SetLSquareUnder(this);

      if(NewOver->IsAnimated())
	IncAnimatedEntities();

      if(!NewOver->IsTransparent())
	Flags &= ~IS_TRANSPARENT;
    }

  GetLevel()->SetWalkability(Pos, GetTheoreticalWalkability());
}

/*void lsquare::SetGLTerrain(glterrain* NewGround) // NOTICE WALKABILITY CHANGE!!
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

      if(!NewOver->IsTransparent())
	Flags &= ~IS_TRANSPARENT;
    }
}*/

void lsquare::ApplyScript(const squarescript* SquareScript, room* Room)
{
  if(SquareScript->AttachRequired())
    GetLevel()->AddToAttachQueue(Pos);

  int EntryIndex = SquareScript->GetEntryIndex();

  if(EntryIndex != NO_ENTRY)
    GetLevel()->SetEntryPos(EntryIndex, Pos);

  const contentscript<character>* CharacterScript = SquareScript->GetCharacter();

  if(CharacterScript)
    {
      character* Char = CharacterScript->Instantiate();
      Char->SetGenerationDanger(GetLevel()->GetDifficulty());

      if(!Char->GetTeam())
	Char->SetTeam(game::GetTeam(*GetLevel()->GetLevelScript()->GetTeamDefault()));

      if(CharacterScript->GetFlags() & IS_LEADER)
	Char->GetTeam()->SetLeader(Char);

      Char->PutToOrNear(Pos);
      Char->CreateHomeData();

      if(Room && CharacterScript->GetFlags() & IS_MASTER)
	Room->SetMasterID(Char->GetID());
    }

  const fearray<contentscript<item> >* Items = SquareScript->GetItems();

  if(Items)
    for(uint c1 = 0; c1 < Items->Size; ++c1)
      {
	const interval* TimesPtr = Items->Data[c1].GetTimes();
	int Times = TimesPtr ? TimesPtr->Randomize() : 1;

	for(int c2 = 0; c2 < Times; ++c2)
	  {
	    item* Item = Items->Data[c1].Instantiate();

	    if(Item)
	      {
		int SquarePosition = Items->Data[c1].GetSquarePosition();

		if(SquarePosition != CENTER)
		  Item->SignalSquarePositionChange(SquarePosition);

		GetStack()->AddItem(Item);
		Item->SpecialGenerationHandler();
	      }
	  }
      }

  const contentscript<glterrain>* GLTerrainScript = SquareScript->GetGTerrain();

  if(GLTerrainScript)
    {
      ChangeGLTerrain(GLTerrainScript->Instantiate());

      if(GLTerrainScript->IsInside())
	if(*GLTerrainScript->IsInside())
	  Flags |= INSIDE;
	else
	  Flags &= ~INSIDE;
    }

  const contentscript<olterrain>* OLTerrainScript = SquareScript->GetOTerrain();

  if(OLTerrainScript)
    ChangeOLTerrain(OLTerrainScript->Instantiate());
}

bool lsquare::CanBeSeenByPlayer(bool IgnoreDarkness) const
{
  return (IgnoreDarkness || !IsDark()) && LastSeen == game::GetLOSTick();
}

bool lsquare::CanBeSeenFrom(vector2d FromPos, long MaxDistance, bool IgnoreDarkness) const
{
  if((Pos - FromPos).GetLengthSquare() <= MaxDistance
  && (IgnoreDarkness || !IsDark()))
    {
      if(Character && Character->IsPlayer()
      && GetNearLSquare(FromPos)->CanBeSeenByPlayer(true))
	return true;

      eyecontroller::Map = GetLevel()->GetMap();
      return mapmath<eyecontroller>::DoLine(FromPos.X, FromPos.Y, GetPos().X, GetPos().Y, SKIP_FIRST);
    }

  return false;
}

void lsquare::StepOn(character* Stepper, lsquare** ComingFrom)
{
  if(RoomIndex)
    {
      bool WasInRoom = false;

      if(ComingFrom)
	for(int c = 0; c < Stepper->GetSquaresUnder(); ++c)
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
    {
      OLTerrain->StepOn(Stepper);

      if(Stepper->DestroysWalls() && OLTerrain->CanBeDestroyed())
	{
	  if(CanBeSeenByPlayer()) 
	    ADD_MESSAGE("%s destroys %s.", Stepper->CHAR_NAME(DEFINITE), OLTerrain->CHAR_NAME(DEFINITE));

	  Stepper->EditAP(-100);
	  OLTerrain->BeDestroyed();
	}
    }

  if(!(Stepper->GetMoveType() & FLY))
    GetStack()->CheckForStepOnEffect(Stepper);
}

void lsquare::ReceiveVomit(character* Who, liquid* Liquid)
{
  if(!GetOLTerrain() || !GetOLTerrain()->ReceiveVomit(Who, Liquid))
    {
      SpillFluid(Who, Liquid);

      if(RoomIndex)
	GetRoom()->ReceiveVomit(Who);
    }
}

void lsquare::SetTemporaryEmitation(color24 What)
{
  color24 Old = TemporaryEmitation;
  TemporaryEmitation = 0;
  SignalEmitationDecrease(Old);
  TemporaryEmitation = What;
  SignalEmitationIncrease(What);
}

void lsquare::ChangeOLTerrainAndUpdateLights(olterrain* NewTerrain)
{
  bool WasTransparent = !!(Flags & IS_TRANSPARENT), Noxified = false;
  emittervector EmitterBackup;

  if(WasTransparent && NewTerrain && !NewTerrain->IsTransparent())
    {
      EmitterBackup = Emitter;
      GetLevel()->ForceEmitterNoxify(EmitterBackup);
      Noxified = true;
    }

  long OldEmit = OLTerrain ? OLTerrain->GetEmitation() : 0;
  ChangeOLTerrain(NewTerrain);

  if(NewTerrain)
    SignalEmitationIncrease(NewTerrain->GetEmitation());

  SignalEmitationDecrease(OldEmit);
  GetStack()->DropSideItems();

  if(!IsFlyable() && Smoke)
    {
      DecAnimatedEntities();

      for(smoke* S = Smoke; S; S = S->Next)
	S->SendToHell();

      Smoke = 0;
      SmokeAlphaSum = 0;
    }

  if(WasTransparent == !CalculateIsTransparent())
    {
      if(Noxified)
	GetLevel()->ForceEmitterEmitation(EmitterBackup, SunEmitter, FORCE_ADD);
      else
	GetLevel()->ForceEmitterEmitation(Emitter, SunEmitter);

      CalculateLuminance();

      if(LastSeen == game::GetLOSTick())
	game::SendLOSUpdateRequest();
    }
}

void lsquare::DrawParticles(long Color, bool DrawHere)
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

  for(int c = 0; c < 10; ++c)
    DOUBLE_BUFFER->PutPixel(Pos + vector2d(1 + RAND() % 14, 1 + RAND() % 14), Color);

  Flags |= STRONG_NEW_DRAW_REQUEST; // Clean the pixels from the screen afterwards

  if(DrawHere)
    {
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.02 * CLOCKS_PER_SEC);
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

void lsquare::SignalSeen(ulong Tick)
{
  if(LastSeen < Tick - 2)
    Flags |= STRONG_NEW_DRAW_REQUEST;

  Flags &= ~(IN_SQUARE_STACK|PERFECTLY_QUADRI_HANDLED);
  LastSeen = Tick;

  if(!(Flags & IS_TRANSPARENT))
    {
      color24 OldLuminance = Luminance;
      CalculateLuminance();

      if(OldLuminance != Luminance)
	{
	  Flags |= NEW_DRAW_REQUEST;

	  if(IsDark() != game::IsDark(OldLuminance))
	    Flags |= MEMORIZED_UPDATE_REQUEST|DESCRIPTION_CHANGE;
	}
    }

  if(IsDark())
    {
      vector2d Dist = Pos - PLAYER->GetPos();

      if(abs(Dist.X) > 1 || abs(Dist.Y) > 1)
	{
	  LastSeen -= 2;
	  return;
	}
    }

  if(!Memorized)
    CreateMemorized();

  UpdateMemorized();
  UpdateMemorizedDescription();

  if(Character)
    Character->CheckIfSeen();
}

void lsquare::DrawMemorized()
{
  LastSeen = 0;
  Flags &= ~STRONG_NEW_DRAW_REQUEST;
  vector2d BitPos = game::CalculateScreenCoordinates(Pos);

  if(FowMemorized)
    FowMemorized->LuminanceBlit(DOUBLE_BUFFER, 0, 0, BitPos.X, BitPos.Y, 16, 16, ivanconfig::GetContrastLuminance());
  else
    DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

  if(Character && Character->CanBeSeenByPlayer())
    Character->Draw(DOUBLE_BUFFER, BitPos, ivanconfig::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);
}

void lsquare::DrawMemorizedCharacter()
{
  if(Character && Character->CanBeSeenByPlayer())
    {
      vector2d BitPos = game::CalculateScreenCoordinates(Pos);

      if(FowMemorized)
	FowMemorized->LuminanceBlit(DOUBLE_BUFFER, 0, 0, BitPos, 16, 16, ivanconfig::GetContrastLuminance());
      else
	DOUBLE_BUFFER->Fill(BitPos, 16, 16, 0);

      Character->Draw(DOUBLE_BUFFER, BitPos, ivanconfig::GetContrastLuminance(), Character->GetSquareIndex(Pos), true);
      Flags |= STRONG_NEW_DRAW_REQUEST;
    }
}

bool lsquare::IsDangerousForAIToStepOn(const character* Who) const
{
  return (!(Who->GetMoveType() & FLY)
       && Stack->IsDangerousForAIToStepOn(Who))
      || IsDangerousForAIToBreathe(Who);
}

bool lsquare::IsScaryForAIToStepOn(const character* Who) const
{
  return IsScaryForAIToBreathe(Who);
}

stack* lsquare::GetStackOfAdjacentSquare(int I) const
{
  lsquare* Square = 0;

  switch(I)
    {
    case LEFT:  Square = NeighbourLSquare[3]; break;
    case DOWN:  Square = NeighbourLSquare[6]; break;
    case UP:    Square = NeighbourLSquare[1]; break;
    case RIGHT: Square = NeighbourLSquare[4]; break;
    }

  return Square ? Square->Stack : 0;
}

void lsquare::SendMemorizedUpdateRequest()
{
  if(!(Flags & FREEZED))
    {
      Flags |= MEMORIZED_UPDATE_REQUEST|DESCRIPTION_CHANGE;

      if(!game::IsGenerating() && CanBeSeenByPlayer())
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
  SaveFile.Write(reinterpret_cast<const char*>(&Emitter), sizeof(Emitter));
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, emitter& Emitter)
{
  SaveFile.Read(reinterpret_cast<char*>(&Emitter), sizeof(Emitter));
  return SaveFile;
}

void lsquare::AddItem(item* Item)
{
  Stack->AddItem(Item);
}

vector2d lsquare::DrawLightning(vector2d StartPos, long Color, int Direction, bool DrawHere)
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
  Empty.ActivateFastFlag();

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

      for(int d = 0; d < 4; ++d)
	while(!Empty.CreateLightning(StartPos + Dir[d], vector2d(0, 0), 10, Color));
    }

  if(DrawHere)
    game::DrawEverythingNoBlit();

  Empty.NormalMaskedBlit(DOUBLE_BUFFER, 0, 0, game::CalculateScreenCoordinates(GetPos()), 16, 16);
  Flags |= STRONG_NEW_DRAW_REQUEST;

  if(DrawHere)
    {
      graphics::BlitDBToScreen();
      while(clock() - StartTime < 0.02 * CLOCKS_PER_SEC);
    }

  return StartPos;
}

bool lsquare::Polymorph(const beamdata& Beam)
{
  GetStack()->Polymorph(Beam.Owner);

  if(GetOLTerrain())
    GetOLTerrain()->Polymorph(Beam.Owner);

  character* Character = GetCharacter();

  if(Character)
    {
      if(Beam.Owner && Character->GetTeam() != Beam.Owner->GetTeam())
	Beam.Owner->Hostility(Character);

      Character->PolymorphRandomly(1, 999999, 500 + RAND() % 500);
    }

  return false;
}

bool lsquare::Strike(const beamdata& Beam) 
{
  int Damage = 50 + RAND() % 21 - RAND() % 21;
  GetStack()->ReceiveDamage(Beam.Owner, Damage, ENERGY, Beam.Direction);

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("You are hit by a burst of energy!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("%s is hit by a burst of energy!", Char->CHAR_NAME(DEFINITE));

      if(Beam.Owner)
	Beam.Owner->Hostility(Char);

      Char->ReceiveDamage(Beam.Owner, Damage, ENERGY, ALL);
      Char->CheckDeath(Beam.DeathMsg, Beam.Owner);
    }

  if(GetOLTerrain())
    GetOLTerrain()->ReceiveDamage(Beam.Owner, Damage, ENERGY);

  return false;
}

bool lsquare::FireBall(const beamdata& Beam) 
{ 
  if(!IsFlyable() || GetCharacter())
    {
      GetLevel()->Explosion(Beam.Owner, Beam.DeathMsg, Pos, 75 + RAND() % 25 - RAND() % 25);
      return true;
    }

  return false;
}

bool lsquare::Teleport(const beamdata& Beam) 
{ 
  if(GetCharacter())
    {
      if(Beam.Owner && Character->GetTeam() != Beam.Owner->GetTeam())
	Beam.Owner->Hostility(GetCharacter());

      GetCharacter()->TeleportRandomly();
    }

  if(RoomIndex)
    GetLevel()->GetRoom(RoomIndex)->TeleportSquare(Beam.Owner, this);

  GetStack()->TeleportRandomly();
  return false;
}

bool lsquare::Haste(const beamdata&)
{
  character* Dude = GetCharacter();

  if(Dude)
    Dude->BeginTemporaryState(HASTE, 500 + RAND() % 1000);

  return false;
}

bool lsquare::Slow(const beamdata& Beam)
{
  character* Dude = GetCharacter();

  if(Dude)
    {
      if(Beam.Owner)
	Beam.Owner->Hostility(Dude);

      Dude->BeginTemporaryState(SLOW, 500 + RAND() % 1000);
    }

  return false;
}

bool lsquare::Resurrect(const beamdata& Beam)
{
  if(GetCharacter())
    return GetCharacter()->RaiseTheDead(Beam.Owner);
  else if(GetStack()->RaiseTheDead(Beam.Owner))
    return true;
  else
    return false;
}

bool lsquare::Invisibility(const beamdata&) 
{
  if(GetCharacter())
    GetCharacter()->BeginTemporaryState(INVISIBLE, 1000 + RAND() % 1001);

  return false;
}

bool lsquare::Duplicate(const beamdata& Beam)
{
  bool DuplicatedSomething = false;
  character* Character = GetCharacter();

  if(Character)
    DuplicatedSomething = !!Character->DuplicateToNearestSquare(Beam.Owner, Beam.SpecialParameters);

  if(GetStack()->Duplicate(DuplicatedSomething ? 4 : 5, Beam.SpecialParameters))
    DuplicatedSomething = true;

  return DuplicatedSomething;
}

bool lsquare::Lightning(const beamdata& Beam)
{
  int Damage = 20 + RAND() % 6 - RAND() % 6;
  GetStack()->ReceiveDamage(Beam.Owner, Damage, ELECTRICITY, Beam.Direction);

  character* Char = GetCharacter();

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("A massive burst of electricity runs through your body!");
      else if(Char->CanBeSeenByPlayer())
	ADD_MESSAGE("A massive burst of electricity runs through %s!", Char->CHAR_NAME(DEFINITE));

      if(Beam.Owner)
	Beam.Owner->Hostility(Char);

      Char->ReceiveDamage(Beam.Owner, Damage, ELECTRICITY, ALL);
      Char->CheckDeath(Beam.DeathMsg, Beam.Owner);
    }

  if(GetOLTerrain())
    GetOLTerrain()->ReceiveDamage(Beam.Owner, Damage, ELECTRICITY);

  return false;
}

bool lsquare::DoorCreation(const beamdata& Beam)
{
  if((!GetOLTerrain()
  ||  GetOLTerrain()->IsSafeToCreateDoor())
  && !GetCharacter()
  && (GetLevel()->IsOnGround()
  || (Pos.X > 0 && Pos.Y > 0
  &&  Pos.X < GetLevel()->GetXSize() - 1 && Pos.Y < GetLevel()->GetYSize() - 1)))
    {
      if(Beam.Owner && GetRoom())
	GetRoom()->HostileAction(Beam.Owner);

      door* Door = new door(0, NO_MATERIALS);
      Door->InitMaterials(MAKE_MATERIAL(STEEL));

      if(RAND() % 10)
	Door->Lock();

      ChangeOLTerrainAndUpdateLights(Door);
      return true;
    }

  return false;
}

bool (lsquare::*BeamEffect[BEAM_EFFECTS])(const beamdata&) =
{
  &lsquare::Polymorph,
  &lsquare::Strike,
  &lsquare::FireBall,
  &lsquare::Teleport,
  &lsquare::Haste,
  &lsquare::Slow,
  &lsquare::Resurrect,
  &lsquare::Invisibility,
  &lsquare::Duplicate,
  &lsquare::Lightning,
  &lsquare::DoorCreation,
  &lsquare::AcidRain
};

bool (lsquare::*lsquare::GetBeamEffect(int I))(const beamdata&)
{
  return BeamEffect[I];
}

bool lsquare::CheckKick(const character* Kicker) const
{
  if(Character && Kicker->CheckIfTooScaredToHit(Character))
    return false;

  if(RoomIndex && !GetLevel()->GetRoom(RoomIndex)->CheckKickSquare(Kicker, this))
    return false;

  return true;
}

void lsquare::GetHitByExplosion(const explosion* Explosion)
{
  if(Explosion->ID == LastExplosionID)
    return;

  LastExplosionID = Explosion->ID;
  int DistanceSquare = (Pos - Explosion->Pos).GetLengthSquare();

  if(DistanceSquare > Explosion->RadiusSquare)
    return;

  int Damage = Explosion->Strength / (DistanceSquare + 1);

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

int lsquare::GetSpoiledItems() const
{
  return GetStack()->GetSpoiledItems();
}

bool lsquare::LowerEnchantment(const beamdata& Beam)
{
  character* Char = GetCharacter();
  itemvector AllItems;
  SortAllItems(AllItems, Beam.Owner, &item::IsEnchantable);
  item* RandomItem;

  if(!AllItems.empty())
    RandomItem = AllItems[RAND() % AllItems.size()];
  else
    return false;

  if(Char)
    {
      if(Char->IsPlayer())
	ADD_MESSAGE("%s glows blue for a moment!", RandomItem->CHAR_NAME(DEFINITE));

      if(Beam.Owner)
	Beam.Owner->Hostility(Char);
    }

  if(RandomItem->GetEnchantment() > -5)
    RandomItem->EditEnchantment(-1);

  return true;
}

void lsquare::SortAllItems(itemvector& AllItems, const character* Character, sorter Sorter)
{
  if(GetCharacter())
    GetCharacter()->SortAllItems(AllItems, Character, Sorter);

  GetStack()->SortAllItems(AllItems, Character, Sorter);
}

void lsquare::RemoveSmoke(smoke* ToBeRemoved)
{
  smoke* S = Smoke;

  if(S == ToBeRemoved)
    {
      Smoke = S->Next;

      if(!S)
	DecAnimatedEntities();
    }
  else
    {
      smoke* LS;

      do
	{
	  LS = S;
	  S = S->Next;
	}
      while(S != ToBeRemoved);

      LS->Next = S->Next;
    }
}

void lsquare::AddSmoke(gas* ToBeAdded)
{
  smoke* S = Smoke;

  if(!S)
    {
      Smoke = new smoke(ToBeAdded, this);
      IncAnimatedEntities();
    }
  else
    {
      smoke* LS;

      do
	{
	  if(ToBeAdded->IsSameAs(S->GetGas()))
	    {
	      S->Merge(ToBeAdded);
	      return;
	    }

	  LS = S;
	  S = S->Next;
	}
      while(S);

      LS->Next = new smoke(ToBeAdded, this);
    }
}

void lsquare::ShowSmokeMessage() const
{
  for(const smoke* S = Smoke; S; S = S->Next)
    S->AddBreatheMessage();
}

void lsquare::SignalSmokeAlphaChange(int What)
{
  if(Flags & IS_TRANSPARENT)
    {
      if(SmokeAlphaSum + What >= 175)
	{
	  emittervector EmitterBackup = Emitter;
	  GetLevel()->ForceEmitterNoxify(EmitterBackup);
	  SmokeAlphaSum += What;
	  Flags &= ~IS_TRANSPARENT;
	  GetLevel()->ForceEmitterEmitation(EmitterBackup, SunEmitter, FORCE_ADD);
	  CalculateLuminance();
	  Flags |= DESCRIPTION_CHANGE;

	  if(LastSeen == game::GetLOSTick())
	    game::SendLOSUpdateRequest();
	}
      else
	SmokeAlphaSum += What;
    }
  else
    {
      SmokeAlphaSum += What;

      if(CalculateIsTransparent())
	{
	  GetLevel()->ForceEmitterEmitation(Emitter, SunEmitter);
	  CalculateLuminance();
	  Flags |= DESCRIPTION_CHANGE;

	  if(LastSeen == game::GetLOSTick())
	    game::SendLOSUpdateRequest();
	}
    }
}

int lsquare::GetDivineMaster() const
{
  return RoomIndex ? GetLevel()->GetRoom(RoomIndex)->GetDivineMaster() : 0;
}

void lsquare::DisplaySmokeInfo(festring& Msg) const
{
  if(Smoke)
    if(!Smoke->Next)
      Msg << " A cloud of " << Smoke->GetGas()->GetName(false, false) << " surrounds the square.";
    else
      Msg << " A lot of gases hover over the square.";
}

void lsquare::ReceiveEarthQuakeDamage()
{
  GetStack()->ReceiveDamage(0, 5 + RAND() % 10, PHYSICAL_DAMAGE);

  /* Gum solution */

  if(GetOLTerrain() && GetOLTerrain()->IsDoor())
    GetOLTerrain()->ReceiveDamage(0, 5 + RAND() % 10, PHYSICAL_DAMAGE);
}

bool lsquare::IsDangerous(character* ToWhom) const
{
  return GetStack()->IsDangerous(ToWhom) || Smoke;
}

bool lsquare::CanBeFeltByPlayer() const
{
  return OLTerrain && !PLAYER->CanMoveOn(this) && Pos.IsAdjacent(PLAYER->GetPos());
}

void lsquare::PreProcessForBone()
{
  DestroyMemorized();
  LastSeen = 0;

  if(Smoke)
    {
      DecAnimatedEntities();

      for(smoke* S = Smoke; S; S = S->Next)
	S->SendToHell();

      Smoke = 0;
      SmokeAlphaSum = 0;
    }

  if(Character && !Character->PreProcessForBone())
    {
      Character->SendToHell();
      Character->Remove();
    }

  GetStack()->PreProcessForBone();
}

void lsquare::PostProcessForBone(double& DangerSum, int& Enemies)
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
  for(const smoke* S = Smoke; S; S = S->Next)
    if(S->IsDangerousForAIToBreathe(Who))
      return true;

  return false;
}

bool lsquare::IsScaryForAIToBreathe(const character* Who) const
{
  for(const smoke* S = Smoke; S; S = S->Next)
    if(S->IsScaryForAIToBreathe(Who))
      return true;

  return false;
}

struct borderpartner
{
  olterrain* Terrain;
  int SquareIndex;
};

bool BorderPartnerOrderer(const borderpartner& BP1, const borderpartner& BP2)
{
  return BP1.Terrain->GetBorderTilePriority() < BP2.Terrain->GetBorderTilePriority();
}

void lsquare::CalculateBorderPartners()
{
  borderpartner BorderPartner[8];
  int Index = 0;
  int Priority = OLTerrain ? OLTerrain->GetBorderTilePriority() : 0;

  for(int d = 0; d < 8; ++d)
    {
      lsquare* Square = NeighbourLSquare[d];

      if(Square)
	{
	  olterrain* Terrain = Square->GetOLTerrain();

	  if(Terrain && Terrain->UseBorderTiles()
	  && Terrain->GetBorderTilePriority() > Priority)
	    {
	      BorderPartner[Index].Terrain = Terrain;
	      BorderPartner[Index].SquareIndex = 7 - d;
	      ++Index;
	    }
	}
    }

  std::sort(BorderPartner, BorderPartner + Index, BorderPartnerOrderer);
  BorderPartnerInfo = 0;

  for(int c = 0; c < Index; ++c)
    {
      BorderPartnerTerrain[c] = BorderPartner[c].Terrain;
      BorderPartnerInfo |= BorderPartner[c].SquareIndex << ((c << 1) + c);
    }

  BorderPartnerInfo |= Index << 24;
}

void lsquare::RequestForBorderPartnerUpdates()
{
  if(!game::IsGenerating())
    for(int d = 0; d < 8; ++d)
      {
	lsquare* Square = NeighbourLSquare[d];

	if(Square)
	  {
	    Square->CalculateBorderPartners();
	    Square->SendNewDrawRequest();
	    Square->SendMemorizedUpdateRequest();
	  }
      }
}

int lsquare::GetWalkability() const
{
  if(!GetLevel()->IsOnGround())
    {
      if(Pos.X >= 1 && Pos.Y >= 1 && Pos.X < GetLevel()->GetXSize() - 1 && Pos.Y < GetLevel()->GetYSize() - 1)
	return OLTerrain ? OLTerrain->GetWalkability() & GLTerrain->GetWalkability() : GLTerrain->GetWalkability(); 
      else
	return 0;
    }
  else
    return OLTerrain ? OLTerrain->GetWalkability() & GLTerrain->GetWalkability() : GLTerrain->GetWalkability(); 
}

template <class type> void RemoveLinkedListElement(type*& Element, const type* ToRemove)
{
  type* E = Element;

  if(E == ToRemove)
    Element = E->Next;
  else
    {
      type* LE;

      do
	{
	  LE = E;
	  E = E->Next;
	}
      while(E != ToRemove);

      LE->Next = E->Next;
    }
}

void lsquare::RemoveFluid(fluid* ToRemove)
{
  RemoveLinkedListElement(Fluid, ToRemove);
  SignalEmitationDecrease(ToRemove->GetEmitation());
}

void lsquare::AddFluid(liquid* ToBeAdded)
{
  fluid* F = Fluid;

  if(!F)
    Fluid = new fluid(ToBeAdded, this);
  else
    {
      fluid* LF;

      do
	{
	  if(ToBeAdded->IsSameAs(F->GetLiquid()))
	    {
	      F->AddLiquidAndVolume(ToBeAdded->GetVolume());
	      delete ToBeAdded;
	      goto end;
	    }

	  LF = F;
	  F = F->Next;
	}
      while(F);

      LF->Next = new fluid(ToBeAdded, this);
    }

  SignalEmitationIncrease(ToBeAdded->GetEmitation());
end:
  SendNewDrawRequest();
  SendMemorizedUpdateRequest();
}

void lsquare::DisplayFluidInfo(festring& Msg) const
{
  if(Fluid)
    {
      Msg << " There is ";
      fluid::AddFluidInfo(Fluid, Msg);

      /* Does this cause problems? */

      if(IsFlyable())
	Msg << " on the ground.";
      else
	Msg << " on the wall.";
    }
}

void lsquare::SpillFluid(character* Spiller, liquid* Liquid, bool ForceHit, bool ShowMsg)
{
  if(!Liquid->GetVolume())
    {
      delete Liquid;
      return;
    }

  if(IsFlyable())
    {
      if(GetCharacter())
	{
	  if(Spiller && !GetCharacter()->IsAlly(Spiller))
	    Spiller->Hostility(GetCharacter());

	  long CharVolume = GetCharacter()->GetVolume();
	  double ChanceMultiplier = 1. / (300 + sqrt(GetStack()->GetVolume() + CharVolume));
	  double Root = sqrt(CharVolume);

	  if(ForceHit || Root > RAND() % 400 || Root > RAND() % 400)
	    {
	      long SpillVolume = long(Liquid->GetVolume() * Root * ChanceMultiplier);

	      if(SpillVolume)
		{
		  if(ShowMsg && (GetCharacter()->IsPlayer() || GetCharacter()->CanBeSeenByPlayer()))
		    ADD_MESSAGE("%s is spilled all over %s.", Liquid->GetName(false, false).CStr(), GetCharacter()->CHAR_DESCRIPTION(DEFINITE));

		  Liquid->EditVolume(-SpillVolume);
		  GetCharacter()->SpillFluid(Spiller, Liquid->CloneLiquid(SpillVolume), GetCharacter()->GetSquareIndex(GetPos()));
		}
	    }
	}

      GetStack()->SpillFluid(Spiller, Liquid, Liquid->GetVolume());
    }

  if(Liquid->GetVolume())
    AddFluid(Liquid);
  else
    delete Liquid;
}

void lsquare::DrawStacks(bitmap* Bitmap, vector2d Pos, color24 Luminance, bool RealDraw) const
{
  Stack->Draw(PLAYER, Bitmap, Pos, Luminance, CENTER, RealDraw);

  for(int c = 0; c < 4; ++c)
    {
      stack* Stack = GetStackOfAdjacentSquare(c);

      if(Stack)
	Stack->Draw(PLAYER, Bitmap, Pos, Luminance, 3 - c, RealDraw);
    }
}

void lsquare::RemoveRain(rain* ToBeRemoved)
{
  SendNewDrawRequest();
  rain* R = Rain;

  if(ToBeRemoved->IsEnabled())
    DecAnimatedEntities();

  if(R == ToBeRemoved)
    Rain = R->Next;
  else
    {
      rain* LR;

      do
	{
	  LR = R;
	  R = R->Next;
	}
      while(R != ToBeRemoved);

      LR->Next = R->Next;
    }

  SignalEmitationDecrease(ToBeRemoved->GetEmitation());
}

void lsquare::AddRain(liquid* RainLiquid, vector2d Speed, int Team, bool OwnLiquid)
{
  rain* R = Rain, * NewRain = new rain(RainLiquid, this, Speed, Team, OwnLiquid);

  if(NewRain->IsEnabled())
    IncAnimatedEntities();

  if(!R)
    Rain = NewRain;
  else
    {
      rain* LR;

      do
	{
	  LR = R;
	  R = R->Next;
	}
      while(R);

      LR->Next = NewRain;
    }
}

void lsquare::RemoveSunLight()
{
  SunLightLuminance = 0;
  SunEmitter.clear();
}

void lsquare::CheckIfIsSecondarySunLightEmitter()
{
  color24 OldEmitation = SecondarySunLightEmitation;

  if(Flags & IS_TRANSPARENT && (!(Flags & INSIDE) || SunLightLuminance))
    for(int d = 0; d < 8; ++d)
      {
	lsquare* Neighbour = NeighbourLSquare[d];

	if(Neighbour && Neighbour->Flags & INSIDE)
	  {
	    color24 NewEmitation = GetLevel()->GetAmbientLuminance();

	    if(OldEmitation != NewEmitation)
	      {
		SecondarySunLightEmitation = NewEmitation;

		if(game::CompareLights(NewEmitation, OldEmitation) >= 0)
		  Emitate(NewEmitation, SECONDARY_SUN_LIGHT);
		else
		  {
 		    Noxify(OldEmitation, SECONDARY_SUN_LIGHT);
		    Emitate(NewEmitation, SECONDARY_SUN_LIGHT|FORCE_ADD);
		  }
	      }

	    return;
	  }
      }

  if(OldEmitation)
    {
      Noxify(OldEmitation, SECONDARY_SUN_LIGHT);
      SecondarySunLightEmitation = 0;
    }
}

void lsquare::CalculateNeighbourLSquares()
{
  int XSize = GetLevel()->GetXSize();
  int YSize = GetLevel()->GetYSize();

  for(int d = 0; d < 8; ++d)
    {
      vector2d NPos = Pos + game::GetMoveVector(d);

      if(NPos.X >= 0 && NPos.Y >= 0 && NPos.X < XSize && NPos.Y < YSize)
	NeighbourLSquare[d] = GetLevel()->GetLSquare(NPos);
      else
	NeighbourLSquare[d] = 0;
    }
}

void lsquare::RemoveLuminance(color24& Emitation)
{
  color24 OldLuminance = Luminance;
  color24 OldEmitation = Emitation;
  Emitation = 0;

  if(game::CompareLights(OldEmitation, OldLuminance) < 0)
    return;

  if(!(Flags & IS_TRANSPARENT))
    {
      Flags |= NEW_DRAW_REQUEST;

      if(LastSeen == game::GetLOSTick())
	game::SendLOSUpdateRequest();
    }
  else
    {
      CalculateLuminance();

      if(OldLuminance == Luminance)
	return;

      Flags |= NEW_DRAW_REQUEST;

      if(!Luminance)
	{
	  Flags |= MEMORIZED_UPDATE_REQUEST|DESCRIPTION_CHANGE;

	  if(LastSeen == game::GetLOSTick())
	    game::SendLOSUpdateRequest();
	}
    }
}

void lsquare::ChangeLuminance(color24& Emitation, color24 NewLuminance)
{
  color24 OldLuminance = Luminance;

  if(!(Flags & IS_TRANSPARENT))
    {
      Emitation = NewLuminance;
      Flags |= NEW_DRAW_REQUEST;

      if(LastSeen == game::GetLOSTick())
	game::SendLOSUpdateRequest();

      return;
    }

  bool EmitationInsignificant = !Emitation
			     || game::CompareLights(Emitation, OldLuminance) < 0;
  Emitation = NewLuminance;

  if(game::CompareLights(NewLuminance, OldLuminance) > 0
  && EmitationInsignificant)
    game::CombineLights(Luminance, NewLuminance);
  else
    {
      if(EmitationInsignificant)
	return;

      CalculateLuminance();

      if(OldLuminance == Luminance)
	return;
    }

  Flags |= NEW_DRAW_REQUEST;

  if(!OldLuminance)
    {
      Flags |= MEMORIZED_UPDATE_REQUEST|DESCRIPTION_CHANGE;

      if(LastSeen == game::GetLOSTick())
	game::SendLOSUpdateRequest();
    }
}

void lsquare::EnableGlobalRain()
{
  for(rain* R = Rain; R; R = R->Next)
    if(!R->HasOwnLiquid())
      {
	R->Enable();
	IncAnimatedEntities();
      }
}

void lsquare::DisableGlobalRain()
{
  SendNewDrawRequest();

  for(rain* R = Rain; R; R = R->Next)
    if(!R->HasOwnLiquid())
      {
	R->Disable();
	DecAnimatedEntities();
      }
}

void lsquare::InitLastSeen()
{
  LastSeen = LastSeen == game::GetLOSTick() ? 2 : 0;
  SquarePartLastSeen = 0;
}

bool lsquare::Engrave(const festring& What)
{
  if(Engraved)
    delete [] Engraved;

  if(!What.IsEmpty())
    {
      Engraved = new char[What.GetSize() + 1];
      strcpy(Engraved, What.CStr());
    }
  else
    Engraved = 0;

  return true;
}

void lsquare::SendSunLightSignals()
{
  if(Flags & IS_TRANSPARENT)
    {
      color24 OldLuminance = Luminance;
      CalculateLuminance();

      if(Luminance != OldLuminance)
	{
	  Flags |= NEW_DRAW_REQUEST;

	  if(!Luminance != !OldLuminance)
	    {
	      Flags |= MEMORIZED_UPDATE_REQUEST|DESCRIPTION_CHANGE;

	      if(LastSeen == game::GetLOSTick())
		game::SendLOSUpdateRequest();
	    }
	}
    }
  else
    {
      Flags |= NEW_DRAW_REQUEST;

      if(LastSeen == game::GetLOSTick())
	game::SendLOSUpdateRequest();
    }
}

void lsquare::ZeroReSunEmitatedFlags()
{
  sunemittervector::iterator i, End = SunEmitter.end();

  for(i = SunEmitter.begin(); i != End; ++i)
    *i &= ~RE_SUN_EMITATED;
}

bool lsquare::CalculateIsTransparent()
{
  if((!OLTerrain || OLTerrain->IsTransparent()) && SmokeAlphaSum < 175)
    {
      Flags |= IS_TRANSPARENT;
      return true;
    }
  else
    {
      Flags &= ~IS_TRANSPARENT;
      return false;
    }
}

void lsquare::CalculateSunLightLuminance(ulong SeenBitMask)
{
  sunemittervector::const_iterator i, SunEnd = SunEmitter.end();
  int S = 0, L = 0;

  for(i = SunEmitter.begin(); i != SunEnd; ++i)
    {
      ulong ShadowFlag = 1 << EMITTER_SHADOW_SHIFT;
      ulong SquarePartFlag = 1 << EMITTER_SQUARE_PART_SHIFT;

      for(int c = 0; c < 4; ++c, ShadowFlag <<= 1, SquarePartFlag <<= 1)
	if(SeenBitMask & *i & SquarePartFlag)
	  if(*i & ShadowFlag)
	    ++S;
	  else
	    ++L;
    }

  if(!L)
    SunLightLuminance = 0;
  else if(!S)
    SunLightLuminance = GetLevel()->GetSunLightEmitation();
  else
    {
      color24 ShadowColor = GetLevel()->GetAmbientLuminance();
      color24 LightColor = GetLevel()->GetSunLightEmitation();
      SunLightLuminance = MakeRGB24((GetRed24(LightColor) * L
				   + GetRed24(ShadowColor) * S) / (S + L),
				    (GetGreen24(LightColor) * L
				   + GetGreen24(ShadowColor) * S) / (S + L),
				    (GetBlue24(LightColor) * L
				   + GetBlue24(ShadowColor) * S) / (S + L));
    }
}

void lsquare::CreateMemorized()
{
  Memorized = new bitmap(16, 16);
  Memorized->ActivateFastFlag();
  FowMemorized = new bitmap(16, 16);
  FowMemorized->ActivateFastFlag();
}

bool lsquare::AcidRain(const beamdata& Beam) 
{ 
  if(!IsFlyable() || GetCharacter() || Beam.Direction == YOURSELF)
    {
      int StackSize = GetLevel()->AddRadiusToSquareStack(Pos, 9);
      lsquare** Stack = GetLevel()->GetSquareStack();
      vector2d Speed = vector2d(512, 512);
      int Team = Beam.Owner ? Beam.Owner->GetTeam()->GetID() : MONSTER_TEAM;

      for(int c = 0; c < StackSize; ++c)
	{
	  Stack[c]->AddRain(new liquid(SULPHURIC_ACID, 300), Speed, Team, true);
	  Stack[c]->Flags &= ~IN_SQUARE_STACK;
	}

      if(Beam.Owner && Character && Character->GetTeam() != Beam.Owner->GetTeam())
	Beam.Owner->Hostility(Character);

      return true;
    }

  return false;
}

bool lsquare::DetectMaterial(const material* Material) const
{
  if(GLTerrain->DetectMaterial(Material)
  || OLTerrain && OLTerrain->DetectMaterial(Material)
  || Stack->DetectMaterial(Material)
  || Character && Character->DetectMaterial(Material))
    return true;

  for(const fluid* F = Fluid; F; F = F->Next)
    if(F->GetLiquid()->IsSameAs(Material))
      return true;

  for(const smoke* S = Smoke; S; S = S->Next)
    if(S->GetGas()->IsSameAs(Material))
      return true;

  for(const rain* R = Rain; R; R = R->Next)
    if(R->GetLiquid()->IsSameAs(Material))
      return true;

  return false;
}

void lsquare::Reveal(ulong Tick, bool IgnoreDarkness)
{
  if(!Memorized)
    CreateMemorized();

  LastSeen = Tick;

  if(IgnoreDarkness)
    Luminance = NORMAL_LUMINANCE;
  else
    {
      SquarePartLastSeen = 0;

      for(int c = 0; c < 4; ++c)
	SquarePartLastSeen |= (Tick << (c << 3));

      CalculateLuminance();
    }

  Flags |= NEW_DRAW_REQUEST
	 | MEMORIZED_UPDATE_REQUEST
	 | DESCRIPTION_CHANGE;
  UpdateMemorized();
  UpdateMemorizedDescription();
}

void lsquare::DestroyMemorized()
{
  delete Memorized;
  delete FowMemorized;
  Memorized = 0;
  FowMemorized = 0;
}

void lsquare::SwapMemorized(lsquare* Square)
{
  Swap(Memorized, Square->Memorized);
  Swap(FowMemorized, Square->FowMemorized);
  MemorizedDescription.SwapData(Square->MemorizedDescription);
}
