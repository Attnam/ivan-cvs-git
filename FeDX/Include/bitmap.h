#ifndef __BITMAP_H__
#define __BITMAP_H__

#pragma warning(disable : 4786)

#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#include <string>
#include <list>

#include "typedef.h"
#include "vector2d.h"

class CSurface;
struct IDirectDrawSurface7;
struct _DDSURFACEDESC2;
typedef _DDSURFACEDESC2 DDSURFACEDESC2;

class outputfile;
class inputfile;

class bitmap
{
public:
	friend class graphics;
	friend class colorizablebitmap;
	bitmap(std::string);
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
	void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar) const;
	void AlphaBlit(bitmap*, ushort, ushort) const;
	void FastBlit(bitmap*) const;
	void FastMaskedBlit(bitmap*) const;
	void DrawLine(ushort, ushort, ushort, ushort, ushort = 0xFFFF, bool = false);
	ushort GetXSize() const { return XSize; }
	ushort GetYSize() const { return YSize; }
	void DrawPolygon(vector2d, ushort, ushort, ushort, bool = false, double = 0);
	void CreateAlphaMap(uchar);
	void SetAlpha(ushort X, ushort Y, uchar Alpha) { AlphaMap[X][Y] = Alpha; }
	uchar GetAlpha(ushort X, ushort Y) const { return AlphaMap[X][Y]; }
	void Outline(ushort);
	bool FadeAlpha(uchar);
	void CreateOutlineBitmap(bitmap*, ushort);
protected:
	bitmap(IDirectDrawSurface7*, ushort, ushort);
	void AttachSurface(IDirectDrawSurface7*, ushort, ushort);
	void Backup();
	void Restore();
	CSurface* GetDXSurface() { return DXSurface; }
	CSurface* DXSurface;
	ushort XSize, YSize;
	ushort* BackupBuffer;
	DDSURFACEDESC2* TempDDSD;
	std::list<bitmap*>::iterator BitmapContainerIterator;
	bool DoubleBuffer;
	uchar** AlphaMap;
};

#endif
