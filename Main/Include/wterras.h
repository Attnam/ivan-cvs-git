#ifndef __WTERRAS_H__
#define __WTERRAS_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "wterra.h"

class GWTERRAIN
(
  ocean,
  gwterrain,
 public:
  virtual void VirtualConstructor(bool) { SetAnimationFrames(32); }
  virtual std::string GetNameStem() const;
  virtual bool LongerArticle() const { return true; }
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 10; }
  virtual bool IsWalkable(const character*) const;
  virtual std::string SurviveMessage() const;
  virtual std::string MonsterSurviveMessage() const;
  virtual std::string DeathMessage() const;
  virtual std::string ScoreEntry() const;
  virtual bool IsFatalToStay() const { return true; }
);

class GWTERRAIN
(
  glacier,
  gwterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 90; }
);

class GWTERRAIN
(
  desert,
  gwterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 20; }
);

class GWTERRAIN
(
  snow,
  gwterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 80; }
);

class GWTERRAIN
(
  jungle,
  gwterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 50; }
);

class GWTERRAIN
(
  leafyforest,
  gwterrain,
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 60; }
);

class GWTERRAIN
(
  evergreenforest,
  gwterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual bool LongerArticle() const { return true; }
  virtual uchar Priority() const { return 70; }
);

class GWTERRAIN
(
  steppe,
  gwterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar Priority() const { return 30; }
);

class OWTERRAIN
(
  atmosphere,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual void Draw(bitmap*, vector2d, ulong, bool) const { }
);

class OWTERRAIN
(
  attnam,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar GetAttachedDungeon() const;
  virtual bool Enter(bool) const;
);

class OWTERRAIN
(
  elpuricave,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar GetAttachedDungeon() const;
);

class OWTERRAIN
(
  newattnam,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar GetAttachedDungeon() const;
);

class OWTERRAIN
(
  underwatertunnel,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar GetAttachedDungeon() const;
  virtual bool LongerArticle() const { return true; }
);

class OWTERRAIN
(
  underwatertunnelexit,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar GetAttachedDungeon() const;
  virtual bool LongerArticle() const { return true; }
  virtual uchar GetAttachedArea() const { return 2; }
);

class OWTERRAIN
(
  mondedr,
  owterrain,
 public:
  virtual std::string GetNameStem() const;
  virtual vector2d GetBitmapPos(ushort) const;
  virtual uchar GetAttachedDungeon() const;
);

#endif
