/*
 *
 *  Iter Vehemens ad Necem (IVAN)
 *  Copyright (C) Timo Kiviluoto
 *  Released under the GNU General
 *  Public License
 *
 *  See LICENSING which should be included
 *  along with this file for more details
 *
 */

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

	for(festring::sizetype c = 0; c < Input.GetSize(); ++c)
	    std::cout << c + 1 << " " << Input.GetEntry(c).CStr() << " " << Input.GetScore(c) << "\n";

	return 0;
    }

    if(argc == 4)
    {
	highscore Input;
	Input.Load(argv[1]);
//	    return 2;
	highscore Output;

	Output.Load(argv[2]);	
	if(!Input.MergeToFile(&Output))
	    std::cout << "\n All entries also found in " << argv[2] << ". Oh well.\n";

	Output.Save(argv[3]);
	return 0;
    }

    std::cout << "\nTo list scores in highscore file:\n    mihail -l [filename] \n\nTo merge 2 files\n    mihail [1st input filename] [2nd input filename] [output filename] \n\n";
    return 1;
}
