#include "graphics.h"
#include "bitmap.h"
#include "strover.h"
#include "hscore.h"
#include "whandler.h"
#include "save.h"
#include "colorbit.h"
#include "felist.h"

bool highscore::Add(long NewScore, std::string NewEntry)
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
      Desc += int(c + 1);
      Desc.resize(5, ' ');
      Desc += int(Score[c]);
      Desc.resize(13, ' ');
      Desc += Entry[c];
      List.AddEntry(Desc, c == LastAdd ? RED : LIGHTGRAY);
    }

  List.Draw(vector2d(10, 10), 780, 50, false, false, false, true);
}

void highscore::Save(std::string File) const
{
  outputfile HighScore(File);
  HighScore << ushort(HIGHSCORE_VERSION) << Score << Entry << LastAdd;
}

void highscore::Load(std::string File)
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

  if(HVersion == 110)
    HighScore >> Score >> Entry >> LastAdd;
}

