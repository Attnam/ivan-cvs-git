#define __FILE_OF_STATIC_ROOM_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "room.h"

std::vector<room::prototype*> protocontainer<room>::ProtoData;
valuemap protocontainer<room>::CodeNameMap;

#include "rooms.h"

#undef __FILE_OF_STATIC_ROOM_PROTOTYPE_DEFINITIONS__

#include "message.h"
#include "char.h"
#include "god.h"
#include "game.h"
#include "stack.h"
#include "team.h"

#include "room.cpp"
#include "rooms.cpp"
