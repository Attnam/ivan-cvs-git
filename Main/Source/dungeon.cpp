#include "dungeon.h"
#include "level.h"
#include "festring.h"
#include "script.h"
#include "error.h"
#include "game.h"
#include "ivandef.h"

dungeon::dungeon() { }

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
  std::map<uchar, dungeonscript>::const_iterator DungeonIterator = game::GetGameScript()->GetDungeon().find(Index);

  if(DungeonIterator != game::GetGameScript()->GetDungeon().end())
    DungeonScript = &DungeonIterator->second;
  else
    {
      ABORT("Unknown dungeon #%d requested!", int(Index));
      return;
    }

  Level = new level*[GetLevels()];
  Generated = new bool[GetLevels()];

  for(ushort c = 0; c < GetLevels(); ++c)
    Level[c] = 0;
}

const levelscript* dungeon::GetLevelScript(ushort Index)
{
  std::map<uchar, levelscript>::const_iterator LevelIterator = DungeonScript->GetLevel().find(Index);

  const levelscript* LevelScript;

  if(LevelIterator != DungeonScript->GetLevel().end())
    LevelScript = &LevelIterator->second;
  else
    LevelScript = DungeonScript->GetLevelDefault();

  return LevelScript;
}

/* Returns whether the level has been visited before */

bool dungeon::PrepareLevel(ushort Index, bool Visual)
{
  if(Generated[Index])
    {
      level* NewLevel = LoadLevel(game::SaveName(), Index);
      game::SetCurrentArea(NewLevel);
      game::SetCurrentLevel(NewLevel);
      game::SetCurrentLSquareMap(NewLevel->GetMap());
      return true;
    }
  else
    {
      level* NewLevel = Level[Index] = new level;
      NewLevel->SetDungeon(this);
      NewLevel->SetIndex(Index);
      NewLevel->SetLevelScript(GetLevelScript(Index));
      game::SetCurrentArea(NewLevel);
      game::SetCurrentLevel(NewLevel);

      if(Visual)
	game::TextScreen("Entering " + GetLevelDescription(Index) + "...\n\nThis may take some time, please wait.", WHITE, false, &game::BusyAnimation);

      game::SetIsGenerating(true);
      NewLevel->Generate(Index);
      game::SetCurrentLSquareMap(NewLevel->GetMap());
      Generated[Index] = true;
      game::BusyAnimation();

      if(*NewLevel->GetLevelScript()->GenerateMonsters())
	NewLevel->GenerateNewMonsters(NewLevel->GetIdealPopulation(), false);

      game::SetIsGenerating(false);
      return false;
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

level* dungeon::LoadLevel(const std::string& SaveName, ushort Number)
{
  inputfile SaveFile(SaveName + "." + Index + Number);
  SaveFile >> Level[Number];
  Level[Number]->SetDungeon(this);
  Level[Number]->SetIndex(Number);
  Level[Number]->SetLevelScript(GetLevelScript(Number));
  return Level[Number];
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

ushort dungeon::GetLevels() const
{
  return *DungeonScript->GetLevels();
}

std::string dungeon::GetLevelDescription(ushort Index)
{
  if(GetLevel(Index)->GetLevelScript()->GetDescription())
    return *GetLevel(Index)->GetLevelScript()->GetDescription();
  else
    return *DungeonScript->GetDescription() + " level " + (Index + 1);
}

std::string dungeon::GetShortLevelDescription(ushort Index)
{
  if(GetLevel(Index)->GetLevelScript()->GetShortDescription())
    return *GetLevel(Index)->GetLevelScript()->GetShortDescription();
  else
    return *DungeonScript->GetShortDescription() + " level " + (Index + 1);
}

outputfile& operator<<(outputfile& SaveFile, const dungeon* Dungeon)
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
