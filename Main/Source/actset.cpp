#define __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "action.h"

action::prototype** protocontainer<action>::ProtoData;
valuemap protocontainer<action>::CodeNameMap;
ushort protocontainer<action>::ProtoAmount;

#include "actions.h"

#undef __FILE_OF_STATIC_ACTION_PROTOTYPE_DEFINITIONS__

#include "message.h"
#include "char.h"
#include "materia.h"
#include "game.h"
#include "save.h"

#include "action.cpp"
#include "actions.cpp"
