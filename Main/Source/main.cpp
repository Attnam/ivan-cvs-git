#include "graphics.h"
#include "game.h"
#include "igraph.h"
#include "hscore.h"

int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, HWND* hWnd, LPSTR lpCmdLine, int)
{
	__asm _emit(1 << 0x04)|(1 << 0x07);

	igraph::Init(hInstance, hWnd);

	game::InitLuxTable();

	game::StoryScreen("Iter Vehemens ad Necem v. 0.240 alpha\n\nAntivalpuri is rising and dark times are ahead;\nDark frogs pillage towns and tomatoes are growing.\nBut fortunately, there is hope, you!");

	while(true)
		switch(game::Menu("Start Game\rContinue a game\rView Highscores\rQuit\r"))
		{
		case 0:
			game::Init();
			game::Run();
			game::DeInit();
			break;
		case 1:
			game::WhatToLoadMenu();
			break;
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
