#include <cctype>
#include <cstdio>
#include <cstdarg>

#include "message.h"
#include "felist.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"
#include "festring.h"
#include "error.h"
#include "save.h"

felist msgsystem::MessageHistory("Message history", WHITE, 128);
std::string msgsystem::LastMessage;
ushort msgsystem::Times;
ulong msgsystem::Begin, msgsystem::End;
bool msgsystem::Enabled = true;

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
  festring::Capitalize(Buffer);

  short c;

  if(!Buffer.length())
    ABORT("Empty message request!");

  /* Comment the first line and uncomment the second before the release! */

  if(isalpha(Buffer[Buffer.length() - 1]))
    Buffer += " (this sentence isn't terminated correctly because Hex doesn't know grammar rules)";
    //Buffer += ".";

  if(Buffer == LastMessage)
    {
      while(MessageHistory.Length() && MessageHistory.GetColor(MessageHistory.LastEntryIndex()) == WHITE)
	MessageHistory.Pop();

      ++Times;
      End = game::GetTicks() / 10;
    }
  else
    {
      for(c = MessageHistory.LastEntryIndex(); c >= 0 && MessageHistory.GetColor(c) == WHITE; --c)
	MessageHistory.SetColor(c, LIGHTGRAY);

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

  MessageHistory.AddEntry(Temp, WHITE, Marginal);
  MessageHistory.SetSelected(MessageHistory.Length() - 1);
}

void msgsystem::Draw()
{
  MessageHistory.QuickDraw(vector2d(13, RES.Y - 122), (game::GetScreenSize().X << 4) + 6, 8);
}

void msgsystem::DrawMessageHistory()
{
  MessageHistory.Draw(vector2d(26, 42), 652, 32, MakeRGB(0, 0, 16), false, true, true, false, true);
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
  if(MessageHistory.GetSelected() < MessageHistory.Length() - 1)
    MessageHistory.EditSelected(1);
}

void msgsystem::ScrollUp()
{
  if(MessageHistory.GetSelected())
    MessageHistory.EditSelected(-1);
}

