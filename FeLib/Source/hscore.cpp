/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

#include "hscore.h"
#include "save.h"
#include "felist.h"
#include "feio.h"
#include "femath.h"

/* Increment this if changes make highscores incompatible */
#define HIGH_SCORE_VERSION 120

const festring& highscore::GetEntry(int I) const { return Entry[I]; }
long highscore::GetScore(int I) const { return Score[I]; }
long highscore::GetSize() const { return Entry.size(); }

highscore::highscore(const festring& File) : LastAdd(0xFF) { Load(File); }

truth highscore::Add(long NewScore, const festring& NewEntry,
		     time_t NewTime, long NewRandomID)
{
  for(uint c = 0; c < Score.size(); ++c)
    if(Score[c] < NewScore)
    {
      Entry.insert(Entry.begin() + c, NewEntry);
      Score.insert(Score.begin() + c, NewScore);
      Time.insert(Time.begin() + c, NewTime);
      RandomID.insert(RandomID.begin() + c, NewRandomID);

      if(Score.size() > MAX_HIGHSCORES)
      {
	Entry.resize(MAX_HIGHSCORES, festring());
	Score.resize(MAX_HIGHSCORES);
	Time.resize(MAX_HIGHSCORES);
	RandomID.resize(MAX_HIGHSCORES);
      }

      LastAdd = c;
      return true;
    }

  if(Score.size() < MAX_HIGHSCORES)
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
    LastAdd = MAX_HIGHSCORES;
    return false;
  }
}

void highscore::Draw() const
{
  if(Score.empty())
  {
    iosystem::TextScreen(CONST_S("There are no entries yet. "
				 "Play a game to correct this."));
    return;
  }

  felist List(CONST_S("Adventurers' Hall of Fame"));
  festring Desc;

  for(uint c = 0; c < Score.size(); ++c)
  {
    Desc.Empty();
    Desc << c + 1;
    Desc.Resize(5, ' ');
    Desc << Score[c];
    Desc.Resize(13, ' ');
    Desc << Entry[c];
    List.AddEntry(Desc, c == uint(LastAdd) ? WHITE : LIGHT_GRAY, 13);
  }

  List.SetFlags(FADE);
  List.SetPageLength(40);
  List.Draw();
}

void highscore::Save(const festring& File) const
{
  outputfile HighScore(File);
  long CheckSum = HIGH_SCORE_VERSION + LastAdd;
  for(ushort c = 0; c < Score.size(); ++c)
  {
    CheckSum += Score[c] + Entry[c].GetCheckSum() + RandomID[c];
  }

  HighScore << ushort(HIGH_SCORE_VERSION) << Score
	    << Entry << Time << RandomID << LastAdd << CheckSum;
}

/* This function needs much more error handling */
truth highscore::Load(const festring& File)
{
  {
    inputfile HighScore(File, 0, false);

    if(!HighScore.IsOpen())
      return false;

    HighScore.Get();

    if(HighScore.Eof())
      return true; 
  }

  inputfile HighScore(File, 0, false);
  ushort HVersion;
  HighScore >> HVersion;

  if(HVersion == HIGH_SCORE_VERSION)
    HighScore >> Score >> Entry >> Time >> RandomID >> LastAdd;
}

truth highscore::MergeToFile(highscore* To) const
{
  truth MergedSomething = false;

  for(uint c = 0; c < Score.size(); ++c)
    if(!To->Find(Score[c], Entry[c], Time[c], RandomID[c]))
    {
      To->Add(Score[c], Entry[c], Time[c], RandomID[c]);
      MergedSomething = true;
    }

  return MergedSomething;
}

truth highscore::Add(long NewScore, const festring& NewEntry)
{
  return Add(NewScore, NewEntry, time(0), RAND());
}

/* Because of major stupidity this return the number of NEXT
   from the right entry, 0 = not found */

int highscore::Find(long AScore, const festring& AEntry,
		    time_t ATime, long ARandomID)
{
  for(uint c = 0; c < Score.size(); ++c)
  {
    if(AScore == Score[c] && Entry[c] == AEntry
       && ATime == Time[c] && ARandomID == RandomID[c])
      return c + 1;
  }

  return 0;
}

truth highscore::LastAddFailed() const
{ return LastAdd == MAX_HIGHSCORES; }
