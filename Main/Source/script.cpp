#include "script.h"

/*void groundterrainscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(SaveFile.ReadWord() == "=")
		Word = SaveFile.ReadWord();

	for(ushort c = 1; protocontainer<material>::GetProto(c); ++c)
		if(protocontainer<material>::GetProto(c)->ClassName() == Word)
		{
			MaterialType = c;
			OverrideMap |= 1;
			Word = SaveFile.ReadWord();
			break;
		}

	for(ushort c = 1; protocontainer<groundlevelterrain>::GetProto(c); ++c)
		if(protocontainer<groundlevelterrain>::GetProto(c)->ClassName() == Word)
		{
			TerrainType = c;
			OverrideMap |= 2;
			Word = SaveFile.ReadWord();
			break;
		}

	if(Word != ";")
		ABORT("Script error in GroundTerrainScript!");
}

void overterrainscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(SaveFile.ReadWord() == "=")
		Word = SaveFile.ReadWord();

	for(ushort c = 1; protocontainer<material>::GetProto(c); ++c)
		if(protocontainer<material>::GetProto(c)->ClassName() == Word)
		{
			MaterialType = c;
			OverrideMap |= 1;
			Word = SaveFile.ReadWord();
			break;
		}

	for(ushort c = 1; protocontainer<overlevelterrain>::GetProto(c); ++c)
		if(protocontainer<overlevelterrain>::GetProto(c)->ClassName() == Word)
		{
			TerrainType = c;
			OverrideMap |= 2;
			Word = SaveFile.ReadWord();
			break;
		}

	if(Word != ";")
		ABORT("Script error in GroundTerrainScript!");
}

void characterscript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(SaveFile.ReadWord() == "=")
		Word = SaveFile.ReadWord();

	for(ushort c = 1; protocontainer<material>::GetProto(c); ++c)
		if(protocontainer<material>::GetProto(c)->ClassName() == Word)
		{
			MaterialType = c;
			OverrideMap |= 1;
			Word = SaveFile.ReadWord();
			break;
		}

	for(ushort c = 1; protocontainer<character>::GetProto(c); ++c)
		if(protocontainer<character>::GetProto(c)->ClassName() == Word)
		{
			TerrainType = c;
			OverrideMap |= 2;
			Word = SaveFile.ReadWord();
			break;
		}

	if(Word != ";")
		ABORT("Script error in GroundTerrainScript!");
}

void squarescript::ReadFrom(inputfile& SaveFile)
{
	std::string Word = SaveFile.ReadWord();

	if(Word == "GroundTerrain")
		GroundTerrain.ReadFrom(SaveFile);

	if(Word == "GroundTerrain")
		GroundTerrain.ReadFrom(SaveFile);
}

void levelscript::ReadFrom(inputfile& SaveFile)
{
}

void roomscript::ReadFrom(inputfile& SaveFile)
{
}*/
