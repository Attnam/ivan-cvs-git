#ifndef __COMMAND_H__
#define __COMMAND_H__

#ifdef VC
#pragma warning(disable : 4786)
#endif

#include <string>

#include "typedef.h"

class character;

class command
{
 public:
  command(bool (*)(character*), const std::string&, char, bool, bool = false);
  bool (*GetLinkedFunction() const)(character*) { return LinkedFunction; }
  std::string GetDescription() const { return Description; }
  char GetKey() const { return Key; }
  bool IsUsableInWilderness() const { return UsableInWilderness; }
  bool IsWizardModeFunction() const { return WizardModeFunction; }
 private:
  bool (*LinkedFunction)(character*);
  std::string Description;
  char Key;
  bool UsableInWilderness;
  bool WizardModeFunction;
};

class commandsystem
{
public:
  static command* GetCommand(ushort Index) { return Command[Index]; }
private:
  static bool Apply(character*);
  static bool Close(character*);
  static bool Eat(character*);
  static bool Drink(character*);
  static bool Dip(character*);
  static bool DrawMessageHistory(character*);
  static bool Drop(character*);
  static bool ForceVomit(character*);
  static bool GainAllItems(character*);
  static bool GoDown(character*);
  static bool GoUp(character*);
  static bool Kick(character*);
  static bool Look(character*);
  static bool LowerStats(character*);
  static bool NOP(character*);
  static bool Offer(character*);
  static bool Open(character*);
  static bool PickUp(character*);
  static bool Pray(character*);
  static bool Quit(character*);
  static bool RaiseStats(character*);
  static bool Read(character*);
  static bool Save(character*);
  static bool SeeWholeMap(character*);
  static bool ShowInventory(character*);
  static bool ShowKeyLayout(character*);
  static bool ShowWeaponSkills(character*);
  static bool Talk(character*);
  static bool Throw(character*);
  static bool WalkThroughWalls(character*);
  static bool EquipmentScreen(character*);
  static bool WhatToEngrave(character*);
  static bool WizardMode(character*);
  static bool Zap(character*);
  static bool RestUntilHealed(character*);
  static bool OutlineCharacters(character*);
  static bool OutlineItems(character*);
  static bool RaiseGodRelations(character*);
  static bool LowerGodRelations(character*);
  static bool GainDivineKnowledge(character*);
  static bool Sit(character*);
  static bool Go(character*);
  static bool ShowConfigScreen(character*);
  static bool SecretKnowledge(character*);
  static bool ScrollMessagesDown(character*);
  static bool ScrollMessagesUp(character*);
  static bool DetachBodyPart(character*);
  static bool ReloadDatafiles(character*);
  static bool SummonMonster(character*);
  static bool ShowBattleInfo(character*);
  static bool LevelTeleport(character*);
  static bool WieldInRightArm(character*);
  static bool WieldInLeftArm(character*);
  static bool AssignName(character*);
  static bool Search(character*);
  static command* Command[];
};

#endif
