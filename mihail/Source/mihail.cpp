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
#include "save.h"

truth DebugDraw(festring Filename);

int Main(int argc, char** argv)
{
    if(argc == 3)
    {
	festring Arg = festring(argv[1]);
	if(Arg == "-l") 
	{
	    highscore Input(argv[2]);
	    std::cout << std::endl;

	    for(festring::sizetype c = 0; c < Input.GetSize(); ++c)
		std::cout << c + 1 << " " << Input.GetEntry(c).CStr() << " " << Input.GetScore(c) << "\n";

	    return 0;
	} 
	else if(Arg == "-d")
	{
	    if(!DebugDraw(argv[2])) 
	    {
		std::cout << "\nProblem showing version number\n";
		return 1;
	    }
	    return 0;
	}
	else 
	    return 1;
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

    std::cout << "\nTo list scores in highscore file:\n    mihail -l [filename] \n\nTo merge 2 files\n    mihail [1st input filename] [2nd input filename] [output filename] \n\nTo give version number of highscore file\n    mihail -d [filename]\n\n";
    return 1;
}

truth DebugDraw(festring Filename) 
{
    {
	inputfile HighScore(Filename, 0, false);

	if(!HighScore.IsOpen())
	    return false;

	HighScore.Get();

	if(HighScore.Eof())
	    return false; 
    }
    inputfile HighScore(Filename, 0, false);
    ushort HVersion;
    HighScore >> HVersion;
    std::cout << "\nFile is version " << HVersion << std::endl;
    return true;
}
