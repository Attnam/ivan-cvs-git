#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define ADD_MESSAGE globalmessagingsystem::AddMessage
#define DRAW_MESSAGES globalmessagingsystem::Draw
#define EMPTY_MESSAGES globalmessagingsystem::Empty

#include "typedef.h"

class list;

class globalmessagingsystem
{
public:
	static void AddMessage(const char*, ...);
	static void Draw(void);
	static void Empty(void);
	static void DrawMessageHistory(void);
	static void Format(void);
private:
	static char* MessageBuffer;
	static ushort BufferLength;
	static list MessageHistory;
};

#endif
