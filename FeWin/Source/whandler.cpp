#include "whandler.h"
#include "winuser.h"
#include "graphics.h"
#include "igraph.h"
#include "error.h"

dynarray<int> globalwindowhandler::KeyBuffer;
bool globalwindowhandler::Active = true;
bool globalwindowhandler::KeyPressed = false;
char globalwindowhandler::KeyboardLayoutName[KL_NAMELENGTH];
bool globalwindowhandler::InGetKey = false;

LRESULT CALLBACK globalwindowhandler::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)									// Check For Windows Messages
	{	
		case WM_SIZE:
			if(wParam == SIZE_MAXIMIZED)
			{
				graphics::SwitchMode();
				break;
			}
		case WM_MOVE:
			graphics::UpdateBounds();
			if(InGetKey)
				graphics::BlitDBToScreen();
			break;

		case WM_PAINT:
			if(InGetKey)
				graphics::BlitDBToScreen();

			break;

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			//PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			if(wParam == VK_F4)
				graphics::SwitchMode();
			else
			{
				KeyPressed = true;
				KeyBuffer.Add(wParam);
			}

			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			ushort Index = KeyBuffer.Search(wParam);

			if(Index != 0xFFFF)
				KeyBuffer.Remove(Index);

			if(!KeyBuffer.Length())
				KeyPressed = false;

			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int globalwindowhandler::GetKey(void)
{	
	bool Shift = false;
	while(KeyBuffer.Length())
		KeyBuffer.Remove(0); // Shift-key == 0x10

	KeyPressed = false;

	MSG		msg;

	InGetKey = true;

	while(true)										// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				exit(0);
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			if(KeyBuffer.Length())
			{
				int Key =  KeyBuffer.Remove(0);
				int BackUp = Key;

				unsigned int ScanCode = MapVirtualKeyEx(Key, 0, CKeyboardLayoutName());
				unsigned short ToBeReturned;	
				unsigned char KeyboardBuffer[256];

				if(Key == VK_LEFT) return 0x14B;
				if(Key == VK_HOME) return 0x147;
				if(Key == VK_UP) return 0x148;
				if(Key == VK_PRIOR) return 0x149;	// page up! Belive it, or not!
				if(Key == VK_RIGHT) return 0x14D;
				if(Key == VK_NEXT) return 0x151;	// page down! Believe it, or not!
				if(Key == VK_DOWN) return 0x150;
				if(Key == VK_END) return 0x14F;

				if(!GetKeyboardState(KeyboardBuffer))
					return 'x';
				ToAsciiEx(Key, ScanCode, KeyboardBuffer, &ToBeReturned, 0, LoadKeyboardLayout(CKeyboardLayoutName(), KLF_SUBSTITUTE_OK | KLF_REPLACELANG | KLF_ACTIVATE ));

				if(ToBeReturned != 0 && ToBeReturned != 0xFFFF)
					return ToBeReturned;
			}
		}
	}
}


void globalwindowhandler::Init(HINSTANCE hInst, HWND* phWnd, const char* Title)
{
	WNDCLASS wc;
	HWND hWnd;

	// Register the Window Class
	wc.lpszClassName = Title;
	wc.lpfnWndProc   = (WNDPROC) globalwindowhandler::WndProc;
	wc.style         = CS_NOCLOSE | CS_OWNDC;
	wc.hInstance     = hInst;
	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName  = NULL;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;

	if(!RegisterClass( &wc ))
		ABORT("No Window register.");

	// Create and show the main window
	hWnd = CreateWindowEx(NULL, Title, Title, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL );

	if(!hWnd)
		ABORT("No Windows.");

	ShowWindow(hWnd, SW_SHOW);
	SetFocus(hWnd);
	UpdateWindow(hWnd);

	*phWnd = hWnd;

	GetKeyboardLayoutName(CKeyboardLayoutName());
}
