#include <cstdio>

#include "graphics.h"
#include "game.h"
#include "igraph.h"
#include "hscore.h"
#include "feio.h"
#include "whandler.h"
#include "charde.h"
#include "config.h"
#include "bitmap.h"

#ifdef WIN32
int Main(HINSTANCE hInstance, HINSTANCE, HWND* hWnd, LPSTR, int)
#else
int Main()
#endif
{
	/* You are not expected to understand this */

#ifdef VC

	__asm _emit(1 << 0x04)|(1 << 0x07);

#endif

	game::InitLuxTable();
	game::InitScript();
	configuration::Load();
#ifdef WIN32
	igraph::Init(hInstance, hWnd);
#else
	igraph::Init();
#endif
	globalwindowhandler::SetQuitMessageHandler(game::HandleQuitMessage);

	elpuri Elpuri(true, false, false, false);

	while(true)
		switch(iosystem::Menu(Elpuri.GetPicture(), "Iter Vehemens ad Necem\rMain Menu\r", "Start Game\rContinue Game\rConfiguration\rHighscores\rQuit\r", BLUE, WHITE, true))
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
			configuration::ShowConfigScreen();
			break;
		case 3:
		{
			highscore HScore;
			HScore.Draw();
			break;
		}
		case 4:
			configuration::Save();
			return 0;
		}
}
