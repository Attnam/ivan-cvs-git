#ifndef __SQUARE_H__
#define __SQUARE_H__

#include <string>

#include "typedef.h"
#include "vector2d.h"

class area;
class material;
class bitmap;
class character;
class groundterrain;
class overterrain;
class outputfile;
class inputfile;
class squarescript;

class square
{
public:
	square(area*, vector2d);
	virtual ~square();
	virtual void Save(outputfile&) const;
	virtual void Load(inputfile&);
	virtual void DrawMemorized() const;
	virtual void UpdateMemorizedAndDraw() = 0;
	virtual void DrawToTileBuffer() const = 0;
	virtual void SetCharacter(character* What ) { Character = What; }
	virtual void AddCharacter(character*);
	virtual void RemoveCharacter();
	virtual character* GetCharacter() const		{ return Character; }
	virtual bool GetKnown() const				{ return Known; }
	virtual vector2d GetPos() const			{ return Pos; }
	virtual void SetKnown(bool What) { Known = What; }
	virtual ushort GetPopulation() const { if(Character) return 1; else return 0; }
	virtual area* GetAreaUnder() const { return AreaUnder; }
	virtual void EmptyFlag()			{ Flag = false; }
	virtual void SetFlag()			{ Flag = true; }
	virtual bool RetrieveFlag() const		{ return Flag; }
	virtual groundterrain* GetGroundTerrain() const = 0;
	virtual overterrain* GetOverTerrain() const = 0;
	virtual std::string GetMemorizedDescription() { return MemorizedDescription; }
	virtual void SetMemorizedDescription(std::string What) { MemorizedDescription = What; }
	virtual void UpdateMemorizedDescription() = 0;
	virtual bool CanBeSeen() const;
	virtual void DrawCheat() = 0;
	virtual void SendNewDrawRequest() { NewDrawRequested = true; }
	virtual bitmap* GetMemorized() const { return Memorized; }
	virtual void SetDescriptionChanged(bool) {}
	virtual bool CanBeSeenFrom(vector2d) const;
protected:
	std::string MemorizedDescription;
	area* AreaUnder;
	character* Character;
	vector2d Pos;
	bool Known, Flag, NewDrawRequested;
	bitmap* Memorized;
};

inline outputfile& operator<<(outputfile& SaveFile, square* Square)
{
	Square->Save(SaveFile);
	return SaveFile;
}

#endif
