#ifndef __SAVE_H__
#define __SAVE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cstdio>
#include <vector>
#include <list>
#include <string>
#include <map>

#include "rect.h"
#include "error.h"

typedef std::map<std::string, long> valuemap;

/* fstream seems to bug with DJGPP, so we use FILE* here */

class outputfile
{
 public:
  outputfile(const std::string&, bool = true);
  ~outputfile();
  void Put(char What) { fputc(What, Buffer); }
  void Write(const char* Offset, long Size) { fwrite(Offset, 1, Size, Buffer); }
  bool IsOpen() { return Buffer != 0; }
 private:
  FILE* Buffer;
};

class inputfile
{
 public:
  inputfile(const std::string&, const valuemap* = 0, bool = true);
  ~inputfile();
  std::string ReadWord(bool = true);
  void ReadWord(std::string&, bool = true);
  char ReadLetter(bool = true);
  long ReadNumber(uchar = 0xFF);
  vector2d ReadVector2d();
  rect ReadRect();
  bool ReadBool();
  int Get() { return fgetc(Buffer); }
  void Read(char* Offset, long Size) { fread(Offset, 1, Size, Buffer); }
  bool IsOpen() { return Buffer != 0; }
  bool Eof() { return feof(Buffer) != 0; }
  void ClearFlags() { clearerr(Buffer); }
  void SeekPosBegin(long Offset) { fseek(Buffer, Offset, SEEK_SET); }
  void SeekPosCurrent(long Offset) { fseek(Buffer, Offset, SEEK_CUR); }
  void SeekPosEnd(long Offset) { fseek(Buffer, Offset, SEEK_END); }
  long TellPos() { return ftell(Buffer); }
  ulong TellLine() { return TellLineOfPos(TellPos()); }
  ulong TellLineOfPos(long);
  const std::string& GetFileName() const { return FileName; }
  void UnGet(int Char) { ungetc(Char, Buffer); }
 private:
  FILE* Buffer;
  std::string FileName;
  const valuemap* ValueMap;
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

inline void ReadData(bool& Type, inputfile& SaveFile) { Type = SaveFile.ReadBool(); }
inline void ReadData(char& Type, inputfile& SaveFile) { Type = SaveFile.ReadNumber(); }
inline void ReadData(uchar& Type, inputfile& SaveFile) { Type = SaveFile.ReadNumber(); }
inline void ReadData(short& Type, inputfile& SaveFile) { Type = SaveFile.ReadNumber(); }
inline void ReadData(ushort& Type, inputfile& SaveFile) { Type = SaveFile.ReadNumber(); }
inline void ReadData(long& Type, inputfile& SaveFile) { Type = SaveFile.ReadNumber(); }
inline void ReadData(ulong& Type, inputfile& SaveFile) { Type = SaveFile.ReadNumber(); }
inline void ReadData(vector2d& Type, inputfile& SaveFile) { Type = SaveFile.ReadVector2d(); }
inline void ReadData(rect& Type, inputfile& SaveFile) { Type = SaveFile.ReadRect(); }
void ReadData(std::string&, inputfile&);
void ReadData(std::vector<long>&, inputfile&);
void ReadData(std::vector<std::string>&, inputfile&);

template <class type> inline void ReadData(std::vector<type>& Vector, inputfile& SaveFile)
{
  Vector.clear();
  std::string Word;
  SaveFile.ReadWord(Word);

  if(Word == "=")
    SaveFile.ReadWord(Word);

  if(Word == "=")
    {
      Vector.push_back(type());
      ReadData(Vector[0], SaveFile);
      return;
    }

  if(Word != "{")
    ABORT("Array syntax error \"%s\" found in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());

  ushort Size = SaveFile.ReadNumber();

  for(ushort c = 0; c < Size; ++c)
    {
      Vector.push_back(type());
      ReadData(Vector.back(), SaveFile);
    }

  if(SaveFile.ReadWord() != "}")
    ABORT("Illegal array terminator \"%s\" encountered in file %s, line %d!", Word.c_str(), SaveFile.GetFileName().c_str(), SaveFile.TellLine());
}

inline outputfile& operator<<(outputfile& SaveFile, bool Value)
{
  SaveFile.Put(Value);
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, bool& Value)
{
  Value = SaveFile.Get() != 0;
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
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, long& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ulong Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ulong& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, longlong Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, longlong& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ulonglong Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ulonglong& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, float Value)
{
  SaveFile.Write(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, float& Value)
{
  SaveFile.Read(reinterpret_cast<char*>(&Value), sizeof(Value));
  return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, vector2d Vector)
{
  SaveFile.Write(reinterpret_cast<char*>(&Vector), sizeof(Vector));
  return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, vector2d& Vector)
{
  SaveFile.Read(reinterpret_cast<char*>(&Vector), sizeof(Vector));
  return SaveFile;
}

outputfile& operator<<(outputfile&, const std::string&);
inputfile& operator>>(inputfile&, std::string&);

template <class type> inline outputfile& operator<<(outputfile& SaveFile, const std::vector<type>& Vector)
{
  SaveFile << ulong(Vector.size());

  for(ulong c = 0; c < Vector.size(); ++c)
    SaveFile << Vector[c];

  return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, std::vector<type>& Vector)
{
  Vector.resize(ReadType<ulong>(SaveFile), type());

  for(ulong c = 0; c < Vector.size(); ++c)
    SaveFile >> Vector[c];

  return SaveFile;
}

template <class type> inline outputfile& operator<<(outputfile& SaveFile, const std::list<type>& List)
{
  SaveFile << ulong(List.size());

  for(typename std::list<type>::const_iterator i = List.begin(); i != List.end(); ++i)
    SaveFile << *i;

  return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, std::list<type>& List)
{
  List.resize(ReadType<ulong>(SaveFile), type());

  for(typename std::list<type>::iterator i = List.begin(); i != List.end(); ++i)
    SaveFile >> *i;

  return SaveFile;
}

template <class type1, class type2> inline outputfile& operator<<(outputfile& SaveFile, const std::map<type1, type2>& Map)
{
  SaveFile << ulong(Map.size());

  for(typename std::map<type1, type2>::const_iterator i = Map.begin(); i != Map.end(); ++i)
    SaveFile << i->first << i->second;

  return SaveFile;
}

template <class type1, class type2> inline inputfile& operator>>(inputfile& SaveFile, std::map<type1, type2>& Map)
{
  ulong Size;
  SaveFile >> Size;

  for(ushort c = 0; c < Size; ++c)
    {
      type1 First;
      SaveFile >> First;
      SaveFile >> Map[First];
    }

  return SaveFile;
}

#endif
