#include "message.h"
#include "list.h"
#include "game.h"
#include "graphics.h"
#include "igraph.h"
#include "bitmap.h"
#include "whandler.h"

char* globalmessagingsystem::MessageBuffer = 0;
ushort globalmessagingsystem::BufferLength = 0;
list globalmessagingsystem::MessageHistory(200);

void globalmessagingsystem::AddMessage(const char* Format, ...)
{
	char Message[256];
	char Buffer[256];

	va_list AP;
	va_start(AP, Format);
	vsprintf(Message, Format, AP);
	va_end(AP);

	if(Message[0] > 0x60 && Message[0] < 0x7B)
		Message[0] &= ~0x20;              // Very guru and odd. Capitalizes the first letter!

	if(MessageBuffer)
	{
		ushort NewLength = BufferLength + strlen(Message) + 1;

		char* TempBuffer = new char[NewLength];

		{
		for(ushort c = 0; c < BufferLength; ++c)
			TempBuffer[c] = MessageBuffer[c];
		}

		TempBuffer[BufferLength] = ' ';

		for(ushort c = BufferLength + 1, i = 0; c < NewLength; ++c, ++i)
			TempBuffer[c] = Message[i];

		delete [] MessageBuffer;

		MessageBuffer = TempBuffer;

		BufferLength = NewLength;
	}
	else
	{
		BufferLength = strlen(Message);

		MessageBuffer = new char[BufferLength];

		for(ushort c = 0; c < BufferLength; ++c)
			MessageBuffer[c] = Message[c];
	}

	sprintf(Buffer, "%d - %s", int(game::GetTurns()), Message);

	MessageHistory.AddString(Buffer);
}

void globalmessagingsystem::Draw()
{
	DOUBLEBUFFER->ClearToColor(0, 0, 800, 32);

	ulong Length = BufferLength, Pointer = 0;

	char Buffer[99];

	Buffer[98] = 0;

	if(MessageBuffer)
	while(Length)
	{
		for(ushort y = 0; y < 2; y++)
		{
			if(Length <= 98)
			{
				for(ulong c = 0; c < Length; ++c)
					Buffer[c] = MessageBuffer[c + Pointer];

				Buffer[Length] = 0;

				FONTW->Printf(DOUBLEBUFFER, 7, 7 + y * 10, "%s", Buffer);

				Length = 0;

				break;
			}
			else
			{
				ulong i = 97;

				for(; i; i--)
					if(MessageBuffer[Pointer + i] == ' ')
					{
						for(ulong c = 0; c < i; ++c)
							Buffer[c] = MessageBuffer[c + Pointer];

						Buffer[i] = 0;

						Pointer += i + 1;

						Length -= i + 1;

						break;
					}

				if(!i)
				{
					for(ulong c = 0; c < 98; ++c)
						Buffer[c] = MessageBuffer[c + Pointer];

					Pointer += 98;

					Length -= 98;
				}

				FONTW->Printf(DOUBLEBUFFER, 7, 7 + y * 10, "%s", Buffer);
			}
		}
		if(Length)
		{
			graphics::BlitDBToScreen();

			GETKEY();

			DOUBLEBUFFER->ClearToColor(0, 0, 800, 32);
		}
	}
}

void globalmessagingsystem::Empty()
{
	delete [] MessageBuffer;

	MessageBuffer = 0;

	BufferLength = 0;
}

void globalmessagingsystem::DrawMessageHistory()
{
	MessageHistory.Draw(FONTW, FONTB, false);
}

void globalmessagingsystem::Format()
{
	MessageHistory.Empty();
}
