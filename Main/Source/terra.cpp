/* Compiled through areaset.cpp */

std::string terrain::SurviveMessage() const { return "somehow you survive"; }
std::string terrain::MonsterSurviveMessage() const { return "somehow survives"; }
std::string terrain::DeathMessage() const { return "strangely enough, you die"; }
std::string terrain::MonsterDeathVerb() const { return "dies"; }
std::string terrain::ScoreEntry() const { return "died on unfriendly terrain"; }

bool gterrain::IsWalkable(const character* ByWho) const
{
  return !ByWho || ByWho->CanWalk() || ByWho->CanFly();
}
