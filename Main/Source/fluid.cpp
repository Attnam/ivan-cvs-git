#include "fluid.h"
#include "bitmap.h"
#include "femath.h"
#include "graphics.h"
#include "game.h"
#include "lsquare.h"
#include "materba.h"
#include "igraph.h"

fluid::fluid()
{
  SetHasBe(true);
  Picture = new bitmap(16, 16);
  Picture->Fill(TRANSPARENTCOL);
  Picture->CreateAlphaMap(0);
}

fluid::~fluid()
{
  delete Picture;
}

void fluid::SpillFluid(uchar Amount, ulong Color, ushort Lumpiness, ushort Variation)
{
  for(ushort c = 0; c < Amount; ++c)
    {
      vector2d Cords(1 + RAND() % 14, 1 + RAND() % 14);
      GetPicture()->PutPixel(Cords, Color);
      GetPicture()->SetAlpha(Cords, 150 + RAND() % 106);

      for(ushort d = 0; d < 8; ++d)
	if(RAND() % Lumpiness)
	  {
	    char Change[3];

	    for(ushort x = 0; x < 3; ++x)
	      Change[x] = RAND() % Variation - RAND() % Variation;

	    if(short(GET_RED(Color) + Change[0]) < 0) Change[0] = -GET_RED(Color);
	    if(short(GET_GREEN(Color) + Change[1]) < 0) Change[1] = -GET_GREEN(Color);
	    if(short(GET_BLUE(Color) + Change[2]) < 0) Change[2] = -GET_BLUE(Color);

	    if(short(GET_RED(Color) + Change[0]) > 255) Change[0] = 255 - GET_RED(Color);
	    if(short(GET_GREEN(Color) + Change[1]) > 255) Change[1] = 255 - GET_GREEN(Color);
	    if(short(GET_BLUE(Color) + Change[2]) > 255) Change[2] = 255 - GET_BLUE(Color);

	    GetPicture()->PutPixel(Cords + game::GetMoveVector(d), MAKE_RGB(GET_RED(Color) + Change[0], GET_GREEN(Color) + Change[1], GET_BLUE(Color) + Change[2]));
	    GetPicture()->SetAlpha(Cords + game::GetMoveVector(d), 50 + RAND() % 206);
	  }
    }
}

void fluid::Be()
{
  if(!(RAND() % 25))
    {
      if(!GetPicture()->ChangeAlpha(-1))
	{
	  GetLSquareUnder()->RemoveFluid();
	  SetExists(false);
	}

      GetSquareUnder()->SendNewDrawRequest();
      GetSquareUnder()->SendMemorizedUpdateRequest();
    }
}

void fluid::Save(outputfile& SaveFile) const
{
  unit::Save(SaveFile);
  SaveFile << Picture;
}

void fluid::Load(inputfile& SaveFile)
{
  unit::Load(SaveFile);
  SaveFile >> Picture;
}

void fluid::DrawToTileBuffer() const
{
  Picture->AlphaBlit(igraph::GetTileBuffer());
}

