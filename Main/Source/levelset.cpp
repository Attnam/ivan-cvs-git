#define __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "lterra.h"

glterrain::prototype** protocontainer<glterrain>::ProtoData;
olterrain::prototype** protocontainer<olterrain>::ProtoData;
valuemap protocontainer<glterrain>::CodeNameMap;
valuemap protocontainer<olterrain>::CodeNameMap;
ushort protocontainer<glterrain>::ProtoAmount;
ushort protocontainer<olterrain>::ProtoAmount;
glterrainprototype glterrain_ProtoType(0, 0, "glterrain");
olterrainprototype olterrain_ProtoType(0, 0, "olterrain");
const glterrainprototype* glterrain::GetProtoType() const { return &glterrain_ProtoType; }
const olterrainprototype* olterrain::GetProtoType() const { return &olterrain_ProtoType; }

#include "lterras.h"

#undef __FILE_OF_STATIC_LTERRAIN_PROTOTYPE_DEFINITIONS__

#include <algorithm>
#include <ctime>

#include "char.h"
#include "team.h"
#include "action.h"
#include "message.h"
#include "stack.h"
#include "config.h"
#include "miscitem.h"
#include "room.h"
#include "game.h"
#include "graphics.h"
#include "bitmap.h"
#include "god.h"
#include "bitmap.h"
#include "materias.h"
#include "confdef.h"
#include "fluid.h"
#include "database.h"
#include "nonhuman.h"
#include "smoke.h"
#include "save.h"
#include "allocate.h"
#include "whandler.h"

#include "level.cpp"
#include "lsquare.cpp"
#include "lterra.cpp"
#include "lterras.cpp"
