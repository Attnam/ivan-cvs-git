#ifndef __BITMAP_H__
#define __BITMAP_H__

#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#include <string>

#include "typedef.h"

class CSurface;
struct IDirectDrawSurface7;

class outputfile;
class inputfile;

class bitmap
{
public:
	friend class graphics;
	bitmap(const char*);
	bitmap(ushort, ushort);
	~bitmap();
	void Save(outputfile&) const;
	void Load(inputfile&);
	void Save(outputfile&, ushort, ushort, ushort, ushort) const;
	void Load(inputfile&, ushort, ushort, ushort, ushort);
	void Save(std::string) const;
	void PutPixel(ushort, ushort, ushort);
	ushort GetPixel(ushort, ushort) const;
	void ClearToColor(ushort = 0);
	void ClearToColor(ushort, ushort, ushort, ushort, ushort = 0);
	void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
	void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort) const;
	void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
	void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort) const;
	void FastBlit(bitmap*) const;
	void FastMaskedBlit(bitmap*) const;
	void Printf(bitmap*, ushort, ushort, const char*, ...) const;
	ushort GetXSize() const { return XSize; }
	ushort GetYSize() const { return YSize; }
protected:
	bitmap(IDirectDrawSurface7*);
	void AttachSurface(IDirectDrawSurface7*);
	void Backup(ushort = 0, ushort = 0, bool = true);
	void Restore(ushort = 0, ushort = 0, bool = true);
	CSurface* GetDXSurface() { return DXSurface; }
	static char BMPHeader[];
	CSurface* DXSurface;
	ushort XSize, YSize;
	ushort* BackupBuffer;
};

#endif
