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

/* Compiled through roomset.cpp */

roomprototype::roomprototype(room* (*Cloner)(bool), const char* ClassID) : Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<room>::Add(this); }

room::room(donothing) : LastMasterSearchTick(0), MasterID(0) { }
room::~room() { }

void room::Save(outputfile& SaveFile) const
{
  SaveFile << (ushort)GetType();
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

  if(game::BoolQuestion(CONST_S("Are you sure you want to do this? [y/N]")))
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

      if(!game::BoolQuestion(CONST_S("Do you still want to do this? [y/N]")))
	return false;
    }
  return true;
}

character* room::GetMaster() const
{
  ulong Tick = game::GetTick();

  if(LastMasterSearchTick == Tick)
    return Master;
  else
    {
      LastMasterSearchTick = Tick;
      return Master = game::SearchCharacter(MasterID);
    }
}
