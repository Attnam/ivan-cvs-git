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

#define __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "char.h"

characterprototype** protocontainer<character>::ProtoData;
valuemap protocontainer<character>::CodeNameMap;
int protocontainer<character>::Size;
characterprototype character_ProtoType(0, 0, "character");
const characterprototype* character::GetProtoType() const { return &character_ProtoType; }

#include "human.h"
#include "nonhuman.h"

#undef __FILE_OF_STATIC_CHARACTER_PROTOTYPE_DEFINITIONS__

#include "stack.h"
#include "message.h"
#include "actions.h"
#include "wterras.h"
#include "rawbit.h"
#include "team.h"
#include "iconf.h"
#include "god.h"
#include "felist.h"
#include "miscitem.h"
#include "gear.h"
#include "confdef.h"
#include "room.h"
#include "game.h"
#include "graphics.h"
#include "materias.h"
#include "bitmap.h"
#include "lterras.h"
#include "save.h"
#include "database.h"
#include "traps.h"

#include "human.cpp"
#include "nonhuman.cpp"
