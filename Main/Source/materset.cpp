#define __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "materia.h"

material::prototype** protocontainer<material>::ProtoData;
valuemap protocontainer<material>::CodeNameMap;
ushort protocontainer<material>::ProtoAmount;
material* material_Clone(ushort Config, ulong Volume, bool Load) { return new material(Config, Volume, Load); }
materialprototype material_ProtoType(0, &material_Clone, "material");
const materialprototype* material::GetProtoType() const { return &material_ProtoType; }

#include "materias.h"

#undef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "char.h"
#include "database.h"
#include "confdef.h"
#include "message.h"
#include "save.h"
#include "fluid.h"
#include "smoke.h"
#include "bitmap.h"
#include "game.h"
#include "colorbit.h"
#include "whandler.h"

#include "materia.cpp"
#include "materias.cpp"
#include "fluid.cpp"
#include "smoke.cpp"
