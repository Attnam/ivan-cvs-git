#include "dungeon.h"
#include "level.h"
#include "strover.h"
#include "script.h"
#include "feio.h"
#include "save.h"

dungeon::dungeon(uchar Index) : Index(Index)
{
  Initialize();

  for(ushort c = 0; c < GetLevels(); ++c)
    Generated[c] = false;
}

dungeon::~dungeon()
{
  for(ushort c = 0; c < GetLevels(); ++c)
    delete Level[c];

  delete [] Level;
}

void dungeon::Initialize()
{
  std::map<uchar, dungeonscript*>::const_iterator DungeonIterator = game::GetGameScript()->GetDungeon().find(Index);

  if(DungeonIterator != game::GetGameScript()->GetDungeon().end())
    DungeonScript = DungeonIterator->second;
  else
    DungeonScript = game::GetGameScript()->GetDungeonDefault();

  Level = new level*[GetLevels()];
  Generated = new bool[GetLevels()];

  for(ushort c = 0; c < GetLevels(); ++c)
    Level[c] = 0;
}

levelscript* dungeon::GetLevelScript(ushort Index)
{
  std::map<uchar, levelscript*>::const_iterator LevelIterator = DungeonScript->GetLevel().find(Index);

  levelscript* LevelScript;

  if(LevelIterator != DungeonScript->GetLevel().end())
    {
      LevelScript = LevelIterator->second;

      if(*LevelScript->GetReCalculate())
	{
	  inputfile ScriptFile(GAME_DIR + "Script/dungeon.dat");
	  LevelScript->ReadFrom(ScriptFile, true);
	}
    }
  else
    {
      LevelScript = DungeonScript->GetLevelDefault();

      if(*LevelScript->GetReCalculate())
	{
	  inputfile ScriptFile(GAME_DIR + "Script/dungeon.dat");
	  LevelScript->ReadFrom(ScriptFile, true);
	}
    }

  return LevelScript;
}

void dungeon::PrepareLevel(ushort Index)
{
  if(Generated[Index])
    {
      LoadLevel(game::SaveName(), Index);
    }
  else
    {
      iosystem::TextScreen("Generating level...\n\nThis may take some time, please wait.", WHITE, false, &game::BusyAnimation);
      Level[Index] = new level;
      Level[Index]->Generate(GetLevelScript(Index));
      Generated[Index] = true;
      game::BusyAnimation();

      if(*Level[Index]->GetLevelScript()->GetGenerateMonsters())
	Level[Index]->GenerateNewMonsters(Level[Index]->GetIdealPopulation(), false);

      game::GetCurrentArea()->SendNewDrawRequest();
    }
}

void dungeon::SaveLevel(const std::string& SaveName, ushort Number, bool DeleteAfterwards)
{
  outputfile SaveFile(SaveName + "." + Index + Number);

  SaveFile << Level[Number];

  if(DeleteAfterwards)
    {
      delete Level[Number];
      Level[Number] = 0;
    }
}

void dungeon::LoadLevel(const std::string& SaveName, ushort Number)
{
  inputfile SaveFile(SaveName + "." + Index + Number);

  Level[Number] = new level;
  Level[Number]->Load(SaveFile);
  Level[Number]->SetLevelScript(GetLevelScript(Number));
}

void dungeon::Save(outputfile& SaveFile) const
{
  SaveFile << Index << WorldMapPos;

  for(ushort c = 0; c < GetLevels(); ++c)
    SaveFile << Generated[c];
}

void dungeon::Load(inputfile& SaveFile)
{
  SaveFile >> Index >> WorldMapPos;

  Initialize();

  for(ushort c = 0; c < GetLevels(); ++c)
    SaveFile >> Generated[c];
}

uchar dungeon::GetLevels() const
{
  return *DungeonScript->GetLevels();
}

std::string dungeon::GetLevelDescription(ushort Index)
{
  if(GetLevel(Index)->GetLevelScript()->GetDescription(false))
    return *GetLevel(Index)->GetLevelScript()->GetDescription();
  else
    return std::string("level ") + (Index + 1);
}

outputfile& operator<<(outputfile& SaveFile, dungeon* Dungeon)
{
  if(Dungeon)
    {
      SaveFile.Put(1);
      Dungeon->Save(SaveFile);
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inputfile& operator>>(inputfile& SaveFile, dungeon*& Dungeon)
{
  if(SaveFile.Get())
    {
      Dungeon = new dungeon;
      Dungeon->Load(SaveFile);
    }

  return SaveFile;
}
