#include <fstream>

#include "config.h"
#include "game.h"
#include "felist.h"
#include "festring.h"
#include "feio.h"
#include "area.h"
#include "bitmap.h"
#include "graphics.h"
#include "femath.h"
#include "save.h"

festring configuration::DefaultName;
festring configuration::DefaultPetName = CONST_S("Kenny");
ushort configuration::AutoSaveInterval = 100;
ushort configuration::Contrast = 100;
bool configuration::AutoDropLeftOvers = true;
bool configuration::OutlineCharacters = false;
bool configuration::OutlineItems = false;
ushort configuration::CharacterOutlineColor = MakeRGB16(48, 48, 48);
ushort configuration::ItemOutlineColor = MakeRGB16(48, 48, 100);
bool configuration::BeepOnCritical = false;
bool configuration::FullScreenMode = false;
ulong configuration::ContrastLuminance = NORMAL_LUMINANCE;
bool configuration::LookZoom = false;

void configuration::Save()
{
  std::ofstream SaveFile(GetConfigFileName().CStr(), std::ios::out);

  if(!SaveFile.is_open())
    return;

  SaveFile << "DefaultName = \"" << DefaultName.CStr() << "\";\n";
  SaveFile << "DefaultPetName = \"" << DefaultPetName.CStr() << "\";\n";
  SaveFile << "AutoSaveInterval = " << AutoSaveInterval << ";\n";
  SaveFile << "Contrast = " << Contrast << ";\n";
  SaveFile << "AutoDropLeftOvers = " << AutoDropLeftOvers << ";\n";
  SaveFile << "OutlineCharacters = " << OutlineCharacters << ";\n";
  SaveFile << "OutlineItems = " << OutlineItems << ";\n";
  SaveFile << "CharacterOutlineColor = " << GetRed16(CharacterOutlineColor) << ", " << GetGreen16(CharacterOutlineColor) << ", " << GetBlue16(CharacterOutlineColor) << ";\n";
  SaveFile << "ItemOutlineColor = " << GetRed16(ItemOutlineColor) << ", " << GetGreen16(ItemOutlineColor) << ", " << GetBlue16(ItemOutlineColor) << ";\n";
  SaveFile << "FullScreenMode = " << FullScreenMode << ";\n";
  SaveFile << "LookZoom = " << LookZoom <<";\n";
}

void configuration::Load()
{
  inputfile SaveFile(GetConfigFileName(), 0, false);

  if(!SaveFile.IsOpen())
    return;

  festring Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      if(Word == "DefaultName")
	{
	  SaveFile.ReadWord();
	  SetDefaultName(SaveFile.ReadWord());
	}

      if(Word == "DefaultPetName")
	{
	  SaveFile.ReadWord();
	  SetDefaultPetName(SaveFile.ReadWord());
	}

      if(Word == "AutoSaveInterval")
	SetAutoSaveInterval(SaveFile.ReadNumber());

      if(Word == "Contrast")
	SetContrast(SaveFile.ReadNumber());

      if(Word == "AutoDropLeftOvers")
	SetAutoDropLeftOvers(SaveFile.ReadBool());

      if(Word == "OutlineCharacters")
	SetOutlineCharacters(SaveFile.ReadBool());

      if(Word == "OutlineItems")
	SetOutlineItems(SaveFile.ReadBool());

      if(Word == "CharacterOutlineColor")
	{
	  uchar Red = SaveFile.ReadNumber();
	  uchar Green = SaveFile.ReadNumber();
	  uchar Blue = SaveFile.ReadNumber();

	  SetCharacterOutlineColor(MakeRGB16(Red, Green, Blue));
	}

      if(Word == "ItemOutlineColor")
	{
	  uchar Red = SaveFile.ReadNumber();
	  uchar Green = SaveFile.ReadNumber();
	  uchar Blue = SaveFile.ReadNumber();

	  SetItemOutlineColor(MakeRGB16(Red, Green, Blue));
	}

      if(Word == "FullScreenMode")
	SetFullScreenMode(SaveFile.ReadBool());

      if(Word == "LookZoom")
	SetLookZoom(SaveFile.ReadBool());
    }
}

void configuration::EditContrast(short Change)
{
  Contrast += Change;

  if(short(Contrast) < 0) Contrast = 0;
  if(short(Contrast) > 200) Contrast = 200;

  ushort Element = Min((Contrast << 7) / 100, 255);
  ContrastLuminance = MakeRGB24(Element, Element, Element);
}

void configuration::ShowConfigScreen()
{
  vector2d QuestionPos = game::IsRunning() ? vector2d(16, 6) : vector2d(30, 46);
  ushort Chosen;
  bool BoolChange = false;

  felist List(CONST_S("Which setting do you wish to configure?"));
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Setting                                    Value"));

  while(true)
    {
      if(game::IsRunning())
	{
	  game::DrawEverythingNoBlit();
	  DOUBLE_BUFFER->Fill(16, 6, game::GetScreenXSize() << 4, 23, 0);
	}

      List.Empty();
      List.AddEntry(CONST_S("Player's default name:                  ") + (DefaultName.GetSize() ? DefaultName : "-"), LIGHT_GRAY);
      List.AddEntry(CONST_S("Starting pet's default name:            ") + (DefaultPetName.GetSize() ? DefaultPetName : "-"), LIGHT_GRAY);

      if(AutoSaveInterval)
	List.AddEntry(CONST_S("Autosave interval:                      ") + AutoSaveInterval + " turn" + (AutoSaveInterval != 1 ? "s" : ""), LIGHT_GRAY);
      else
	List.AddEntry(CONST_S("Autosave interval:                      disabled"), LIGHT_GRAY);

      List.AddEntry(CONST_S("Contrast:                               ") + Contrast + "/100", LIGHT_GRAY);
      List.AddEntry(CONST_S("Drop food leftovers automatically:      ") + (AutoDropLeftOvers ? "yes" : "no"), LIGHT_GRAY);
      List.AddEntry(CONST_S("Outline all characters:                 ") + (OutlineCharacters ? "yes" : "no"), LIGHT_GRAY);
      List.AddEntry(CONST_S("Outline all items:                      ") + (OutlineItems ? "yes" : "no"), LIGHT_GRAY);
      List.AddEntry(CONST_S("Zoom feature in look mode:              ") + (LookZoom ? "yes" : "no"), LIGHT_GRAY);

#ifndef __DJGPP__
      List.AddEntry(CONST_S("Run the game in full screen mode:       ") + (FullScreenMode ? "yes" : "no"), LIGHT_GRAY);
#endif

      if(game::IsRunning())
	game::SetStandardListAttributes(List);

      List.SetFlags(SELECTABLE|(game::IsRunning() ? DRAW_BACKGROUND_AFTERWARDS : 0)|(!game::IsRunning() && !BoolChange ? FADE : 0));
      Chosen = List.Draw();

      switch(Chosen)
	{
	case 0:
	  SetDefaultName(iosystem::StringQuestion(CONST_S("Set new default name (3-20 letters):"), QuestionPos, WHITE, 0, 20, !game::IsRunning(), true));
	  BoolChange = false;
	  break;
	case 1:
	  SetDefaultPetName(iosystem::StringQuestion(CONST_S("Set new default name for the starting pet (3-20 letters):"), QuestionPos, WHITE, 0, 20, !game::IsRunning(), true));
	  BoolChange = false;
	  break;
	case 2:
	  SetAutoSaveInterval(iosystem::NumberQuestion(CONST_S("Set new autosave interval (1-50000 turns, 0 for never):"), QuestionPos, WHITE, !game::IsRunning()));
	  BoolChange = false;
	  break;
	case 3:
	  iosystem::ScrollBarQuestion(CONST_S("Set new contrast value (0-200, '<' and '>' move the slider):"), QuestionPos, Contrast, 5, 0, 200, Contrast, WHITE, LIGHT_GRAY, DARK_GRAY, !game::IsRunning(), &ContrastHandler);
	  BoolChange = false;
	  break;
	case 4:
	  SetAutoDropLeftOvers(!GetAutoDropLeftOvers());
	  BoolChange = true;
	  break;
	case 5:
	  SetOutlineCharacters(!GetOutlineCharacters());
	  if(game::IsRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = true;
	  break;
	case 6:
	  SetOutlineItems(!GetOutlineItems());
	  if(game::IsRunning()) game::GetCurrentArea()->SendNewDrawRequest();
	  BoolChange = true;
	  break;
	case 7:
	  SetLookZoom(!GetLookZoom());
	  BoolChange = true;
	  break;
#ifndef __DJGPP__
	case 8:
	  graphics::SwitchMode();
	  BoolChange = true;
	  break;
#endif
	default:
	  Save();
	  return;
	}
    }
}

void configuration::SetDefaultName(const festring& What)
{
  if(What.GetSize())
    DefaultName = What;
  else
    DefaultName.Empty();
}

void configuration::SetDefaultPetName(const festring& What)
{
  if(What.GetSize())
    DefaultPetName = What;
  else
    DefaultPetName.Empty();
}

void configuration::SetAutoSaveInterval(long What)
{
  if(What < 0) What = 0;
  if(What > 50000) What = 50000;

  AutoSaveInterval = What;
}

void configuration::SetContrast(short What)
{
  if(What < 0) What = 0;
  if(What > 200) What = 200;

  Contrast = What;
  ushort Element = Min((Contrast << 7) / 100, 255);
  ContrastLuminance = MakeRGB24(Element, Element, Element);
}

void configuration::SwitchModeHandler()
{
  SetFullScreenMode(!GetFullScreenMode());
  Save();
}

void configuration::ContrastHandler(long Value)
{
  SetContrast(Value);

  if(game::IsRunning())
    {
      game::GetCurrentArea()->SendNewDrawRequest();
      game::DrawEverythingNoBlit();
    }
}

ulong configuration::ApplyContrastTo(ulong L)
{
  return MakeRGB24(GetRed24(L) * Contrast / 100, GetGreen24(L) * Contrast / 100, GetBlue24(L) * Contrast / 100);
}

festring configuration::GetConfigFileName()
{
#if defined(WIN32) || defined(__DJGPP__)
  return "ivan.cfg";
#endif
  return festring(getenv("HOME")) + "/.ivan.conf";
}
