#include <iostream>
#include <stdlib.h>

#ifdef __DJGPP__
#include <conio.h>
#include "graphics.h"
#endif

#include "error.h"

#ifdef WIN32
int Main(HINSTANCE, HINSTANCE, HWND*, LPSTR, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  HWND hWnd;
  globalerrorhandler::SetWindow(&hWnd);
#else

int Main(int, char**);

int main(int argc, char** argv)
{
#endif
  globalerrorhandler::Install();

  try
    {
#ifdef WIN32
      return Main(hInstance, hPrevInstance, &hWnd, lpCmdLine, nCmdShow);
#else
      return Main(argc, argv);
#endif
    }
  catch(...)
    {
#ifdef WIN32
      ShowWindow(hWnd, SW_HIDE);
      MessageBox(NULL, "Fatal Error: Unknown exception thrown.", "Program aborted!", MB_OK|MB_ICONEXCLAMATION);
#endif
#ifdef USE_SDL
      std::cout << "Fatal Error: Unknown exception thrown." << std::endl;
#endif
#ifdef __DJGPP__
      graphics::DeInit();
      std::cout << "Fatal Error: Unknown exception thrown." << std::endl;
#endif
      exit(3);

    }

  exit(0);
}
