#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#define ADD_MESSAGE msgsystem::AddMessage

#include <string>

#include "typedef.h"

class felist;
class outputfile;
class inputfile;

class msgsystem
{
 public:
  static void AddMessage(const char*, ...);
  static void Draw();
  static void DrawMessageHistory();
  static void Format();
  static void Save(outputfile&);
  static void Load(inputfile&);
  static void ScrollDown();
  static void ScrollUp();
 private:
  static felist MessageHistory;
  static std::string LastMessage;
  static ushort Times;
  static ulong Begin, End;
};

#endif

