/* Compiled through roomset.cpp */

roomprototype::roomprototype(room* (*Cloner)(bool), const char* ClassId) : Cloner(Cloner), ClassId(ClassId) { Index = protocontainer<room>::Add(this); }

room::room(donothing) : MasterID(0) { }
room::~room() { }
character* room::GetMaster() const { return game::SearchCharacter(MasterID); }

void room::Save(outputfile& SaveFile) const
{
  SaveFile << GetType();
  SaveFile << Pos << Size << Index << DivineMaster << MasterID;
}

void room::Load(inputfile& SaveFile)
{
  SaveFile >> Pos >> Size >> Index >> DivineMaster >> MasterID;
}

room* roomprototype::CloneAndLoad(inputfile& SaveFile) const
{
  room* Room = Cloner(true);
  Room->Load(SaveFile);
  return Room;
}

void room::DestroyTerrain(character* Who)
{
  if(Who && MasterIsActive())
    Who->Hostility(GetMaster());

  if(Who && Who->IsPlayer() && DivineMaster)
    game::GetGod(DivineMaster)->AdjustRelation(GetGodRelationAdjustment());
}

/* returns true if player agrees to continue */

bool room::CheckDestroyTerrain(character* Infidel) 
{
  if(!MasterIsActive() || Infidel == GetMaster() || GetMaster()->GetRelation(Infidel) == HOSTILE)
    return true;

  ADD_MESSAGE("%s might not like this.", GetMaster()->CHAR_NAME(DEFINITE));

  if(game::BoolQuestion("Are you sure you want to do this? [y/N]"))
    {
      DestroyTerrain(Infidel);
      return true;
    }
  else
    return false; 
}

bool room::MasterIsActive() const
{
  character* Master = GetMaster();
  return Master && Master->IsEnabled();
}

bool room::CheckKickSquare(const character* Kicker, const lsquare* LSquare) const
{
  if(!AllowKick(Kicker, LSquare))
    {
      ADD_MESSAGE("That would be vandalism.");

      if(!game::BoolQuestion("Do you still want to do this? [y/N]"))
	return false;
    }
  return true;
}
