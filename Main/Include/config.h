#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "typedef.h"

class festring;

class configuration
{
 public:
  static const festring& GetDefaultName() { return DefaultName; }
  static void SetDefaultName(const festring&);
  static ushort GetAutoSaveInterval() { return AutoSaveInterval; }
  static void SetAutoSaveInterval(long);
  static void EditContrast(short);
  static ushort GetContrast() { return Contrast; }
  static void SetContrast(short);
  static bool GetAutoDropLeftOvers() { return AutoDropLeftOvers; }
  static void SetAutoDropLeftOvers(bool What) { AutoDropLeftOvers = What; }
  static bool GetBeepOnCritical() { return BeepOnCritical; }
  static void SetBeepOnCritical(bool What) { BeepOnCritical = What; }
  static bool GetFullScreenMode() { return FullScreenMode; }
  static void SetFullScreenMode(bool What) { FullScreenMode = What; }
  static void Save();
  static void Load();
  static void ShowConfigScreen();
  static void SwitchModeHandler();
  static ulong GetContrastLuminance() { return ContrastLuminance; }
  static ulong ApplyContrastTo(ulong);
  static void SetLookZoom(bool What) { LookZoom = What; }
  static bool GetLookZoom() { return LookZoom; }
  static const festring& GetDefaultPetName() { return DefaultPetName; }
  static void SetDefaultPetName(const festring&);
  static festring GetConfigFileName();
  static bool GetUseNumberPad() { return UseNumberPad; }
  static void SetUseNumberPad(bool What) { UseNumberPad = What; }
 private:
  static void ContrastHandler(long);
  static festring DefaultName;
  static festring DefaultPetName;
  static ushort AutoSaveInterval;
  static ushort Contrast;
  static bool AutoDropLeftOvers;
  static bool BeepOnCritical;
  static bool FullScreenMode;
  static ulong ContrastLuminance;
  static bool LookZoom;
  static bool UseNumberPad;
};

#endif
