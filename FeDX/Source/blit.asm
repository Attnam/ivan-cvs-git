BITS 32

GLOBAL _BlitMirror__FUlUlUlUlUsUs
GLOBAL _BlitFlip__FUlUlUlUlUsUs
GLOBAL _BlitMirrorFlip__FUlUlUlUlUsUs
GLOBAL _BlitRotate90__FUlUlUlUlUlUsUs
GLOBAL _BlitMirrorRotate90__FUlUlUlUlUlUsUs
GLOBAL _BlitFlipRotate90__FUlUlUlUlUlUsUs
GLOBAL _BlitMirrorFlipRotate90__FUlUlUlUlUlUsUs
GLOBAL _BlitLuminated__FUlUlUlUlUlUsUs
GLOBAL _MaskedBlitMirror__FUlUlUlUlUsUs
GLOBAL _MaskedBlitFlip__FUlUlUlUlUsUs
GLOBAL _MaskedBlitMirrorFlip__FUlUlUlUlUsUs
GLOBAL _MaskedBlitRotate90__FUlUlUlUlUlUsUs
GLOBAL _MaskedBlitMirrorRotate90__FUlUlUlUlUlUsUs
GLOBAL _MaskedBlitFlipRotate90__FUlUlUlUlUlUsUs
GLOBAL _MaskedBlitMirrorFlipRotate90__FUlUlUlUlUlUsUs
GLOBAL _MaskedBlitLuminated__FUlUlUlUlUlUsUs
GLOBAL _AlphaBlit__FUlUlUlUlUsUsUc
GLOBAL _AlphaBlit__FUlUlUlUlUlUsUs
GLOBAL _DrawLine__FUlUlUsUsUsUsUsUsUs

SECTION .text

;-------------------------------------

_BlitMirror__FUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov ebx, [TrueSourceXMove]
	mov edx, [TrueDestXMove]
	cld
BlitMirror1:
	mov cx, [Width]
BlitMirror2:
	lodsw
	std
	stosw
	cld
	dec cx
	jnz BlitMirror2
	add esi, ebx
	add edi, edx
	dec word [Height]
	jnz BlitMirror1
	pop es
	popad
	ret

;-------------------------------------

_BlitFlip__FUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov bx, [Height]
	shr dx, 0x01
	cld
BlitFlip3:
	mov cx, dx
	rep movsd
	add esi, [TrueSourceXMove]
	sub edi, [TrueDestXMove]
	dec bx
	jnz BlitFlip3
	pop es
	popad
	ret

;-------------------------------------

_BlitMirrorFlip__FUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov ebx, [TrueSourceXMove]
	mov edx, [TrueDestXMove]
	cld
BlitMirrorFlip6:
	mov cx, [Width]
BlitMirrorFlip7:
	lodsw
	std
	stosw
	cld
	dec cx
	jnz BlitMirrorFlip7
	add esi, ebx
	sub edi, edx
	dec word [Height]
	jnz BlitMirrorFlip6
	pop es
	popad
	ret

;-------------------------------------

_BlitRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
BlitRotate904:
	mov cx, dx
BlitRotate905:
	lodsw
	mov [edi], ax
	add edi, ebx
	dec cx
	jnz BlitRotate905
	add esi, [TrueSourceXMove]
	sub edi, [TrueDestYMove]
	dec word [Height]
	jnz BlitRotate904
	pop es
	popad
	ret

;-------------------------------------

_BlitMirrorRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
BlitMirrorRotate908:
	mov cx, dx
BlitMirrorRotate909:
	lodsw
	mov [edi], ax
	add edi, ebx
	dec cx
	jnz BlitMirrorRotate909
	add esi, [TrueSourceXMove]
	sub edi, [TrueDestYMove]
	dec word [Height]
	jnz BlitMirrorRotate908
	pop es
	popad
	ret

;-------------------------------------

_BlitFlipRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
BlitFlipRotate9010:
	mov cx, dx
BlitFlipRotate9011:
	lodsw
	mov [edi], ax
	sub edi, ebx
	dec cx
	jnz BlitFlipRotate9011
	add esi, [TrueSourceXMove]
	add edi, [TrueDestYMove]
	dec word [Height]
	jnz BlitFlipRotate9010
	pop es
	popad
	ret

;-------------------------------------

_BlitMirrorFlipRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
BlitMirrorFlipRotate9012:
	mov cx, dx
BlitMirrorFlipRotate9013:
	lodsw
	mov [edi], ax
	sub edi, ebx
	dec cx
	jnz BlitMirrorFlipRotate9013
	add esi, [TrueSourceXMove]
	add edi, [TrueDestYMove]
	dec word [Height]
	jnz BlitMirrorFlipRotate9012
	pop es
	popad
	ret

;-------------------------------------

_BlitLuminated__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Luminance], ax
	mov ax, word [ebp+12]
	mov [Height], ax
	mov ax, word [ebp+16]
	mov [Width], ax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	xor edx, edx
	mov es, ax
	mov dx, [Luminance]
	xor ecx, ecx
	sub dx, 0x100
	cld
BlitLuminated1:
	mov cx, [Width]
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
	jnz near BlitLuminated2
	add esi, [TrueSourceXMove]
	add edi, [TrueDestXMove]
	dec word [Height]
	jnz near BlitLuminated1
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitMirror__FUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov ebx, [TrueSourceXMove]
	mov edx, [TrueDestXMove]
	cld
MaskedBlitMirror1:
	mov cx, [Width]
MaskedBlitMirror2:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip1
	std
	stosw
	cld
	dec cx
	jnz MaskedBlitMirror2
	add esi, ebx
	add edi, edx
	dec word [Height]
	jnz MaskedBlitMirror1
	jmp MaskedNextLine1
MaskSkip1:
	sub edi, 0x02
	dec cx
	jnz MaskedBlitMirror2
	add esi, ebx
	add edi, edx
	dec word [Height]
	jnz MaskedBlitMirror1
MaskedNextLine1:
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitFlip__FUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov ebx, [TrueSourceXMove]
	mov edx, [TrueDestXMove]
	cld
MaskedBlitFlip3:
	mov cx, [Width]
MaskedBlitFlip55:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip2
	stosw
	dec cx
	jnz MaskedBlitFlip55
	add esi, ebx
	sub edi, edx
	dec word [Height]
	jnz MaskedBlitFlip3
	jmp MaskedNextLine2
MaskSkip2:
	add edi, 0x02
	dec cx
	jnz MaskedBlitFlip55
	add esi, ebx
	sub edi, edx
	dec word [Height]
	jnz MaskedBlitFlip3
MaskedNextLine2:
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitMirrorFlip__FUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov ebx, [TrueSourceXMove]
	mov edx, [TrueDestXMove]
	cld
MaskedBlitMirrorFlip6:
	mov cx, [Width]
MaskedBlitMirrorFlip7:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip3
	std
	stosw
	cld
	dec cx
	jnz MaskedBlitMirrorFlip7
	add esi, ebx
	sub edi, edx
	dec word [Height]
	jnz MaskedBlitMirrorFlip6
	jmp MaskedNextLine3
MaskSkip3:
	sub edi, 0x02
	dec cx
	jnz MaskedBlitMirrorFlip7
	add esi, ebx
	sub edi, edx
	dec word [Height]
	jnz MaskedBlitMirrorFlip6
MaskedNextLine3:
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
MaskedBlitRotate904:
	mov cx, dx
MaskedBlitRotate905:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip4
	mov [edi], ax
MaskSkip4:
	add edi, ebx
	dec cx
	jnz MaskedBlitRotate905
	add esi, [TrueSourceXMove]
	sub edi, [TrueDestYMove]
	dec word [Height]
	jnz MaskedBlitRotate904
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitMirrorRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
MaskedBlitMirrorRotate908:
	mov cx, dx
MaskedBlitMirrorRotate909:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip5
	mov [edi], ax
MaskSkip5:
	add edi, ebx
	dec cx
	jnz MaskedBlitMirrorRotate909
	add esi, [TrueSourceXMove]
	sub edi, [TrueDestYMove]
	dec word [Height]
	jnz MaskedBlitMirrorRotate908
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitFlipRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
MaskedBlitFlipRotate9010:
	mov cx, dx
MaskedBlitFlipRotate9011:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip6
	mov [edi], ax
MaskSkip6:
	sub edi, ebx
	dec cx
	jnz MaskedBlitFlipRotate9011
	add esi, [TrueSourceXMove]
	add edi, [TrueDestYMove]
	dec word [Height]
	jnz MaskedBlitFlipRotate9010
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitMirrorFlipRotate90__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestYMove], eax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	xor ecx, ecx
	mov dx, [Width]
	mov ebx, [TrueDestXMove]
	cld
MaskedBlitMirrorFlipRotate9012:
	mov cx, dx
MaskedBlitMirrorFlipRotate9013:
	lodsw
	cmp ax, 0xF81F
	je MaskSkip7
	mov [edi], ax
MaskSkip7:
	sub edi, ebx
	dec cx
	jnz MaskedBlitMirrorFlipRotate9013
	add esi, [TrueSourceXMove]
	add edi, [TrueDestYMove]
        dec word [Height]
	jnz MaskedBlitMirrorFlipRotate9012
	pop es
	popad
	ret

;-------------------------------------

_MaskedBlitLuminated__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Luminance], ax
	mov ax, word [ebp+12]
	mov [Height], ax
	mov ax, word [ebp+16]
	mov [Width], ax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	xor edx, edx
	mov es, ax
	mov dx, [Luminance]
	xor ecx, ecx
	sub dx, 0x100
	cld
MaskedBlitLuminated1:
	mov cx, [Width]
MaskedBlitLuminated2:
	lodsw
	cmp ax, 0xF81F
	je near MaskSkipL1
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
	jnz near MaskedBlitLuminated2
	add esi, [TrueSourceXMove]
	add edi, [TrueDestXMove]
	dec word [Height]
	jnz near MaskedBlitLuminated1
	jmp MaskedNextLineL1
MaskSkipL1:
	add edi, 0x02
	dec cx
	jnz near MaskedBlitLuminated2
	add esi, [TrueSourceXMove]
	add edi, [TrueDestXMove]
	dec word [Height]
	jnz near MaskedBlitLuminated1
MaskedNextLineL1:
	pop es
	popad
	ret

;-------------------------------------

_AlphaBlit__FUlUlUlUlUsUsUc
	push ebp
	mov ebp, esp
	mov al, byte [ebp+8]
	mov [Alpha], al
	mov ax, word [ebp+12]
	mov [Height], ax
	mov ax, word [ebp+16]
	mov [Width], ax
	mov eax, dword [ebp+20]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+28]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+32]
	mov [TrueSourceOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	mov es, ax
	mov dl, [Alpha]
	cld
AlphaBlit11:
	mov cx, [Width]
AlphaBlit12:
	lodsw
	cmp ax, 0xF81F
	je near MaskSkipA1
	mov bx, [edi]
	mov [DColor], bx
	mov bx, ax
	and ax, 0x1F
	shl ax, 0x03
	mul dl
	mov [SColor], ax
	mov ax, [DColor]
	and ax, 0x1F
	shl ax, 0x03
	not dl
	mul dl
	not dl
	add ax, [SColor]
	shr ax, 0x0B
	and bx, 0xFFE0
	or bx, ax
	ror bx, 0x05
	ror word [DColor], 0x05
	mov ax, bx
	and ax, 0x3F
	shl ax, 0x02
	mul dl
	mov [SColor], ax
	mov ax, [DColor]
	and ax, 0x3F
	shl ax, 0x02
	not dl
	mul dl
	not dl
	add ax, [SColor]
	shr ax, 0x0A
	and bx, 0xFFC0
	or bx, ax
	ror bx, 0x06
	ror word [DColor], 0x06
	mov ax, bx
	and ax, 0x1F
	shl ax, 0x03
	mul dl
	mov [SColor], ax
	mov ax, [DColor]
	and ax, 0x1F
	shl ax, 0x03
	not dl
	mul dl
	not dl
	add ax, [SColor]
	shr ax, 0x0B
	and bx, 0xFFE0
	or bx, ax
	ror bx, 0x05
	mov ax, bx
	stosw
	dec cx
	jnz near AlphaBlit12
	add esi, [TrueSourceXMove]
	add edi, [TrueDestXMove]
	dec word [Height]
	jnz near AlphaBlit11
	jmp MaskedNextLineA1
MaskSkipA1:
	add edi, 0x02
	dec cx
	jnz near AlphaBlit12
	add esi, [TrueSourceXMove]
	add edi, [TrueDestXMove]
	dec word [Height]
	jnz near AlphaBlit11
MaskedNextLineA1:
	pop es
	popad
	ret

;-------------------------------------

_AlphaBlit__FUlUlUlUlUlUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Height], ax
	mov ax, word [ebp+12]
	mov [Width], ax
	mov eax, dword [ebp+16]
	mov [TrueDestXMove], eax
	mov eax, dword [ebp+20]
	mov [TrueSourceXMove], eax
	mov eax, dword [ebp+24]
	mov [TrueDestOffset], eax
	mov eax, dword [ebp+28]
	mov [TrueSourceOffset], eax
	mov eax, dword [ebp+32]
	mov [AlphaMapOffset], eax
	pop ebp

	pushad
	push es
	mov ax, ds
	mov es, ax
	mov esi, [TrueSourceOffset]
	mov edi, [TrueDestOffset]
	push dword [AlphaMapOffset]
	cld
AlphaBlit21:
	mov cx, [Width]
AlphaBlit22:
	lodsw
	pop ebx
	mov dl, [ebx]
	inc ebx
	push ebx
	cmp ax, 0xF81F
	je near MaskSkipA2
	mov bx, [edi]
	mov [DColor], bx
	mov bx, ax
	and ax, 0x1F
	shl ax, 0x03
	mul dl
	mov [SColor], ax
	mov ax, [DColor]
	and ax, 0x1F
	shl ax, 0x03
	not dl
	mul dl
	not dl
	add ax, [SColor]
	shr ax, 0x0B
	and bx, 0xFFE0
	or bx, ax
	ror bx, 0x05
	ror word [DColor], 0x05
	mov ax, bx
	and ax, 0x3F
	shl ax, 0x02
	mul dl
	mov [SColor], ax
	mov ax, [DColor]
	and ax, 0x3F
	shl ax, 0x02
	not dl
	mul dl
	not dl
	add ax, [SColor]
	shr ax, 0x0A
	and bx, 0xFFC0
	or bx, ax
	ror bx, 0x06
	ror word [DColor], 0x06
	mov ax, bx
	and ax, 0x1F
	shl ax, 0x03
	mul dl
	mov [SColor], ax
	mov ax, [DColor]
	and ax, 0x1F
	shl ax, 0x03
	not dl
	mul dl
	not dl
	add ax, [SColor]
	shr ax, 0x0B
	and bx, 0xFFE0
	or bx, ax
	ror bx, 0x05
	mov ax, bx
	stosw
	dec cx
	jnz near AlphaBlit22
	jmp MaskedNextLineA2
MaskSkipA2:
	add edi, 0x02
	dec cx
	jnz near AlphaBlit22
MaskedNextLineA2:
	add esi, [TrueSourceXMove]
	add edi, [TrueDestXMove]
	dec word [Height]
	jnz near AlphaBlit21
	pop ebx
	pop es
	popad
	ret

;-------------------------------------

_DrawLine__FUlUlUsUsUsUsUsUsUs
	push ebp
	mov ebp, esp
	mov ax, word [ebp+8]
	mov [Color], ax
	mov ax, word [ebp+12]
	mov [ThisXSize], ax
	mov ax, word [ebp+16]
	mov [ThisYSize], ax
	mov ax, word [ebp+20]
	mov [ToY], ax
	mov ax, word [ebp+24]
	mov [ToX], ax
	mov ax, word [ebp+28]
	mov [FromY], ax
	mov ax, word [ebp+32]
	mov [FromX], ax
	mov eax, dword [ebp+36]
	mov [Pitch], eax
	mov eax, dword [ebp+40]
	mov [Surface], eax
	pop ebp

	push	ax
	push	bx
	push	cx
	push	dx
	xor	eax,	eax
	xor	ebx,	ebx
	mov	bx,	[FromX]
	mov	ax,	[FromY]
	mov	dx,	[ToX]
	mov	cx,	[ToY]
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
	mov	[DistaX], dx
	mov	[DistaY], cx
	mov	bx,	[FromX]
	mov	ax,	[FromY]
	xor	cl,	cl
	jmp	LineXYNoSwap
LineXYSwap:
	mov	[DistaY], dx
	mov	[DistaX], cx
	mov	ax,	[FromX]
	mov	bx,	[FromY]
	mov	dx,	[ToX]
	mov	cx,	[ToY]
	mov	[ToY],	dx
	mov	[ToX],	cx
	mov	cx,	si
	mov	dx,	di
	mov	di,	cx
	mov	si,	dx
	mov	dx,	[DistaX]
	mov	cl,	0x01
LineXYNoSwap:
	shr	dx,	0x01
	inc	dx
	mov	[BTemp], dx
	mov	dh,	cl
	xor	cx,	cx
	jmp	LineLoopBegin
LineXPlus:
	add	cx,	[DistaY]
LineLoopBegin:
	cmp	cx,	[BTemp]
	jl	LineNotYPlus
	sub	cx,	[DistaX]
	add	ax,	di
LineNotYPlus:
	call	SPutPixel
	add	bx,	si
        cmp     bx,     [ToX]
	jnz	LineXPlus
	add	cx,	[DistaY]
	cmp	cx,	[BTemp]
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
	cmp	bx,	[ThisXSize]
	jae	NPutPixelEnd
	cmp	ax,	[ThisYSize]
	jae	NPutPixelEnd
	push	eax
	push	ebx
	push	edx
	mul	dword [Pitch]
	add	eax,	[Surface]
	shl	ebx,	0x01
	add	eax,	ebx
	mov	ebx,	eax
	mov	ax,	[Color]
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
	cmp	bx,	[ThisXSize]
	jae	SPutPixelEnd
	cmp	ax,	[ThisYSize]
	jae	SPutPixelEnd
	push	edx
	mul	dword [Pitch]
	add	eax,	[Surface]
	shl	ebx,	0x01
	add	eax,	ebx
	mov	ebx,	eax
	mov	ax,	[Color]
	mov	[ebx],	ax
	pop	edx
SPutPixelEnd:
	pop	ebx
	pop	eax
	ret

End:
	ret

;-------------------------------------

SECTION .data

TrueSourceOffset:	dd 0x00
TrueDestOffset:		dd 0x00
TrueSourceXMove:	dd 0x00
TrueDestXMove:		dd 0x00
TrueDestYMove:		dd 0x00
AlphaMapOffset:		dd 0x00
Width:			dw 0x00
Height:			dw 0x00
Luminance:		dw 0x00
SColor:			dw 0x00
DColor:			dw 0x00
Alpha:			db 0x00
Surface:		dd 0x00
Pitch:			dd 0x00
FromX:			dw 0x00
FromY:			dw 0x00
ToX:			dw 0x00
ToY:			dw 0x00
ThisXSize:		dw 0x00
ThisYSize:		dw 0x00
DistaX:			dw 0x00
DistaY:			dw 0x00
BTemp:			dw 0x00
Color:			dw 0x00
