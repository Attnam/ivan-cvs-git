#include <iostream>
#include <stdlib.h>
#include "error.h"

#ifdef __DJGPP__
#include <conio.h>
#endif

#ifdef WIN32
int Main(HINSTANCE, HINSTANCE, HWND*, LPSTR, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

	globalerrorhandler::SetWindow(&hWnd);
#else

int Main();

int main()
{
#endif
	globalerrorhandler::Install();

	try
	{
#ifdef WIN32
		return Main(hInstance, hPrevInstance, &hWnd, lpCmdLine, nCmdShow);
#else
		return Main();
#endif
	}
	catch(...)
	{
#ifdef WIN32
	  ShowWindow(hWnd, SW_HIDE);

	  MessageBox(NULL, "Fatal Error: Unknown exception thrown.", "Program aborted!", MB_OK|MB_ICONEXCLAMATION);
#endif
#ifdef SDL
	std::cout << "Fatal Error: Unknown exception thrown." << std::endl;
#endif
#ifdef __DJGPP__
	textmode(0x3);
	std::cout << "Fatal Error: Unknown exception thrown." << std::endl;
#endif
		exit(3);

	}

	exit(0);
}

