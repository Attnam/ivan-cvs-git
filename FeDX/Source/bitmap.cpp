#include <string>
#include <cmath>

#include "bitmap.h"
#include "graphics.h"
#include "ddutil.h"
#include "error.h"
#include "save.h"
#include "allocate.h"

bitmap::bitmap(std::string FileName) : BackupBuffer(0), DoubleBuffer(false), AlphaMap(0)
{
	std::ifstream File(FileName.c_str(), std::ios::in | std::ios::binary);

	if(!File.is_open())
		ABORT("Bitmap %s not found!", FileName.c_str());

	File.seekg(-768, std::ios::end);

	uchar Palette[768];

	for(ulong c = 0; c < 768; ++c)
		Palette[c] = File.get();

	File.seekg(8, std::ios::beg);

	XSize  =  File.get();
	XSize += (File.get() << 8) + 1;
	YSize  =  File.get();
	YSize += (File.get() << 8) + 1;

	File.seekg(128, std::ios::beg);

	graphics::GetDXDisplay()->CreateSurface(&DXSurface, XSize, YSize);

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ushort* Buffer = (ushort*)ddsd.lpSurface;

	for(ushort y = 0; y < YSize; ++y)
	{
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

	DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
	DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);

	BitmapContainerIterator = graphics::BitmapContainer.insert(graphics::BitmapContainer.end(), this);
}

bitmap::bitmap(ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), BackupBuffer(0), DoubleBuffer(false), AlphaMap(0)
{
	HRESULT hr = graphics::GetDXDisplay()->CreateSurface(&DXSurface, XSize, YSize);

	if(hr != S_OK)
		ABORT("Bitmap initialization failed with code 0x%X!", hr);

	DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
	DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);

	BitmapContainerIterator = graphics::BitmapContainer.insert(graphics::BitmapContainer.end(), this);
}

bitmap::bitmap(IDirectDrawSurface7* DDSurface, ushort XSize, ushort YSize) : XSize(XSize), YSize(YSize), BackupBuffer(0), DoubleBuffer(true), AlphaMap(0)
{
	DXSurface = new CSurface;

	DXSurface->Create(DDSurface);

	DDSURFACEDESC2 ddsd;

	DDSurface->GetSurfaceDesc(&ddsd);

	DDCOLORKEY ColorKey = { 0xF81F, 0xF81F }; // purple
	DXSurface->GetDDrawSurface()->SetColorKey(DDCKEY_SRCBLT, &ColorKey);

	BitmapContainerIterator = graphics::BitmapContainer.insert(graphics::BitmapContainer.end(), this);
}

bitmap::~bitmap()
{
	delete DXSurface;
	delete [] BackupBuffer;
	delete [] AlphaMap;

	graphics::BitmapContainer.erase(BitmapContainerIterator);
}

void bitmap::Save(outputfile& SaveFile) const
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong Buffer = ulong(ddsd.lpSurface);

	for(ushort y = 0; y < YSize; ++y, Buffer += ddsd.lPitch)
		SaveFile.GetBuffer().write((char*)Buffer, XSize << 1);

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
}

void bitmap::Load(inputfile& SaveFile)
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong Buffer = ulong(ddsd.lpSurface);

	for(ushort y = 0; y < YSize; ++y, Buffer += ddsd.lPitch)
		SaveFile.GetBuffer().read((char*)Buffer, XSize << 1);

	DXSurface->GetDDrawSurface()->Unlock(NULL); 
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

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	BMPHeader[0x12] =  XSize       & 0xFF;
	BMPHeader[0x13] = (XSize >> 8) & 0xFF;
	BMPHeader[0x16] =  YSize       & 0xFF;
	BMPHeader[0x17] = (YSize >> 8) & 0xFF;

	SaveFile.GetBuffer().write(BMPHeader, 0x36);

	ulong Off = ulong(ddsd.lpSurface) + (YSize - 1) * ddsd.lPitch;

	for(ushort y = 0; y < YSize; ++y, Off -= (ddsd.lPitch << 1))
		for(ushort x = 0; x < XSize; ++x, Off += 2)
		{
			ushort Pixel = *(ushort*)Off;

			SaveFile << char(Pixel << 3) << char((Pixel >> 5) << 2) << char((Pixel >> 11) << 3);
		}

	DXSurface->GetDDrawSurface()->Unlock(NULL);
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

void bitmap::Lock()
{
	TempDDSD = new DDSURFACEDESC2;
	ZeroMemory( TempDDSD,sizeof(*TempDDSD) );
	TempDDSD->dwSize = sizeof(*TempDDSD);
	DXSurface->GetDDrawSurface()->Lock( NULL, TempDDSD, DDLOCK_WAIT, NULL );
}

void bitmap::Release()
{
	DXSurface->GetDDrawSurface()->Unlock(NULL); 
	delete TempDDSD;
}

void bitmap::FastPutPixel(ushort X, ushort Y, ushort Color)
{
	((ushort*)TempDDSD->lpSurface)[Y * (TempDDSD->lPitch >> 1) + X] = Color;
}

ushort bitmap::FastGetPixel(ushort X, ushort Y) const
{
	return ((ushort*)TempDDSD->lpSurface)[Y * (TempDDSD->lPitch >> 1) + X];
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
		Bitmap->GetDXSurface()->GetDDrawSurface()->Blt(&RDest, DXSurface->GetDDrawSurface(), &RSource, DDBLT_WAIT, NULL); 
		return;
	}

	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;

	if(Bitmap != this)
	{
		ZeroMemory( &destddsd,sizeof(destddsd) );
		destddsd.dwSize = sizeof(destddsd);
		Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );
	}
	else
		destddsd = srcddsd;

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

	if(Bitmap != this)
		Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::Blit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
{
	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;

	if(Bitmap != this)
	{
		ZeroMemory( &destddsd,sizeof(destddsd) );
		destddsd.dwSize = sizeof(destddsd);
		Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );
	}
	else
		destddsd = srcddsd;

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

	if(Bitmap != this)
		Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Flags) const
{
	Flags &= 0x7;

	if(!Flags)
	{
		RECT RDest = { DestX, DestY, DestX + Width, DestY + Height }, RSource = { SourceX, SourceY, SourceX + Width, SourceY + Height };
		Bitmap->GetDXSurface()->GetDDrawSurface()->Blt(&RDest, DXSurface->GetDDrawSurface(), &RSource, DDBLT_WAIT | DDBLT_KEYSRC, NULL);
	}

	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;

	if(Bitmap != this)
	{
		ZeroMemory( &destddsd,sizeof(destddsd) );
		destddsd.dwSize = sizeof(destddsd);
		Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );
	}
	else
		destddsd = srcddsd;

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

	if(Bitmap != this)
		Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::MaskedBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, ushort Luminance) const
{
	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;

	if(Bitmap != this)
	{
		ZeroMemory( &destddsd,sizeof(destddsd) );
		destddsd.dwSize = sizeof(destddsd);
		Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );
	}
	else
		destddsd = srcddsd;

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

	if(Bitmap != this)
		Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort SourceX, ushort SourceY, ushort DestX, ushort DestY, ushort Width, ushort Height, uchar Alpha) const
{
	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;

	if(Bitmap != this)
	{
		ZeroMemory( &destddsd,sizeof(destddsd) );
		destddsd.dwSize = sizeof(destddsd);
		Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );
	}
	else
		destddsd = srcddsd;

	ulong TrueSourceOffset = ulong(srcddsd.lpSurface) + (SourceY * srcddsd.lPitch + (SourceX << 1));
	ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1));
	ulong TrueSourceXMove = srcddsd.lPitch - (Width << 1);
	ulong TrueDestXMove = destddsd.lPitch - (Width << 1);

	ushort SColor, DColor;

	__asm
	{
		pushad
		push es
		mov ax, ds
		mov es, ax
		mov esi, TrueSourceOffset
		mov edi, TrueDestOffset
		mov dl, Alpha
		cld
	MaskedLoop1:
		mov cx, Width
	MaskedLoop2:
		lodsw
		cmp ax, 0xF81F
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
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		pop es
		popad
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	if(Bitmap != this)
		Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::AlphaBlit(bitmap* Bitmap, ushort DestX, ushort DestY) const
{
	if(!AlphaMap)
		ABORT("AlphaMap not available!");

	DDSURFACEDESC2 srcddsd;
	ZeroMemory( &srcddsd,sizeof(srcddsd) );
	srcddsd.dwSize = sizeof(srcddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &srcddsd, DDLOCK_WAIT, NULL );

	DDSURFACEDESC2 destddsd;

	if(Bitmap != this)
	{
		ZeroMemory( &destddsd,sizeof(destddsd) );
		destddsd.dwSize = sizeof(destddsd);
		Bitmap->DXSurface->GetDDrawSurface()->Lock( NULL, &destddsd, DDLOCK_WAIT, NULL );
	}
	else
		destddsd = srcddsd;

	ulong TrueSourceOffset = ulong(srcddsd.lpSurface);
	ulong TrueDestOffset = ulong(destddsd.lpSurface) + (DestY * destddsd.lPitch + (DestX << 1));
	ulong TrueSourceXMove = srcddsd.lPitch - (XSize << 1);
	ulong TrueDestXMove = destddsd.lPitch - (XSize << 1);
	ulong AlphaMapOffset = ulong(AlphaMap[0]);

	ushort SColor, DColor, Width = XSize, Height = YSize;

	__asm
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
		cmp ax, 0xF81F
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
		add esi, TrueSourceXMove
		add edi, TrueDestXMove
		dec Height
		jnz MaskedLoop1
		pop ebx
		pop es
		popad
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	if(Bitmap != this)
		Bitmap->DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::FastBlit(bitmap* Bitmap) const
{
	Bitmap->GetDXSurface()->GetDDrawSurface()->BltFast(0,0, DXSurface->GetDDrawSurface(), NULL, DDBLTFAST_WAIT);
}

void bitmap::FastMaskedBlit(bitmap* Bitmap) const
{
	Bitmap->GetDXSurface()->GetDDrawSurface()->BltFast(0,0, DXSurface->GetDDrawSurface(), NULL, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
}

void bitmap::DrawLine(ushort OrigFromX, ushort OrigFromY, ushort OrigToX, ushort OrigToY, ushort Color, bool Wide)
{
	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ushort ThisXSize = XSize, ThisYSize = YSize, DistaX, DistaY, BTemp;

	ulong Pitch = ddsd.lPitch, Surface = ulong(ddsd.lpSurface);

	static vector2d Point[] = {vector2d(0, 0), vector2d(0, -1), vector2d(-1, 0), vector2d(1, 0), vector2d(0, 1)};

	for(uchar c = 0; c < (Wide ? 5 : 1); ++c)
	{
		ushort FromX = OrigFromX + Point[c].X;
		ushort FromY = OrigFromY + Point[c].Y;
		ushort ToX = OrigToX + Point[c].X;
		ushort ToY = OrigToY + Point[c].Y;

		__asm
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
			cmp	ax,	dx
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
		}
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);
}

void bitmap::Backup()
{
	BackupBuffer = new ushort[XSize * YSize];

	DDSURFACEDESC2 ddsd;
	ZeroMemory( &ddsd,sizeof(ddsd) );
	ddsd.dwSize = sizeof(ddsd);
	DXSurface->GetDDrawSurface()->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );

	ulong SrcBuffer = ulong(ddsd.lpSurface);
	ulong DestBuffer = ulong(BackupBuffer);

	for(ushort y = 0; y < YSize; ++y)
	{
		memcpy((void*)DestBuffer, (void*)SrcBuffer, XSize << 1);

		SrcBuffer += ddsd.lPitch;
		DestBuffer += XSize << 1;
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	if(!DoubleBuffer)
		delete DXSurface;
}

void bitmap::Restore()
{
	if(!DoubleBuffer)
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

	for(ushort y = 0; y < YSize; ++y)
	{
		memcpy((void*)DestBuffer, (void*)SrcBuffer, XSize << 1);

		SrcBuffer += XSize << 1;
		DestBuffer += ddsd.lPitch;
	}

	DXSurface->GetDDrawSurface()->Unlock(NULL);

	delete [] BackupBuffer;

	BackupBuffer = 0;
}

void bitmap::AttachSurface(IDirectDrawSurface7* DDSurface, ushort NewXSize, ushort NewYSize)
{
	DXSurface->Create(DDSurface);

	DDSURFACEDESC2 ddsd;

	DDSurface->GetSurfaceDesc(&ddsd);

	XSize = NewXSize;
	YSize = NewYSize;
}

void bitmap::DrawPolygon(vector2d Center, ushort Radius, ushort NumberOfSides, ushort Color, bool DrawDiameters, double Rotation)
{
	std::vector<vector2d> Points;
	
	for(ushort c = 0; c < NumberOfSides; ++c)
	{
		double PosX =  sin((2 * 3.1415926535 / NumberOfSides) * c + Rotation) * Radius, PosY = cos((2 * 3.1415926535 / NumberOfSides) * c + Rotation) * Radius;
		Points.push_back(vector2d(PosX, PosY) + Center);
	}

	if(DrawDiameters)
		for(c = 0; c < Points.size(); ++c)
			for(ushort a = 0; a < Points.size(); ++a)
			{
				if(abs(int(c) - a) > 1 && !((a == 0) && c == Points.size() - 1) && !((c == 0) && a == Points.size() - 1)) DrawLine(Points[c].X, Points[c].Y, Points[a].X, Points[a].Y, Color, true);
			}
	else
		for(c = 0; c < NumberOfSides; ++c)
			DrawLine(Points[c].X, Points[c].Y, Points[(c + 1) % Points.size()].X, Points[(c + 1) % Points.size()].Y, Color, true);
}

void bitmap::CreateAlphaMap(uchar InitialValue)
{
	if(AlphaMap)
		ABORT("Alpha leak detected!");

	Alloc2D<uchar>(AlphaMap, XSize, YSize, InitialValue);
}
