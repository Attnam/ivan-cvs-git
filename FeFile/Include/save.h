#ifndef __SAVE_H__
#define __SAVE_H__

#pragma warning(disable : 4786)

#include <fstream>
#include <vector>
#include <map>

#include "typedef.h"
#include "vector2d.h"

class outputfile
{
public:
	outputfile(std::string FileName) : Buffer(FileName.c_str(), std::ios::out | std::ios::binary) {}
	std::ofstream& GetBuffer() { return Buffer; }
private:
	std::ofstream Buffer;
};

class inputfile
{
public:
	inputfile(std::string FileName) : Buffer(FileName.c_str(), std::ios::in | std::ios::binary) {}
	std::ifstream& GetBuffer() { return Buffer; }
	std::string ReadWord(bool = true);
	long ReadNumber(std::map<std::string, long>, uchar = 0xFF);
	vector2d ReadVector2d(std::map<std::string, long>);
	bool ReadBool();
private:
	std::ifstream Buffer;
};

inline outputfile& operator<<(outputfile& SaveFile, bool Value)
{
	SaveFile.GetBuffer().put(Value);
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, bool& Value)
{
	Value = SaveFile.GetBuffer().get() ? true : false;
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, char Value)
{
	SaveFile.GetBuffer().put(Value);
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, char& Value)
{
	Value = SaveFile.GetBuffer().get();
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, uchar Value)
{
	SaveFile.GetBuffer().put(Value);
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, uchar& Value)
{
	Value = SaveFile.GetBuffer().get();
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, short Value)
{
	SaveFile.GetBuffer().put(Value);
	SaveFile.GetBuffer().put(Value >> 8);
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, short& Value)
{
	Value  = SaveFile.GetBuffer().get();
	Value |= SaveFile.GetBuffer().get() << 8;
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ushort Value)
{
	SaveFile.GetBuffer().put(Value);
	SaveFile.GetBuffer().put(Value >> 8);
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ushort& Value)
{
	Value  = SaveFile.GetBuffer().get();
	Value |= SaveFile.GetBuffer().get() << 8;
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, long Value)
{
	SaveFile.GetBuffer().write((char*)&Value, sizeof(Value));
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, long& Value)
{
	SaveFile.GetBuffer().read((char*)&Value, sizeof(Value));
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, float Value)
{
	SaveFile.GetBuffer().write((char*)&Value, sizeof(Value));
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, float& Value)
{
	SaveFile.GetBuffer().read((char*)&Value, sizeof(Value));
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, ulong Value)
{
	SaveFile.GetBuffer().write((char*)&Value, sizeof(Value));
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, ulong& Value)
{
	SaveFile.GetBuffer().read((char*)&Value, sizeof(Value));
	return SaveFile;
}

inline outputfile& operator<<(outputfile& SaveFile, vector2d Vector)
{
	SaveFile.GetBuffer().write((char*)&Vector, sizeof(Vector));
	return SaveFile;
}

inline inputfile& operator>>(inputfile& SaveFile, vector2d& Vector)
{
	SaveFile.GetBuffer().read((char*)&Vector, sizeof(Vector));
	return SaveFile;
}

outputfile& operator<<(outputfile&, std::string);

inputfile& operator>>(inputfile&, std::string&);

template <class type> inline outputfile& operator<<(outputfile& SaveFile, std::vector<type> Vector)
{
	ulong Size = Vector.size();

	SaveFile.GetBuffer().write((char*)&Size, sizeof(Size));

	for(ulong c = 0; c < Vector.size(); ++c)
		SaveFile << Vector[c];

	return SaveFile;
}

template <class type> inline inputfile& operator>>(inputfile& SaveFile, std::vector<type>& Vector)
{
	ulong Size;

	SaveFile.GetBuffer().read((char*)&Size, sizeof(Size));

	Vector.resize(Size, type());

	for(ulong c = 0; c < Vector.size(); ++c)
		SaveFile >> Vector[c];

	return SaveFile;
}

#endif
