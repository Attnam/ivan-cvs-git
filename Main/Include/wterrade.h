#ifndef __WTERRADE_H__
#define __WTERRADE_H__

#include "wterraba.h"

class GROUNDWORLDMAPTERRAIN
(
	ocean,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "ocean"; }
	virtual std::string Article() const { return "an"; }
	virtual vector2d GetBitmapPos() const { return vector2d(208, 64); }
	virtual uchar Priority() const { return 10; }
);

class GROUNDWORLDMAPTERRAIN
(
	glacier,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "glacier"; }
	virtual vector2d GetBitmapPos() const { return vector2d(16, 16); }
	virtual uchar Priority() const { return 90; }
);

class GROUNDWORLDMAPTERRAIN
(
	desert,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "desert"; }
	virtual vector2d GetBitmapPos() const { return vector2d(64, 16); }
	virtual uchar Priority() const { return 20; }
);

class GROUNDWORLDMAPTERRAIN
(
	snow,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "tundra"; }
	virtual vector2d GetBitmapPos() const { return vector2d(112, 16); }
	virtual uchar Priority() const { return 80; }
);

class GROUNDWORLDMAPTERRAIN
(
	jungle,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "jungle"; }
	virtual vector2d GetBitmapPos() const { return vector2d(208, 16); }
	virtual uchar Priority() const { return 50; }
);

class GROUNDWORLDMAPTERRAIN
(
	swamp,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "swamp"; }
	virtual vector2d GetBitmapPos() const { return vector2d(256, 16); }
	virtual uchar Priority() const { return 40; }
);

class GROUNDWORLDMAPTERRAIN
(
	leafyforest,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "leafy forest"; }
	virtual vector2d GetBitmapPos() const { return vector2d(304, 16); }
	virtual uchar Priority() const { return 60; }
);

class GROUNDWORLDMAPTERRAIN
(
	evergreenforest,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "evergreen forest"; }
	virtual std::string Article() const { return "an"; }
	virtual vector2d GetBitmapPos() const { return vector2d(352, 16); }
	virtual uchar Priority() const { return 70; }
);

class GROUNDWORLDMAPTERRAIN
(
	steppe,
	groundworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "steppe"; }
	virtual vector2d GetBitmapPos() const { return vector2d(160, 16); }
	virtual uchar Priority() const { return 30; }
);

class OVERWORLDMAPTERRAIN
(
	atmosphere,
	overworldmapterrain,
	{
	},
	virtual std::string NameStem() const { return "atmosphere"; }
	virtual std::string Article() const { return "an"; }
	virtual vector2d GetBitmapPos() const { return vector2d(208, 256); }
);

class OVERWORLDMAPTERRAIN
(
	attnam,
	overworldmapterrain,
	{
		//SetLinkedDungeon(game::GetDungeon(1));
	},
	virtual std::string NameStem() const { return "migthy cathedral reaching the clouds"; }
	virtual vector2d GetBitmapPos() const { return vector2d(0, 48); }
	virtual bool GoDown(character*) const;
);

class OVERWORLDMAPTERRAIN
(
	elpuricave,
	overworldmapterrain,
	{
		//SetLinkedDungeon(game::GetDungeon(0));
	},
	virtual std::string NameStem() const { return "hideous cave entry radiating pure navastating"; }
	virtual vector2d GetBitmapPos() const { return vector2d(16, 48); }
	virtual bool GoDown(character*) const;
);

#endif
