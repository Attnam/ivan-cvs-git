#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include <allegro.h>
#include <string>
#include <fstream>
class rectangle;

class program
{
 public:
  static void Init();
  static void Run();
  static void DeInit();
  static void SetRunning(bool What) { Running = What; }
  static std::string ReadNextWord(ifstream*);
 private:
  static bool Running;
};

#endif

