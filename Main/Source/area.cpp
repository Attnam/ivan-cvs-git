/* Compiled through areaset.cpp */

#include "allocate.h"
#include "char.h"
#include "team.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"

area::area() { }

area::area(ushort InitXSize, ushort InitYSize)
{
  Initialize(InitXSize, InitYSize);
}

void area::Initialize(ushort InitXSize, ushort InitYSize)
{
  XSize = InitXSize;
  YSize = InitYSize;
  XSizeTimesYSize = XSize * YSize;
  Border = rect(0, 0, XSize - 1, YSize - 1);
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
  SaveFile << XSize << YSize << EntryMap;
  SaveFile.Write(reinterpret_cast<char*>(FlagMap[0]), sizeof(ushort) * XSizeTimesYSize);
}

void area::Load(inputfile& SaveFile)
{
  game::SetAreaInLoad(this);
  SaveFile >> XSize >> YSize >> EntryMap;
  XSizeTimesYSize = XSize * YSize;
  Border = rect(0, 0, XSize - 1, YSize - 1);
  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);
  SaveFile.Read(reinterpret_cast<char*>(FlagMap[0]), sizeof(ushort) * XSizeTimesYSize);
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
  ushort Radius = PLAYER->GetLOSRange();
  ulong RadiusSquare = Radius * Radius;
  vector2d Pos = PLAYER->GetPos();

  ushort MaxDist = Pos.X;

  if(Pos.Y > MaxDist)
    MaxDist = Pos.Y;

  if(XSize - Pos.X - 1 > MaxDist)
    MaxDist = XSize - Pos.X - 1;

  if(YSize - Pos.Y - 1 > MaxDist)
    MaxDist = YSize - Pos.Y - 1;

  if(Radius > MaxDist)
    Radius = MaxDist;

  bool (*LOSHandler)(long, long) = game::IsInWilderness() ? game::WorldMapLOSHandler : game::LevelLOSHandler;

  rect Rect;
  femath::CalculateEnvironmentRectangle(Rect, Border, Pos, Radius);

  for(ushort x = Rect.X1; x <= Rect.X2; ++x)
    for(ushort y = Rect.Y1; y <= Rect.Y2; ++y)
      if(ulong(HypotSquare(Pos.X - x, Pos.Y - y)) <= RadiusSquare)
	femath::DoLine(Pos.X, Pos.Y, x, y, LOSHandler);

  if(PLAYER->StateIsActivated(INFRA_VISION) && !game::IsInWilderness())
    for(ushort c = 0; c < game::GetTeams(); ++c)
      for(std::list<character*>::const_iterator i = game::GetTeam(c)->GetMember().begin(); i != game::GetTeam(c)->GetMember().end(); ++i)
	if((*i)->IsEnabled())
	  (*i)->GetSquareUnder()->SendNewDrawRequest();

  game::RemoveLOSUpdateRequest();
}

void area::SendNewDrawRequest()
{
  for(ushort x = game::GetCamera().X; x < XSize && x < game::GetCamera().X + game::GetScreenSize().X; ++x)
    for(ushort y = game::GetCamera().Y; y < YSize && y < game::GetCamera().Y + game::GetScreenSize().Y; ++y)
      Map[x][y]->SendNewDrawRequest();

  DOUBLE_BUFFER->ClearToColor(0);
  DOUBLE_BUFFER->DrawRectangle(14, 30, 17 + (game::GetScreenSize().X << 4), 33 + (game::GetScreenSize().Y << 4), DARK_GRAY, true);
}

void area::MoveCharacter(vector2d From, vector2d To)
{
  character* Backup = GetSquare(From)->GetCharacter();
  GetSquare(From)->RemoveCharacter();
  GetSquare(To)->AddCharacter(Backup);
}

square* area::GetNeighbourSquare(vector2d Pos, ushort Index) const
{
  Pos += game::GetMoveVector(Index);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

void area::SetEntryPos(uchar Index, vector2d Pos)
{
  EntryMap.insert(std::pair<uchar, vector2d>(Index, Pos));
}
