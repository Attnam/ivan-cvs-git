#ifndef __FLUID_H__
#define __FLUID_H__

#include "vector2d.h"
#include "entity.h"
#include "lsquare.h"

class bitmap;

typedef bool (rawbitmap::*pixelpredicate)(vector2d) const;

class fluid : public entity
{
 public:
  /* Come To The Dark Side */
  fluid* Next;
 public:
  fluid();
  fluid(liquid*, lsquare*);
  fluid(liquid*, item*, bool);
  virtual ~fluid();
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Draw(bitmap*, vector2d, color24, bool) const;
  liquid* GetLiquid() const { return Liquid; }
  virtual square* GetSquareUnderEntity(int = 0) const { return LSquareUnder; }
  square* GetSquareUnder() const { return LSquareUnder; }
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual bool IsOnGround() const { return true; }
  void AddLiquid(long);
  void AddLiquidAndVolume(long);
  virtual void SignalVolumeAndWeightChange();
  void SetMotherItem(item*);
  static void AddFluidInfo(const fluid*, festring&);
  void CheckGearPicture(vector2d, int, bool);
  void DrawGearPicture(bitmap*, vector2d, color24, int, bool) const;
  bool FadePictures();
  void DrawBodyArmorPicture(bitmap*, vector2d, color24, int, bool) const;
  void Redistribute();
  virtual material* RemoveMaterial(material*);
  void Destroy();
 protected:
  struct imagedata
  {
    imagedata(bool = true);
    ~imagedata();
    void Animate(bitmap*, vector2d, color24, int) const;
    void AddLiquidToPicture(const rawbitmap*, long, long, color16, pixelpredicate);
    void Save(outputfile&) const;
    void Load(inputfile&);
    bool Fade();
    void Clear(bool);
    /* Only pictures of fluids not on ground have their RandMaps initialized,
       since they are animated. Note that the picture is always unrotated. */
    bitmap* Picture;
    /* Used by Animate() */
    mutable int DripTimer;
    mutable vector2d DripPos;
    mutable color16 DripColor;
    mutable alpha DripAlpha;
    /* Sum of all alphas of Picture. The volume of the liquid is currently
       proportional to AlphaSum of the fluid's Image, limiting it
       considerably. */
    long AlphaSum;
    /* AlphaSum / (non-transparent pixels in Picture), used to synchronise
       gear pictures with the main image */
    packedalpha AlphaAverage;
    /* The position of a gear picture in humanoid.pcx which binds the fluid;
       remembered so that it can be easily determined whether the fluid needs
       to be redistributed due to a major graphics change */
    vector2d ShadowPos;
    /* Animation of gear items needs to know whether the raw picture is
       rotated somehow. Currently this is the case only for an item
       in the left hand of a character. */
    int SpecialFlags;
  };
  liquid* Liquid;
  lsquare* LSquareUnder;
  /* MotherItem == 0 means that the fluid is on ground */
  item* MotherItem;
  imagedata Image;
  /* Data of pictures shown over the player if he equips the item which
     the fluid covers. Note that these are not destroyed when the armor
     or weapon is unequipped. There is no real need, since the existence
     of the fluid is very temporary anyway. */
  imagedata* GearImage;
  /* BodyArmors need six gear pictures instead of one */
  bool HasBodyArmorPictures;
  static const long BodyArmorPartPixels[];
};

outputfile& operator<<(outputfile&, const fluid*);
inputfile& operator>>(inputfile&, fluid*&);

#endif
