#include "hscore.h"
#include "save.h"
#include "felist.h"
#include "feio.h"
#include "festring.h"
#include "femath.h"

#define HIGH_SCORE_VERSION 120 // Increment this if changes make highscores incompatible

highscore::highscore(const festring& File) : LastAdd(0xFF)
{
  Load(File);
}

bool highscore::Add(long NewScore, const festring& NewEntry, time_t NewTime, long NewRandomID)
{
  for(ushort c = 0; c < Score.size(); ++c)
    if(Score[c] < NewScore)
      {
	Entry.insert(Entry.begin() + c, NewEntry);
	Score.insert(Score.begin() + c, NewScore);
	Time.insert(Time.begin() + c, NewTime);
	RandomID.insert(RandomID.begin() + c, NewRandomID);

	if(Score.size() > 100)
	  {
	    Entry.resize(100, festring());
	    Score.resize(100);
	    Time.resize(100);
	    RandomID.resize(100);
	  }

	LastAdd = c;
	return true;
      }

  if(Score.size() < 100)
    {
      LastAdd = Score.size();
      Entry.push_back(NewEntry);
      Score.push_back(NewScore);
      Time.push_back(NewTime);
      RandomID.push_back(NewRandomID);
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
      iosystem::TextScreen(CONST_S("There are no entries yet. Play a game to correct this."));
      return;
    }

  felist List(CONST_S("Adventurers' Hall of Fame"));
  festring Desc;

  for(ushort c = 0; c < Score.size(); ++c)
    {
      Desc.Empty();
      Desc << c + 1;
      Desc.Resize(5);
      Desc << Score[c];
      Desc.Resize(13);
      Desc << Entry[c];
      List.AddEntry(Desc, c == LastAdd ? WHITE : LIGHT_GRAY, 13);
    }

  List.SetFlags(FADE);
  List.SetPageLength(40);
  List.Draw();
}

void highscore::Save(const festring& File) const
{
  outputfile HighScore(File);
  HighScore << ushort(HIGH_SCORE_VERSION) << Score << Entry << Time << RandomID << LastAdd;
}

void highscore::Load(const festring& File)
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
    HighScore >> Score >> Entry >> Time >> RandomID >> LastAdd;
}

void highscore::AddToFile(highscore* To) const
{
  for(ushort c = 0; c < Score.size(); ++c)
    To->Add(Score[c], Entry[c], Time[c], RandomID[c]);
}

bool highscore::Add(long NewScore, const festring& NewEntry)
{
  return Add(NewScore, NewEntry, time(0), RAND());
}
