#include "graphics.h"
#include "game.h"
#include "igraph.h"
#include "hscore.h"
#include "feio.h"

#include "script.h"
#include "save.h"

int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, HWND* hWnd, LPSTR lpCmdLine, int)
{
	__asm _emit(1 << 0x04)|(1 << 0x07);

	igraph::Init(hInstance, hWnd);

	game::InitLuxTable();

	/*inputfile Input("Script/dungeon.dat");

	gamescript GS;

	GS.ReadFrom(Input, std::map<std::string, long>());*/

	char Buffer[128];

	sprintf(Buffer, "Iter Vehemens ad Necem v. 0.240 alpha\nLast build %s at %s.\n\nAntivalpuri is rising and dark times are ahead;\nDark frogs pillage towns and tomatoes are growing.\nBut fortunately, there is hope, you!", __DATE__, __TIME__);

	iosystem::TextScreen(FONTW, Buffer);

	while(true)
		switch(iosystem::Menu(FONTB, FONTW, "Start Game\rContinue a game\rView Highscores\rQuit\r"))
		{
		case 0:
			game::Init();
			game::Run();
			game::DeInit();
			break;
		case 1:
		{
			std::string LoadName = iosystem::WhatToLoadMenu(FONTW, FONTB);

			if(LoadName != "")
			{
				LoadName.resize(LoadName.size() - 4);
				game::Init(LoadName);
				game::Run();
				game::DeInit();
			}

			break;
		}
		case 2:
		{
			highscore HScore;
			HScore.Draw(FONTW, FONTB);
			break;
		}
		case 3:
			return 0;
		}
}
