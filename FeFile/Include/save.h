#ifndef __SAVE_H__
#define __SAVE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <cstdio>
#include <vector>
#include <map>
#include <queue>

#include "typedef.h"
#include "vector2d.h"

class outputfile
{
public:
	outputfile(std::string, bool = true);
//#ifdef __SAVE_H__
	~outputfile() { if(Buffer) fclose(Buffer); }
	void Put(char What) { fputc(What, Buffer); }
	void Write(const char* Offset, long Size) { fwrite(Offset, 1, Size, Buffer); }
	bool IsOpen() { return Buffer != 0; }
private:
	FILE* Buffer;
/*#else
	void Put(char What) { Buffer.put(What); }
	void Write(const char* Offset, long Size) { Buffer.write(Offset, Size); }
	bool IsOpen() { return Buffer.is_open(); }
private:
	std::ofstream Buffer;
#endif*/
};

class inputfile
{
public:
	inputfile(std::string, bool = true);
	std::string ReadWord(bool = true);
	char ReadLetter(bool = true);
	long ReadNumber(std::map<std::string, long>, uchar = 0xFF);
	vector2d ReadVector2d(std::map<std::string, long>);
	bool ReadBool();
//#ifdef __SAVE_H__
	~inputfile() { if(Buffer) fclose(Buffer); }
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
/*#else
	int Get() { return Buffer.get(); }
	void Read(char* Offset, long Size) { Buffer.read(Offset, Size); }
	bool IsOpen() { return Buffer.is_open(); }
	bool Eof() { return Buffer.eof(); }
	void ClearFlags() { Buffer.clear(); }
	void SeekPosBeg(long Offset) { Buffer.seekg(Offset, std::ios::beg); }
	void SeekPosCur(long Offset) { Buffer.seekg(Offset, std::ios::cur); }
	void SeekPosEnd(long Offset) { Buffer.seekg(Offset, std::ios::end); }
	long TellPos() { return Buffer.tellg(); }
	int Peek() { return Buffer.peek(); }
private:
	std::ifstream Buffer;
#endif*/
};

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

outputfile& operator<<(outputfile&, std::string);

inputfile& operator>>(inputfile&, std::string&);

template <class type> inline outputfile& operator<<(outputfile& SaveFile, std::vector<type> Vector)
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

template <class type1, class type2> inline outputfile& operator<<(outputfile& SaveFile, std::map<type1, type2> Map)
{
	ulong Size = Map.size();

	SaveFile.Write((char*)&Size, sizeof(Size));

	for(std::map<ulong, uchar>::iterator i = Map.begin(); i != Map.end(); ++i)
		SaveFile << (*i).first << (*i).second;

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
