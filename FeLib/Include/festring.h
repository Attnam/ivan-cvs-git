#ifndef __FESTRING_H__
#define __FESTRING_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <vector>

#include "typedef.h"
#include "felibdef.h"

#define FESTRING_PAGE 0x7F
#define REFS(ptr) reinterpret_cast<ulong*>(ptr)[-1]

class festring
{
 public:
  typedef ulong sizetype;
  /* I can prove that the code works even if OwnsData is left uninitialized.
     However, Valgrind reports this as a possible error which is annoying */
  festring() : Data(0), Size(0), OwnsData(false) { }
  festring(sizetype, char);
  festring(const char* CStr) : Data(const_cast<char*>(CStr)), Size(strlen(CStr)), OwnsData(false) { }
  festring(const char* CStr, sizetype N) : Data(const_cast<char*>(CStr)), Size(N), OwnsData(false) { }
  festring(const festring&);
  ~festring();
  festring& Capitalize();
  festring CapitalizeCopy() const { return festring(*this).Capitalize(); }
  festring& operator=(const char*);
  festring& operator=(const festring&);
  festring& operator<<(char);
  festring& operator<<(const char*);
  festring& operator<<(const festring&);
  festring& operator<<(short Int) { return Append(Int); }
  festring& operator<<(ushort Int) { return Append(Int); }
  festring& operator<<(int Int) { return Append(Int); }
  festring& operator<<(long Int) { return Append(Int); }
  festring& operator<<(ulong Int) { return Append(Int); }
  bool operator<(const festring&) const;
  bool operator==(const festring&) const;
  bool operator!=(const festring&) const;
  bool operator==(const char*) const;
  bool operator!=(const char*) const;
  const char* CStr() const;
  sizetype GetSize() const { return Size; }
  void Empty();
  void Assign(sizetype, char);
  void Resize(sizetype, char = ' ');
  sizetype Find(char, sizetype = 0);
  sizetype Find(const char* CStr, sizetype Pos = 0) { return Find(CStr, Pos, strlen(CStr)); }
  sizetype Find(const char*, sizetype, sizetype);
  sizetype Find(const festring& S, sizetype Pos = 0) { return Find(S.Data, Pos, S.Size); }
  sizetype FindLast(char, sizetype = NPos);
  void Erase(sizetype, sizetype);
  void Insert(sizetype Pos, const char* CStr) { Insert(Pos, CStr, strlen(CStr)); }
  void Insert(sizetype, const char*, sizetype);
  void Insert(sizetype Pos, const festring& S) { Insert(Pos, S.Data, S.Size); }
  festring& Append(const festring& Str, sizetype N) { return Append(Str.Data, N); }
  static const sizetype NPos;
  static void SplitString(festring&, festring&, sizetype);
  static ushort SplitString(const festring&, std::vector<festring>&, sizetype, sizetype = 0);
  static sizetype IgnoreCaseFind(const festring&, const festring&, sizetype = 0);
  static void SearchAndReplace(festring&, const festring&, const festring&, sizetype = 0);
  static bool IgnoreCaseCompare(const festring&, const festring&);
  bool IsEmpty() const { return !Size; }
  char& operator[](sizetype Index) const { return Data[Index]; }
  void PreProcessForFebot();
  void PostProcessForFebot();
  void SwapData(festring&);
  void ExtractWord(festring&);
 private:
  static void InstallIntegerMap();
  static void DeInstallIntegerMap();
  void CreateOwnData(const char*, sizetype);
  festring& Append(long);
  festring& Append(const char*, sizetype);
  void SlowAppend(char);
  void SlowAppend(const char*, sizetype);
  static char** IntegerMap;
  static char* EmptyString;
  char* Data;
  sizetype Size;
  sizetype Reserved;
  bool OwnsData;
};

class festringpile
{
 public:
  festringpile(const festring& String) : String(String) { }
  festringpile& operator+(char What) { String << What; return *this; }
  festringpile& operator+(const char* What) { String << What; return *this; }
  festringpile& operator+(const festring& What) { String << What; return *this; }
  festringpile& operator+(const festringpile& What) { String << What.String; return *this; }
  festringpile& operator+(short What) { String << What; return *this; }
  festringpile& operator+(ushort What) { String << What; return *this; }
  festringpile& operator+(int What) { String << What; return *this; }
  festringpile& operator+(long What) { String << What; return *this; }
  festringpile& operator+(ulong What) { String << What; return *this; }
  operator festring() const { return String; }
 private:
  festring String;
};

inline festringpile operator+(const festring& S, char What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, const char* What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, const festring& What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, const festringpile& What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, short What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, ushort What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, int What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, long What) { return festringpile(S) + What; }
inline festringpile operator+(const festring& S, ulong What) { return festringpile(S) + What; }

inline festring::festring(const festring& Str) : Data(Str.Data), Size(Str.Size), Reserved(Str.Reserved), OwnsData(Str.OwnsData)
{
  if(Data && OwnsData)
    ++REFS(Data);
}

inline festring::festring(sizetype N, char C) : Size(N), Reserved(N|FESTRING_PAGE), OwnsData(true)
{
  char* Ptr = 4 + new char[Reserved + 5];
  REFS(Ptr) = 0;
  Data = Ptr;
  memset(Ptr, C, N);
}

inline festring::~festring()
{
  if(OwnsData)
    {
      char* Ptr = Data;

      if(Ptr && !REFS(Ptr)--)
	delete [] &REFS(Ptr);
    }
}

inline bool festring::operator<(const festring& Str) const
{
  sizetype ThisSize = Size;
  sizetype StrSize = Str.Size;

  if(ThisSize && StrSize)
    {
      int Comp = memcmp(Data, Str.Data, Min(ThisSize, StrSize));
      return Comp < 0 || (!Comp && StrSize > ThisSize);
    }
  else
    return !ThisSize && StrSize;
}

inline bool festring::operator==(const festring& Str) const
{
  sizetype StrSize = Str.Size;
  return Size == StrSize && (!StrSize || !memcmp(Data, Str.Data, StrSize));
}

inline bool festring::operator!=(const festring& Str) const
{
  sizetype StrSize = Str.Size;
  return Size != StrSize || (StrSize && memcmp(Data, Str.Data, StrSize));
}

inline bool festring::operator==(const char* CStr) const
{
  sizetype StrSize = strlen(CStr);
  return Size == StrSize && (!StrSize || !memcmp(Data, CStr, StrSize));
}

inline bool festring::operator!=(const char* CStr) const
{
  sizetype StrSize = strlen(CStr);
  return Size != StrSize || (StrSize && memcmp(Data, CStr, StrSize));
}

inline const char* festring::CStr() const
{
  char* Ptr = Data;

  if(Ptr)
    {
      if(OwnsData)
	Ptr[Size] = 0;

      return Ptr;
    }
  else
    return EmptyString;
}

inline void festring::Empty()
{
  Size = 0;

  if(OwnsData)
    {
      char* Ptr = Data;

      if(Ptr && REFS(Ptr))
	{
	  --REFS(Ptr);
	  Data = 0;
	}
    }
  else
    Data = 0;
}

inline festring& festring::operator<<(char Char)
{
  char* OldPtr = Data;
  sizetype OldSize = Size;

  if(OwnsData && OldPtr && !REFS(OldPtr) && OldSize < Reserved)
    {
      OldPtr[OldSize] = Char;
      ++Size;
    }
  else
    SlowAppend(Char);

  return *this;
}

inline festring& festring::operator<<(const char* CStr)
{
  sizetype N = strlen(CStr);
  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;
  char* OldPtr = Data;

  if(OwnsData && OldPtr && !REFS(OldPtr) && NewSize <= Reserved)
    {
      memcpy(OldPtr + OldSize, CStr, N);
      Size = NewSize;
    }
  else
    SlowAppend(CStr, N);

  return *this;
}

inline festring& festring::operator<<(const festring& Str)
{
  sizetype N = Str.Size;
  sizetype OldSize = Size;
  sizetype NewSize = OldSize + N;
  char* OldPtr = Data;
  char* OtherPtr = Str.Data;

  if(OwnsData && OldPtr && !REFS(OldPtr) && NewSize <= Reserved)
    {
      memcpy(OldPtr + OldSize, OtherPtr, N);
      Size = NewSize;
    }
  else
    SlowAppend(OtherPtr, N);

  return *this;
}

struct charcomparer
{
  bool operator()(const char* const& S1, const char* const& S2) const { return strcmp(S1, S2) < 0; }
};

#define CONST_S(str) festring(str, sizeof(str) - 1)

/* This macro doesn't evaluate with if what is not found so it's often faster */

#define SEARCH_N_REPLACE(what, with) if(Msg.Find(what) != festring::NPos) festring::SearchAndReplace(Msg, what, with);

#endif
