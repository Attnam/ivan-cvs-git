#include "area.h"
#include "allocate.h"
#include "square.h"
#include "bitmap.h"
#include "charba.h"
#include "graphics.h"
#include "error.h"
#include "femath.h"
#include "game.h"
#include "save.h"

area::area(ushort InitXSize, ushort InitYSize)
{
  Initialize(InitXSize, InitYSize);
}

void area::Initialize(ushort InitXSize, ushort InitYSize)
{
  XSize = InitXSize;
  YSize = InitYSize;

  XSizeTimesYSize = XSize * YSize;

  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);

  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    FlagMap[0][c] = 0;
}

area::~area()
{
  for(ulong c = 0; c < XSizeTimesYSize; ++c)
    delete Map[0][c];

  delete [] Map;
  delete [] FlagMap;
}

void area::Save(outputfile& SaveFile) const
{
  SaveFile << XSize << YSize;
  SaveFile.Write((char*)FlagMap[0], sizeof(ushort) * XSizeTimesYSize);
}

void area::Load(inputfile& SaveFile)
{
  game::SetAreaInLoad(this);
  SaveFile >> XSize >> YSize;
  XSizeTimesYSize = XSize * YSize;
  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);
  SaveFile.Read((char*)FlagMap[0], sizeof(ushort) * XSizeTimesYSize);
}

void area::RemoveCharacter(vector2d Pos)
{
  Map[Pos.X][Pos.Y]->RemoveCharacter();
}

void area::AddCharacter(vector2d Pos, character* Guy)
{
  Map[Pos.X][Pos.Y]->AddCharacter(Guy);
}

void area::UpdateLOS()
{
  game::LOSTurn();

  ushort Radius = game::GetPlayer()->LOSRange();
  ulong RadiusSquare = Radius * Radius;

  DO_FILLED_RECTANGLE(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, 0, 0, GetXSize() - 1, GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(long(game::GetPlayer()->GetPos().X) - XPointer, long(game::GetPlayer()->GetPos().Y) - YPointer)) <= RadiusSquare)
      femath::DoLine(game::GetPlayer()->GetPos().X, game::GetPlayer()->GetPos().Y, XPointer, YPointer, game::LOSHandler);
  });

  game::RemoveLOSUpdateRequest();
}

void area::SendNewDrawRequest()
{
  ushort XMax = GetXSize() < game::GetCamera().X + game::GetScreenSize().X ? GetXSize() : game::GetCamera().X + game::GetScreenSize().X;
  ushort YMax = GetYSize() < game::GetCamera().Y + game::GetScreenSize().Y ? GetYSize() : game::GetCamera().Y + game::GetScreenSize().Y;

  for(ushort x = game::GetCamera().X; x < XMax; ++x)
    for(ushort y = game::GetCamera().Y; y < YMax; ++y)
      Map[x][y]->SendNewDrawRequest();

  DOUBLEBUFFER->Fill(0);
  DOUBLEBUFFER->DrawLine(14, 30, 17 + (game::GetScreenSize().X << 4), 30, DARKGRAY, true);
  DOUBLEBUFFER->DrawLine(14, 30, 14, 33 + (game::GetScreenSize().Y << 4), DARKGRAY, true);
  DOUBLEBUFFER->DrawLine(17 + (game::GetScreenSize().X << 4), 30, 17 + (game::GetScreenSize().X << 4), 33 + (game::GetScreenSize().Y << 4), DARKGRAY, true);
  DOUBLEBUFFER->DrawLine(14, 33 + (game::GetScreenSize().Y << 4), 17 + (game::GetScreenSize().X << 4), 33 + (game::GetScreenSize().Y << 4), DARKGRAY, true);
}

void area::MoveCharacter(vector2d From, vector2d To)
{
  character* Backup = GetSquare(From)->GetCharacter();
  GetSquare(From)->RemoveCharacter();
  GetSquare(To)->AddCharacter(Backup);
}

vector2d area::FreeSquareSeeker(character* Char, vector2d StartPos, vector2d Prohibited, uchar MaxDistance)
{
  DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
  {
    vector2d Vector = vector2d(DoX, DoY);

    if(GetSquare(Vector)->IsWalkable(Char) && !GetSquare(Vector)->GetCharacter() && Vector != Prohibited)
      return Vector;
  });

  if(MaxDistance)
    DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
    {
      vector2d Vector = vector2d(DoX, DoY);

      if(GetSquare(Vector)->IsWalkable(Char) && Vector != Prohibited)
	{
	  Vector = FreeSquareSeeker(Char, Vector, StartPos, MaxDistance - 1);

	  if(Vector.X != -1)
	    return Vector;
	}
    });

  return vector2d(-1, -1);
}

vector2d area::GetNearestFreeSquare(character* Char, vector2d StartPos)
{
  if(GetSquare(StartPos)->IsWalkable(Char) && !GetSquare(StartPos)->GetCharacter())
    return StartPos;

  DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
  {
    vector2d Vector = vector2d(DoX, DoY);

    if(GetSquare(Vector)->IsWalkable(Char) && !GetSquare(Vector)->GetCharacter())
      return Vector;
  });

  for(ushort c = 0; c < 20; ++c)
    DO_FOR_SQUARES_AROUND(StartPos.X, StartPos.Y, GetXSize(), GetYSize(),
    {
      vector2d Vector = vector2d(DoX, DoY);

      if(GetSquare(Vector)->IsWalkable(Char))
	{
	  Vector = FreeSquareSeeker(Char, Vector, StartPos, c);

	  if(Vector.X != -1)
	    return Vector;
	}
    });

  ABORT("No room for character. Character unhappy.");

  return vector2d(-1, -1);
}

void area::Draw() const
{
  ushort XMax = GetXSize() < game::GetCamera().X + game::GetScreenSize().X ? GetXSize() : game::GetCamera().X + game::GetScreenSize().X;
  ushort YMax = GetYSize() < game::GetCamera().Y + game::GetScreenSize().Y ? GetYSize() : game::GetCamera().Y + game::GetScreenSize().Y;

  if(!game::GetSeeWholeMapCheat())
    for(ushort x = game::GetCamera().X; x < XMax; ++x)
      for(ushort y = game::GetCamera().Y; y < YMax; ++y)
	if(Map[x][y]->GetLastSeen() == game::GetLOSTurns())
	  Map[x][y]->Draw();
	else
	  Map[x][y]->DrawMemorized();
  else
    for(ushort x = game::GetCamera().X; x < XMax; ++x)
      for(ushort y = game::GetCamera().Y; y < YMax; ++y)
	Map[x][y]->Draw();
}

