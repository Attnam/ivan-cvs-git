#ifndef __SAVE_H__
#define __SAVE_H__

#include <vector>

#include "typedef.h"
#include "vector.h"

inline std::ofstream& operator<<(std::ofstream& SaveFile, uchar Value)
{
	SaveFile.put(Value);
	return SaveFile;
}

inline std::ifstream& operator>>(std::ifstream& SaveFile, uchar& Value)
{
	Value = SaveFile.get();
	return SaveFile;
}

inline std::ofstream& operator<<(std::ofstream& SaveFile, vector Vector)
{
	SaveFile.write((char*)&Vector, sizeof(Vector));
	return SaveFile;
}

inline std::ifstream& operator>>(std::ifstream& SaveFile, vector& Vector)
{
	SaveFile.read((char*)&Vector, sizeof(Vector));
	return SaveFile;
}

template <class type> inline std::ofstream& operator<<(std::ofstream& SaveFile, std::vector<type> Vector)
{
	ulong Size = Vector.size();

	SaveFile.write((char*)&Size, sizeof(Size));

	for(ulong c = 0; c < Vector.size(); c++)
		SaveFile << Vector[c];

	return SaveFile;
}

template <class type> inline std::ifstream& operator>>(std::ifstream& SaveFile, std::vector<type>& Vector)
{
	ulong Size;

	SaveFile.read((char*)&Size, sizeof(Size));

	Vector.resize(Size, type());

	for(ulong c = 0; c < Vector.size(); c++)
		SaveFile >> Vector[c];

	return SaveFile;
}

#endif
