#ifndef __WTERRADE_H__
#define __WTERRADE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "wterraba.h"

class GWTERRAIN
(
  ocean,
  gwterrain,
 public:
  virtual void VirtualConstructor(bool) { SetAnimationFrames(16); }
  virtual std::string GetNameStem() const { return "ocean"; }
  virtual bool LongerArticle() const { return true; }
  virtual vector2d GetBitmapPos(ushort Frame) const { return vector2d(32 + Frame * 16, 160); }
  virtual uchar Priority() const { return 10; }
  virtual bool IsWalkable(character*) const;
  virtual std::string SurviveMessage() const { return "you manage to reach the shore"; }
  virtual std::string DeathMessage() const { return "you drown"; }
  virtual std::string ScoreEntry() const { return "drowned"; }
);

class GWTERRAIN
(
  glacier,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "glacier"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 16); }
  virtual uchar Priority() const { return 90; }
);

class GWTERRAIN
(
  desert,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "desert"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(64, 16); }
  virtual uchar Priority() const { return 20; }
);

class GWTERRAIN
(
  snow,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "tundra"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(112, 16); }
  virtual uchar Priority() const { return 80; }
);

class GWTERRAIN
(
  jungle,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "jungle"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(208, 16); }
  virtual uchar Priority() const { return 50; }
);

class GWTERRAIN
(
  swamp,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "swamp"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(256, 16); }
  virtual uchar Priority() const { return 40; }
);

class GWTERRAIN
(
  leafyforest,
  gwterrain,
  virtual std::string GetNameStem() const { return "leafy forest"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(304, 16); }
  virtual uchar Priority() const { return 60; }
);

class GWTERRAIN
(
  evergreenforest,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "evergreen forest"; }
  virtual bool LongerArticle() const { return true; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(352, 16); }
  virtual uchar Priority() const { return 70; }
);

class GWTERRAIN
(
  steppe,
  gwterrain,
 public:
  virtual std::string GetNameStem() const { return "steppe"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(160, 16); }
  virtual uchar Priority() const { return 30; }
);

class OWTERRAIN
(
  atmosphere,
  owterrain,
 public:
  virtual std::string GetNameStem() const { return ""; }
  virtual bool LongerArticle() const { return true; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 0); }
  virtual void Draw(bitmap*, vector2d, ushort, bool, bool) const { }
);

class OWTERRAIN
(
  attnam,
  owterrain,
 public:
  virtual std::string GetNameStem() const { return "migthy cathedral reaching the clouds"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(0, 48); }
  virtual bool GoDown(character*) const;
);

class OWTERRAIN
(
  elpuricave,
  owterrain,
 public:
  virtual std::string GetNameStem() const { return "hideous cave entry radiating pure navastating"; }
  virtual vector2d GetBitmapPos(ushort) const { return vector2d(16, 48); }
  virtual bool GoDown(character*) const;
);

#endif

