#include <ctime>

#include "graphics.h"
#include "bitmap.h"
#include "whandler.h"
#include "save.h"

int Main(HINSTANCE hInst, HINSTANCE hPrevInst, HWND* hWnd, LPSTR pCmdLine, int nCmdShow)
{
	MessageBox(NULL, "Running IVAN Library Collection Test", "Message from FEP", MB_OK);

	graphics::SetMode(hInst, hWnd, "Esko", 800, 600, 16, false);

	bitmap Font("Graphics/FontR.pcx");
	bitmap Pertti("Graphics/Char.pcx");

	inputfile IFile("Script/test.dat");

	std::string Buffer;

	srand(time(0));

	std::map<std::string, long> Map;

	Map["pepe"] = 4;

	long Value = IFile.ReadNumber(Map);

	Buffer += IFile.ReadWord();
	Buffer += IFile.ReadWord();
	bool B = IFile.ReadBool();
	Buffer += IFile.ReadWord();

	DOUBLEBUFFER->ClearToColor(0xFFFF);
	DOUBLEBUFFER->ClearToColor(50, 350, 700, 200, 0xF81F);
	Font.Printf(DOUBLEBUFFER, 212, 16, "Valpuri rulaa!!! Ja muuten, %d * %d on %d. 0x%X %s", 42, 666, 42 * 666, Value, Buffer.c_str());

	for(uchar x = 0; x < 8; x++)
	{
		Pertti.Blit(DOUBLEBUFFER, 160, 128, x << 4, 0, 16, 16, x);
		Pertti.MaskedBlit(DOUBLEBUFFER, 160, 128, x << 4, 16, 16, 16, x);
	}

	Pertti.MaskedBlit(DOUBLEBUFFER, 96, 128, 392, 292, 16, 16, ushort(256));

	for(ushort z = 511; z >= 256; z--)
	{
		Pertti.MaskedBlit(DOUBLEBUFFER, 96, 128, 392, 292, 16, 16, z);
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

	outputfile O("p.bmp");

	Font.Save(O);

	GETKEY();

	return 0;
}
