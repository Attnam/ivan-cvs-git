#define __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "item.h"

item::prototype** protocontainer<item>::ProtoData;
valuemap protocontainer<item>::CodeNameMap;
ushort protocontainer<item>::ProtoAmount;
itemprototype item_ProtoType(0, 0, "item");
const itemprototype* item::GetProtoType() const { return &item_ProtoType; }

#include "bodypart.h"
#include "gear.h"
#include "miscitem.h"

#undef __FILE_OF_STATIC_ITEM_PROTOTYPE_DEFINITIONS__

#include <ctime>

#include "char.h"
#include "message.h"
#include "database.h"
#include "stack.h"
#include "wskill.h"
#include "felist.h"
#include "confdef.h"
#include "room.h"
#include "game.h"
#include "materias.h"
#include "human.h"
#include "team.h"
#include "wskill.h"
#include "god.h"
#include "config.h"
#include "team.h"
#include "smoke.h"
#include "save.h"
#include "whandler.h"
#include "bitmap.h"

#include "item.cpp"
#include "bodypart.cpp"
#include "gear.cpp"
#include "miscitem.cpp"
