#include "graphics.h"
#include "game.h"
#include "igraph.h"
#include "hscore.h"
#include "feio.h"
#include "whandler.h"

int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, HWND* hWnd, LPSTR lpCmdLine, int)
{
	__asm _emit(1 << 0x04)|(1 << 0x07);

	igraph::Init(hInstance, hWnd);

	game::InitLuxTable();
	game::InitScript();

	char Buffer[256];

	sprintf(Buffer, "Iter Vehemens ad Necem v. 0.30 alpha\n\n"
			"Last build %s at %s.\n\nAntivalpuri is rising and dark times are ahead;\n"
			"Dark frogs pillage towns and tomatoes are growing.\n"
			"But fortunately, there is hope, you!", __DATE__, __TIME__);

	iosystem::TextScreen(Buffer);

	while(true)
		switch(iosystem::Menu("Start Game\rContinue a game\rView Highscores\rQuit\r", BLUE, WHITE))
		{
		case 0:
			game::Init();
			game::Run();
			game::DeInit();
			break;
		case 1:
		{
			std::string LoadName = iosystem::WhatToLoadMenu(WHITE, BLUE);

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
			HScore.Draw();
			break;
		}
		case 3:
			return 0;
		}
}
