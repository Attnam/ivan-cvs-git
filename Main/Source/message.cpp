#include <cstdarg>
#include <cctype>

#include "message.h"
#include "festring.h"
#include "felist.h"
#include "game.h"
#include "save.h"
#include "graphics.h"

felist msgsystem::MessageHistory("Message history", WHITE, 128);
std::string msgsystem::LastMessage;
std::string msgsystem::BigMessage;
ushort msgsystem::Times;
ulong msgsystem::Begin, msgsystem::End;
bool msgsystem::Enabled = true;
bool msgsystem::BigMessageMode = false;

void msgsystem::AddMessage(const char* Format, ...)
{
  if(!Enabled)
    return;

  char Message[1024];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Message, Format, AP);
  va_end(AP);

  std::string Buffer(Message);

  if(!Buffer.length())
    ABORT("Empty message request!");

  festring::Capitalize(Buffer);

  /* Comment the first line and uncomment the second before the release! */

  if(isalpha(Buffer[Buffer.length() - 1]))
    Buffer << " (this sentence isn't terminated correctly because Hex doesn't know grammar rules)";
    //Buffer << ".";

  if(BigMessageMode)
    {
      if(BigMessage.length())
	BigMessage << ' ';

      BigMessage << Buffer;
      return;
    }

  if(Buffer == LastMessage)
    {
      while(MessageHistory.GetLength() && MessageHistory.GetColor(MessageHistory.GetLastEntryIndex()) == WHITE)
	MessageHistory.Pop();

      ++Times;
      End = game::GetTicks() / 10;
    }
  else
    {
      for(short c = MessageHistory.GetLastEntryIndex(); c >= 0 && MessageHistory.GetColor(c) == WHITE; --c)
	MessageHistory.SetColor(c, LIGHT_GRAY);

      Times = 1;
      Begin = End = game::GetTicks() / 10;
      LastMessage = Buffer;
    }

  std::string Temp;
  Temp << Begin;

  if(Begin != End)
    Temp << "-" << End;

  if(Times != 1)
    Temp << " (" << Times << "x)";

  Temp << " ";
  ushort Marginal = Temp.length();
  Temp << Buffer;

  std::vector<std::string> Chapter;
  festring::SplitString(Temp, Chapter, 78, Marginal);

  for(ushort c = 0; c < Chapter.size(); ++c)
    MessageHistory.AddEntry(Chapter[c], WHITE);

  MessageHistory.SetSelected(MessageHistory.GetLastEntryIndex());
}

void msgsystem::Draw()
{
  bool WasInBigMessageMode = BigMessageMode;
  LeaveBigMessageMode();
  MessageHistory.QuickDraw(vector2d(13, RES.Y - 122), (game::GetScreenSize().X << 4) + 6, 8);

  if(WasInBigMessageMode)
    EnterBigMessageMode();
}

void msgsystem::DrawMessageHistory()
{
  game::SetStandardListAttributes(MessageHistory); // optimize
  MessageHistory.AddFlags(INVERSE_MODE);
  MessageHistory.Draw();
}

void msgsystem::Format()
{
  MessageHistory.Empty();
  LastMessage.resize(0);
}

void msgsystem::Save(outputfile& SaveFile)
{
  MessageHistory.Save(SaveFile);
  SaveFile << LastMessage << Times << Begin << End;
}

void msgsystem::Load(inputfile& SaveFile)
{
  MessageHistory.Load(SaveFile);
  SaveFile >> LastMessage >> Times >> Begin >> End;
}

void msgsystem::ScrollDown()
{
  if(MessageHistory.GetSelected() < MessageHistory.GetLastEntryIndex())
    MessageHistory.EditSelected(1);
}

void msgsystem::ScrollUp()
{
  if(MessageHistory.GetSelected())
    MessageHistory.EditSelected(-1);
}

void msgsystem::LeaveBigMessageMode()
{
  BigMessageMode = false;

  if(BigMessage.length())
    {
      AddMessage("%s", BigMessage.c_str());
      BigMessage.resize(0);
    }
}
