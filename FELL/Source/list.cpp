#include "list.h"

#include "graphics.h"
#include "bitmap.h"
#include "igraph.h"
#include "whandler.h"

ushort list::Draw(bool WillDrawNumbers) const
{
	DrawDescription();

	bitmap* BackGround = new bitmap(800, 600);

	BackGround->ReadFromDB(0,0);

	for(ushort Min = 0, c = 0;; c++)
	{
		graphics::ClearDBToColor(20, 56 + (c - Min + Description.Length()) * 10, 758, 20, 128);

		if(WillDrawNumbers)
			FONTR->PrintfToDB(30, 56 + (c - Min + Description.Length()) * 10, "%c: %s", 'A' + c - Min, String.Access(c).c_str());
		else
			FONTR->PrintfToDB(30, 56 + (c - Min + Description.Length()) * 10, "%s", String.Access(c).c_str());

		if(c - Min == 19 && c != String.Length() - 1)
		{
			graphics::ClearDBToColor(20, 266 + Description.Length() * 10, 758, 20, 128);
			FONTW->PrintfToDB(30, 266 + Description.Length() * 10, "- Press SPACE to continue, ESC to exit -");
		}

		if(c - Min == 19 || c == String.Length() - 1)
		{
			graphics::BlitDBToScreen();

			int Pressed = GETKEY();

			if(Pressed > 64 && Pressed < 91)
			{
				delete BackGround;
				return Pressed - 65 + Min < String.Length() ? Pressed - 65 + Min : 0xFFFF;
			}

			if(Pressed > 96 && Pressed < 123)
			{
				delete BackGround;
				return Pressed - 97 + Min < String.Length() ? Pressed - 97 + Min : 0xFFFF;
			}

			if(Pressed == '-')
			{
				delete BackGround;
				return 0xFFFE;
			}

			if(Pressed == 0x1B)
			{
				delete BackGround;
				return 0xFFFD;
			}

			if(c == String.Length() - 1)
			{
				delete BackGround;
				return 0xFFFF;
			}
			else
			{
				BackGround->WriteToDB(0,0);
				DrawDescription();
				Min += 20;
			}
		}
	}
}

void list::DrawDescription(void) const
{
	graphics::ClearDBToColor(20, 36, 758, 20, 128);

	for(ushort c = 0; c < Description.Length(); c++)
	{
		graphics::ClearDBToColor(20, 46 + c * 10, 758, 10, 128);
		FONTR->PrintfToDB(30, 46 + c * 10, Description.Access(c).c_str());
	}

	graphics::ClearDBToColor(20, 46 + Description.Length() * 10, 758, 10, 128);
}

void list::Empty(void)
{
	while(String.Length())
		String.Remove(0);
}
