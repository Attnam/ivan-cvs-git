#include "whandler.h"
#include "graphics.h"
#include "error.h"
#include "bitmap.h"
#include "resource.h"

dynarray<int> globalwindowhandler::KeyBuffer;
char globalwindowhandler::KeyboardLayoutName[KL_NAMELENGTH];
bool globalwindowhandler::Initialized = false;
bool (*globalwindowhandler::QuitMessageHandler)() = 0;

LRESULT CALLBACK globalwindowhandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
			if(wParam == SIZE_MAXIMIZED)
			{
				graphics::SwitchMode();
				break;
			}

		case WM_MOVE:
		{
			graphics::UpdateBounds();

			if(Initialized)
				graphics::BlitDBToScreen();

			break;
		}

		case WM_PAINT:
		{
			if(Initialized)
				graphics::BlitDBToScreen();

			break;
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
			}

			break;
		}

		case WM_CLOSE:
		{
			if(!QuitMessageHandler || QuitMessageHandler())
				PostQuitMessage(0);

			return 0;
		}

		case WM_KEYDOWN:
		{
			if(wParam == VK_F2)
			{
				DOUBLEBUFFER->Save("Scrshot.bmp");
				return 0;
			}

			if(wParam == VK_F4)
			{
				graphics::SwitchMode();
				return 0;
			}

			KeyBuffer.Add(wParam);

			return 0;
		}

		case WM_KEYUP:
		{
			ushort Index = KeyBuffer.Search(wParam);

			if(Index != 0xFFFF)
				KeyBuffer.Remove(Index);

			return 0;
		}
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int globalwindowhandler::GetKey(bool EmptyBuffer, bool AcceptCommandKeys)
{
	MSG msg;

	if(EmptyBuffer)
	{
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			if(msg.message == WM_QUIT)
				exit(0);
			else
			{
				TranslateMessage(&msg);

				if(msg.message != WM_SYSKEYUP)
					DispatchMessage(&msg);
			}

		while(KeyBuffer.Length())
			KeyBuffer.Remove(0);
	}

	while(true)
		if(KeyBuffer.Length())
		{
			int Key =  KeyBuffer.Remove(0);
			int BackUp = Key;

			HKL MappedVirtualKey = LoadKeyboardLayout(KeyboardLayoutName, KLF_SUBSTITUTE_OK | KLF_REPLACELANG | KLF_ACTIVATE );

			ulong ScanCode = MapVirtualKeyEx(Key, 0, MappedVirtualKey);
			ushort ToBeReturned;	
			uchar KeyboardBuffer[256];

			if(Key == VK_LEFT || Key == VK_NUMPAD4) return 0x14B;
			if(Key == VK_HOME || Key == VK_NUMPAD7) return 0x147;
			if(Key == VK_UP || Key == VK_NUMPAD8) return 0x148;
			if(Key == VK_PRIOR || Key == VK_NUMPAD9) return 0x149;	// page up! Believe it, or not!
			if(Key == VK_RIGHT || Key == VK_NUMPAD6) return 0x14D;
			if(Key == VK_NEXT || Key == VK_NUMPAD3) return 0x151;	// page down! Believe it, or not!
			if(Key == VK_DOWN || Key == VK_NUMPAD2) return 0x150;
			if(Key == VK_END || Key == VK_NUMPAD1) return 0x14F;
			if(Key == VK_NUMPAD5) return '.';

			if(!GetKeyboardState(KeyboardBuffer))
				return 'x';

			ToAsciiEx(Key, ScanCode, KeyboardBuffer, &ToBeReturned, 0, MappedVirtualKey);

			if(ToBeReturned != 0 && ToBeReturned != 0xFFFF)
				return ToBeReturned;
			else
				if(AcceptCommandKeys)
					return 0;
		}
		else
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
				if(msg.message == WM_QUIT)
					exit(0);
				else
				{
					TranslateMessage(&msg);

					if(msg.message != WM_SYSKEYUP)
						DispatchMessage(&msg);
				}
}

void globalwindowhandler::Init(HINSTANCE hInst, HWND* phWnd, const char* Title)
{
	WNDCLASS wc;
	HWND hWnd;

	wc.lpszClassName = Title;
	wc.lpfnWndProc   = (WNDPROC) globalwindowhandler::WndProc;
	wc.style         = CS_OWNDC;
	wc.hInstance     = hInst;
	wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_LOGO));
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName  = NULL;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;

	if(!RegisterClass( &wc ))
		ABORT("No Window register.");

	hWnd = CreateWindowEx(NULL, Title, Title, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL );

	if(!hWnd)
		ABORT("No Windows.");

	ShowWindow(hWnd, SW_SHOW);
	SetFocus(hWnd);
	UpdateWindow(hWnd);

	*phWnd = hWnd;

	GetKeyboardLayoutName(KeyboardLayoutName);

	Initialized = true;
}

int globalwindowhandler::ReadKey()
{
	MSG msg;

	while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		if(msg.message == WM_QUIT)
			exit(0);
		else
		{
			TranslateMessage(&msg);

			if(msg.message != WM_SYSKEYUP)
				DispatchMessage(&msg);
		}

	if(KeyBuffer.Length())
		return GetKey(false, true);
	else
		return 0;
}
