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

#define __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "action.h"

action::prototype** protocontainer<action>::ProtoData;
valuemap protocontainer<action>::CodeNameMap;
int protocontainer<action>::Size;

#include "actions.h"

#undef __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__

#include "message.h"
#include "char.h"
#include "materia.h"
#include "game.h"
#include "save.h"
#include "iconf.h"
#include "stack.h"

#include "action.cpp"
#include "actions.cpp"
