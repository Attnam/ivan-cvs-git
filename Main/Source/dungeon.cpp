#include "dungeon.h"

void dungeon::Generate(void)
{
	Areas = 3;

	BlockMap = new uchar**[Areas];

	for(uchar c = 0; c < Areas; c++)
	{
		BlockMap[c] = new uchar*[40];

		for(uchar q = 0; q < 40; q++)
			BlockMap[c][q] = new uchar[40];
	}

	for(uchar c = 0; c < Areas; c++)
		Area[c] = new area(vector(40, 40), vector(0, 0), c);

	for(uchar c = 0; c < Areas; c++)
		Area[c]->Generate();

	Area[0]->Generate

	for(uchar c = 0; c < Areas - 1; c++)
	{
		
	}
}


