#include <windowsx.h>

#include "graphics.h"
#include "bitmap.h"
#include "ddutil.h"
#include "whandler.h"
#include "error.h"
#include "colorbit.h"

HWND			graphics::hWnd;
bool			graphics::FullScreen;
CDisplay*		graphics::DXDisplay;
bitmap*			graphics::DoubleBuffer;
ushort			graphics::XRes;
ushort			graphics::YRes;
uchar			graphics::ColorDepth;
colorizablebitmap*	graphics::DefaultFont = 0;
void			(*graphics::SwitchModeHandler)();

extern DWORD GetDXVersion();

void graphics::Init()
{
	static bool AlreadyInstalled = false;

	if(!AlreadyInstalled)
	{
		AlreadyInstalled = true;

		atexit(graphics::DeInit);
	}
}

void graphics::DeInit()
{
	delete DefaultFont;
}

void graphics::SetMode(HINSTANCE hInst, HWND* phWnd, const char* Title, ushort NewXRes, ushort NewYRes, uchar NewColorDepth, bool FScreen)
{
	FullScreen = FScreen;

	globalwindowhandler::Init(hInst, phWnd, Title);

	hWnd = *phWnd;

	if(FullScreen)
		ShowCursor(false);

	DXDisplay = new CDisplay();

	if(FullScreen)
	{
		if(FAILED(DXDisplay->CreateFullScreenDisplay(hWnd, NewXRes, NewYRes, NewColorDepth)))
			ABORT("This system does not support %dx%dx%d in fullscreen mode!", NewXRes, NewYRes, NewColorDepth);
	}
	else
		if(FAILED(DXDisplay->CreateWindowedDisplay(hWnd, NewXRes, NewYRes)))
			ABORT("This system does not support %dx%dx%d in window mode!", NewXRes, NewYRes, NewColorDepth);

	DoubleBuffer = new bitmap(NewXRes, NewYRes);

	XRes = NewXRes;
	YRes = NewYRes;
	ColorDepth = NewColorDepth;

	if(!FullScreen)
	{
		DDPIXELFORMAT DDPixelFormat;
		ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
		DDPixelFormat.dwSize = sizeof(DDPixelFormat);

		DXDisplay->GetBackBuffer()->GetPixelFormat(&DDPixelFormat);

		if(DDPixelFormat.dwRGBBitCount != ColorDepth)
			SwitchMode();
	}

	globalwindowhandler::SetInitialized(true);
}

void graphics::BlitDBToScreen()
{
	if(DXDisplay->GetDirectDraw()->RestoreAllSurfaces() == DD_OK)
	{
		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd,sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);

		if(FullScreen)
			DXDisplay->GetFrontBuffer()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
		else
			DXDisplay->GetBackBuffer()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

		ulong TrueSourceOffset = ulong(DoubleBuffer->Data[0]);
		ulong TrueSourceXMove = 0;
		ulong TrueDestOffset = ulong(ddsd.lpSurface);
		ulong TrueDestXMove = ddsd.lPitch - (XRES << 1);
		ushort Width = XRES;
		ushort Height = YRES;

		__asm
		{
			pushad
			push es
			mov ax, ds
			mov esi, TrueSourceOffset
			mov edi, TrueDestOffset
			mov es, ax
			xor ecx, ecx
			mov dx, Width
			mov bx, Height
			shr dx, 0x01
			cld
		MaskedLoop3:
			mov cx, dx
			rep movsd
			add esi, TrueSourceXMove
			add edi, TrueDestXMove
			dec bx
			jnz MaskedLoop3
			pop es
			popad
		}

		if(FullScreen)
			DXDisplay->GetFrontBuffer()->Unlock(NULL);
		else
		{
			DXDisplay->GetBackBuffer()->Unlock(NULL);
			DXDisplay->Present();
		}
	}
}

HRESULT CDisplay::CreateFullScreenDisplay( HWND hWnd, DWORD dwWidth,
                                           DWORD dwHeight, DWORD dwBPP )
{
	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) )
		return E_FAIL;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );

	if( FAILED(hr) )
		return E_FAIL;

	// Set the display mode
	if( FAILED( m_pDD->SetDisplayMode( dwWidth, dwHeight, dwBPP, 0, 0 ) ) )
		return E_FAIL;

	// Create primary surface (with backbuffer attached)
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize            = sizeof( ddsd );
	ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
			     DDSCAPS_COMPLEX | DDSCAPS_3DDEVICE;
	ddsd.dwBackBufferCount = 1;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
		return E_FAIL;

	// Get a pointer to the back buffer
	DDSCAPS2 ddscaps;
	ZeroMemory( &ddscaps, sizeof( ddscaps ) );
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

	if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps, &m_pddsBackBuffer ) ) )
		return E_FAIL;

	m_pddsBackBuffer->AddRef();

	m_hWnd      = hWnd;
	m_bWindowed = FALSE;
	UpdateBounds();

	DWORD dwStyle = GetWindowLong( hWnd, GWL_STYLE );
	dwStyle |= WS_POPUP;
	dwStyle &= ~(WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
	SetWindowLong( hWnd, GWL_STYLE, dwStyle );

	return S_OK;
}

HRESULT CDisplay::CreateWindowedDisplay( HWND hWnd, DWORD dwWidth, DWORD dwHeight )
{
	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) )
		return E_FAIL;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );

	if( FAILED(hr) )
		return E_FAIL;

	RECT  rcWork;
	RECT  rc;
	DWORD dwStyle;

	// If we are still a WS_POPUP window we should convert to a normal app
	// window so we look like a windows app.
	dwStyle  = GetWindowStyle( hWnd );
	dwStyle &= ~WS_POPUP;
	dwStyle |= WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	SetWindowLong( hWnd, GWL_STYLE, dwStyle );

	// Set window size
	SetRect( &rc, 0, 0, 800, 600 );

	AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd) );

	SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

	SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	// Make sure our window does not hang outside of the work area
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
	GetWindowRect( hWnd, &rc );
	if( rc.left < rcWork.left ) rc.left = rcWork.left;
	if( rc.top  < rcWork.top )  rc.top  = rcWork.top;
	SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0,
		  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

	LPDIRECTDRAWCLIPPER pcClipper;

	// Create the primary surface
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	ddsd.dwSize         = sizeof( ddsd );
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
		return E_FAIL;

	// Create the backbuffer surface
	ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;    
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_3DDEVICE;
	ddsd.dwWidth        = dwWidth;
	ddsd.dwHeight       = dwHeight;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) )
		return E_FAIL;

	if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) )
		return E_FAIL;

	if( FAILED( hr = pcClipper->SetHWnd( 0, hWnd ) ) )
	{
		pcClipper->Release();
		return E_FAIL;
	}

	if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pcClipper ) ) )
	{
		pcClipper->Release();
		return E_FAIL;
	}

	// Done with clipper
	pcClipper->Release();

	m_hWnd      = hWnd;
	m_bWindowed = TRUE;
	UpdateBounds();

	return S_OK;
}

void graphics::UpdateBounds()
{
	if(DXDisplay)
		DXDisplay->UpdateBounds();
}

void graphics::SwitchMode()
{
	globalwindowhandler::SetInitialized(false);
	BlitDBToScreen();
	FullScreen = !FullScreen;

	if(SwitchModeHandler)
		SwitchModeHandler();

	delete DXDisplay;
	DXDisplay = new CDisplay();

	if(FullScreen)
	{
		if(FAILED(DXDisplay->CreateFullScreenDisplay(hWnd, XRes, YRes, ColorDepth)))
			ABORT("This system does not support %dx%dx%d in fullscreen mode!", XRes, YRes, ColorDepth);

		BlitDBToScreen();

		ShowCursor(false);
	}
	else
	{
		if(FAILED(DXDisplay->CreateWindowedDisplay(hWnd, XRes, YRes)))
			ABORT("This system does not support %dx%dx%d in window mode!", XRes, YRes, ColorDepth);

		BlitDBToScreen();

		ShowCursor(true);

		DDPIXELFORMAT DDPixelFormat;
		ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
		DDPixelFormat.dwSize = sizeof(DDPixelFormat);

		DXDisplay->GetBackBuffer()->GetPixelFormat(&DDPixelFormat);

		if(DDPixelFormat.dwRGBBitCount != ColorDepth)
		{
			MessageBox(NULL, "This alpha release of IVAN supports only 16-bit color format. Change your monitor settings to it before trying to run the game in windowed mode.", "Incorrect color depth detected!", MB_OK|MB_ICONEXCLAMATION);

			FullScreen = !FullScreen;

			if(SwitchModeHandler)
				SwitchModeHandler();

			delete DXDisplay;

			DXDisplay = new CDisplay();

			if(FAILED(DXDisplay->CreateFullScreenDisplay(hWnd, XRes, YRes, ColorDepth)))
				ABORT("This system does not support %dx%dx%d in fullscreen mode!", XRes, YRes, ColorDepth);

			ShowCursor(false);
		}
	}

	globalwindowhandler::SetInitialized(true);
}

void graphics::LoadDefaultFont(std::string FileName)
{
	DefaultFont = new colorizablebitmap(FileName);
}
