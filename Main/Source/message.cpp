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
#include "stdover.h"
#include "error.h"
#include "save.h"

felist msgsystem::MessageHistory("Message history", WHITE, 128);
std::string msgsystem::LastMessage;
ushort msgsystem::Times;
ulong msgsystem::Begin, msgsystem::End;

void msgsystem::AddMessage(const char* Format, ...)
{
  char Message[1024];

  va_list AP;
  va_start(AP, Format);
  vsprintf(Message, Format, AP);
  va_end(AP);

  std::string Buffer(Message);
  Capitalize(Buffer);

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

  bool First = true;

  ushort LineLength = 80;
  ushort Marginal = 0;

  for(c = 0; Buffer.length(); ++c)
    {
      std::string Temp;

      if(First)
	{
	  Temp += Begin;

	  if(Begin != End)
	    Temp += std::string("-") + End;

	  if(Times != 1)
	    Temp += std::string(" (") + Times + "x)";

	  Temp += " ";
	  Marginal = Temp.length();
	  First = false;
	}
      else
	Temp.resize(Marginal, ' ');

      long Pos;

      if(ushort(Buffer.length()) > LineLength - Marginal)
	Pos = Buffer.find_last_of(' ', LineLength - Marginal);
      else
	Pos = Buffer.length();

      if(Pos < 0)
	{
	  Temp += Buffer.substr(0, LineLength - Marginal);
	  Buffer.erase(0, LineLength - Marginal);
	}
      else
	{
	  Temp += Buffer.substr(0, Pos);
	  Buffer.erase(0, Pos + 1);
	}

      MessageHistory.AddEntry(Temp, WHITE);
    }

  MessageHistory.SetSelected(MessageHistory.Length() > 8 ? MessageHistory.Length() - 8 : 0);
}

void msgsystem::Draw()
{
  MessageHistory.QuickDraw(vector2d(13, RES.Y - 122), (game::GetScreenSize().X << 4) + 6, 8);
}

void msgsystem::DrawMessageHistory()
{
  MessageHistory.Draw(vector2d(26, 42), 652, 32, MAKE_RGB(0, 0, 16), false, true, true, false, true);
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
  if(MessageHistory.GetSelected() < MessageHistory.Length() - 8)
    MessageHistory.EditSelected(1);
}

void msgsystem::ScrollUp()
{
  if(MessageHistory.GetSelected())
    MessageHistory.EditSelected(-1);
}

