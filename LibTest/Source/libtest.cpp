#include <complex>
#include <ctime>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "save.h"

#include "feio.h"

int Main(HINSTANCE hInst, HINSTANCE hPrevInst, HWND* hWnd, LPSTR pCmdLine, int nCmdShow)
{
	MessageBox(NULL, "Running IVAN Library Collection Test", "Message from FEP", MB_OK);

	graphics::SetMode(hInst, hWnd, "Esko", 800, 600, 16, false);

	DOUBLEBUFFER->ClearToColor(0xF81F);

	bitmap NovaChar("Graphics/NovaChar.pcx", false);

	ushort Color[1];

	Color[0] = MAKE_RGB(128, 0, 0);
	bitmap* CharRed = NovaChar.ColorizeTo16Bit(vector2d(0, 16), vector2d(32, 32), Color);
	Color[0] = MAKE_RGB(0, 128, 0);
	bitmap* CharGreen = NovaChar.ColorizeTo16Bit(vector2d(0, 16), vector2d(32, 32), Color);
	Color[0] = MAKE_RGB(0, 0, 128);
	bitmap* CharBlue = NovaChar.ColorizeTo16Bit(vector2d(0, 16), vector2d(32, 32), Color);

	bitmap Font("Graphics/FontR.pcx");
	bitmap Pertti("Graphics/Char.pcx");

	Font.Printf(DOUBLEBUFFER, 212, 450, "Valpuri rulaa!!! Ja muuten, %d * %d on %d.", 42, 666, 42 * 666);

	CharRed->MaskedBlit(DOUBLEBUFFER, 0, 0, 352, 308, 32, 32);
	CharGreen->MaskedBlit(DOUBLEBUFFER, 0, 0, 384, 308, 32, 32);
	CharBlue->MaskedBlit(DOUBLEBUFFER, 0, 0, 416, 308, 32, 32);

	for(uchar x = 0; x < 8; x++)
	{
		Pertti.Blit(DOUBLEBUFFER, 160, 128, 336 + (x << 4), 356, 16, 16, x);
		Pertti.MaskedBlit(DOUBLEBUFFER, 160, 128, 336 + (x << 4), 372, 16, 16, x);
	}

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

	bitmap Backup(800, 600);

	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, 800, 600);

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

		Backup.MaskedBlit(DOUBLEBUFFER, 0, 0, 0, 0, 800, 600);
		graphics::BlitDBToScreen();

		if(globalwindowhandler::ReadKey())
			return 0;
	}

	GETKEY();

	return 0;
}
