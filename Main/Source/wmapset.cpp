#define __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "wterra.h"

std::vector<gwterrain::prototype*> protocontainer<gwterrain>::ProtoData;
std::vector<owterrain::prototype*> protocontainer<owterrain>::ProtoData;
valuemap protocontainer<gwterrain>::CodeNameMap;
valuemap protocontainer<owterrain>::CodeNameMap;

#include "wterras.h"

#undef __FILE_OF_STATIC_WTERRAIN_PROTOTYPE_DEFINITIONS__

#include <algorithm>

#include "allocate.h"
#include "char.h"
#include "cont.h"
#include "game.h"
#include "festring.h"
#include "cont.h"
#include "femath.h"
#include "config.h"
#include "graphics.h"
#include "whandler.h"
#include "message.h"
#include "igraph.h"
#include "bitmap.h"
#include "team.h"

#include "cont.cpp"
#include "worldmap.cpp"
#include "wsquare.cpp"
#include "wterra.cpp"
#include "wterras.cpp"
