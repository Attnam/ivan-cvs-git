/*
 *
 *  Iter Vehemens ad Necem 
 *  Copyright (C) Timo Kiviluoto
 *  Released under GNU General Public License
 *
 *  See LICENSING which should included with 
 *  this file for more details
 *
 */

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
  static bool GetWarnAboutDanger() { return WarnAboutDanger.Value; }
  static bool GetAutoDropLeftOvers() { return AutoDropLeftOvers.Value; }
  static bool GetLookZoom() { return LookZoom.Value; }
  static bool GetUseAlternativeKeys() { return UseAlternativeKeys.Value; }
#ifndef __DJGPP__
  static bool GetFullScreenMode() { return FullScreenMode.Value; }
  static void SwitchModeHandler();
#else
  static bool GetFullScreenMode() { return true; }
#endif
  static long ApplyContrastTo(long);
  static void Save() { configsystem::Save(); }
  static void Load() { configsystem::Load(); }
  static void CalculateContrastLuminance();
  static color24 GetContrastLuminance() { return ContrastLuminance; }
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
  static booloption WarnAboutDanger;
  static booloption AutoDropLeftOvers;
  static booloption LookZoom;
  static booloption UseAlternativeKeys;
#ifndef __DJGPP__
  static booloption FullScreenMode;
#endif
  static color24 ContrastLuminance;
};

#endif
