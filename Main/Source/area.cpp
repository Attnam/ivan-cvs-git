/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

/* Compiled through areaset.cpp */

#include "allocate.h"
#include "char.h"
#include "team.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"

area::area() { }

area::area(int InitXSize, int InitYSize)
{
  Initialize(InitXSize, InitYSize);
}

void area::Initialize(int InitXSize, int InitYSize)
{
  XSize = InitXSize;
  YSize = InitYSize;
  XSizeTimesYSize = XSize * YSize;
  Border = rect(0, 0, XSize - 1, YSize - 1);
  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);
  memset(FlagMap[0], 0, XSizeTimesYSize * sizeof(uchar));
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
  SaveFile.Write(reinterpret_cast<char*>(FlagMap[0]), XSizeTimesYSize * sizeof(uchar));
}

void area::Load(inputfile& SaveFile)
{
  game::SetAreaInLoad(this);
  SaveFile >> XSize >> YSize >> EntryMap;
  XSizeTimesYSize = XSize * YSize;
  Border = rect(0, 0, XSize - 1, YSize - 1);
  Alloc2D(Map, XSize, YSize);
  Alloc2D(FlagMap, XSize, YSize);
  SaveFile.Read(reinterpret_cast<char*>(FlagMap[0]), XSizeTimesYSize * sizeof(uchar));
}

void area::SendNewDrawRequest()
{
  for(int x = game::GetCamera().X; x < XSize && x < game::GetCamera().X + game::GetScreenXSize(); ++x)
    for(int y = game::GetCamera().Y; y < YSize && y < game::GetCamera().Y + game::GetScreenYSize(); ++y)
      Map[x][y]->SendStrongNewDrawRequest();

  DOUBLE_BUFFER->ClearToColor(0);
  DOUBLE_BUFFER->DrawRectangle(14, 30, 17 + (game::GetScreenXSize() << 4), 33 + (game::GetScreenYSize() << 4), DARK_GRAY, true);
}

square* area::GetNeighbourSquare(vector2d Pos, int I) const
{
  Pos += game::GetMoveVector(I);

  if(Pos.X >= 0 && Pos.Y >= 0 && Pos.X < XSize && Pos.Y < YSize)
    return Map[Pos.X][Pos.Y];
  else
    return 0;
}

void area::SetEntryPos(int I, vector2d Pos)
{
  EntryMap.insert(std::pair<int, vector2d>(I, Pos));
}
