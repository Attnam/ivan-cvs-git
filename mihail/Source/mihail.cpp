#include <iostream>
#include <fstream>

#define HIGH_SCORE_FILENAME ""

#include "hscore.h"

int Main(int argc, char** argv)
{
  if(argc != 4)
    {
      std::cout << "\nUsage: mihail [1. input] [2. input] [output]" << std::endl;
      return 1;
    }

  highscore Input(argv[1]);
  highscore Output(argv[2]);
  Input.AddToFile(&Output);
  Output.Save(argv[3]);
  return 0;
}
