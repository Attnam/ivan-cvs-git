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
  long PosX = game::GetPlayer()->GetPos().X;
  long PosY = game::GetPlayer()->GetPos().Y;

  ushort MaxDist = PosX;

  if(PosY > MaxDist)
    MaxDist = PosY;

  if(GetXSize() - PosX - 1 > MaxDist)
    MaxDist = GetXSize() - PosX - 1;

  if(GetYSize() - PosY - 1 > MaxDist)
    MaxDist = GetYSize() - PosY - 1;

  if(Radius > MaxDist)
    Radius = MaxDist;

  bool (*LOSHandler)(long, long) = game::IsInWilderness() ? game::WorldMapLOSHandler : game::LevelLOSHandler;

  DO_FILLED_RECTANGLE(PosX, PosY, 0, 0, GetXSize() - 1, GetYSize() - 1, Radius,
  {
    if(ulong(GetHypotSquare(PosX - XPointer, PosY - YPointer)) <= RadiusSquare)
      femath::DoLine(PosX, PosY, XPointer, YPointer, LOSHandler);
  });

  game::RemoveLOSUpdateRequest();
}

void area::SendNewDrawRequest()
{
  for(ushort x = game::GetCamera().X; x < game::GetCamera().X + game::GetScreenSize().X; ++x)
    for(ushort y = game::GetCamera().Y; y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
      Map[x][y]->SendNewDrawRequest();

  DOUBLEBUFFER->Fill(0);
  DOUBLEBUFFER->DrawRectangle(14, 30, 17 + (game::GetScreenSize().X << 4), 33 + (game::GetScreenSize().Y << 4), DARKGRAY, true);
}

void area::MoveCharacter(vector2d From, vector2d To)
{
  character* Backup = GetSquare(From)->GetCharacter();
  GetSquare(From)->RemoveCharacter();
  GetSquare(To)->AddCharacter(Backup);
}

vector2d area::FreeSquareSeeker(character* Char, vector2d StartPos, vector2d Prohibited, uchar MaxDistance)
{
  ushort c;

  for(c = 0; c < 8; ++c)
    {
      vector2d Pos = StartPos + game::GetMoveVector(c);

      if(IsValidPos(Pos) && GetSquare(Pos)->IsWalkable(Char) && !GetSquare(Pos)->GetCharacter() && Pos != Prohibited)
	return Pos;
    }

  if(MaxDistance)
    for(c = 0; c < 8; ++c)
      {
	vector2d Pos = StartPos + game::GetMoveVector(c);

	if(IsValidPos(Pos))
	  {
	    if(GetSquare(Pos)->IsWalkable(Char) && Pos != Prohibited)
	      {
		Pos = FreeSquareSeeker(Char, Pos, StartPos, MaxDistance - 1);

		if(Pos.X != -1)
		  return Pos;
	      }
	  }
      }

  return vector2d(-1, -1);
}

vector2d area::GetNearestFreeSquare(character* Char, vector2d StartPos)
{
  if(GetSquare(StartPos)->IsWalkable(Char) && !GetSquare(StartPos)->GetCharacter())
    return StartPos;

  ushort c;

  for(c = 0; c < 8; ++c)
    {
      vector2d Pos = StartPos + game::GetMoveVector(c);

      if(IsValidPos(Pos) && GetSquare(Pos)->IsWalkable(Char) && !GetSquare(Pos)->GetCharacter())
	return Pos;
    }

  for(ushort Dist = 0; Dist < 20; ++Dist)
    for(c = 0; c < 8; ++c)
      {
	vector2d Pos = StartPos + game::GetMoveVector(c);

	if(IsValidPos(Pos) && GetSquare(Pos)->IsWalkable(Char))
	  {
	    Pos = FreeSquareSeeker(Char, Pos, StartPos, Dist);

	    if(Pos.X != -1)
	      return Pos;
	  }
      }

  ABORT("No room for character. Character unhappy.");
  return vector2d(-1, -1);
}

square* area::GetNeighbourSquare(vector2d Pos, ushort Index) const
{
  Pos += game::GetMoveVector(Index);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}
