#ifndef __SAVE_H__
#define __SAVE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cstdio>
#include <vector>
#include <list>

#include "typedef.h"
#include "vector2d.h"

/* fstream seems to bug with DJGPP, so we use FILE* here */

class outputfile
{
 public:
  outputfile(const std::string&, bool = true);
  ~outputfile() { if(Buffer) fclose(Buffer); }
  void Put(char What) { fputc(What, Buffer); }
  void Write(const char* Offset, long Size) { fwrite(Offset, 1, Size, Buffer); }
  bool IsOpen() { return Buffer != 0; }
 private:
  FILE* Buffer;
};

class inputfile
{
 public:
  inputfile(const std::string&, bool = true);
  ~inputfile() { if(Buffer) fclose(Buffer); }
  std::string ReadWord(bool = true);
  void ReadWord(std::string&, bool = true);
  char ReadLetter(bool = true);
  long ReadNumber(const valuemap&, uchar = 0xFF);
  long ReadNumber();
  vector2d ReadVector2d(const valuemap&);
  bool ReadBool();
  int Get() { return fgetc(Buffer); }
  void Read(char* Offset, long Size) { fread(Offset, 1, Size, Buffer); }
  bool IsOpen() { return Buffer != 0; }
  bool Eof();
  void ClearFlags() { clearerr(Buffer); }
  void SeekPosBeg(long Offset) { fseek(Buffer, Offset, SEEK_SET); }
  void SeekPosCur(long Offset) { fseek(Buffer, Offset, SEEK_CUR); }
  void SeekPosEnd(long Offset) { fseek(Buffer, Offset, SEEK_END); }
  long TellPos() { return ftell(Buffer); }
  int Peek();
 private:
  FILE* Buffer;
};

/*
 * Reads a binary form variable of type type and returns it.
 * An inputfile template member function would be far more elegant,
 * but VC doesn't seem to understand it.
 */

template <class type> inline type ReadType(inputfile& SaveFile)
{
  type Variable;
  SaveFile >> Variable;
  return Variable;
}

/*
 * Reads a formatted data variable Type and initializes it.
 */

template <class type> inline void ReadData(type& Type, inputfile& SaveFile, const valuemap& ValueMap)
{
  Type.SetValueMap(ValueMap);
  Type.ReadFrom(SaveFile);
}

inline void ReadData(bool& Type, inputfile& SaveFile, const valuemap&) { Type = SaveFile.ReadBool(); }
inline void ReadData(char& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadNumber(ValueMap); }
inline void ReadData(uchar& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadNumber(ValueMap); }
inline void ReadData(short& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadNumber(ValueMap); }
inline void ReadData(ushort& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadNumber(ValueMap); }
inline void ReadData(long& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadNumber(ValueMap); }
inline void ReadData(ulong& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadNumber(ValueMap); }
inline void ReadData(vector2d& Type, inputfile& SaveFile, const valuemap& ValueMap) { Type = SaveFile.ReadVector2d(ValueMap); }
void ReadData(std::string&, inputfile&, const valuemap&);

inline outputfile& operator<<(outputfile& SaveFile, bool Value)
{
  SaveFile.Put(Value);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, bool& Value)
{
  Value = SaveFile.Get() ? true : false;
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, char Value)
{
  SaveFile.Put(Value);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, char& Value)
{
  Value = SaveFile.Get();
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, uchar Value)
{
  SaveFile.Put(Value);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, uchar& Value)
{
  Value = SaveFile.Get();
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, short Value)
{
  SaveFile.Put(Value);
  SaveFile.Put(Value >> 8);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, short& Value)
{
  Value  = SaveFile.Get();
  Value |= SaveFile.Get() << 8;
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ushort Value)
{
  SaveFile.Put(Value);
  SaveFile.Put(Value >> 8);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ushort& Value)
{
  Value  = SaveFile.Get();
  Value |= SaveFile.Get() << 8;
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, long Value)
{
  SaveFile.Write((char*)&Value, sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, long& Value)
{
  SaveFile.Read((char*)&Value, sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ulong Value)
{
  SaveFile.Write((char*)&Value, sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ulong& Value)
{
  SaveFile.Read((char*)&Value, sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, float Value)
{
  SaveFile.Write((char*)&Value, sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, float& Value)
{
  SaveFile.Read((char*)&Value, sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, vector2d Vector)
{
  SaveFile.Write((char*)&Vector, sizeof(Vector));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, vector2d& Vector)
{
  SaveFile.Read((char*)&Vector, sizeof(Vector));
  return SaveFile;
}

outputfile& operator<<(outputfile&, const std::string&);

inputfile& operator>>(inputfile&, std::string&);

template <class type> inline outputfile& operator<<(outputfile& SaveFile, const std::vector<type>& Vector)
{
  ulong Size = Vector.size();
  SaveFile.Write((char*)&Size, sizeof(Size));

  for(ulong c = 0; c < Vector.size(); ++c)
    SaveFile << Vector[c];

  return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, std::vector<type>& Vector)
{
  ulong Size;
  SaveFile.Read((char*)&Size, sizeof(Size));
  Vector.resize(Size, type());

  for(ulong c = 0; c < Vector.size(); ++c)
    SaveFile >> Vector[c];

  return SaveFile;
}

template <class type> inline outputfile& operator<<(outputfile& SaveFile, const std::list<type>& List)
{
  ulong Size = List.size();
  SaveFile.Write((char*)&Size, sizeof(Size));

  for(std::list<type>::const_iterator i = List.begin(); i != List.end(); ++i)
    SaveFile << *i;

  return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, std::list<type>& List)
{
  ulong Size;
  SaveFile.Read((char*)&Size, sizeof(Size));
  List.resize(Size, type());

  for(std::list<type>::iterator i = List.begin(); i != List.end(); ++i)
    SaveFile >> *i;

  return SaveFile;
}

template <class type1, class type2> inline outputfile& operator<<(outputfile& SaveFile, const std::map<type1, type2>& Map)
{
  ulong Size = Map.size();
  SaveFile.Write((char*)&Size, sizeof(Size));

  for(std::map<ulong, uchar>::const_iterator i = Map.begin(); i != Map.end(); ++i)
    SaveFile << i->first << i->second;

  return SaveFile;
}

template <class type1, class type2> inline inputfile& operator>>(inputfile& SaveFile, std::map<type1, type2>& Map)
{
  ulong Size;
  SaveFile.Read((char*)&Size, sizeof(Size));

  for(ushort c = 0; c < Size; ++c)
    {
      type1 First;
      type2 Second;
      SaveFile >> First >> Second;
      Map[First] = Second;
    }

  return SaveFile;
}

#endif
