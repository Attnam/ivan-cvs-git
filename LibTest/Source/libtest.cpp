#include <windows.h>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "ddutil.h"
#include "error.h"

int Main(HINSTANCE hInst, HINSTANCE hPrevInst, HWND* hWnd, LPSTR pCmdLine, int nCmdShow)
{
	MessageBox(NULL, "Running IVAN Library Collection Test", "Message from FEP", MB_OK);

	graphics::SetMode(hInst, hWnd, "Esko", 800, 600, 16, false);

	bitmap* Font = new bitmap("Graphics/FontR.pcx");

	bitmap* Pertti = new bitmap("Graphics/Char.pcx");

	{
		std::ofstream SaveFile("Kalle.sav", std::ios::out | std::ios::binary);

		Font->Save(&SaveFile, 10, 10, 200, 200);

		SaveFile.close();
	}
	{
		std::ifstream SaveFile("Kalle.sav", std::ios::out | std::ios::binary);

		Font->Load(&SaveFile, 10, 10, 200, 200);

		SaveFile.close();
	}

	DOUBLEBUFFER->ClearToColor(0xFFFF);
	DOUBLEBUFFER->ClearToColor(50, 350, 700, 200, 0xF81F);
	DOUBLEBUFFER->Printf(Font, 212, 16, "Valpuri rulaa!!! Ja muuten, %d * %d on %d.", 42, 666, 42 * 666);
	graphics::BlitDBToScreen();

	DOUBLEBUFFER->ClearToColor(0xFFFF);
	DOUBLEBUFFER->ClearToColor(50, 350, 700, 200, 0xF81F);
	Font->PrintfToDB(212, 16, "Valpuri rulaa!!! Ja muuten, %d * %d on %d.", 42, 666, 42 * 666);

	for(uchar x = 0; x < 8; x++)
	{
		Pertti->Blit(DOUBLEBUFFER, 160, 128, x << 4, 0, 16,16, x);
		Pertti->MaskedBlit(DOUBLEBUFFER, 160, 128, x << 4, 16, 16,16, x);
	}

	Pertti->MaskedBlit(DOUBLEBUFFER, 96, 128, 392, 292, 16, 16, ushort(256));

	for(ushort z = 511; z >= 256; z--)
	{
		Pertti->MaskedBlit(DOUBLEBUFFER, 96, 128, 392, 292, 16, 16, z);
		graphics::BlitDBToScreen();
	}

	for(float a = 30, b = -100, d = -10, e = 100, c = 0; c < 4; c += 0.0005f)
	{
		ushort x = ushort(400 + c * 100), y = ushort(300 - (c * c * c * c * a + c * c * c * b + c * c * d + c * e));

		if(y < 600)
			DOUBLEBUFFER->PutPixel(x, y, 0);

		static ushort p = 0;

		if(p++ == 10)
		{
			graphics::BlitDBToScreen();

			p = 0;
		}
	}

	DOUBLEBUFFER->Save("Scrshot.bmp");

	globalwindowhandler::GetKey();

	delete Pertti;
	delete Font;

	return 0;
}
