#include "graphics.h"
#include "bitmap.h"
#include "festring.h"
#include "hscore.h"
#include "whandler.h"
#include "save.h"
#include "colorbit.h"
#include "felist.h"

#define HIGHSCORE_VERSION 110 // Increment this if changes make highscores incompatible

bool highscore::Add(long NewScore, const std::string& NewEntry)
{
  for(ushort c = 0; c < Score.size(); ++c)
    if(Score[c] < NewScore)
      {
	Entry.insert(Entry.begin() + c, NewEntry);
	Score.insert(Score.begin() + c, NewScore);

	if(Score.size() > 100)
	  {
	    Entry.resize(100, std::string());
	    Score.resize(100);
	  }

	LastAdd = c;
	return true;
      }

  if(Score.size() < 100)
    {
      LastAdd = Score.size();
      Entry.push_back(NewEntry);
      Score.push_back(NewScore);
      return true;
    }
  else
  {
    LastAdd = 100;
    return false;
  }
}

void highscore::Draw() const
{
  felist List("Adventurers' Hall of Fame", WHITE, 0);

  for(ushort c = 0; c < Score.size(); ++c)
    {
      std::string Desc;
      Desc += c + 1;
      Desc.resize(5, ' ');
      Desc += Score[c];
      Desc.resize(13, ' ');
      Desc += Entry[c];
      List.AddEntry(Desc, c == LastAdd ? WHITE : LIGHTGRAY, 13);
    }

  List.SetFlags(FADE);
  List.SetPageLength(50);
  List.Draw();
}

void highscore::Save(const std::string& File) const
{
  outputfile HighScore(File);
  HighScore << ushort(HIGHSCORE_VERSION) << Score << Entry << LastAdd;
}

void highscore::Load(const std::string& File)
{
  {
    inputfile HighScore(File, false);

    if(!HighScore.IsOpen())
      return;

    HighScore.Get();

    if(HighScore.Eof())
      return;
  }

  inputfile HighScore(File, false);
  ushort HVersion;
  HighScore >> HVersion;

  if(HVersion == HIGHSCORE_VERSION)
    HighScore >> Score >> Entry >> LastAdd;
}
