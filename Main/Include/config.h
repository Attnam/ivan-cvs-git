#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

#include "typedef.h"

class configuration
{
public:
	static void EditContrast(char);
	static uchar GetContrast() { return Contrast; }
	static void SetContrast(uchar What) { Contrast = What; }
	static bool GetBeepOnCritical() { return BeepOnCritical; }
	static void SetBeepOnCritical(bool What) { BeepOnCritical = What; }
	static std::string GetDefaultName() { return DefaultName; }
	static void SetDefaultName(std::string What) { DefaultName = What; }
	static bool GetAutodropLeftOvers() { return AutodropLeftOvers; }
	static void SetAutodropLeftOvers(bool What) { AutodropLeftOvers = What; }
	static ushort GetAutosaveInterval() { return AutosaveInterval; }
	static void SetAutosaveInterval(ushort What) { AutosaveInterval = What; }
	static void Save();
	static void Load();
private:
	static uchar Contrast;
	static bool BeepOnCritical;
	static std::string DefaultName;
	static bool AutodropLeftOvers;
	static ushort AutosaveInterval;
};

#endif
