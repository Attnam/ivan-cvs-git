#include <cstdio>
#include <cstdarg>

#include "message.h"
#include "felist.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "colorbit.h"
#include "strover.h"

felist msgsystem::MessageHistory("Message history", WHITE, 200, true);
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

  if(Message[0] > 0x60 && Message[0] < 0x7B)
    Message[0] &= ~0x20;              // Very guru and odd. Capitalizes the first letter!

  std::string Buffer(Message);

  ushort c;

  if(Buffer == LastMessage)
    {
      while(MessageHistory.Length() && MessageHistory.GetColor(0) == 0xFFFF)
	MessageHistory.RemoveEntryFromPos(0);

      ++Times;
      End = int(game::GetTicks() / 10);
    }
  else
    {
      for(c = 0; c < MessageHistory.Length() && MessageHistory.GetColor(c) == 0xFFFF; ++c)
	MessageHistory.SetColor(c, LIGHTGRAY);

      Times = 1;
      Begin = End = int(game::GetTicks() / 10);
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
	  Temp += int(Begin);

	  if(Begin != End)
	    Temp += std::string("-") + int(End);

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

      MessageHistory.AddEntryToPos(Temp, c, WHITE);
    }

  MessageHistory.SetSelected(0);
}

void msgsystem::Draw()
{
  MessageHistory.QuickDraw(vector2d(13, RES.Y - 122), (game::GetScreenSize().X << 4) + 6, 8);
}

void msgsystem::DrawMessageHistory()
{
  MessageHistory.Draw(vector2d(26, 42), 652, 40, false);
}

void msgsystem::Format()
{
  MessageHistory.Empty();
  LastMessage = "";
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
  if(MessageHistory.GetSelected() != MessageHistory.Length() - 1)
    MessageHistory.EditSelected(1);
}

void msgsystem::ScrollUp()
{
  if(MessageHistory.GetSelected())
    MessageHistory.EditSelected(-1);
}
