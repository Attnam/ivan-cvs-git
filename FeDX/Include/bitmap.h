#ifndef __BITMAP_H__
#define __BITMAP_H__

#include <fstream>

#include "typedef.h"

#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

class CSurface;
struct IDirectDrawSurface7;

class bitmap
{
public:
	friend class graphics;
	bitmap(const char*);
	bitmap(ushort, ushort);
	~bitmap(void);
	void Save(std::ofstream*, ushort, ushort, ushort, ushort);
	void Load(std::ifstream*, ushort, ushort, ushort, ushort);
	void Save(std::string);
	void PutPixel(ushort, ushort, ushort);
	ushort GetPixel(ushort, ushort);
	void ClearToColor(ushort = 0);
	void ClearToColor(ushort, ushort, ushort, ushort, ushort = 0);
	void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0);
	void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort);
	void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0);
	void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort);
	void BlitToDB(ushort, ushort, ushort, ushort, ushort, ushort);
	void BlitToDB(ushort, ushort, ushort, ushort, ushort, ushort, ushort);
	void MaskedBlitToDB(ushort, ushort, ushort, ushort, ushort, ushort);
	void MaskedBlitToDB(ushort, ushort, ushort, ushort, ushort, ushort, ushort);
	void ReadFromDB(ushort, ushort);
	void WriteToDB(ushort, ushort);
	void FastBlit(bitmap*);
	void FastMaskedBlit(bitmap*);
	void Printf(bitmap*, ushort, ushort, const char*, ...);
	void PrintfToDB(ushort, ushort, const char*, ...);
	ushort GetXSize(void) { return XSize; }
	ushort GetYSize(void) { return YSize; }
protected:
	bitmap(IDirectDrawSurface7*);
	void AttachSurface(IDirectDrawSurface7*);
	void Backup(ushort = 0, ushort = 0, bool = true);
	void Restore(ushort = 0, ushort = 0, bool = true);
	CSurface* CDXSurface(void) { return DXSurface; }
	static char BMPHeader[];
	CSurface* DXSurface;
	ushort XSize, YSize;
	ushort* BackupBuffer;
};

#endif
