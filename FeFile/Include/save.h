#ifndef __SAVE_H__
#define __SAVE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <fstream>
#include <vector>
#include <map>
#include <queue>

#include "typedef.h"
#include "vector2d.h"

class outputfile
{
public:
	outputfile(std::string, bool = true);
	std::ofstream& GetBuffer() { return Buffer; }
private:
	std::ofstream Buffer;
};

class inputfile
{
public:
	inputfile(std::string, bool = true);
	std::ifstream& GetBuffer() { return Buffer; }
	std::string ReadWord(bool = true);
	char ReadLetter(bool = true);
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

template <class type1, class type2> inline outputfile& operator<<(outputfile& SaveFile, std::map<type1, type2> Map)
{
	ulong Size = Map.size();

	SaveFile.GetBuffer().write((char*)&Size, sizeof(Size));

	for(std::map<ulong, uchar>::iterator i = Map.begin(); i != Map.end(); ++i)
		SaveFile << (*i).first << (*i).second;

	return SaveFile;
}

template <class type1, class type2> inline inputfile& operator>>(inputfile& SaveFile, std::map<type1, type2>& Map)
{
	ulong Size;

	SaveFile.GetBuffer().read((char*)&Size, sizeof(Size));

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



