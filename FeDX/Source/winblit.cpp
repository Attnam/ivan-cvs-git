#include "typedef.h"

/* This file has been converted to Emacs style. That's why it looks so horrible. */

void Fill(ulong TrueDestOffset, ulong TrueDestXMove, ushort Width, ushort Height, ushort Color)
{
  __asm
    {
      pushad
      push es
      mov ax, ds
      mov edi, TrueDestOffset
      mov es, ax
      xor ecx, ecx
      mov dx, Width
      mov bx, Height
      mov ax, Color
      cld
    MaskedLoop38:
      mov cx, dx
      rep stosw
      add edi, TrueDestXMove
      dec bx
      jnz MaskedLoop38
      pop es
      popad
    }
}

void BlitToDB(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueDestXMove, ushort Width, ushort Height)
{
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
	cld
	MaskedLoop3:
      mov cx, dx
	rep movsw
	add edi, TrueDestXMove
	dec bx
	jnz MaskedLoop3
	pop es
	popad
	}
}

void BlitNoFlags(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height)
{
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
	cld
	MaskedLoop39:
      mov cx, dx
	rep movsw
	add esi, TrueSourceXMove
	add edi, TrueDestXMove
	dec bx
	jnz MaskedLoop39
	pop es
	popad
	}
}

void BlitMirror(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height)
{
  __asm
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
	BlitMirror1:
      mov cx, Width
	BlitMirror2:
      lodsw
	std
	stosw
	cld
	dec cx
	jnz BlitMirror2
	add esi, ebx
	add edi, edx
	dec Height
	jnz BlitMirror1
	pop es
	popad
	}
}

void BlitFlip(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height)
{
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
	cld
	BlitFlip3:
      mov cx, dx
	rep movsw
	add esi, TrueSourceXMove
	sub edi, TrueDestXMove
	dec bx
	jnz BlitFlip3
	pop es
	popad
	}
}

void BlitMirrorFlip(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height)
{
  __asm
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
	BlitMirrorFlip6:
      mov cx, Width
	BlitMirrorFlip7:
      lodsw
	std
	stosw
	cld
	dec cx
	jnz BlitMirrorFlip7
	add esi, ebx
	sub edi, edx
	dec Height
	jnz BlitMirrorFlip6
	pop es
	popad
	}
}

void BlitRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height)
{
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
	mov ebx, TrueDestXMove
	cld
	BlitRotate904:
      mov cx, dx
	BlitRotate905:
      lodsw
	mov [edi], ax
	add edi, ebx
	dec cx
	jnz BlitRotate905
	add esi, TrueSourceXMove
	sub edi, TrueDestYMove
	dec Height
	jnz BlitRotate904
	pop es
	popad
	}
}

void BlitMirrorRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height)
{
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
	mov ebx, TrueDestXMove
	cld
	BlitMirrorRotate908:
      mov cx, dx
	BlitMirrorRotate909:
      lodsw
	mov [edi], ax
	add edi, ebx
	dec cx
	jnz BlitMirrorRotate909
	add esi, TrueSourceXMove
	sub edi, TrueDestYMove
	dec Height
	jnz BlitMirrorRotate908
	pop es
	popad
	}
}

void BlitFlipRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height)
{
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
	mov ebx, TrueDestXMove
	cld
	BlitFlipRotate9010:
      mov cx, dx
	BlitFlipRotate9011:
      lodsw
	mov [edi], ax
	sub edi, ebx
	dec cx
	jnz BlitFlipRotate9011
	add esi, TrueSourceXMove
	add edi, TrueDestYMove
	dec Height
	jnz BlitFlipRotate9010
	pop es
	popad
	}
}

void BlitMirrorFlipRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height)
{
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
	mov ebx, TrueDestXMove
	cld
	BlitMirrorFlipRotate9012:
      mov cx, dx
	BlitMirrorFlipRotate9013:
      lodsw
	mov [edi], ax
	sub edi, ebx
	dec cx
	jnz BlitMirrorFlipRotate9013
	add esi, TrueSourceXMove
	add edi, TrueDestYMove
	dec Height
	jnz BlitMirrorFlipRotate9012
	pop es
	popad
	}
}

void BlitLuminated(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, ushort Luminance)
{
  __asm
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
	BlitLuminated1:
      mov cx, Width
	BlitLuminated2:
      lodsw
	mov bx, ax
	and bx, 0x1F
	shl bx, 0x03
	add bx, dx
	jns BlitLuminatedNext1
	xor bx, bx
	jmp BlitLuminatedNext2
	BlitLuminatedNext1:
      cmp bx, 0x100
	jb BlitLuminatedNext2
	mov bx, 0xFF
	BlitLuminatedNext2:
      shr bx, 0x03
	and ax, 0xFFE0
	or ax, bx
	ror ax, 0x05
	mov bx, ax
	and bx, 0x3F
	shl bx, 0x02
	add bx, dx
	jns BlitLuminatedNext3
	xor bx, bx
	jmp BlitLuminatedNext4
	BlitLuminatedNext3:
      cmp bx, 0x100
	jb BlitLuminatedNext4
	mov bx, 0xFF
	BlitLuminatedNext4:
      shr bx, 0x02
	and ax, 0xFFC0
	or ax, bx
	ror ax, 0x06
	mov bx, ax
	and bx, 0x1F
	shl bx, 0x03
	add bx, dx
	jns BlitLuminatedNext5
	xor bx, bx
	jmp BlitLuminatedNext6
	BlitLuminatedNext5:
      cmp bx, 0x100
	jb BlitLuminatedNext6
	mov bx, 0xFF
	BlitLuminatedNext6:
      shr bx, 0x03
	and ax, 0xFFE0
	or ax, bx
	ror ax, 0x05
	stosw
	dec cx
	jnz BlitLuminated2
	add esi, TrueSourceXMove
	add edi, TrueDestXMove
	dec Height
	jnz BlitLuminated1
	pop es
	popad
	}
}

void MaskedBlitNoFlags(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, ushort MaskColor)
{
  __asm
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
	add edi, edx
	dec Height
	jnz MaskedLoop37
	jmp MaskedNextLine27
	MaskSkip27:
      add edi, 0x02
	dec cx
	jnz MaskedLoop557
	add esi, ebx
	add edi, edx
	dec Height
	jnz MaskedLoop37
	MaskedNextLine27:
      pop es
	popad
	}
}

void MaskedBlitMirror(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, ushort MaskColor)
{
  __asm
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
	MaskedBlitMirror1:
      mov cx, Width
	MaskedBlitMirror2:
      lodsw
	cmp ax, MaskColor
	je MaskSkip1
	std
	stosw
	cld
	dec cx
	jnz MaskedBlitMirror2
	add esi, ebx
	add edi, edx
	dec Height
	jnz MaskedBlitMirror1
	jmp MaskedNextLine1
	MaskSkip1:
      sub edi, 0x02
	dec cx
	jnz MaskedBlitMirror2
	add esi, ebx
	add edi, edx
	dec Height
	jnz MaskedBlitMirror1
	MaskedNextLine1:
      pop es
	popad
	}
}

void MaskedBlitFlip(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, ushort MaskColor)
{
  __asm
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
	MaskedBlitFlip3:
      mov cx, Width
	MaskedBlitFlip55:
      lodsw
	cmp ax, MaskColor
	je MaskSkip2
	stosw
	dec cx
	jnz MaskedBlitFlip55
	add esi, ebx
	sub edi, edx
	dec Height
	jnz MaskedBlitFlip3
	jmp MaskedNextLine2
	MaskSkip2:
      add edi, 0x02
	dec cx
	jnz MaskedBlitFlip55
	add esi, ebx
	sub edi, edx
	dec Height
	jnz MaskedBlitFlip3
	MaskedNextLine2:
      pop es
	popad
	}
}

void MaskedBlitMirrorFlip(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, ushort MaskColor)
{
  __asm
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
	MaskedBlitMirrorFlip6:
      mov cx, Width
	MaskedBlitMirrorFlip7:
      lodsw
	cmp ax, MaskColor
	je MaskSkip3
	std
	stosw
	cld
	dec cx
	jnz MaskedBlitMirrorFlip7
	add esi, ebx
	sub edi, edx
	dec Height
	jnz MaskedBlitMirrorFlip6
	jmp MaskedNextLine3
	MaskSkip3:
      sub edi, 0x02
	dec cx
	jnz MaskedBlitMirrorFlip7
	add esi, ebx
	sub edi, edx
	dec Height
	jnz MaskedBlitMirrorFlip6
	MaskedNextLine3:
      pop es
	popad
	}
}

void MaskedBlitRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height, ushort MaskColor)
{
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
	mov ebx, TrueDestXMove
	cld
	MaskedBlitRotate904:
      mov cx, dx
	MaskedBlitRotate905:
      lodsw
	cmp ax, MaskColor
	je MaskSkip4
	mov [edi], ax
	MaskSkip4:
      add edi, ebx
	dec cx
	jnz MaskedBlitRotate905
	add esi, TrueSourceXMove
	sub edi, TrueDestYMove
	dec Height
	jnz MaskedBlitRotate904
	pop es
	popad
	}
}

void MaskedBlitMirrorRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height, ushort MaskColor)
{
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
	mov ebx, TrueDestXMove
	cld
	MaskedBlitMirrorRotate908:
      mov cx, dx
	MaskedBlitMirrorRotate909:
      lodsw
	cmp ax, MaskColor
	je MaskSkip5
	mov [edi], ax
	MaskSkip5:
      add edi, ebx
	dec cx
	jnz MaskedBlitMirrorRotate909
	add esi, TrueSourceXMove
	sub edi, TrueDestYMove
	dec Height
	jnz MaskedBlitMirrorRotate908
	pop es
	popad
	}
}

void MaskedBlitFlipRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height, ushort MaskColor)
{
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
	mov ebx, TrueDestXMove
	cld
	MaskedBlitFlipRotate9010:
      mov cx, dx
	MaskedBlitFlipRotate9011:
      lodsw
	cmp ax, MaskColor
	je MaskSkip6
	mov [edi], ax
	MaskSkip6:
      sub edi, ebx
	dec cx
	jnz MaskedBlitFlipRotate9011
	add esi, TrueSourceXMove
	add edi, TrueDestYMove
	dec Height
	jnz MaskedBlitFlipRotate9010
	pop es
	popad
	}
}

void MaskedBlitMirrorFlipRotate90(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ulong TrueDestYMove, ushort Width, ushort Height, ushort MaskColor)
{
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
	mov ebx, TrueDestXMove
	cld
	MaskedBlitMirrorFlipRotate9012:
      mov cx, dx
	MaskedBlitMirrorFlipRotate9013:
      lodsw
	cmp ax, MaskColor
	je MaskSkip7
	mov [edi], ax
	MaskSkip7:
      sub edi, ebx
	dec cx
	jnz MaskedBlitMirrorFlipRotate9013
	add esi, TrueSourceXMove
	add edi, TrueDestYMove
	dec Height
	jnz MaskedBlitMirrorFlipRotate9012
	pop es
	popad
	}
}

void MaskedBlitLuminated(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, ushort Luminance, ushort MaskColor)
{
  __asm
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
	MaskedBlitLuminated1:
      mov cx, Width
	MaskedBlitLuminated2:
      lodsw
	cmp ax, MaskColor
	je MaskSkipL1
	mov bx, ax
	and bx, 0x1F
	shl bx, 0x03
	add bx, dx
	jns MaskedBlitLuminatedNext1
	xor bx, bx
	jmp MaskedBlitLuminatedNext2
	MaskedBlitLuminatedNext1:
      cmp bx, 0x100
	jb MaskedBlitLuminatedNext2
	mov bx, 0xFF
	MaskedBlitLuminatedNext2:
      shr bx, 0x03
	and ax, 0xFFE0
	or ax, bx
	ror ax, 0x05
	mov bx, ax
	and bx, 0x3F
	shl bx, 0x02
	add bx, dx
	jns MaskedBlitLuminatedNext3
	xor bx, bx
	jmp MaskedBlitLuminatedNext4
	MaskedBlitLuminatedNext3:
      cmp bx, 0x100
	jb MaskedBlitLuminatedNext4
	mov bx, 0xFF
	MaskedBlitLuminatedNext4:
      shr bx, 0x02
	and ax, 0xFFC0
	or ax, bx
	ror ax, 0x06
	mov bx, ax
	and bx, 0x1F
	shl bx, 0x03
	add bx, dx
	jns MaskedBlitLuminatedNext5
	xor bx, bx
	jmp MaskedBlitLuminatedNext6
	MaskedBlitLuminatedNext5:
      cmp bx, 0x100
	jb MaskedBlitLuminatedNext6
	mov bx, 0xFF
	MaskedBlitLuminatedNext6:
      shr bx, 0x03
	and ax, 0xFFE0
	or ax, bx
	ror ax, 0x05
	stosw
	dec cx
	jnz MaskedBlitLuminated2
	add esi, TrueSourceXMove
	add edi, TrueDestXMove
	dec Height
	jnz MaskedBlitLuminated1
	jmp MaskedNextLineL1
	MaskSkipL1:
      add edi, 0x02
	dec cx
	jnz MaskedBlitLuminated2
	add esi, TrueSourceXMove
	add edi, TrueDestXMove
	dec Height
	jnz MaskedBlitLuminated1
	MaskedNextLineL1:
      pop es
	popad
	}
}

void AlphaBlit(ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueSourceXMove, ulong TrueDestXMove, ushort Width, ushort Height, uchar Alpha, ushort MaskColor)
{
  ushort DColor, SColor;

  __asm
    {
      pushad
	push es
	mov ax, ds
	mov esi, TrueSourceOffset
	mov edi, TrueDestOffset
	mov es, ax
	mov dl, Alpha
	cld
	AlphaBlit11:
      mov cx, Width
	AlphaBlit12:
      lodsw
	cmp ax, MaskColor
	je MaskSkipA1
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
	jnz AlphaBlit12
	add esi, TrueSourceXMove
	add edi, TrueDestXMove
	dec Height
	jnz AlphaBlit11
	jmp MaskedNextLineA1
	MaskSkipA1:
      add edi, 0x02
	dec cx
	jnz AlphaBlit12
	add esi, TrueSourceXMove
	add edi, TrueDestXMove
	dec Height
	jnz AlphaBlit11
	MaskedNextLineA1:
      pop es
	popad
	}
}

void AlphaBlit(ulong AlphaMapOffset, ulong TrueSourceOffset, ulong TrueDestOffset, ulong TrueDestXMove, ushort Width, ushort Height, ushort MaskColor)
{
  ushort DColor, SColor;

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
	AlphaBlit21:
      mov cx, Width
	AlphaBlit22:
      lodsw
	pop ebx
	mov dl, [ebx]
	inc ebx
	push ebx
	cmp ax, MaskColor
	je MaskSkipA2
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
	jnz AlphaBlit22
	jmp MaskedNextLineA2
	MaskSkipA2:
      add edi, 0x02
	dec cx
	jnz AlphaBlit22
	MaskedNextLineA2:
      add edi, TrueDestXMove
	dec Height
	jnz AlphaBlit21
	pop ebx
	pop es
	popad
	}
}

void DrawLine(ulong Surface, ulong Pitch, ushort FromX, ushort FromY, ushort ToX, ushort ToY, ushort ThisXSize, ushort ThisYSize, ushort Color)
{
  ushort DistaX, DistaY, BTemp;

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
	mov	DistaX, dx
	mov	DistaY, cx
	mov	bx,	FromX
	mov	ax,	FromY
	xor	cl,	cl
	jmp	LineXYNoSwap
	LineXYSwap:
      mov	DistaY, dx
	mov	DistaX, cx
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
	mov	BTemp, dx
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
	cmp     bx,     ToX
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
