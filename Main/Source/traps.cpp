/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

/* Compiled through trapset.cpp */

web::~web()
{
  game::RemoveTrapID(TrapData.TrapID);
}

void web::VirtualConstructor(bool Load)
{
  if(!Load)
    {
      TrapData.TrapID = game::CreateNewTrapID(this);
      TrapData.VictimID = 0;
      Picture = new bitmap(16,16, TRANSPARENT_COLOR);
      bitmap Temp(16, 16, TRANSPARENT_COLOR);
      Temp.ActivateFastFlag();
      packedcolor16 Color = MakeRGB16(250, 250, 250);
      igraph::GetRawGraphic(GR_EFFECT)->MaskedBlit(&Temp, RAND_2 ? 64 : 80, 32, 0,0,16,16, &Color);
      Temp.NormalBlit(Picture, Flags);
    }
}

bool web::TryToUnStick(character* Victim, vector2d)
{
  ulong TrapID = GetTrapID();
  int Modifier = 10 * GetTrapBaseModifier() / Max(Victim->GetAttribute(DEXTERITY) + Victim->GetAttribute(ARM_STRENGTH), 1);

  if(Modifier <= 1 || !RAND_N(Modifier))
    {
      Victim->RemoveTrap(TrapID);
      TrapData.VictimID = 0;

      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to free yourself from the web.");
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s manages to free %sself from the web.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN);

      Victim->EditAP(-500);
      return true;
    }

  if(!Modifier || !RAND_N(Modifier << 1))
    {
      Victim->RemoveTrap(TrapID);
      TrapData.VictimID = 0;
      GetLSquareUnder()->RemoveTrap(this);
      SendToHell();

      if(Victim->IsPlayer())
	ADD_MESSAGE("You tear the web down.");
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s tears the web down.", Victim->CHAR_NAME(DEFINITE));

      Victim->EditAP(-500);
      return true;
    }

  Modifier = Max(GetTrapBaseModifier() * (Victim->GetAttribute(DEXTERITY) + Victim->GetAttribute(ARM_STRENGTH)) / 75, 2);

  if(Victim->CanChokeOnWeb(this) && !RAND_N(Modifier << 2))
    {
      if(Victim->IsPlayer())
	ADD_MESSAGE("You manage to choke yourself on the web.");
      else if(Victim->CanBeSeenByPlayer())
	ADD_MESSAGE("%s chokes %sself on the web.", Victim->CHAR_NAME(DEFINITE),Victim->CHAR_OBJECT_PRONOUN);

      Victim->LoseConsciousness(250 + RAND_N(250));
      Victim->EditAP(-1000);
      return true;
    }

  if(!RAND_N(Modifier))
    {
      int VictimBodyPart = Victim->RandomizeTryToUnStickBodyPart(ALL_BODYPART_FLAGS&~TrapData.BodyParts);

      if(VictimBodyPart != NONE_INDEX)
	{
	  TrapData.BodyParts |= 1 << VictimBodyPart;
	  Victim->AddTrap(GetTrapID(), 1 << VictimBodyPart);

	  if(Victim->IsPlayer())
	    ADD_MESSAGE("You fail to free yourself from the web and your %s is stuck in it in the attempt.", Victim->GetBodyPartName(VictimBodyPart).CStr());
	  else if(Victim->CanBeSeenByPlayer())
	    ADD_MESSAGE("%s tries to free %sself from the web but is stuck more tightly in it in the attempt.", Victim->CHAR_NAME(DEFINITE), Victim->CHAR_OBJECT_PRONOUN);

	  Victim->EditAP(-1000);
	  return true;
	}
    }

  if(Victim->IsPlayer())
    ADD_MESSAGE("You are unable to escape from the web.");

  Victim->EditAP(-1000);
  return false;
}

void web::Save(outputfile& SaveFile) const
{
  trap::Save(SaveFile);
  SaveFile << TrapData << Strength << Picture;
}

void web::Load(inputfile& SaveFile)
{
  trap::Load(SaveFile);
  SaveFile >> TrapData >> Strength >> Picture;
  game::AddTrapID(this, TrapData.TrapID);
}

void web::StepOnEffect(character* Stepper)
{
  if(Stepper->IsImmuneToStickiness())
    return;

  int StepperBodyPart = Stepper->GetRandomBodyPart();

  if(StepperBodyPart == NONE_INDEX)
    return;

  TrapData.VictimID = Stepper->GetID();
  TrapData.BodyParts = 1 << StepperBodyPart;
  Stepper->AddTrap(GetTrapID(), 1 << StepperBodyPart);

  if(Stepper->IsPlayer())
    ADD_MESSAGE("You try to step through the web but your %s sticks in it.", Stepper->GetBodyPartName(StepperBodyPart).CStr());
  else if(Stepper->CanBeSeenByPlayer())
    ADD_MESSAGE("%s gets stuck in the web.", Stepper->CHAR_NAME(DEFINITE));
}

void web::AddDescription(festring& Msg) const
{
  Msg << ". A web envelops the square";
}

void web::AddTrapName(festring& String, int) const
{
  String << "a spider web";
}

void web::Draw(bitmap* Bitmap, vector2d Pos, color24 Luminance) const
{
  Picture->NormalMaskedBlit(Bitmap, 0, 0, Pos, 16, 16, Luminance);
}

bool web::IsStuckToBodyPart(int I) const
{
  return !!(1 << I & TrapData.BodyParts);
}
