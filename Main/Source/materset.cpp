#define __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "proto.h"
#include "materia.h"

std::vector<material::prototype*> protocontainer<material>::ProtoData;
valuemap protocontainer<material>::CodeNameMap;
material* material_Clone(ushort Config, ulong Volume, bool Load) { return new material(Config, Volume, Load); }
materialprototype material_ProtoType(0, &material_Clone, "material");
const materialprototype* material::GetProtoType() const { return &material_ProtoType; }

#include "materias.h"

#undef __FILE_OF_STATIC_MATERIAL_PROTOTYPE_DEFINITIONS__

#include "char.h"
#include "festring.h"
#include "database.h"
#include "confdef.h"
#include "message.h"

#include "materia.cpp"
#include "materias.cpp"
