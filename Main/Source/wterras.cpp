/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

/* Compiled through wmapset.cpp */

const char* ocean::GetNameStem() const { return "ocean"; }
vector2d ocean::GetBitmapPos(int Frame) const { return vector2d(48 + ((Frame << 3)&~8), 48); }
const char* ocean::SurviveMessage() const { return "you manage to reach the shore"; }
const char* ocean::MonsterSurviveMessage() const { return "manages to reach the shore"; }
const char* ocean::DeathMessage() const { return "you drown"; }
const char* ocean::ScoreEntry() const { return "drowned"; }

const char* glacier::GetNameStem() const { return "glacier"; }
vector2d glacier::GetBitmapPos(int) const { return vector2d(16, 16); }

const char* desert::GetNameStem() const { return "desert"; }
vector2d desert::GetBitmapPos(int) const { return vector2d(64, 16); }

const char* snow::GetNameStem() const { return "tundra"; }
vector2d snow::GetBitmapPos(int) const { return vector2d(112, 16); }

const char* jungle::GetNameStem() const { return "jungle"; }
vector2d jungle::GetBitmapPos(int) const { return vector2d(208, 16); }

const char* leafyforest::GetNameStem() const { return "leafy forest"; }
vector2d leafyforest::GetBitmapPos(int) const { return vector2d(304, 16); }

const char* evergreenforest::GetNameStem() const { return "evergreen forest"; }
vector2d evergreenforest::GetBitmapPos(int) const { return vector2d(352, 16); }

const char* steppe::GetNameStem() const { return "steppe"; }
vector2d steppe::GetBitmapPos(int) const { return vector2d(160, 16); }

const char* attnam::GetNameStem() const { return "migthy cathedral reaching the clouds"; }
vector2d attnam::GetBitmapPos(int) const { return vector2d(0, 48); }
int attnam::GetAttachedDungeon() const { return ATTNAM; }

const char* elpuricave::GetNameStem() const { return "hideous cave entry radiating pure evil"; }
vector2d elpuricave::GetBitmapPos(int) const { return vector2d(16, 48); }
int elpuricave::GetAttachedDungeon() const { return ELPURI_CAVE; }

const char* newattnam::GetNameStem() const { return "primitive village"; }
vector2d newattnam::GetBitmapPos(int) const { return vector2d(16, 64); }
int newattnam::GetAttachedDungeon() const { return NEW_ATTNAM; }

const char* underwatertunnel::GetNameStem() const { return "entrance to an underwater tunnel"; }
vector2d underwatertunnel::GetBitmapPos(int) const { return vector2d(32, 64); }
int underwatertunnel::GetAttachedDungeon() const { return UNDER_WATER_TUNNEL; }

const char* underwatertunnelexit::GetNameStem() const { return "exit from an underwater tunnel"; }
vector2d underwatertunnelexit::GetBitmapPos(int) const { return vector2d(32, 64); }
int underwatertunnelexit::GetAttachedDungeon() const { return UNDER_WATER_TUNNEL; }

int ocean::GetWalkability() const { return ANY_MOVE&~WALK; }
