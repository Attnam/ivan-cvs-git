#ifndef __BLIT_H__
#define __BLIT_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

void Fill(ulong, ulong, ushort, ushort, ushort);
#ifndef __DJGPP__
void BlitToDB(ulong, ulong, ulong, ushort, ushort);
#else
void BlitToDB(ulong, ulong, ulong, ushort, ushort, ushort);
#endif
void BlitNoFlags(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirror(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitFlip(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirrorFlip(ulong, ulong, ulong, ulong, ushort, ushort);
void BlitRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirrorRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitMirrorFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort);
void BlitLuminated(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitNoFlags(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirror(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitFlip(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirrorFlip(ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirrorRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitMirrorFlipRotate90(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void MaskedBlitLuminated(ulong, ulong, ulong, ulong, ushort, ushort, ushort, ushort);
void SimpleAlphaBlit(ulong, ulong, ulong, ulong, ushort, ushort, uchar, ushort);
void AlphaBlitNoFlags(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitMirror(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitFlip(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitMirrorFlip(ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitRotate90(ulong, ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitMirrorRotate90(ulong, ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitFlipRotate90(ulong, ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void AlphaBlitMirrorFlipRotate90(ulong, ulong, ulong, ulong, ulong, ulong, ushort, ushort, ushort);
void DrawLine(ulong, ulong, ushort, ushort, ushort, ushort, ushort, ushort, ushort);

#endif
