#ifndef __BITMAP_H__
#define __BITMAP_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define NONE 0
#define MIRROR 1
#define FLIP 2
#define ROTATE_90 4

#include <string>
#include <list>

#include "typedef.h"
#include "vector2d.h"
#include "save.h"

class outputfile;
class inputfile;

class bitmap
{
 public:
  friend class graphics;
  friend class colorizablebitmap;
  bitmap(std::string);
  bitmap(bitmap*);
  bitmap(ushort, ushort);
  bitmap(ushort, ushort, ushort);
  bitmap(bitmap*, ushort, ushort, ushort, ushort);
  ~bitmap();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Save(std::string) const;
  void PutPixel(ushort, ushort, ushort);
  ushort GetPixel(ushort, ushort) const;
  void Fill(ushort Color) { Fill(0, 0, XSize, YSize, Color); }
  void Fill(ushort, ushort, ushort, ushort, ushort);
  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0) const;
  void Blit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort) const;
  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar = 0, ushort = 0xF81F) const;
  void MaskedBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, ushort, ushort = 0xF81F) const;
  void AlphaBlit(bitmap*, ushort, ushort, ushort, ushort, ushort, ushort, uchar, ushort = 0xF81F) const;
  void AlphaBlit(bitmap*, ushort, ushort, ushort = 0xF81F) const;
  void DrawLine(ushort, ushort, ushort, ushort, ushort = 0xFFFF, bool = false);
  ushort GetXSize() const { return XSize; }
  ushort GetYSize() const { return YSize; }
  void DrawPolygon(vector2d, ushort, ushort, ushort, bool = true, bool = false, double = 0);
  void CreateAlphaMap(uchar);
  bool ChangeAlpha(char);
  void SetAlpha(ushort, ushort, uchar);
  uchar GetAlpha(ushort, ushort) const;
  void Outline(ushort);
  void CreateOutlineBitmap(bitmap*, ushort);
  void FadeToScreen();
 protected:
  ushort XSize, YSize;
  bool IsIndependent;
  ushort** GetImage() const { return Data.Base.Image; }
  void SetImage(ushort** What) { Data.Base.Image = What; }
  uchar** GetAlphaMap() const { return Data.Base.AlphaMap; }
  void SetAlphaMap(uchar** What) { Data.Base.AlphaMap = What; }
  bitmap* GetMotherBitmap() const { return Data.Child.MotherBitmap; }
  void SetMotherBitmap(bitmap* What) { Data.Child.MotherBitmap = What; }
  ushort GetXPos() const { return Data.Child.XPos; }
  void SetXPos(ushort What) { Data.Child.XPos = What; }
  ushort GetYPos() const { return Data.Child.YPos; }
  void SetYPos(ushort What) { Data.Child.YPos = What; }
  union data
  {
    struct base
    {
      ushort** Image;
      uchar** AlphaMap;
    } Base;
    struct child
    {
      bitmap* MotherBitmap;
      ushort XPos, YPos;
    } Child;
  } Data;
};

inline outputfile& operator<<(outputfile& SaveFile, bitmap* Bitmap)
{
  if(Bitmap)
    {
      SaveFile.Put(1);
      SaveFile << Bitmap->GetXSize() << Bitmap->GetYSize();
      Bitmap->Save(SaveFile);
    }
  else
    SaveFile.Put(0);

  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, bitmap*& Bitmap)
{
  if(SaveFile.Get())
    {
      ushort XSize, YSize;
      SaveFile >> XSize >> YSize;
      Bitmap = new bitmap(XSize, YSize);
      Bitmap->Load(SaveFile);
    }

  return SaveFile;
}

#endif
