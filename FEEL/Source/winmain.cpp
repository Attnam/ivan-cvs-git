#include <windows.h>
#include <cstdlib>
#include <iostream>

#include "error.h"

int Main(HINSTANCE, HINSTANCE, HWND*, LPSTR, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

	globalerrorhandler::SetWindow(&hWnd);

	globalerrorhandler::Install();

	try
	{
		return Main(hInstance, hPrevInstance, &hWnd, lpCmdLine, nCmdShow);
	}
	catch(...)
	{
		ShowWindow(hWnd, SW_HIDE);

		MessageBox(NULL, "Fatal Error: Unknown exception thrown.", "Program aborted!", MB_OK|MB_ICONEXCLAMATION);

		exit(3);
	}

	exit(0);
}
