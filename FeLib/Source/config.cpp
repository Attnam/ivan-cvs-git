#include "config.h"
#include "save.h"
#include "felist.h"
#include "feio.h"

configoption* configsystem::Option[MAX_CONFIG_OPTIONS];
ushort configsystem::Options;
festring configsystem::ConfigFileName;

void configsystem::AddOption(configoption* O) { Option[Options++] = O; }
void configsystem::NormalStringChanger(stringoption* O, const festring& What) { O->Value = What; }
void configsystem::NormalNumberChanger(numberoption* O, long What) { O->Value = What; }
void configsystem::NormalBoolChanger(booloption* O, bool What) { O->Value = What; }

configoption::configoption(const char* Name, const char* Description) : Name(Name), Description(Description) { }

stringoption::stringoption(	  const char* Name, const char* Description, const festring& Value,
				  void (*ValueDisplayer)(const stringoption*, festring&),
				  bool (*ChangeInterface)(stringoption*),
				  void (*ValueChanger)(stringoption*, const festring&)) :
				  configoption(Name, Description),
				  Value(Value), ValueDisplayer(ValueDisplayer),
				  ChangeInterface(ChangeInterface),
				  ValueChanger(ValueChanger) { }

numberoption::numberoption(	  const char* Name, const char* Description, long Value,
				  void (*ValueDisplayer)(const numberoption*, festring&),
				  bool (*ChangeInterface)(numberoption*),
				  void (*ValueChanger)(numberoption*, long)) :
				  configoption(Name, Description),
				  Value(Value), ValueDisplayer(ValueDisplayer),
				  ChangeInterface(ChangeInterface),
				  ValueChanger(ValueChanger) { }

scrollbaroption::scrollbaroption( const char* Name, const char* Description, long Value,
				  void (*ValueDisplayer)(const numberoption*, festring&),
				  bool (*ChangeInterface)(numberoption*),
				  void (*ValueChanger)(numberoption*, long),
				  void (*BarHandler)(long)) :
				  numberoption(Name, Description, Value, ValueDisplayer, ChangeInterface, ValueChanger),
				  BarHandler(BarHandler) { }
				  

booloption::booloption(		  const char* Name, const char* Description, bool Value,
				  void (*ValueDisplayer)(const booloption*, festring&),
				  bool (*ChangeInterface)(booloption*),
				  void (*ValueChanger)(booloption*, bool)) :
				  configoption(Name, Description),
				  Value(Value), ValueDisplayer(ValueDisplayer),
				  ChangeInterface(ChangeInterface),
				  ValueChanger(ValueChanger) { }

bool configsystem::Save()
{
  std::ofstream SaveFile(ConfigFileName.CStr(), std::ios::out);

  if(!SaveFile.is_open())
    return false;

  for(ushort c = 0; c < Options; ++c)
    {
      SaveFile << Option[c]->Name << " = ";
      Option[c]->SaveValue(SaveFile);
      SaveFile << ";\n";
    }

  return true;
}

bool configsystem::Load()
{
  inputfile SaveFile(ConfigFileName, 0, false);

  if(!SaveFile.IsOpen())
    return false;

  festring Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      /* Inefficient, but speed is probably not an issue here */

      for(ushort c = 0; c < Options; ++c)
	if(Word == Option[c]->Name)
	  Option[c]->LoadValue(SaveFile);
    }

  return true;
}

void configsystem::Show(void (*BackGroundDrawer)(), void (*ListAttributeInitializer)(felist&), bool SlaveScreen)
{
  ushort Chosen;
  bool BoolChange = false;

  felist List(CONST_S("Which setting do you wish to configure?"));
  List.AddDescription(CONST_S(""));
  List.AddDescription(CONST_S("Setting                                                        Value"));

  while(true)
    {
      if(SlaveScreen)
	BackGroundDrawer();

      List.Empty();

      for(ushort c = 0; c < Options; ++c)
	{
	  festring Entry = Option[c]->Description;
	  Entry.Capitalize();
	  Entry.Resize(60);
	  Option[c]->DisplayeValue(Entry);
	  List.AddEntry(Entry, LIGHT_GRAY);
	}

      if(SlaveScreen && ListAttributeInitializer)
	ListAttributeInitializer(List);

      List.SetFlags(SELECTABLE|(SlaveScreen ? DRAW_BACKGROUND_AFTERWARDS : 0)|(!SlaveScreen && !BoolChange ? FADE : 0));
      Chosen = List.Draw();
      festring String;

      if(Chosen < Options)
	BoolChange = Option[Chosen]->ActivateChangeInterface();
      else
	{
	  Save();
	  return;
	}
    }
}

void configsystem::NormalStringDisplayer(const stringoption* O, festring& Entry)
{
  if(!O->Value.IsEmpty())
    Entry << O->Value;
  else
    Entry << '-';
}

void configsystem::NormalNumberDisplayer(const numberoption* O, festring& Entry)
{
  Entry << O->Value;
}

void configsystem::NormalBoolDisplayer(const booloption* O, festring& Entry)
{
  Entry << (O->Value ? "yes" : "no");
}

bool configsystem::NormalBoolChangeInterface(booloption* O)
{
  O->ChangeValue(!O->Value);
  return true;
}

bool configsystem::NormalStringChangeInterface(stringoption* O)
{
  festring String;

  if(iosystem::StringQuestion(String, CONST_S("Set new ") + O->Description + ':', vector2d(30, 30), WHITE, 0, 80, true, true) == NORMAL_EXIT)
    O->ChangeValue(String);

  return false;
}

bool configsystem::NormalNumberChangeInterface(numberoption* O)
{
  O->ChangeValue(iosystem::NumberQuestion(CONST_S("Set new ") + O->Description + ':', vector2d(30, 30), WHITE, true));
  return false;
}

void stringoption::SaveValue(std::ofstream& SaveFile) const
{
  SaveFile << '\"' << Value.CStr() << '\"';
}

void stringoption::LoadValue(inputfile& SaveFile)
{
  SaveFile.ReadWord();
  SaveFile.ReadWord(Value);
}

/* ??? */

void numberoption::SaveValue(std::ofstream& SaveFile) const { SaveFile << Value; }
void numberoption::LoadValue(inputfile& SaveFile) { Value = SaveFile.ReadNumber(); }
void booloption::SaveValue(std::ofstream& SaveFile) const { SaveFile << Value; }
void booloption::LoadValue(inputfile& SaveFile) { Value = SaveFile.ReadBool(); }
