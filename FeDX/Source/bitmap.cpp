#include <cstdarg>
#include <cstdio>
#include <fstream>

#include "bitmap.h"
#include "graphics.h"
#include "ddutil.h"
#include "error.h"

char bitmap::BMPHeader[] =	{char(0x42), char(0x4D), char(0xB6), char(0x4F), char(0x12), char(0x00),
				 char(0x00), char(0x00), char(0x00), char(0x00), char(0x36), char(0x00),
				 char(0x00), char(0x00), char(0x28), char(0x00), char(0x00), char(0x00),
				 char(0x20), char(0x03), char(0x00), char(0x00), char(0xF4), char(0x01),
				 char(0x00), char(0x00), char(0x01), char(0x00), char(0x18), char(0x00),
				 char(0x00), char(0x00), char(0x00), char(0x00), char(0x80), char(0x4F),
				 char(0x12), char(0x00), char(0x33), char(0x0B), char(0x00), char(0x00),
				 char(0x33), char(0x0B), char(0x00), char(0x00), char(0x00), char(0x00),
				 char(0x00), char(0x00), char(0x00), char(0x00), char(0x00), char(0x00)};

bitmap::bitmap(const char* FileName) : BackupBuffer(0)
{
	FILE* Handle = fopen(FileName, "rb");

	if(!Handle)
		ABORT("Bitmap %s not found!", FileName);

	fseek(Handle, -768, SEEK_END);

	uchar Palette[768];

	for(ulong c = 0; c < 768; c++)
		Palette[c] = fgetc(Handle);

	fseek(Handle, 8, SEEK_SET);

	XSize = fgetc(Handle) + (fgetc(Handle) << 8) + 1;
	YSize = fgetc(Handle) + (fgetc(Handle) << 8) + 1;

	graphics::GetDXDisplay()->CreateSurface(&DXSurface, XSize, YSize);

	fseek(Handle, 128, SEEK_SET);

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ushort* Buffer = (ushort*)ddsd.lpSurface;

	for(ushort y = 0; y < YSize; y++)
	{
		for(ushort x = 0; x < XSize; x++)
		{
			int Char1 = fgetc(Handle);

			if(Char1 > 192)
			{
				int Char2 = fgetc(Handle);

				x--;

				for(; Char1 > 192; Char1--)
				{
                			*(Buffer++) = ushort(Palette[Char2 + (Char2 << 1)] >> 3) << 11 | ushort(Palette[Char2 + (Char2 << 1) + 1] >> 2) << 5 | ushort(Palette[Char2 + (Char2 << 1) + 2] >> 3);

					if(++x == XSize)
					{
						x = 0;
						y++;
						Buffer = (ushort*)(ulong(Buffer) + ddsd.lPitch - (XSize << 1));
					}
				}
        		}
			else
				*(Buffer++) = ushort(Palette[Char1 + (Char1 << 1)] >> 3) << 11 | ushort(Palette[Char1 + (Char1 << 1) + 1] >> 2) << 5 | ushort(Palette[Char1 + (Char1 << 1) + 2] >> 3);
		}

		Buffer = (ushort*)(ulong(Buffer) + ddsd.lPitch - (XSize << 1));
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	fclose(Handle);

	DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
	DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);

	graphics::BitmapContainer.push_back(this);
}

bitmap::bitmap(ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), BackupBuffer(0)
{
	graphics::GetDXDisplay()->CreateSurface(&DXSurface, XSize, YSize);

	DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
	DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);

	graphics::BitmapContainer.push_back(this);
}

bitmap::bitmap(IDirectDrawSurface7* DDSurface) : BackupBuffer(0)
{
	DXSurface = new CSurface;

	DXSurface->Create(DDSurface);

	DDSURFACEDESC2 ddsd;

	DDSurface->GetSurfaceDesc(&ddsd);

	XSize = ddsd.dwWidth;
	YSize = ddsd.dwHeight;

	DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
	DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);
}

bitmap::~bitmap(void)
{
	delete DXSurface;
	delete [] BackupBuffer;

	for(ulong c = 0; c < graphics::BitmapContainer.size(); c++)
		if(graphics::BitmapContainer[c] == this)
			graphics::BitmapContainer.erase(graphics::BitmapContainer.begin() + c);
}

void bitmap::Save(std::ofstream* SaveFile, ushort XPos, ushort YPos, ushort XSize, ushort YSize) const
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong Buffer = ulong(ddsd.lpSurface) + YPos * ddsd.lPitch + (XPos << 1);

	for(ushort y = YPos; y < YPos + YSize; y++, Buffer += ddsd.lPitch)
		SaveFile->write((char*)Buffer, XSize << 1);

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
}

void bitmap::Load(std::ifstream* SaveFile, ushort XPos, ushort YPos, ushort XSize, ushort YSize)
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong Buffer = ulong(ddsd.lpSurface) + YPos * ddsd.lPitch + (XPos << 1);

	for(ushort y = YPos; y < YPos + YSize; y++, Buffer += ddsd.lPitch)
		SaveFile->read((char*)Buffer, XSize << 1);

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
}

void bitmap::Save(std::string FileName) const
{
	std::ofstream SaveFile(FileName.c_str(), std::ios::out | std::ios::binary);

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ushort XSize = ddsd.dwWidth, YSize = ddsd.dwHeight, XMove = XSize << 2;

	BMPHeader[0x12] =  XSize       & 0xFF;
	BMPHeader[0x13] = (XSize >> 8) & 0xFF;
	BMPHeader[0x16] =  YSize       & 0xFF;
	BMPHeader[0x17] = (YSize >> 8) & 0xFF;

	SaveFile.write(BMPHeader, 0x36);

	ulong Off = ulong(ddsd.lpSurface) + (((YSize - 1) * XSize) << 1);

	for(ushort y = 0; y < YSize; y++, Off -= XMove)
		for(ushort x = 0; x < XSize; x++, Off += 2)
		{
			ushort Pixel = *(ushort*)Off;

			SaveFile.put(char(Pixel << 3));
			SaveFile.put(char((Pixel >> 5) << 2));
			SaveFile.put(char((Pixel >> 11) << 3));
		}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	SaveFile.close();
}

void bitmap::PutPixel(ushort X, ushort Y, ushort Color)
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
	((ushort*)ddsd.lpSurface)[Y * (ddsd.lPitch >> 1) + X] = Color;
	DXSurface->GetDDrawSurface()->Unlock(NULL); 
}

ushort bitmap::GetPixel(ushort X, ushort Y) const
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
	ushort Color = ((ushort*)ddsd.lpSurface)[Y * (ddsd.lPitch >> 1) + X];
	DXSurface->GetDDrawSurface()->Unlock(NULL); 
	return Color;
}

void bitmap::ClearToColor(ushort Color)
{
	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = Color;

	DXSurface->GetDDrawSurface()->Blt( NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx );	
}

void bitmap::ClearToColor(ushort DestX, ushort DestY, ushort SizeX, ushort SizeY, ushort Color)
{
	bitmap Temp(SizeX, SizeY);
	Temp.ClearToColor(Color);
	Temp.Blit(this, 0,0, DestX, DestY, SizeX, SizeY);
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags) const
{
	Flags &= 0x7;

	if(!Flags)
	{
		RECT RDest = { DestX, DestY, DestX + Width, DestY + Height }, RSource = { SourceX, SourceY, SourceX + Width, SourceY + Height };
		Bitmap->CDXSurface()->GetDDrawSurface()->Blt(&RDest, DXSurface->GetDDrawSurface(), &RSource, DDBLT_WAIT, NULL); 
		return;
	}

	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;
	ZeroMemory( &destddsd,sizeof(destddsd) );
	destddsd.dwSize = sizeof(destddsd);
	Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );

	ulong TrueSourceOffset = ulong(srcddsd.lpSurface) + (SourceY * srcddsd.lPitch + (SourceX << 1));
	ulong TrueSourceXMove = srcddsd.lPitch - (Width << 1);

	switch(Flags)
	{
		case MIRROR:
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch + (Width << 1);

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
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
				add esi, TrueSourceXMove
				add edi, TrueDestXMove
				dec Height
				jnz MaskedLoop1
				pop es
				popad
			}

			break;
		}

		case FLIP:
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1));
			ulong TrueDestXMove = destddsd.lPitch + (Width << 1);

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				shr dx, 0x01
				cld
			MaskedLoop3:
				mov cx, dx
				rep movsd
				add esi, TrueSourceXMove
				sub edi, TrueDestXMove
				dec Height
				jnz MaskedLoop3
				pop es
				popad
			}

			break;
		}

		case (MIRROR | FLIP):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch - (Width << 1);

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
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
				add esi, TrueSourceXMove
				sub edi, TrueDestXMove
				dec Height
				jnz MaskedLoop6
				pop es
				popad
			}

			break;
		}

		case ROTATE_90:
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch + 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop4:
				mov cx, dx
			MaskedLoop5:
				lodsw
				mov [edi], ax
				add edi, TrueDestXMove
				dec cx
				jnz MaskedLoop5
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop4
				pop es
				popad
			}

			break;
		}

		case (MIRROR | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1));
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch - 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop8:
				mov cx, dx
			MaskedLoop9:
				lodsw
				mov [edi], ax
				add edi, TrueDestXMove
				dec cx
				jnz MaskedLoop9
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop8
				pop es
				popad
			}

			break;
		}

		case (FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch - 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop10:
				mov cx, dx
			MaskedLoop11:
				lodsw
				mov [edi], ax
				sub edi, TrueDestXMove
				dec cx
				jnz MaskedLoop11
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop10
				pop es
				popad
			}

			break;
		}

		case (MIRROR | FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1));
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch + 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop12:
				mov cx, dx
			MaskedLoop13:
				lodsw
				mov [edi], ax
				sub edi, TrueDestXMove
				dec cx
				jnz MaskedLoop13
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop12
				pop es
				popad
			}
		}
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
	Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
{
	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;
	ZeroMemory( &destddsd,sizeof(destddsd) );
	destddsd.dwSize = sizeof(destddsd);
	Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );

	ulong TrueSourceOffset = ulong(srcddsd.lpSurface) + (SourceY * srcddsd.lPitch + (SourceX << 1));
	ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1));
	ulong TrueSourceXMove = srcddsd.lPitch - (Width << 1);
	ulong TrueDestXMove = destddsd.lPitch - (Width << 1);

	__asm
	{
		pushad
		push es
		mov ax, ds
		mov es, ax
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		xor ecx, ecx
		xor edx, edx
		mov dx, Luminance
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
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
	Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags) const
{
	Flags &= 0x7;

	if(!Flags)
	{
		RECT RDest = { DestX, DestY, DestX + Width, DestY + Height }, RSource = { SourceX, SourceY, SourceX + Width, SourceY + Height };
		Bitmap->CDXSurface()->GetDDrawSurface()->Blt(&RDest, DXSurface->GetDDrawSurface(), &RSource, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	}

	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;
	ZeroMemory( &destddsd,sizeof(destddsd) );
	destddsd.dwSize = sizeof(destddsd);
	Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );

	ulong TrueSourceOffset = ulong(srcddsd.lpSurface) + (SourceY * srcddsd.lPitch + (SourceX << 1));
	ulong TrueSourceXMove = srcddsd.lPitch - (Width << 1);

	switch(Flags)
	{
		case MIRROR:
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch + (Width << 1);

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				cld
			MaskedLoop1:
				mov cx, Width
			MaskedLoop2:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip1
				std
				stosw
				cld
				dec cx
				jnz MaskedLoop2
				jmp MaskedNextLine1
			MaskSkip1:
				sub edi, 0x02
				dec cx
				jnz MaskedLoop2
			MaskedNextLine1:
				add esi, TrueSourceXMove
				add edi, TrueDestXMove
				dec Height
				jnz MaskedLoop1
				pop es
				popad
			}

			break;
		}

		case FLIP:
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1));
			ulong TrueDestXMove = destddsd.lPitch + (Width << 1);

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop3:
				mov cx, dx
			MaskedLoop55:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip2
				stosw
				dec cx
				jnz MaskedLoop55
				jmp MaskedNextLine2
			MaskSkip2:
				add edi, 0x02
				dec cx
				jnz MaskedLoop55
			MaskedNextLine2:
				add esi, TrueSourceXMove
				sub edi, TrueDestXMove
				dec Height
				jnz MaskedLoop3
				pop es
				popad
			}

			break;
		}

		case (MIRROR | FLIP):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch - (Width << 1);

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				cld
			MaskedLoop6:
				mov cx, Width
			MaskedLoop7:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip3
				std
				stosw
				cld
				dec cx
				jnz MaskedLoop7
				jmp MaskedNextLine3
			MaskSkip3:
				sub edi, 0x02
				dec cx
				jnz MaskedLoop7
			MaskedNextLine3:
				add esi, TrueSourceXMove
				sub edi, TrueDestXMove
				dec Height
				jnz MaskedLoop6
				pop es
				popad
			}

			break;
		}

		case ROTATE_90:
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch + 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop4:
				mov cx, dx
			MaskedLoop5:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip4
				mov [edi], ax
				add edi, TrueDestXMove
				dec cx
				jnz MaskedLoop5
				jmp MaskedNextLine4
			MaskSkip4:
				add edi, TrueDestXMove
				dec cx
				jnz MaskedLoop5
			MaskedNextLine4:
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop4
				pop es
				popad
			}

			break;
		}

		case (MIRROR | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1));
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch - 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop8:
				mov cx, dx
			MaskedLoop9:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip5
				mov [edi], ax
				add edi, TrueDestXMove
				dec cx
				jnz MaskedLoop9
				jmp MaskedNextLine5
			MaskSkip5:
				add edi, TrueDestXMove
				dec cx
				jnz MaskedLoop9
			MaskedNextLine5:
				add esi, TrueSourceXMove
				sub edi, TrueDestYMove
				dec Height
				jnz MaskedLoop8
				pop es
				popad
			}

			break;
		}

		case (FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1) + (Width << 1) - 2);
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch - 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop10:
				mov cx, dx
			MaskedLoop11:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip6
				mov [edi], ax
				sub edi, TrueDestXMove
				dec cx
				jnz MaskedLoop11
				jmp MaskedNextLine6
			MaskSkip6:
				sub edi, TrueDestXMove
				dec cx
				jnz MaskedLoop11
			MaskedNextLine6:
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop10
				pop es
				popad
			}

			break;
		}

		case (MIRROR | FLIP | ROTATE_90):
		{
			ulong TrueDestOffset = ulong(destddsd.lpSurface) + ((DestY + Height - 1) * destddsd.lPitch + (DestX << 1));
			ulong TrueDestXMove = destddsd.lPitch;
			ulong TrueDestYMove = Height * destddsd.lPitch + 2;

			__asm
			{
				pushad
				push es
				mov ax, ds
				mov es, ax
				mov esi, TrueSourceOffset
				mov edi, TrueDestOffset
				xor ecx, ecx
				mov dx, Width
				cld
			MaskedLoop12:
				mov cx, dx
			MaskedLoop13:
				lodsw
				cmp ax, 0xF81F
				je MaskSkip7
				mov [edi], ax
				sub edi, TrueDestXMove
				dec cx
				jnz MaskedLoop13
				jmp MaskedNextLine7
			MaskSkip7:
				sub edi, TrueDestXMove
				dec cx
				jnz MaskedLoop13
			MaskedNextLine7:
				add esi, TrueSourceXMove
				add edi, TrueDestYMove
				dec Height
				jnz MaskedLoop12
				pop es
				popad
			}
		}
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
	Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
{
	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;
	ZeroMemory( &destddsd,sizeof(destddsd) );
	destddsd.dwSize = sizeof(destddsd);
	Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );

	ulong TrueSourceOffset = ulong(srcddsd.lpSurface) + (SourceY * srcddsd.lPitch + (SourceX << 1));
	ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1));
	ulong TrueSourceXMove = srcddsd.lPitch - (Width << 1);
	ulong TrueDestXMove = destddsd.lPitch - (Width << 1);

	__asm
	{
		pushad
		push es
		mov ax, ds
		mov es, ax
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		xor ecx, ecx
		xor edx, edx
		mov dx, Luminance
		sub dx, 0x100
		cld
	MaskedLoop1:
		mov cx, Width
	MaskedLoop2:
		lodsw
		cmp ax, 0xF81F
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
		jmp MaskedNextLine
	MaskSkip:
		add edi, 0x02
		dec cx
		jnz MaskedLoop2
	MaskedNextLine:
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		pop es
		popad
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
	Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::BlitToDB(ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height) const
{
	Blit(DOUBLEBUFFER, SourceX, SourceY, DestX, DestY, Width, Height);
}

void bitmap::BlitToDB(ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Bright) const
{
	Blit(DOUBLEBUFFER, SourceX, SourceY, DestX, DestY, Width, Height, Bright);
}

void bitmap::MaskedBlitToDB(ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height) const
{
	MaskedBlit(DOUBLEBUFFER, SourceX, SourceY, DestX, DestY, Width, Height);
}

void bitmap::MaskedBlitToDB(ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
{
	MaskedBlit(DOUBLEBUFFER, SourceX, SourceY, DestX, DestY, Width, Height, Luminance);
}

void bitmap::FastBlit(bitmap* Bitmap) const
{
	Bitmap->CDXSurface()->GetDDrawSurface()->BltFast(0,0, DXSurface->GetDDrawSurface(), NULL, DDBLTFAST_WAIT);
}

void bitmap::FastMaskedBlit(bitmap* Bitmap) const
{
	Bitmap->CDXSurface()->GetDDrawSurface()->BltFast(0,0, DXSurface->GetDDrawSurface(), NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void bitmap::Printf(bitmap* Bitmap, ushort X, ushort Y, const char* Format, ...) const
{
	char Buffer[256];

	va_list AP;
	va_start(AP, Format);
	vsprintf(Buffer, Format, AP);
	va_end(AP);

	for(uchar c = 0; c < strlen(Buffer); c++)
	{
		ushort FX = ((Buffer[c] - 0x20) & 0xF) << 4, FY = (Buffer[c] - 0x20) & 0xF0;

		MaskedBlit(Bitmap, FX, FY, X + (c << 3), Y, 8, 8);
	}
}

void bitmap::ReadFromDB(ushort X, ushort Y)
{
	DOUBLEBUFFER->Blit(this, X, Y, 0, 0, XSize, YSize);
}

void bitmap::WriteToDB(ushort X, ushort Y) const
{
	Blit(DOUBLEBUFFER, 0, 0, X, Y, XSize, YSize);
}

void bitmap::PrintfToDB(ushort X, ushort Y, const char* Format, ...) const
{
	char Buffer[256];

	va_list AP;
	va_start(AP, Format);
	vsprintf(Buffer, Format, AP);
	va_end(AP);

	Printf(DOUBLEBUFFER, X, Y, Buffer);
}

void bitmap::Backup(ushort X, ushort Y, bool DestroySurface)
{
	if(!X) X = XSize;
	if(!Y) Y = YSize;

	BackupBuffer = new ushort[X * Y];

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong SrcBuffer = ulong(ddsd.lpSurface);
	ulong DestBuffer = ulong(BackupBuffer);

	for(ushort y = 0; y < Y; y++)
	{
		memcpy((void*)DestBuffer, (void*)SrcBuffer, X << 1);

		SrcBuffer += ddsd.lPitch;
		DestBuffer += X << 1;
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	if(DestroySurface)
		delete DXSurface;
}

void bitmap::Restore(ushort X, ushort Y, bool CreateSurface)
{
	if(!X) X = XSize;
	if(!Y) Y = YSize;

	if(CreateSurface)
	{
		graphics::GetDXDisplay()->CreateSurface(&DXSurface, XSize, YSize);
		DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
		DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);
	}

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong SrcBuffer = ulong(BackupBuffer);
	ulong DestBuffer = ulong(ddsd.lpSurface);

	for(ushort y = 0; y < Y; y++)
	{
		memcpy((void*)DestBuffer, (void*)SrcBuffer, X << 1);

		SrcBuffer += X << 1;
		DestBuffer += ddsd.lPitch;
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	delete [] BackupBuffer;

	BackupBuffer = 0;
}

void bitmap::AttachSurface(IDirectDrawSurface7* DDSurface)
{
	DXSurface->Create(DDSurface);

	DDSURFACEDESC2 ddsd;

	DDSurface->GetSurfaceDesc(&ddsd);

	XSize = ddsd.dwWidth;
	YSize = ddsd.dwHeight;
}
