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

#define __FILE_OF_STATIC_TRAP_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "trap.h"
#include "felibdef.h"
#include "igraph.h"
#include "rawbit.h"

trap::prototype** protocontainer<trap>::ProtoData;
valuemap protocontainer<trap>::CodeNameMap;
int protocontainer<trap>::Size;

#include "traps.h"

#undef __FILE_OF_STATIC_TRAP_PROTOTYPE_DEFINITIONS__

#include "lsquare.h"
#include "game.h"
#include "char.h"
#include "message.h"
#include "lsquare.h"
#include "bitmap.h"


#include "trap.cpp"
#include "traps.cpp"
