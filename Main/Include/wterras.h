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
  virtual const char* GetNameStem() const;
  virtual bool LongerArticle() const { return true; }
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 10; }
  virtual const char* SurviveMessage() const;
  virtual const char* MonsterSurviveMessage() const;
  virtual const char* DeathMessage() const;
  virtual const char* MonsterDeathVerb() const;
  virtual const char* ScoreEntry() const;
  virtual bool IsFatalToStay() const { return true; }
  virtual int GetWalkability() const;
);

class GWTERRAIN
(
  glacier,
  gwterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 90; }
);

class GWTERRAIN
(
  desert,
  gwterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 20; }
);

class GWTERRAIN
(
  snow,
  gwterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 80; }
);

class GWTERRAIN
(
  jungle,
  gwterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 50; }
);

class GWTERRAIN
(
  leafyforest,
  gwterrain,
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 60; }
);

class GWTERRAIN
(
  evergreenforest,
  gwterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual bool LongerArticle() const { return true; }
  virtual int GetPriority() const { return 70; }
);

class GWTERRAIN
(
  steppe,
  gwterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetPriority() const { return 30; }
);

class OWTERRAIN
(
  attnam,
  owterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
);

class OWTERRAIN
(
  elpuricave,
  owterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
);

class OWTERRAIN
(
  newattnam,
  owterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
);

class OWTERRAIN
(
  underwatertunnel,
  owterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual bool LongerArticle() const { return true; }
);

class OWTERRAIN
(
  underwatertunnelexit,
  owterrain,
 public:
  virtual const char* GetNameStem() const;
  virtual vector2d GetBitmapPos(int) const;
  virtual int GetAttachedDungeon() const;
  virtual bool LongerArticle() const { return true; }
  virtual int GetAttachedArea() const { return 2; }
);

#endif
