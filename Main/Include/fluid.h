#ifndef __FLUID_H__
#define __FLUID_H__

#include "vector2d.h"
#include "entity.h"
#include "lsquare.h"

class bitmap;

typedef std::list<fluid*> fluidlist;

class fluid : public entity
{
 public:
  fluid();
  fluid(liquid*, lsquare*);
  fluid(liquid*, item*);
  virtual ~fluid();
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Draw(bitmap*, vector2d, ulong, bool) const;
  liquid* GetLiquid() const { return Liquid; }
  virtual square* GetSquareUnderEntity(ushort = 0) const { return LSquareUnder; }
  square* GetSquareUnder() const { return LSquareUnder; }
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual bool IsOnGround() const { return true; }
  void AddLiquid(ulong);
  virtual void SignalVolumeAndWeightChange();
  void SetMotherItem(item*);
  static void AddFluidInfo(const fluidlist&, festring&);
  void CheckGearPicture(vector2d, ushort, bool);
  void DrawGearPicture(bitmap*, vector2d, ulong, ushort, bool) const;
  bool FadePictures();
  void DrawBodyArmorPicture(bitmap*, vector2d, ulong, ushort, bool) const;
 protected:
  struct imagedata
  {
    imagedata();
    ~imagedata();
    void Animate(bitmap*, vector2d, ulong, ushort) const;
    void AddLiquidToPicture(const colorizablebitmap*, ulong, ulong, ushort);
    void Save(outputfile&) const;
    void Load(inputfile&);
    bool Fade();
    /* Only pictures of fluids not on ground have their RandMaps initialized,
       since they are animated. Note that the picture is always unrotated. */
    bitmap* Picture;
    /* Used by Animate() */
    mutable short DripTimer;
    mutable vector2d DripPos;
    mutable ushort DripColor;
    mutable uchar DripAlpha;
    /* Sum of all alphas of Picture. The volume of the liquid is currently
       proportional to AlphaSum of the fluid's Image, limiting it
       considerably. */
    ulong AlphaSum;
    /* AlphaSum / (non-transparent pixels in Picture), used to synchronise
       gear pictures with the main image */
    uchar AlphaAverage;
    /* The position of a gear picture in humanoid.pcx which binds the fluid;
       remembered so that it can be easily determined whether the fluid needs
       to be redistributed due to a major graphics change */
    vector2d ShadowPos;
    /* Animation of gear items needs to know whether the raw picture is
       rotated somehow. Currently this is the case only for an item
       in the left hand of a character. */
    ushort SpecialFlags;
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
  static const ulong BodyArmorPartPixels[];
};

outputfile& operator<<(outputfile&, const fluid*);
inputfile& operator>>(inputfile&, fluid*&);

#endif
