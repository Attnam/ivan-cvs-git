#include <string>
#include <cmath>
#include <ctime>

#include "bitmap.h"
#include "graphics.h"
#include "error.h"
#include "save.h"
#include "allocate.h"
#include "femath.h"

void BlitNoFlags(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirror(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitFlip(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirrorFlip(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirrorRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirrorFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitLuminated(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void MaskedBlitNoFlags(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirror(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitFlip(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirrorFlip(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirrorRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirrorFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitLuminated(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlit(ulong, ulong, ulong, ulong, ushort, ushort, uchar, ushort);
void AlphaBlit(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void DrawLine(ulong, ulong, ushort, ushort, ushort, ushort, ushort, ushort, ushort);

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
}

void Fill(ulong, ulong, ushort, ushort, ushort);

void bitmap::Fill(ushort X, ushort Y, ushort Width, ushort Height, ushort Color)
{
	ulong TrueOffset = ulong(&Data[Y][X]);
	ulong TrueXMove = (XSize - Width) << 1;

	::Fill(TrueOffset, TrueXMove, Width, Height, Color);

	/*__asm
	{
	pushad
	push es
	mov ax, ds
	mov edi, TrueOffset
	mov es, ax
	xor ecx, ecx
	mov dx, Width
	mov bx, Height
	mov ax, Color
	cld
MaskedLoop3:
	mov cx, dx
	rep stosw
	add edi, TrueXMove
	dec bx
	jnz MaskedLoop3
	pop es
	popad
	}*/
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

			/*__asm
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
			}*/

			break;
		}

		case MIRROR:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;

			BlitMirror(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov ebx, TrueSourceXMove
				mov edx, TrueDestXMove
				cld
			MaskedLoop1:
				mov cx, Width
			MaskedLoop2:
				lodsw
				std
				stosw
				cld
				dec cx
				jnz MaskedLoop2
				add esi, ebx
				add edi, edx
				dec Height
				jnz MaskedLoop1
				pop es
				popad
			}*/

			break;
		}

		case FLIP:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;

			BlitFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);

			/*__asm
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
			MaskedLoop32:
				mov cx, dx
				rep movsd
				add esi, TrueSourceXMove
				sub edi, TrueDestXMove
				dec bx
				jnz MaskedLoop32
				pop es
				popad
			}*/

			break;
		}

		case (MIRROR | FLIP):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

			BlitMirrorFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov ebx, TrueSourceXMove
				mov edx, TrueDestXMove
				cld
			MaskedLoop6:
				mov cx, Width
			MaskedLoop7:
				lodsw
				std
				stosw
				cld
				dec cx
				jnz MaskedLoop7
				add esi, ebx
				sub edi, edx
				dec Height
				jnz MaskedLoop6
				pop es
				popad
			}*/

			break;
		}

		case ROTATE_90:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;

			BlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop4:
				mov cx, dx
			MaskedLoop5:
				lodsw
				mov [edi], ax
				add edi, ebx
				dec cx
				jnz MaskedLoop5
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop4
				pop es
				popad
			}*/

			break;
		}

		case (MIRROR | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;

			BlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop8:
				mov cx, dx
			MaskedLoop9:
				lodsw
				mov [edi], ax
				add edi, ebx
				dec cx
				jnz MaskedLoop9
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop8
				pop es
				popad
			}*/

			break;
		}

		case (FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;

			BlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop10:
				mov cx, dx
			MaskedLoop11:
				lodsw
				mov [edi], ax
				sub edi, ebx
				dec cx
				jnz MaskedLoop11
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop10
				pop es
				popad
			}*/

			break;
		}

		case (MIRROR | FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;

			BlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop12:
				mov cx, dx
			MaskedLoop13:
				lodsw
				mov [edi], ax
				sub edi, ebx
				dec cx
				jnz MaskedLoop13
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop12
				pop es
				popad
			}*/
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

	/*__asm
	{
		pushad
		push es
		mov ax, ds
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		xor edx, edx
		mov es, ax
		mov dx, Luminance
		xor ecx, ecx
		sub dx, 0x100
		cld
	MaskedLoop1:
		mov cx, Width
	MaskedLoop2:
		lodsw
		mov bx, ax
		and bx, 0x1F
		shl bx, 0x03
		add bx, dx
		jns Next1
		xor bx, bx
		jmp Next2
	Next1:
		cmp bx, 0x100
		jb Next2
		mov bx, 0xFF
	Next2:
		shr bx, 0x03
		and ax, 0xFFE0
		or ax, bx
		ror ax, 0x05
		mov bx, ax
		and bx, 0x3F
		shl bx, 0x02
		add bx, dx
		jns Next3
		xor bx, bx
		jmp Next4
	Next3:
		cmp bx, 0x100
		jb Next4
		mov bx, 0xFF
	Next4:
		shr bx, 0x02
		and ax, 0xFFC0
		or ax, bx
		ror ax, 0x06
		mov bx, ax
		and bx, 0x1F
		shl bx, 0x03
		add bx, dx
		jns Next5
		xor bx, bx
		jmp Next6
	Next5:
		cmp bx, 0x100
		jb Next6
		mov bx, 0xFF
	Next6:
		shr bx, 0x03
		and ax, 0xFFE0
		or ax, bx
		ror ax, 0x05
		stosw
		dec cx
		jnz MaskedLoop2
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		pop es
		popad
	}*/
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

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov ebx, TrueSourceXMove
				mov edx, TrueDestXMove
				cld
			MaskedLoop3:
				mov cx, Width
			MaskedLoop55:
				lodsw
				cmp ax, MaskColor
				je MaskSkip2
				stosw
				dec cx
				jnz MaskedLoop55
				add esi, ebx
				add edi, edx
				dec Height
				jnz MaskedLoop3
				jmp MaskedNextLine2
			MaskSkip2:
				add edi, 0x02
				dec cx
				jnz MaskedLoop55
				add esi, ebx
				add edi, edx
				dec Height
				jnz MaskedLoop3
			MaskedNextLine2:
				pop es
				popad
			}*/

			break;
		}

		case MIRROR:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;

			MaskedBlitMirror(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov ebx, TrueSourceXMove
				mov edx, TrueDestXMove
				cld
			MaskedLoop1:
				mov cx, Width
			MaskedLoop2:
				lodsw
				cmp ax, MaskColor
				je MaskSkip1
				std
				stosw
				cld
				dec cx
				jnz MaskedLoop2
				add esi, ebx
				add edi, edx
				dec Height
				jnz MaskedLoop1
				jmp MaskedNextLine1
			MaskSkip1:
				sub edi, 0x02
				dec cx
				jnz MaskedLoop2
				add esi, ebx
				add edi, edx
				dec Height
				jnz MaskedLoop1
			MaskedNextLine1:
				pop es
				popad
			}*/

			break;
		}

		case FLIP:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = (Bitmap->XSize + Width) << 1;

			MaskedBlitFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov ebx, TrueSourceXMove
				mov edx, TrueDestXMove
				cld
			MaskedLoop37:
				mov cx, Width
			MaskedLoop557:
				lodsw
				cmp ax, MaskColor
				je MaskSkip27
				stosw
				dec cx
				jnz MaskedLoop557
				add esi, ebx
				sub edi, edx
				dec Height
				jnz MaskedLoop37
				jmp MaskedNextLine27
			MaskSkip27:
				add edi, 0x02
				dec cx
				jnz MaskedLoop557
				add esi, ebx
				sub edi, edx
				dec Height
				jnz MaskedLoop37
			MaskedNextLine27:
				pop es
				popad
			}*/

			break;
		}

		case (MIRROR | FLIP):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = (Bitmap->XSize - Width) << 1;

			MaskedBlitMirrorFlip(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov ebx, TrueSourceXMove
				mov edx, TrueDestXMove
				cld
			MaskedLoop6:
				mov cx, Width
			MaskedLoop7:
				lodsw
				cmp ax, MaskColor
				je MaskSkip3
				std
				stosw
				cld
				dec cx
				jnz MaskedLoop7
				add esi, ebx
				sub edi, edx
				dec Height
				jnz MaskedLoop6
				jmp MaskedNextLine3
			MaskSkip3:
				sub edi, 0x02
				dec cx
				jnz MaskedLoop7
				add esi, ebx
				sub edi, edx
				dec Height
				jnz MaskedLoop6
			MaskedNextLine3:
				pop es
				popad
			}*/

			break;
		}

		case ROTATE_90:
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;

			MaskedBlitRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop4:
				mov cx, dx
			MaskedLoop5:
				lodsw
				cmp ax, MaskColor
				je MaskSkip4
				mov [edi], ax
			MaskSkip4:
				add edi, ebx
				dec cx
				jnz MaskedLoop5
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop4
				pop es
				popad
			}*/

			break;
		}

		case (MIRROR | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;

			MaskedBlitMirrorRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop8:
				mov cx, dx
			MaskedLoop9:
				lodsw
				cmp ax, MaskColor
				je MaskSkip5
				mov [edi], ax
			MaskSkip5:
				add edi, ebx
				dec cx
				jnz MaskedLoop9
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop8
				pop es
				popad
			}*/

			break;
		}

		case (FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX + Width - 1]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) - 2;

			MaskedBlitFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop10:
				mov cx, dx
			MaskedLoop11:
				lodsw
				cmp ax, MaskColor
				je MaskSkip6
				mov [edi], ax
			MaskSkip6:
				sub edi, ebx
				dec cx
				jnz MaskedLoop11
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop10
				pop es
				popad
			}*/

			break;
		}

		case (MIRROR | FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(&Bitmap->Data[DestY + Height - 1][DestX]);
			ulong TrueDestXMove = Bitmap->XSize << 1;
			ulong TrueDestYMove = ((Height * Bitmap->XSize) << 1) + 2;

			MaskedBlitMirrorFlipRotate90(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, TrueDestYMove, Width, Height, MaskColor);

			/*__asm
			{
				pushad
				push es
				mov ax, ds
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				mov es, ax
				xor ecx, ecx
				mov dx, Width
				mov ebx, TrueDestXMove
				cld
			MaskedLoop12:
				mov cx, dx
			MaskedLoop13:
				lodsw
				cmp ax, MaskColor
				je MaskSkip7
				mov [edi], ax
			MaskSkip7:
				sub edi, ebx
				dec cx
				jnz MaskedLoop13
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop12
				pop es
				popad
			}*/
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

	/*__asm
	{
		pushad
		push es
		mov ax, ds
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		xor edx, edx
		mov es, ax
		mov dx, Luminance
		xor ecx, ecx
		sub dx, 0x100
		cld
	MaskedLoop1:
		mov cx, Width
	MaskedLoop2:
		lodsw
		cmp ax, MaskColor
		je MaskSkip
		mov bx, ax
		and bx, 0x1F
		shl bx, 0x03
		add bx, dx
		jns Next1
		xor bx, bx
		jmp Next2
	Next1:
		cmp bx, 0x100
		jb Next2
		mov bx, 0xFF
	Next2:
		shr bx, 0x03
		and ax, 0xFFE0
		or ax, bx
		ror ax, 0x05
		mov bx, ax
		and bx, 0x3F
		shl bx, 0x02
		add bx, dx
		jns Next3
		xor bx, bx
		jmp Next4
	Next3:
		cmp bx, 0x100
		jb Next4
		mov bx, 0xFF
	Next4:
		shr bx, 0x02
		and ax, 0xFFC0
		or ax, bx
		ror ax, 0x06
		mov bx, ax
		and bx, 0x1F
		shl bx, 0x03
		add bx, dx
		jns Next5
		xor bx, bx
		jmp Next6
	Next5:
		cmp bx, 0x100
		jb Next6
		mov bx, 0xFF
	Next6:
		shr bx, 0x03
		and ax, 0xFFE0
		or ax, bx
		ror ax, 0x05
		stosw
		dec cx
		jnz MaskedLoop2
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		jmp MaskedNextLine
	MaskSkip:
		add edi, 0x02
		dec cx
		jnz MaskedLoop2
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
	MaskedNextLine:
		pop es
		popad
	}*/
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

	//ushort SColor, DColor;

	::AlphaBlit(TrueSourceOffset, TrueDestOffset, TrueSourceXMove, TrueDestXMove, Width, Height, Alpha, MaskColor);

	/*__asm
	{
		pushad
		push es
		mov ax, ds
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		mov es, ax
		mov dl, Alpha
		cld
	MaskedLoop1:
		mov cx, Width
	MaskedLoop2:
		lodsw
		cmp ax, MaskColor
		je MaskSkip
		mov bx, [edi]
		mov DColor, bx
		mov bx, ax
		and ax, 0x1F
		shl ax, 0x03
		mul dl
		mov SColor, ax
		mov ax, DColor
		and ax, 0x1F
		shl ax, 0x03
		not dl
		mul dl
		not dl
		add ax, SColor
		shr ax, 0x0B
		and bx, 0xFFE0
		or bx, ax
		ror bx, 0x05
		ror DColor, 0x05
		mov ax, bx
		and ax, 0x3F
		shl ax, 0x02
		mul dl
		mov SColor, ax
		mov ax, DColor
		and ax, 0x3F
		shl ax, 0x02
		not dl
		mul dl
		not dl
		add ax, SColor
		shr ax, 0x0A
		and bx, 0xFFC0
		or bx, ax
		ror bx, 0x06
		ror DColor, 0x06
		mov ax, bx
		and ax, 0x1F
		shl ax, 0x03
		mul dl
		mov SColor, ax
		mov ax, DColor
		and ax, 0x1F
		shl ax, 0x03
		not dl
		mul dl
		not dl
		add ax, SColor
		shr ax, 0x0B
		and bx, 0xFFE0
		or bx, ax
		ror bx, 0x05
		mov ax, bx
		stosw
		dec cx
		jnz MaskedLoop2
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		jmp MaskedNextLine
	MaskSkip:
		add edi, 0x02
		dec cx
		jnz MaskedLoop2
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
	MaskedNextLine:
		pop es
		popad
	}*/
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort DestX, ushort DestY, ushort MaskColor) const
{
	if(!AlphaMap)
		ABORT("AlphaMap not available!");

	ulong TrueSourceOffset = ulong(Data[0]);
	ulong TrueDestOffset = ulong(&Bitmap->Data[DestY][DestX]);
	ulong TrueDestXMove = (Bitmap->XSize - XSize) << 1;
	ulong AlphaMapOffset = ulong(AlphaMap[0]);

	ushort /*SColor, DColor, */Width = XSize, Height = YSize;

	::AlphaBlit(AlphaMapOffset, TrueSourceOffset, TrueDestOffset, TrueDestXMove, Width, Height, MaskColor);

	/*__asm
	{
		pushad
		push es
		mov ax, ds
		mov es, ax
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		push AlphaMapOffset
		cld
	MaskedLoop1:
		mov cx, Width
	MaskedLoop2:
		lodsw
		pop ebx
		mov dl, [ebx]
		inc ebx
		push ebx
		cmp ax, MaskColor
		je MaskSkip
		mov bx, [edi]
		mov DColor, bx
		mov bx, ax
		and ax, 0x1F
		shl ax, 0x03
		mul dl
		mov SColor, ax
		mov ax, DColor
		and ax, 0x1F
		shl ax, 0x03
		not dl
		mul dl
		not dl
		add ax, SColor
		shr ax, 0x0B
		and bx, 0xFFE0
		or bx, ax
		ror bx, 0x05
		ror DColor, 0x05
		mov ax, bx
		and ax, 0x3F
		shl ax, 0x02
		mul dl
		mov SColor, ax
		mov ax, DColor
		and ax, 0x3F
		shl ax, 0x02
		not dl
		mul dl
		not dl
		add ax, SColor
		shr ax, 0x0A
		and bx, 0xFFC0
		or bx, ax
		ror bx, 0x06
		ror DColor, 0x06
		mov ax, bx
		and ax, 0x1F
		shl ax, 0x03
		mul dl
		mov SColor, ax
		mov ax, DColor
		and ax, 0x1F
		shl ax, 0x03
		not dl
		mul dl
		not dl
		add ax, SColor
		shr ax, 0x0B
		and bx, 0xFFE0
		or bx, ax
		ror bx, 0x05
		mov ax, bx
		stosw
		dec cx
		jnz MaskedLoop2
		jmp MaskedNextLine
	MaskSkip:
		add edi, 0x02
		dec cx
		jnz MaskedLoop2
	MaskedNextLine:
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		pop ebx
		pop es
		popad
	}*/
}

void bitmap::DrawLine(ushort OrigFromX, ushort OrigFromY, ushort OrigToX, ushort OrigToY, ushort Color, bool Wide)
{
	ushort ThisXSize = XSize, ThisYSize = YSize;//, DistaX, DistaY, BTemp;

	ulong Pitch = XSize << 1, Surface = ulong(Data[0]);

	static vector2d Point[] = {vector2d(0, 0), vector2d(0, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(0, 1)};

	for(uchar c = 0; c < (Wide ? 5 : 1); ++c)
	{
		ushort FromX = OrigFromX + Point[c].X;
		ushort FromY = OrigFromY + Point[c].Y;
		ushort ToX = OrigToX + Point[c].X;
		ushort ToY = OrigToY + Point[c].Y;

		::DrawLine(Surface, Pitch, FromX, FromY, ToX, ToY, ThisXSize, ThisYSize, Color);

		/*__asm
		{
			push	ax
			push	bx
			push	cx
			push	dx
			xor	eax,	eax
			xor	ebx,	ebx
			mov	bx,	FromX
			mov	ax,	FromY
			mov	dx,	ToX
			mov	cx,	ToY
			cmp	bx,	dx
			jnz	LineNotVertical
			cmp	ax,	cx
			jl	VerticalLinePlus
			jg	VerticalLineMinus
			jmp	LineQuickEnd
		VerticalLinePlus:
			call	NPutPixel
			inc	ax
			cmp	ax,	cx
			jl	VerticalLinePlus
			jmp	LineQuickEnd
		VerticalLineMinus:
			call	NPutPixel
			dec	ax
			cmp	ax,	cx
			jg	VerticalLineMinus
			jmp	LineQuickEnd
		LineNotVertical:	
			cmp	ax,	cx
			jnz	LineNotHorizontal
			cmp	bx,	dx
			jl	LineHorizontalPlus
			jg	LineHorizontalMinus
		LineHorizontalPlus:
			call	NPutPixel
			inc	bx
			cmp	bx,	dx
			jl	LineHorizontalPlus
			jmp	LineQuickEnd
		LineHorizontalMinus:
			call	NPutPixel
			dec	bx
			cmp	bx,	dx
			jg	LineHorizontalMinus
		LineQuickEnd:
			call	NPutPixel
			pop	dx
			pop	cx
			pop	bx
			pop	ax
			jmp	End
		LineNotHorizontal:
			push	si
			push	di
			cmp	dx,	bx
			jl	LineXSwapNeeded
			sub	dx,	bx
			mov	si,	0x0001
			jmp	LineNext1
		LineXSwapNeeded:
			push	bx
			sub	bx,	dx
			mov	dx,	bx
			pop	bx
			mov	si,	0xFFFF
		LineNext1:
			cmp	cx,	ax
			jl	LineYSwapNeeded
			sub	cx,	ax
			mov	di,	0x0001
			jmp	LineNext2
		LineYSwapNeeded:
			push	ax
			sub	ax,	cx
			mov	cx,	ax
			pop	ax
			mov	di,	0xFFFF
		LineNext2:
			cmp	dx,	cx
			jl	LineXYSwap
			mov	DistaX,	dx
			mov	DistaY,	cx
			mov	bx,	FromX
			mov	ax,	FromY
			xor	cl,	cl
			jmp	LineXYNoSwap
		LineXYSwap:
			mov	DistaY,	dx
			mov	DistaX,	cx
			mov	ax,	FromX
			mov	bx,	FromY
			mov	dx,	ToX
			mov	cx,	ToY
			mov	ToY,	dx
			mov	ToX,	cx
			mov	cx,	si
			mov	dx,	di
			mov	di,	cx
			mov	si,	dx
			mov	dx,	DistaX
			mov	cl,	0x01
		LineXYNoSwap:
			shr	dx,	0x01
			inc	dx
			mov	BTemp,	dx
			mov	dh,	cl
			xor	cx,	cx
			jmp	LineLoopBegin
		LineXPlus:
			add	cx,	DistaY
		LineLoopBegin:
			cmp	cx,	BTemp
			jl	LineNotYPlus
			sub	cx,	DistaX
			add	ax,	di
		LineNotYPlus:
			call	SPutPixel
			add	bx,	si
			cmp	bx,	ToX
			jnz	LineXPlus
			add	cx,	DistaY
			cmp	cx,	BTemp
			jl	LineEnd
			add	ax,	di
		LineEnd:call	SPutPixel
			pop	di
			pop	si
			pop	dx
			pop	cx
			pop	bx
			pop	ax
			jmp	End

		NPutPixel:
			cmp	bx,	ThisXSize
			jae	NPutPixelEnd
			cmp	ax,	ThisYSize
			jae	NPutPixelEnd
			push	eax
			push	ebx
			push	edx
			mul	Pitch
			add	eax,	Surface
			shl	ebx,	0x01
			add	eax,	ebx
			mov	ebx,	eax
			mov	ax,	Color
			mov	[ebx],	ax
			pop	edx
			pop	ebx
			pop	eax
		NPutPixelEnd:
			ret

		SPutPixel:
			push	eax
			push	ebx
			or	dh,	dh
			jz	SPutPixelNoSwap
			xchg	ax,	bx
		SPutPixelNoSwap:
			cmp	bx,	ThisXSize
			jae	SPutPixelEnd
			cmp	ax,	ThisYSize
			jae	SPutPixelEnd
			push	edx
			mul	Pitch
			add	eax,	Surface
			shl	ebx,	0x01
			add	eax,	ebx
			mov	ebx,	eax
			mov	ax,	Color
			mov	[ebx],	ax
			pop	edx
		SPutPixelEnd:
			pop	ebx
			pop	eax
			ret

		End:
		}*/
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
		Points.push_back(vector2d(PosX, PosY) + Center);
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
		for(ulong c = 0; c < XSize * YSize; ++c)
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
		for(ulong c = 0; c < XSize * YSize; ++c)
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
