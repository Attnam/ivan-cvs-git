#ifndef __BITMAP_H__
#define __BITMAP_H__

#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#include <string>

#include "typedef.h"

class CSurface;
struct IDirectDrawSurface7;
struct _DDSURFACEDESC2;
typedef _DDSURFACEDESC2 DDSURFACEDESC2;

class outputfile;
class inputfile;
class vector2d;

class bitmap
{
public:
	friend class graphics;
	bitmap(std::string, bool = true);
	bitmap(ushort, ushort);
	~bitmap();
	void Save(outputfile&) const;
	void Load(inputfile&);
	void Save(std::string) const;
	void PutPixel(ushort, ushort, ushort);
	ushort GetPixel(ushort, ushort) const;
	void Lock();
	void Release();
	void FastPutPixel(ushort, ushort, ushort);
	ushort FastGetPixel(ushort, ushort) const;
	void ClearToColor(ushort);
	void ClearToColor(ushort, ushort, ushort, ushort, ushort);
	void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
	void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort) const;
	void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
	void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort) const;
	void FastBlit(bitmap*) const;
	void FastMaskedBlit(bitmap*) const;
	void Printf(bitmap*, ushort, ushort, const char*, ...) const;
	void DrawLine(ushort, ushort, ushort, ushort, ushort = 0xFFFF, bool = false);
	ushort GetXSize() const { return XSize; }
	ushort GetYSize() const { return YSize; }
	void DrawPolygon(vector2d, ushort, ushort, ushort, bool = false, double = 0);
	bitmap* ColorizeTo16Bit(ushort*);
	bitmap* ColorizeTo16Bit(vector2d, vector2d, ushort*);
protected:
	bitmap(IDirectDrawSurface7*, ushort, ushort);
	void AttachSurface(IDirectDrawSurface7*, ushort, ushort);
	void Backup(bool = true);
	void Restore(bool = true);
	CSurface* GetDXSurface() { return DXSurface; }
	CSurface* DXSurface;
	ushort XSize, YSize;
	ushort* BackupBuffer;
	DDSURFACEDESC2* TempDDSD;
	bool Is16Bit;
	uchar* Palette;
	uchar* PaletteBuffer;
};

#endif
