#include <iostream>
#include <fstream>
#include <sstream>

#include "hscore.h"

int Main(int argc, char** argv)
{
  if(argc == 3)
    {
      if(festring(argv[1]) != "-l")
	return 1;

      highscore Input(argv[2]);
      std::cout << std::endl;

      for(ushort c = 0; c < Input.GetSize(); ++c)
	std::cout << c << " " << Input.GetEntry(c).CStr() << " " << Input.GetScore(c) << "\n";

      return 0;
    }

  if(argc == 4)
    {
      highscore Input(argv[1]);
      highscore Output(argv[2]);

      if(!Input.MergeToFile(&Output))
	std::cout << "\n All entries also found in " << argv[2] << ". Oh well.\n";

      Output.Save(argv[3]);
      return 0;
    }

  std::cout << "\nTo list scores in highscore file:\n    mihail -l [filename] \n\nTo merge 2 files\n    mihail [1st input filename] [2nd input filename] [output filename] \n\n";
  return 1;
}
