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

trapprototype::trapprototype(trap* (*Cloner)(bool), const char* ClassID) : Cloner(Cloner), ClassID(ClassID) { Index = protocontainer<trap>::Add(this); }

trap::trap() : entity(HAS_BE), Next(0) { }
square* trap::GetSquareUnderEntity(int) const { return LSquareUnder; }

trap::~trap()
{
}

trap::trap(donothing) : entity(HAS_BE), Next(0)
{
}

void trap::Save(outputfile& SaveFile) const
{
}

void trap::Load(inputfile& SaveFile)
{
  LSquareUnder = static_cast<lsquare*>(game::GetSquareInLoad());
}

outputfile& operator<<(outputfile& SaveFile, const trapdata* Data)
{
  SaveFile << *Data;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, trapdata*& Data)
{
  Data = new trapdata;
  SaveFile >> *Data;
  return SaveFile;
}

outputfile& operator<<(outputfile& SaveFile, const trapdata& Data)
{
  SaveFile << Data.TrapID << Data.VictimID << Data.BodyParts;
  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, trapdata& Data)
{
  SaveFile >> Data.TrapID >> Data.VictimID >> Data.BodyParts;
  return SaveFile;
}
