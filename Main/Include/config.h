#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class configuration
{
public:
	static std::string GetDefaultName() { return DefaultName; }
	static void SetDefaultName(std::string);
	static ushort GetAutosaveInterval() { return AutosaveInterval; }
	static void SetAutosaveInterval(long);
	static void EditContrast(char);
	static uchar GetContrast() { return Contrast; }
	static void SetContrast(long);
	static bool GetFullScreenMode() { return FullScreenMode; }
	static void SetFullScreenMode(bool What) { FullScreenMode = What; }
	static bool GetBeepOnCritical() { return BeepOnCritical; }
	static void SetBeepOnCritical(bool What) { BeepOnCritical = What; }
	static bool GetAutodropLeftOvers() { return AutodropLeftOvers; }
	static void SetAutodropLeftOvers(bool What) { AutodropLeftOvers = What; }
	static bool GetOutlineCharacters() { return OutlineCharacters; }
	static void SetOutlineCharacters(bool What) { OutlineCharacters = What; }
	static bool GetOutlineItems() { return OutlineItems; }
	static void SetOutlineItems(bool What) { OutlineItems = What; }
	static ushort GetCharacterOutlineColor() { return CharacterOutlineColor; }
	static void SetCharacterOutlineColor(ushort What) { CharacterOutlineColor = What; }
	static ushort GetItemOutlineColor() { return ItemOutlineColor; }
	static void SetItemOutlineColor(ushort What) { ItemOutlineColor = What; }
	static void Save();
	static void Load();
	static void ShowConfigScreen();
	static void SwitchModeHandler();
private:
	static std::string DefaultName;
	static ushort AutosaveInterval;
	static uchar Contrast;
	static bool FullScreenMode;
	static bool BeepOnCritical;
	static bool AutodropLeftOvers;
	static bool OutlineCharacters;
	static bool OutlineItems;
	static ushort CharacterOutlineColor;
	static ushort ItemOutlineColor;
};

#endif



