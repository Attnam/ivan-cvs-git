#define __FILE_OF_STATIC_GOD_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "god.h"

god::prototype** protocontainer<god>::ProtoData;
valuemap protocontainer<god>::CodeNameMap;
int protocontainer<god>::Size;

#include "gods.h"

#undef __FILE_OF_STATIC_GOD_PROTOTYPE_DEFINITIONS__

#include <algorithm>

#include "human.h"
#include "message.h"
#include "game.h"
#include "nonhuman.h"
#include "stack.h"
#include "lterras.h"
#include "gear.h"
#include "miscitem.h"
#include "confdef.h"
#include "materias.h"
#include "save.h"
#include "team.h"

#include "god.cpp"
#include "gods.cpp"
