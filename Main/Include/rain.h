#ifndef __RAIN_H__
#define __RAIN_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

/*class bitmap;
class liquid;*/

class rain : public entity
{
 public:
  rain() : entity(HAS_BE), OwnLiquid(false), Drops(0) { }
  rain(liquid*, lsquare*, vector2d, bool);
  ~rain();
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Draw(bitmap*, vector2d, ulong) const;
  bool HasOwnLiquid() const { return OwnLiquid; }
  void RandomizeDropPos(ushort) const;
  liquid* GetLiquid() const { return Liquid; }
  virtual square* GetSquareUnderEntity(ushort = 0) const { return LSquareUnder; }
  square* GetSquareUnder() const { return LSquareUnder; }
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual bool IsOnGround() const { return true; }
 protected:
  liquid* Liquid;
  lsquare* LSquareUnder;
  vector2d Speed;
  ulong SpeedAbs;
  bool OwnLiquid;
  mutable ushort Drops;
  mutable vector2d DropStartPos[MAX_RAIN_DROPS];
  mutable ulong DropStartTick[MAX_RAIN_DROPS];
  mutable ushort MaxDropAge[MAX_RAIN_DROPS];
};

outputfile& operator<<(outputfile&, const rain*);
inputfile& operator>>(inputfile&, rain*&);

#endif
