#include <conio.h>
#include <windows.h>

#include "graphics.h"
#include "bitmap.h"
#include "game.h"
#include "igraph.h"
#include "hscore.h"
#include "whandler.h"

int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, HWND* hWnd, LPSTR lpCmdLine, int)
{
	__asm _emit(1 << 0x04)|(1 << 0x07);

	igraph::Init(hInstance, hWnd);

	game::InitLuxTable();

	clock();

	game::StoryScreen("Iter Vehemens ad Necem v. 0.240 alpha\n\nAntivalpuri is rising and dark times are ahead;\nDark frogs pillage towns and tomatoes are growing.\nBut fortunately, there is hope, you!");

	int Select;

	while((Select = game::Menu("Start Game\rContinue a game\rView Highscores\rQuit\r")) != 3)
	{
		if(!Select)
		{
			game::Init();
			game::Run();
			game::DeInit();
		}
		else if(Select == 1)
		{
			game::WhatToLoadMenu();
		}
		else
		{
			highscore HScore;
			HScore.Draw();
		}
	}

	return 0;
}
