#ifndef __RAIN_H__
#define __RAIN_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

class rain : public entity
{
 public:
  /* Come To The Dark Side */
  rain* Next;
 public:
  rain() : entity(HAS_BE), Next(0), Drop(0), Drops(0), OwnLiquid(0) { }
  rain(liquid*, lsquare*, vector2d, int, bool);
  ~rain();
  virtual void Be();
  void Save(outputfile&) const;
  void Load(inputfile&);
  void Draw(bitmap*, vector2d, color24) const;
  bool HasOwnLiquid() const { return !!OwnLiquid; }
  void RandomizeDropPos(int) const;
  liquid* GetLiquid() const { return Liquid; }
  virtual square* GetSquareUnderEntity(int = 0) const { return LSquareUnder; }
  square* GetSquareUnder() const { return LSquareUnder; }
  void SetLSquareUnder(lsquare* What) { LSquareUnder = What; }
  lsquare* GetLSquareUnder() const { return LSquareUnder; }
  virtual bool IsOnGround() const { return true; }
  int GetTeam() const { return Team; }
 protected:
  mutable struct drop
  {
    packedvector2d StartPos;
    ushort StartTick;
    ushort MaxAge;
  }* Drop;
  liquid* Liquid;
  lsquare* LSquareUnder;
  vector2d Speed;
  long SpeedAbs;
  mutable int Drops : 15;
  int OwnLiquid : 1;
  int Team : 8;
};

outputfile& operator<<(outputfile&, const rain*);
inputfile& operator>>(inputfile&, rain*&);

#endif
