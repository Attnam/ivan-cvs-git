/* Compiled through wmapset.cpp */

const char* ocean::GetNameStem() const { return "ocean"; }
vector2d ocean::GetBitmapPos(ushort Frame) const { return vector2d(48 + ((Frame << 3)&~8), 48); }
const char* ocean::SurviveMessage() const { return "you manage to reach the shore"; }
const char* ocean::MonsterSurviveMessage() const { return "manages to reach the shore"; }
const char* ocean::DeathMessage() const { return "you drown"; }
const char* ocean::ScoreEntry() const { return "drowned"; }

const char* glacier::GetNameStem() const { return "glacier"; }
vector2d glacier::GetBitmapPos(ushort) const { return vector2d(16, 16); }

const char* desert::GetNameStem() const { return "desert"; }
vector2d desert::GetBitmapPos(ushort) const { return vector2d(64, 16); }

const char* snow::GetNameStem() const { return "tundra"; }
vector2d snow::GetBitmapPos(ushort) const { return vector2d(112, 16); }

const char* jungle::GetNameStem() const { return "jungle"; }
vector2d jungle::GetBitmapPos(ushort) const { return vector2d(208, 16); }

const char* leafyforest::GetNameStem() const { return "leafy forest"; }
vector2d leafyforest::GetBitmapPos(ushort) const { return vector2d(304, 16); }

const char* evergreenforest::GetNameStem() const { return "evergreen forest"; }
vector2d evergreenforest::GetBitmapPos(ushort) const { return vector2d(352, 16); }

const char* steppe::GetNameStem() const { return "steppe"; }
vector2d steppe::GetBitmapPos(ushort) const { return vector2d(160, 16); }

const char* attnam::GetNameStem() const { return "migthy cathedral reaching the clouds"; }
vector2d attnam::GetBitmapPos(ushort) const { return vector2d(0, 48); }
uchar attnam::GetAttachedDungeon() const { return ATTNAM; }

const char* elpuricave::GetNameStem() const { return "hideous cave entry radiating pure evil"; }
vector2d elpuricave::GetBitmapPos(ushort) const { return vector2d(16, 48); }
uchar elpuricave::GetAttachedDungeon() const { return ELPURI_CAVE; }

const char* newattnam::GetNameStem() const { return "primitive village"; }
vector2d newattnam::GetBitmapPos(ushort) const { return vector2d(16, 64); }
uchar newattnam::GetAttachedDungeon() const { return NEW_ATTNAM; }

const char* underwatertunnel::GetNameStem() const { return "entrance to an underwater tunnel"; }
vector2d underwatertunnel::GetBitmapPos(ushort) const { return vector2d(32, 64); }
uchar underwatertunnel::GetAttachedDungeon() const { return UNDER_WATER_TUNNEL; }

const char* underwatertunnelexit::GetNameStem() const { return "exit from an underwater tunnel"; }
vector2d underwatertunnelexit::GetBitmapPos(ushort) const { return vector2d(32, 64); }
uchar underwatertunnelexit::GetAttachedDungeon() const { return UNDER_WATER_TUNNEL; }

const char* mondedr::GetNameStem() const { return "very unattractive place"; }
vector2d mondedr::GetBitmapPos(ushort) const { return vector2d(0, 64); }
uchar mondedr::GetAttachedDungeon() const { return MONDEDR; }

/*bool attnam::Enter(bool DirectionUp) const
{
  if(!owterrain::Enter(DirectionUp))
    return false;

  if(PLAYER->HasGoldenEagleShirt() && game::GetPetrus() && game::GetTeam(ATTNAM_TEAM)->GetRelation(PLAYER->GetTeam()) != HOSTILE && game::GetStoryState() < 2)
    {
      game::GetCurrentLevel()->GetLSquare(30, 54)->KickAnyoneStandingHereAway();
      game::GetPetrus()->Move(vector2d(30, 54), true);
      game::GetPetrus()->ChangeTeam(game::GetTeam(FINAL_BATTLE_TEAM));
      game::SetStoryState(2);
      game::GetPetrus()->SetHomePos(vector2d(30, 54));

      if(game::GetHaedlac())
	{
	  game::GetCurrentLevel()->GetLSquare(30, 57)->KickAnyoneStandingHereAway();
	  game::GetHaedlac()->Move(vector2d(30, 57), true);
	  game::GetHaedlac()->ChangeTeam(game::GetTeam(FINAL_BATTLE_TEAM));
	  game::GetHaedlac()->SetHomePos(vector2d(30, 57));
	}
    }

  if(configuration::GetAutoSaveInterval())
    game::Save(game::GetAutoSaveFileName().CStr());

  return true;
}*/

bool ocean::IsWalkable(const character* ByWho) const
{
  return ByWho && (ByWho->CanSwim() || ByWho->CanFly());
}
