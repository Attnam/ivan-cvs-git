#include "hscore.h"
#include "save.h"
#include "felist.h"
#include "feio.h"
#include "festring.h"

#define HIGH_SCORE_VERSION 110 // Increment this if changes make highscores incompatible

highscore::highscore(const std::string& File) : LastAdd(0xFF)
{
  Load(File);
}

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
  if(Score.empty())
    {
      iosystem::TextScreen("There are no entries yet. Play a game to correct this.");
      return;
    }

  felist List("Adventurers' Hall of Fame");
  std::string Desc;

  for(ushort c = 0; c < Score.size(); ++c)
    {
      Desc.resize(0);
      Desc += c + 1;
      Desc.resize(5, ' ');
      Desc += Score[c];
      Desc.resize(13, ' ');
      Desc += Entry[c];
      List.AddEntry(Desc, c == LastAdd ? WHITE : LIGHT_GRAY, 13);
    }

  List.SetFlags(FADE);
  List.SetPageLength(50);
  List.Draw();
}

void highscore::Save(const std::string& File) const
{
  outputfile HighScore(File);
  HighScore << ushort(HIGH_SCORE_VERSION) << Score << Entry << LastAdd;
}

void highscore::Load(const std::string& File)
{
  {
    inputfile HighScore(File, 0, false);

    if(!HighScore.IsOpen())
      return;

    HighScore.Get();

    if(HighScore.Eof())
      return;
  }

  inputfile HighScore(File, 0, false);
  ushort HVersion;
  HighScore >> HVersion;

  if(HVersion == HIGH_SCORE_VERSION)
    HighScore >> Score >> Entry >> LastAdd;
}
