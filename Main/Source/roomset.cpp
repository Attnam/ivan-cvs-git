/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

#define __FILE_OF_STATIC_ROOM_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "room.h"

room::prototype** protocontainer<room>::ProtoData;
valuemap protocontainer<room>::CodeNameMap;
int protocontainer<room>::Size;

#include "rooms.h"

#undef __FILE_OF_STATIC_ROOM_PROTOTYPE_DEFINITIONS__

#include "message.h"
#include "char.h"
#include "god.h"
#include "game.h"
#include "stack.h"
#include "team.h"
#include "save.h"

#include "room.cpp"
#include "rooms.cpp"
