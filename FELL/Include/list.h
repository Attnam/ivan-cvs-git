#ifndef __LIST_H__
#define __LIST_H__

#include <string>

#include "dynarray.h"

class list
{
public:
	list(ushort Maximum = 0) : Maximum(Maximum) {}
	list(std::string Topic, ushort Maximum = 0) : Maximum(Maximum) { Description.Add(Topic); }
	void AddString(std::string S);
	void AddDescription(std::string S) { Description.Add(S); }
	void DrawDescription(void) const;
	ushort Draw(bool = true) const;
	void Empty(void);
	std::string GetString(ushort Index) { return String.Access(Index); }
	ushort Length(void) const { return String.Length(); }
protected:
	dynarray<std::string> String;
	dynarray<std::string> Description;
	std::string Topic;
	ushort Maximum;
};

#endif
