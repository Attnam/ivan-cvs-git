/*#ifndef __CONFIG_H__
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
  static ushort GetContrast() { return Contrast; }
  static void SetContrast(short);
  static bool GetAutoDropLeftOvers() { return AutoDropLeftOvers; }
  static void SetAutoDropLeftOvers(bool What) { AutoDropLeftOvers = What; }
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
  static bool GetUseAlternativeKeys() { return UseAlternativeKeys; }
  static void SetUseAlternativeKeys(bool What) { UseAlternativeKeys = What; }
 private:
  static void ContrastHandler(long);
  static festring DefaultName;
  static festring DefaultPetName;
  static ushort AutoSaveInterval;
  static ushort Contrast;
  static bool AutoDropLeftOvers;
  static bool FullScreenMode;
  static ulong ContrastLuminance;
  static bool LookZoom;
  static bool UseAlternativeKeys;
};

#endif*/

#ifndef __ICONF_H__
#define __ICONF_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include "config.h"
#include "vector2d.h"

class ivanconfig
{
 public:
  static const festring& GetDefaultName() { return DefaultName.Value; }
  static const festring& GetDefaultPetName() { return DefaultPetName.Value; }
  static long GetAutoSaveInterval() { return AutoSaveInterval.Value; }
  static long GetContrast() { return Contrast.Value; }
  static bool GetAutoDropLeftOvers() { return AutoDropLeftOvers.Value; }
  static bool GetLookZoom() { return LookZoom.Value; }
  static bool GetUseAlternativeKeys() { return UseAlternativeKeys.Value; }
#ifndef __DJGPP__
  static bool GetFullScreenMode() { return FullScreenMode.Value; }
#endif
  static ulong ApplyContrastTo(ulong);
  static void Save() { configsystem::Save(); }
  static void Load() { configsystem::Load(); }
  static void SwitchModeHandler();
  static void CalculateContrastLuminance();
  static ulong GetContrastLuminance() { return ContrastLuminance; }
  static void Initialize();
  static void Show();
 private:
  static vector2d GetQuestionPos();
  static void AutoSaveIntervalDisplayer(const numberoption*, festring&);
  static void ContrastDisplayer(const numberoption*, festring&);
  static bool DefaultNameChangeInterface(stringoption*);
  static bool DefaultPetNameChangeInterface(stringoption*);
  static bool AutoSaveIntervalChangeInterface(numberoption*);
  static bool ContrastChangeInterface(numberoption*);
  static void AutoSaveIntervalChanger(numberoption*, long);
  static void ContrastChanger(numberoption*, long);
#ifndef __DJGPP__
  static void FullScreenModeChanger(booloption*, bool);
#endif
  static void ContrastHandler(long);
  static void BackGroundDrawer();
  static stringoption DefaultName;
  static stringoption DefaultPetName;
  static numberoption AutoSaveInterval;
  static scrollbaroption Contrast;
  static booloption AutoDropLeftOvers;
  static booloption LookZoom;
  static booloption UseAlternativeKeys;
#ifndef __DJGPP__
  static booloption FullScreenMode;
#endif
  static ulong ContrastLuminance;
};

#endif
