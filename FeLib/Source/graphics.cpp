#ifdef WIN32
#include <windowsx.h>
#include "ddutil.h"
#endif

#ifdef USE_SDL
#include "SDL.h"
#endif

#ifdef __DJGPP__
#include <cstdlib>
#include <dpmi.h>
#include <go32.h>
#include <pc.h>
#include <sys/farptr.h>
#include <conio.h>
#endif

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "error.h"
#include "colorbit.h"
#include "blit.h"

#ifdef WIN32
HWND graphics::hWnd;
bool graphics::FullScreen;
CDisplay* graphics::DXDisplay;
#endif
#if defined (WIN32) || (USE_SDL)
void (*graphics::SwitchModeHandler)();
#endif
#ifdef USE_SDL
SDL_Surface* graphics::screen;
#endif

#ifdef __DJGPP__
ulong graphics::BufferSize;
ushort graphics::ScreenSelector = 0;
#endif

bitmap* graphics::DoubleBuffer;
vector2d graphics::Res;
uchar graphics::ColorDepth;
colorizablebitmap* graphics::DefaultFont = 0;

#ifdef WIN32
extern DWORD GetDXVersion();
#endif

#ifdef __DJGPP__
graphics::vesainfo graphics::VesaInfo;
graphics::modeinfo graphics::ModeInfo;
#endif

void graphics::Init()
{
  static bool AlreadyInstalled = false;

  if(!AlreadyInstalled)
    {
      AlreadyInstalled = true;

#ifdef USE_SDL
      if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE))
	ABORT("Can't initialize SDL.");
#endif

#ifdef __DJGPP__
      VesaInfo.Retrieve();
#endif

      atexit(graphics::DeInit);
    }
}

void graphics::DeInit()
{
  delete DefaultFont;
  DefaultFont = 0;

#ifdef USE_SDL
  SDL_Quit();
#endif

#ifdef __DJGPP__
  if(ScreenSelector)
    {
      __dpmi_free_ldt_descriptor(ScreenSelector);
      ScreenSelector = 0;
      textmode(0x3);
    }
#endif
}

#ifdef WIN32

void graphics::SetMode(HINSTANCE hInst, HWND* phWnd, const char* Title, vector2d NewRes, uchar NewColorDepth, bool FScreen, LPCTSTR IconName)
{
  FullScreen = FScreen;
  globalwindowhandler::Init(hInst, phWnd, Title, IconName);
  hWnd = *phWnd;

  if(FullScreen)
    ShowCursor(false);

  DXDisplay = new CDisplay();

  if(FullScreen)
    {
      if(FAILED(DXDisplay->CreateFullScreenDisplay(hWnd, NewRes.X, NewRes.Y, NewColorDepth)))
	ABORT("This system does not support %dx%dx%d in fullscreen mode!", NewRes.X, NewRes.Y, NewColorDepth);
    }
  else
    if(FAILED(DXDisplay->CreateWindowedDisplay(hWnd, NewRes.X, NewRes.Y)))
      ABORT("This system does not support %dx%dx%d in window mode!", NewRes.X, NewRes.Y, NewColorDepth);

  DoubleBuffer = new bitmap(NewRes);
  Res = NewRes;
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

#endif

#ifdef USE_SDL

void graphics::SetMode(const char* Title, vector2d NewRes, uchar NewColorDepth)
{
  screen = SDL_SetVideoMode(NewRes.X, NewRes.Y, NewColorDepth, SDL_SWSURFACE);

  if(screen == NULL) 
    ABORT("Couldn't set video mode.");

  globalwindowhandler::Init(Title);
  DoubleBuffer = new bitmap(NewRes);
  Res = NewRes;
  ColorDepth = NewColorDepth;
  globalwindowhandler::SetInitialized(true);
}

#endif

#ifdef WIN32

void graphics::BlitDBToScreen()
{
  if(DXDisplay->GetDirectDraw()->RestoreAllSurfaces() == DD_OK)
    {
      DDSURFACEDESC2 ddsd;
      ZeroMemory(&ddsd,sizeof(ddsd));
      ddsd.dwSize = sizeof(ddsd);

      HRESULT Result;

      if(FullScreen)
	Result = DXDisplay->GetFrontBuffer()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
      else
	Result = DXDisplay->GetBackBuffer()->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);

      if(Result != DD_OK)
	return;

      ulong TrueSourceOffset = ulong(DoubleBuffer->GetImage()[0]);
      ulong TrueDestOffset = ulong(ddsd.lpSurface);
      ulong TrueDestXMove = ddsd.lPitch - (RES.X << 1);

      BlitToDB(TrueSourceOffset, TrueDestOffset, TrueDestXMove, RES.X, RES.Y);

      if(FullScreen)
	DXDisplay->GetFrontBuffer()->Unlock(NULL);
      else
	{
	  DXDisplay->GetBackBuffer()->Unlock(NULL);
	  DXDisplay->Present();
	}
    }
}

#endif

#ifdef USE_SDL

void graphics::BlitDBToScreen()
{
  if(SDL_MUSTLOCK(screen) && SDL_LockSurface(screen) < 0)
    ABORT("Can't lock screen");

  ulong TrueSourceOffset = ulong(DoubleBuffer->GetImage()[0]);
  ulong TrueDestOffset = ulong(screen->pixels);
  ulong TrueDestXMove =  screen->pitch - (RES.X << 1);
  BlitToDB(TrueSourceOffset, TrueDestOffset, TrueDestXMove, RES.X, RES.Y);

  if(SDL_MUSTLOCK(screen))
    SDL_UnlockSurface(screen);

  SDL_UpdateRect(screen, 0,0, RES.X, RES.Y);
}

#endif

#ifdef WIN32

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
  SetRect( &rc, 0, 0, dwWidth, dwHeight );

  AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd) );

  SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

  SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
		SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

  // Make sure our window does not hang outside of the work area
  SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
  GetWindowRect( hWnd, &rc );
  if( rc.left < rcWork.left ) rc.left = rcWork.left;
  if( rc.top  < rcWork.top ) rc.top  = rcWork.top;
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

#endif

void graphics::UpdateBounds()
{
#ifdef WIN32
  if(DXDisplay)
    DXDisplay->UpdateBounds();
#endif
}


void graphics::SwitchMode()
{
#ifdef WIN32
  globalwindowhandler::SetInitialized(false);
  BlitDBToScreen();
  FullScreen = !FullScreen;

  if(SwitchModeHandler)
    SwitchModeHandler();

  delete DXDisplay;
  DXDisplay = new CDisplay();

  if(FullScreen)
    {
      if(FAILED(DXDisplay->CreateFullScreenDisplay(hWnd, Res.X, Res.Y, ColorDepth)))
	ABORT("This system does not support %dx%dx%d in fullscreen mode!", Res.X, Res.Y, ColorDepth);

      BlitDBToScreen();
      ShowCursor(false);
    }
  else
    {
      if(FAILED(DXDisplay->CreateWindowedDisplay(hWnd, Res.X, Res.Y)))
	ABORT("This system does not support %dx%dx%d in window mode!", Res.X, Res.Y, ColorDepth);

      BlitDBToScreen();
      ShowCursor(true);
      DDPIXELFORMAT DDPixelFormat;
      ZeroMemory(&DDPixelFormat, sizeof(DDPixelFormat));
      DDPixelFormat.dwSize = sizeof(DDPixelFormat);
      DXDisplay->GetBackBuffer()->GetPixelFormat(&DDPixelFormat);

      if(DDPixelFormat.dwRGBBitCount != ColorDepth)
	{
	  MessageBox(NULL, "This release of IVAN supports only 16-bit color format. Change your monitor settings to it before trying to run the game in windowed mode.", "Incorrect color depth detected!", MB_OK|MB_ICONEXCLAMATION);
	  FullScreen = !FullScreen;

	  if(SwitchModeHandler)
	    SwitchModeHandler();

	  delete DXDisplay;
	  DXDisplay = new CDisplay();

	  if(FAILED(DXDisplay->CreateFullScreenDisplay(hWnd, Res.X, Res.Y, ColorDepth)))
	    ABORT("This system does not support %dx%dx%d in fullscreen mode!", Res.X, Res.Y, ColorDepth);

	  ShowCursor(false);
	}
    }

  globalwindowhandler::SetInitialized(true);
#endif
#ifdef USE_SDL
  if(!SDL_WM_ToggleFullScreen(screen))
    ABORT("This system does not support %dx%dx%d in fullscreen mode!", Res.X, Res.Y, ColorDepth);
#endif
}

void graphics::LoadDefaultFont(const std::string& FileName)
{
  DefaultFont = new colorizablebitmap(FileName);
}

#ifdef __DJGPP__

void graphics::SetMode(ushort Mode)
{
  ModeInfo.Retrieve(Mode);

  if(!ModeInfo.CheckSupport())
    ABORT("Mode 0x%X not supported!", Mode);

  __dpmi_regs Regs;
  Regs.x.ax = 0x4F02;
  Regs.x.bx = Mode | 0x4000;
  __dpmi_int(0x10, &Regs);
  Res.X = ModeInfo.Width;
  Res.Y = ModeInfo.Height;
  BufferSize =	Res.Y * ModeInfo.BytesPerLine;
  delete DoubleBuffer;
  DoubleBuffer = new bitmap(Res);
  __dpmi_meminfo MemoryInfo;
  MemoryInfo.size = BufferSize;
  MemoryInfo.address = ModeInfo.PhysicalLFBAddress;
  __dpmi_physical_address_mapping(&MemoryInfo);
  __dpmi_lock_linear_region(&MemoryInfo);
  ScreenSelector = __dpmi_allocate_ldt_descriptors(1);
  __dpmi_set_segment_base_address(ScreenSelector, MemoryInfo.address);
  __dpmi_set_segment_limit(ScreenSelector, BufferSize - 1);
}

void graphics::BlitDBToScreen()
{
  ulong TrueSourceOffset = ulong(DoubleBuffer->GetImage()[0]);
  ulong TrueDestOffset = 0;
  ulong TrueDestXMove = 0;
  BlitToDB(TrueSourceOffset, TrueDestOffset, TrueDestXMove, ScreenSelector, RES.X, RES.Y);
}

void graphics::vesainfo::Retrieve()
{
  Signature = 0x32454256;
  dosmemput(this, sizeof(vesainfo), __tb);
  __dpmi_regs Regs;
  Regs.x.ax = 0x4F00;
  Regs.x.di =  __tb       & 0x000F;
  Regs.x.es = (__tb >> 4) & 0xFFFF;
  __dpmi_int(0x10, &Regs);
  dosmemget(__tb, sizeof(vesainfo), this);
}

void graphics::modeinfo::Retrieve(ushort Mode)
{
  __dpmi_regs Regs;
  Regs.x.ax = 0x4F01;
  Regs.x.cx = Mode;
  Regs.x.di =  __tb       & 0x000F;
  Regs.x.es = (__tb >> 4) & 0xFFFF;
  dosmemput(this, sizeof(modeinfo), __tb);
  __dpmi_int(0x10, &Regs);
  dosmemget(__tb, sizeof(modeinfo), this);
}

#endif
