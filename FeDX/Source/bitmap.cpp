#include <string>
#include <cmath>
#include <ctime>

#include "bitmap.h"
#include "graphics.h"
#include "error.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"
#include "blit.h"

bitmap::bitmap(std::string FileName) : AlphaMap(0)
{
	std::ifstream File(FileName.c_str(), std::ios::in | std::ios::binary);

	if(!File.is_open())
		ABORT("Bitmap %s not found!", FileName.c_str());

	uchar Palette[768];
	File.seekg(-768, std::ios::end);
	File.read((char*)Palette, 768);

	File.seekg(8, std::ios::beg);

	XSize  =  File.get();
	XSize += (File.get() << 8) + 1;
	YSize  =  File.get();
	YSize += (File.get() << 8) + 1;

	File.seekg(128, std::ios::beg);

	Alloc2D<ushort>(Data, YSize, XSize);

	ushort* Buffer = Data[0];

	for(ushort y = 0; y < YSize; ++y)
		for(ushort x = 0; x < XSize; ++x)
		{
			int Char1 = File.get();

			if(Char1 > 192)
			{
				int Char2 = File.get();

				--x;

				for(; Char1 > 192; --Char1)
				{
                			*(Buffer++) = ushort(Palette[Char2 + (Char2 << 1)] >> 3) << 11 | ushort(Palette[Char2 + (Char2 << 1) + 1] >> 2) << 5 | ushort(Palette[Char2 + (Char2 << 1) + 2] >> 3);

					if(++x == XSize)
					{
						x = 0;
						++y;
					}
				}
        		}
			else
				*(Buffer++) = ushort(Palette[Char1 + (Char1 << 1)] >> 3) << 11 | ushort(Palette[Char1 + (Char1 << 1) + 1] >> 2) << 5 | ushort(Palette[Char1 + (Char1 << 1) + 2] >> 3);
		}
}

bitmap::bitmap(ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), Data(Alloc2D<ushort>(YSize, XSize)), AlphaMap(0)
{
}

bitmap::bitmap(ushort XSize, ushort YSize, ushort Color) : XSize(XSize), YSize(YSize), Data(Alloc2D<ushort>(YSize, XSize)), AlphaMap(0)
{
	Fill(Color);
}

bitmap::~bitmap()
{
	delete [] Data;
	delete [] AlphaMap;
}

void bitmap::Save(outputfile& SaveFile) const
{
	SaveFile.GetBuffer().write((char*)Data[0], (XSize * YSize) << 1);

	if(AlphaMap)
	{
		SaveFile << uchar(1);
		SaveFile.GetBuffer().write((char*)AlphaMap[0], XSize * YSize);
	}
	else
		SaveFile << uchar(0);
}

void bitmap::Load(inputfile& SaveFile)
{
	SaveFile.GetBuffer().read((char*)Data[0], (XSize * YSize) << 1);

	uchar Alpha;
	SaveFile >> Alpha;

	if(Alpha)
	{
		Alloc2D<uchar>(AlphaMap, XSize, YSize);
		SaveFile.GetBuffer().read((char*)AlphaMap[0], XSize * YSize);
	}
}

void bitmap::Save(std::string FileName) const
{
#ifdef WIN32
	static char BMPHeader[] =	{char(0x42), char(0x4D), char(0xB6), char(0x4F), char(0x12), char(0x00),
					 char(0x00), char(0x00), char(0x00), char(0x00), char(0x36), char(0x00),
					 char(0x00), char(0x00), char(0x28), char(0x00), char(0x00), char(0x00),
					 char(0x20), char(0x03), char(0x00), char(0x00), char(0xF4), char(0x01),
					 char(0x00), char(0x00), char(0x01), char(0x00), char(0x18), char(0x00),
					 char(0x00), char(0x00), char(0x00), char(0x00), char(0x80), char(0x4F),
					 char(0x12), char(0x00), char(0x33), char(0x0B), char(0x00), char(0x00),
					 char(0x33), char(0x0B), char(0x00), char(0x00), char(0x00), char(0x00),
					 char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00)};

	outputfile SaveFile(FileName);

	BMPHeader[0x12] =  XSize       & 0xFF;
	BMPHeader[0x13] = (XSize >> 8) & 0xFF;
	BMPHeader[0x16] =  YSize       & 0xFF;
	BMPHeader[0x17] = (YSize >> 8) & 0xFF;

	SaveFile.GetBuffer().write(BMPHeader, 0x36);

	for(long y = YSize - 1; y >= 0; --y)
		for(ushort x = 0; x < XSize; ++x)
		{
			ushort Pixel = Data[y][x];
			SaveFile << char(Pixel << 3) << char((Pixel >> 5) << 2) << char((Pixel >> 11) << 3);
		}
#endif
}

void bitmap::Fill(ushort X, ushort Y, ushort Width, ushort Height, ushort Color)
{
	ulong TrueOffset = ulong(&Data[Y][X]);
	ulong TrueXMove = (XSize - Width) << 1;

	::Fill(TrueOffset, TrueXMove, Width, Height, Color);
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags) const
{
	if(!Width || !Height)
		ABORT("Zero-sized bitmap blit attempt detected!");

	Flags &= 0x7;

	ulong TrueSourceOffset = ulong(&Data[SourceY][SourceX]);
	ulong TrueSourceXMove = (XSize - Width) << 1;

	switch(Flags)
	{
		case 0:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
			ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
			BlitNoFlags(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
			break;
		}

		case MIRROR:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
			BlitMirror(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
			break;
		}

		case FLIP:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
			BlitFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
			break;
		}

		case (MIRROR | FLIP):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
			BlitMirrorFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);
			break;
		}

		case ROTATE_90:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
			BlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
			break;
		}

		case (MIRROR | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
			BlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
			break;
		}

		case (FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
			BlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
			break;
		}

		case (MIRROR | FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
			BlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);
			break;
		}
	}
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
{
	if(Luminance == 256)
	{
		Blit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height);
		return;
	}

	if(!Width || !Height)
		ABORT("Zero-sized bitmap blit attempt detected!");

	ulong TrueSourceOffset = ulong(&Data[SourceY][SourceX]);
	ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
	ulong TrueSourceXMove = (XSize - Width) << 1;
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

	BlitLuminated(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Luminance);
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags, ushort MaskColor) const
{
	if(!Width || !Height)
		ABORT("Zero-sized bitmap blit attempt detected!");

	Flags &= 0x7;

	ulong TrueSourceOffset = ulong(&Data[SourceY][SourceX]);
	ulong TrueSourceXMove = (XSize - Width) << 1;

	switch(Flags)
	{
		case 0:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
			ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
			MaskedBlitNoFlags(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
			break;
		}

		case MIRROR:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
			MaskedBlitMirror(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
			break;
		}

		case FLIP:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;
			MaskedBlitFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
			break;
		}

		case (MIRROR | FLIP):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;
			MaskedBlitMirrorFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);
			break;
		}

		case ROTATE_90:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
			MaskedBlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
			break;
		}

		case (MIRROR | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
			MaskedBlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
			break;
		}

		case (FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;
			MaskedBlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
			break;
		}

		case (MIRROR | FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;
			MaskedBlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);
			break;
		}
	}
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance, ushort MaskColor) const
{
	if(Luminance == 256)
	{
		MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, uchar(0), MaskColor);
		return;
	}

	if(!Width || !Height)
		ABORT("Zero-sized bitmap blit attempt detected!");

	ulong TrueSourceOffset = ulong(&Data[SourceY][SourceX]);
	ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
	ulong TrueSourceXMove = (XSize - Width) << 1;
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

	MaskedBlitLuminated(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Luminance, MaskColor);
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Alpha, ushort MaskColor) const
{
	if(Alpha == 255)
	{
		MaskedBlit(Bitmap, SourceX, SourceY, DestX, DestY, Width, Height, uchar(0), MaskColor);
		return;
	}

	if(!Width || !Height)
		ABORT("Zero-sized bitmap blit attempt detected!");

	ulong TrueSourceOffset = ulong(&Data[SourceY][SourceX]);
	ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
	ulong TrueSourceXMove = (XSize - Width) << 1;
	ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

	::AlphaBlit(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Alpha, MaskColor);
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort DestX, ushort DestY, ushort MaskColor) const
{
	if(!AlphaMap)
		ABORT("AlphaMap not available!");

	ulong TrueSourceOffset = ulong(Data[0]);
	ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize - XSize) << 1;
	ulong AlphaMapOffset = ulong(AlphaMap[0]);

	ushort Width = XSize, Height = YSize;

	::AlphaBlit(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueDestXMove, Width, Height, MaskColor);
}

void bitmap::DrawLine(ushort OrigFromX, ushort OrigFromY, ushort OrigToX, ushort OrigToY, ushort Color, bool Wide)
{
	ushort ThisXSize = XSize, ThisYSize = YSize;

	ulong Pitch = XSize << 1, Surface = ulong(Data[0]);

	static vector2d Point[] = {vector2d(0, 0), vector2d(0, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(0, 1)};

	for(uchar c = 0; c < (Wide ? 5 : 1); ++c)
	{
		ushort FromX = OrigFromX + Point[c].X;
		ushort FromY = OrigFromY + Point[c].Y;
		ushort ToX = OrigToX + Point[c].X;
		ushort ToY = OrigToY + Point[c].Y;

		::DrawLine(Surface, Pitch, FromX, FromY, ToX, ToY, ThisXSize, ThisYSize, Color);
	}
}

void bitmap::DrawPolygon(vector2d Center, ushort Radius, ushort NumberOfSides, ushort Color, bool DrawDiameters, double Rotation)
{
	std::vector<vector2d> Points;

	ushort c;

	for(c = 0; c < NumberOfSides; ++c)
	{
		float PosX = sin((2 * PI / NumberOfSides) * c + Rotation) * Radius;
		float PosY = cos((2 * PI / NumberOfSides) * c + Rotation) * Radius;
		Points.push_back(vector2d(short(PosX), short(PosY)) + Center);
	}

	if(DrawDiameters)
		for(c = 0; c < Points.size(); ++c)
			for(ushort a = 0; a < Points.size(); ++a)
			{
				if(abs(int(c) - a) > 1 && !((a == 0) && c == Points.size() - 1) && !((c == 0) && a == Points.size() - 1))
					DrawLine(Points[c].X, Points[c].Y, Points[a].X, Points[a].Y, Color, true);
			}
	else
		for(c = 0; c < NumberOfSides; ++c)
			DrawLine(Points[c].X, Points[c].Y, Points[(c + 1) % Points.size()].X, Points[(c + 1) % Points.size()].Y, Color, true);
}

void bitmap::CreateAlphaMap(uchar InitialValue)
{
	if(AlphaMap)
		ABORT("Alpha leak detected!");

	Alloc2D<uchar>(AlphaMap, YSize, XSize, InitialValue);
}

bool bitmap::ChangeAlpha(char Amount)
{
	if(!Amount)
		return false;

	bool Changes = false;

	if(!AlphaMap)
		ABORT("No alpha map to fade.");

	if(Amount > 0)
	{
		for(ulong c = 0; c < ulong(XSize * YSize); ++c)
			if(AlphaMap[0][c] < 255 - Amount)
			{
				AlphaMap[0][c] += Amount;
				Changes = true;
			}
			else
				if(AlphaMap[0][c] != 255)
				{
					AlphaMap[0][c] = 255;
					Changes = true;
				}
	}
	else
		for(ulong c = 0; c < ulong(XSize * YSize); ++c)
			if(AlphaMap[0][c] > -Amount)
			{
				AlphaMap[0][c] += Amount;
				Changes = true;
			}
			else
				if(AlphaMap[0][c])
				{
					AlphaMap[0][c] = 0;
					Changes = true;
				}

	return Changes;
}

void bitmap::Outline(ushort Color)
{
	ulong Buffer;

	ushort LastColor, NextColor;

	for(ushort x = 0; x < XSize; ++x)
	{
		Buffer = ulong(&Data[0][x]);

		LastColor = *(ushort*)Buffer;

		for(ushort y = 0; y < YSize - 1; ++y)
		{
			NextColor = *(ushort*)(Buffer + (XSize << 1));

			if((LastColor == 0xF81F || !y) && NextColor != 0xF81F)
				*(ushort*)Buffer = Color;

			Buffer += XSize << 1;

			if(LastColor != 0xF81F && (NextColor == 0xF81F || y == YSize - 2))
				*(ushort*)Buffer = Color;

			LastColor = NextColor;
		}
	}

	for(ushort y = 0; y < YSize; ++y)
	{
		Buffer = ulong(Data[y]);

		LastColor = *(ushort*)Buffer;

		for(ushort x = 0; x < XSize - 1; ++x)
		{
			NextColor = *(ushort*)(Buffer + 2);

			if((LastColor == 0xF81F || !x) && NextColor != 0xF81F)
				*(ushort*)Buffer = Color;

			Buffer += 2;

			if(LastColor != 0xF81F && (NextColor == 0xF81F || x == XSize - 2))
				*(ushort*)Buffer = Color;

			LastColor = NextColor;
		}
	}
}

void bitmap::CreateOutlineBitmap(bitmap* Bitmap, ushort Color)
{
	Bitmap->Fill(0xF81F);

	for(ushort x = 0; x < XSize; ++x)
	{
		ulong SrcBuffer = ulong(&Data[0][x]);
		ulong DestBuffer = ulong(&Bitmap->Data[0][x]);

		ushort LastColor = *(ushort*)SrcBuffer;

		for(ushort y = 0; y < YSize - 1; ++y)
		{
			ushort NextColor = *(ushort*)(SrcBuffer + (XSize << 1));

			if((LastColor == 0xF81F || !y) && NextColor != 0xF81F)
				*(ushort*)DestBuffer = Color;

			SrcBuffer += XSize << 1;
			DestBuffer += Bitmap->XSize << 1;

			if(LastColor != 0xF81F && (NextColor == 0xF81F || y == YSize - 2))
				*(ushort*)DestBuffer = Color;

			LastColor = NextColor;
		}
	}

	for(ushort y = 0; y < YSize; ++y)
	{
		ulong SrcBuffer = ulong(Data[y]);
		ulong DestBuffer = ulong(Bitmap->Data[y]);

		ushort LastSrcColor = *(ushort*)SrcBuffer;
		ushort LastDestColor = *(ushort*)DestBuffer;

		for(ushort x = 0; x < XSize - 1; ++x)
		{
			ushort NextSrcColor = *(ushort*)(SrcBuffer + 2);
			ushort NextDestColor = *(ushort*)(DestBuffer + 2);

			if((LastSrcColor == 0xF81F || !x) && (NextSrcColor != 0xF81F || NextDestColor != 0xF81F))
				*(ushort*)DestBuffer = Color;

			SrcBuffer += 2;
			DestBuffer += 2;

			if((LastSrcColor != 0xF81F || LastDestColor != 0xF81F) && (NextSrcColor == 0xF81F || x == XSize - 2))
				*(ushort*)DestBuffer = Color;

			LastSrcColor = NextSrcColor;
			LastDestColor = NextDestColor;
		}
	}
}

void bitmap::FadeToScreen()
{
	bitmap Backup(XRES, YRES);
	DOUBLEBUFFER->Blit(&Backup, 0, 0, 0, 0, XRES, YRES);

	for(uchar c = 0; c <= 5; ++c)
	{
		clock_t StartTime = clock();
		Backup.MaskedBlit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES, ushort(255 - c * 50), 0);
		AlphaBlit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES, ushort(c * 50), 0);
		graphics::BlitDBToScreen();
		while(clock() - StartTime < 0.05f * CLOCKS_PER_SEC);
	}

	DOUBLEBUFFER->Fill(0);
	MaskedBlit(DOUBLEBUFFER, 0, 0, 0, 0, XRES, YRES, uchar(0), 0);
	graphics::BlitDBToScreen();
}
