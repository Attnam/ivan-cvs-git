/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  See LICENSING which should included with this file
 *
 */

#define __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "item.h"

itemprototype** protocontainer<item>::ProtoData;
valuemap protocontainer<item>::CodeNameMap;
int protocontainer<item>::Size;
itemprototype item_ProtoType(0, 0, "item");
const itemprototype* item::GetProtoType() const { return &item_ProtoType; }

#include "bodypart.h"
#include "gear.h"
#include "miscitem.h"

#undef __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include <algorithm>
#include <ctime>

#include "char.h"
#include "message.h"
#include "database.h"
#include "stack.h"
#include "felist.h"
#include "confdef.h"
#include "room.h"
#include "game.h"
#include "materias.h"
#include "human.h"
#include "team.h"
#include "god.h"
#include "team.h"
#include "smoke.h"
#include "save.h"
#include "whandler.h"
#include "bitmap.h"

#include "fluid.h"
#include "rawbit.h"

#include "item.cpp"
#include "bodypart.cpp"
#include "gear.cpp"
#include "miscitem.cpp"
