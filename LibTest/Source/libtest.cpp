#include <complex>
#include <ctime>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "save.h"

int Main(HINSTANCE hInst, HINSTANCE hPrevInst, HWND* hWnd, LPSTR pCmdLine, int nCmdShow)
{
	MessageBox(NULL, "Running IVAN Library Collection Test", "Message from FEP", MB_OK);

	graphics::SetMode(hInst, hWnd, "Esko", 800, 600, 16, false);

	DOUBLEBUFFER->ClearToColor(MAKE_RGB(255, 255, 255));

	for(ushort y = 0; y < 600; y++)
	{
		DOUBLEBUFFER->Lock();

		for(ushort x = 0; x < 800; x++)
		{
			std::complex<double> C(-2.0f + double(y) / 230, -1.25f + double(x) / 320);

			std::complex<double> Value = 0;

			for(ushort Depth = 0; Depth < 31; ++Depth)
			{
				Value = Value * Value + C;

				if(std::abs(Value) >= 2)
					break;
			}

			Depth <<= 3;

			DOUBLEBUFFER->FastPutPixel(x, y, MAKE_RGB(255 - Depth, 255 - Depth, 255 - Depth));
		}

		DOUBLEBUFFER->Release();

		graphics::BlitDBToScreen();

		if(globalwindowhandler::ReadKey())
			return 0;
	}

	bitmap Font("Graphics/FontR.pcx");
	bitmap Pertti("Graphics/Char.pcx");

	Font.Printf(DOUBLEBUFFER, 212, 450, "Valpuri rulaa!!! Ja muuten, %d * %d on %d.", 42, 666, 42 * 666);

	for(uchar x = 0; x < 8; x++)
	{
		Pertti.Blit(DOUBLEBUFFER, 160, 128, x << 4, 0, 16, 16, x);
		Pertti.MaskedBlit(DOUBLEBUFFER, 160, 128, x << 4, 16, 16, 16, x);
	}

	Pertti.MaskedBlit(DOUBLEBUFFER, 96, 128, 392, 292, 16, 16, ushort(256));

	for(ushort z = 0; z <= 256; z++)
	{
		Pertti.MaskedBlit(DOUBLEBUFFER, 96, 128, 392, 292, 16, 16, z);
		graphics::BlitDBToScreen();
	}

	ushort LastX, LastY;

	for(float a = 30, b = -100, d = -10, e = 100, c = 0; c < 4; c += 0.01f)
	{
		ushort x = ushort(400 + c * 100), y = ushort(300 - (c * c * c * c * a + c * c * c * b + c * c * d + c * e));

		if(c)
			DOUBLEBUFFER->DrawLine(LastX, LastY, x, y, MAKE_RGB(128, 128, 32), true);

		LastX = x;
		LastY = y;

		graphics::BlitDBToScreen();
	}

	GETKEY();

	return 0;
}
