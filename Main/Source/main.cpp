#include "graphics.h"
#include "game.h"
#include "igraph.h"
#include "hscore.h"
#include "feio.h"
#include "whandler.h"
#include "charde.h"
#include "rand.h"
#include "config.h"

int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, HWND* hWnd, LPSTR lpCmdLine, int)
{
	/* You are not expected to understand this */

	__asm _emit(1 << 0x04)|(1 << 0x07);

	configuration::Load();

	igraph::Init(hInstance, hWnd);

	globalwindowhandler::SetQuitMessageHandler(game::HandleQuitMessage);

	game::InitLuxTable();
	game::InitScript();

	char Buffer[256];

	sprintf(Buffer, "Iter Vehemens ad Necem v. 0.301 alpha\n\n"
			"Last build %s at %s.\n\n"
			"Antivalpuri is rising and dark times are ahead;\n"
			"Dark frogs pillage towns and tomatoes are growing.\n"
			"But fortunately, there is hope, you!", __DATE__, __TIME__);

	iosystem::TextScreen(Buffer);

	elpuri Elpuri(true, false, false, false);

	while(true)
		switch(iosystem::Menu(Elpuri.GetPicture(), "Start Game\rContinue Game\rConfiguration\rHighscores\rQuit\r", BLUE, WHITE))
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
