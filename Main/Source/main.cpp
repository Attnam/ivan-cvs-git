#include <iostream>
#include <ctime>

#ifdef __DJGPP__
#include <go32.h>
#include <sys/farptr.h>
#endif

#include "game.h"
#include "database.h"
#include "feio.h"
#include "igraph.h"
#include "config.h"
#include "whandler.h"
#include "hscore.h"
#include "graphics.h"
#include "script.h"
#include "message.h"

int Main(int argc, char **argv)
{
  if(argc > 1 && festring(argv[1]) == "--version")
  {
    std::cout << "Iter Vehemens ad Necem version " << IVAN_VERSION << std::endl;
    return 0;
  }

#ifdef __DJGPP__

  /* Saves numlock state and toggles it off */

  char ShiftByteState = _farpeekb(_dos_ds, 0x417);
  _farpokeb(_dos_ds, 0x417, 0);

#endif /* __DJGPP__ */

  femath::SetSeed(time(0));
  game::InitGlobalValueMap();
  scriptsystem::Initialize();
  databasesystem::CreateDataBaseMemberMaps();
  databasesystem::Initialize();
  game::InitLuxTable();
  configuration::Load();
  igraph::Init();
  game::CreateBusyAnimationCache();
  globalwindowhandler::SetQuitMessageHandler(game::HandleQuitMessage);
  msgsystem::Init();

  while(true)
    switch(iosystem::Menu(igraph::GetMenuGraphic(), vector2d(RES_X / 2 - 130, RES_Y / 2 + 20), CONST_S("\r"), CONST_S("Start Game\rContinue Game\rConfiguration\rHighscores\rQuit\r"), LIGHT_GRAY, CONST_S("Released under the GNU\rGeneral Public License\rMore info: see COPYING\r"), CONST_S("IVAN v" IVAN_VERSION "\r")))
      {
      case 0:
	if(game::Init())
	  {
	    game::Run();
	    game::DeInit();
	  }
	break;
      case 1:
	{
	  festring LoadName = iosystem::ContinueMenu(WHITE, LIGHT_GRAY, game::GetSaveDir());

	  if(LoadName.GetSize())
	    {
	      LoadName.Resize(LoadName.GetSize() - 4);

	      if(game::Init(LoadName))
		{
		  game::Run();
		  game::DeInit();
		}
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

#ifdef __DJGPP__

	/* Loads numlock state */

	_farpokeb(_dos_ds, 0x417, ShiftByteState);

#endif

	return 0;
      }
}
