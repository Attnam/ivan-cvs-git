#include "febot.h"
#include "femath.h"
#include "save.h"

febot::fword* febot::fword::GetRandomLink() const { return Link[RAND() % Link.size()]; }

febot::febot() { FWordSet.insert(fword(CONST_S(""))); }
febot::fword* febot::GetControlFWord() const { return const_cast<fword*>(&*FWordSet.begin()); }

febot::fword* febot::CreateFWord(const festring& String)
{
  return const_cast<fword*>(&*FWordSet.insert(fword(String)).first);
}

void febot::Save(outputfile& SaveFile) const
{
  SaveFile << ulong(FWordSet.size());
  fwordset::const_iterator i1;
  std::map<const fword*, ulong> FWordIndexMap;
  ulong c;

  for(i1 = FWordSet.begin(), c = 0; i1 != FWordSet.end(); ++i1, ++c)
    {
      SaveFile << i1->String;
      FWordIndexMap[&*i1] = c;
    }

  for(i1 = FWordSet.begin(); i1 != FWordSet.end(); ++i1)
    {
      SaveFile << ulong(i1->Link.size());

      for(c = 0; c < i1->Link.size(); ++c)
	SaveFile << FWordIndexMap.find(i1->Link[c])->second;
    }
}

void febot::Load(inputfile& SaveFile)
{
  FWordSet.clear();
  ulong MapSize;
  SaveFile >> MapSize;
  std::map<ulong, fword*> FWordPtrMap;

  for(ulong c = 0; c < MapSize; ++c)
    FWordPtrMap[c] = const_cast<fword*>(&*FWordSet.insert(FWordSet.end(), fword(ReadType<festring>(SaveFile))));

  for(fwordset::const_iterator i1 = FWordSet.begin(); i1 != FWordSet.end(); ++i1)
    {
      ulong LinkSize;
      SaveFile >> LinkSize;
      fword* FWord = const_cast<fword*>(&*i1);
      FWord->Link.resize(LinkSize);

      for(ulong c = 0; c < LinkSize; ++c)
	FWord->Link[c] = FWordPtrMap.find(ReadType<ulong>(SaveFile))->second;
    }
}

void febot::BeTalkedTo(festring String)
{
  String.PreProcessForFebot();

  if(!String.IsEmpty())
    {
      festring Word;
      String.ExtractWord(Word);
      fword* OldFWord = CreateFWord(Word);
      GetControlFWord()->Link.push_back(OldFWord);

      for(String.ExtractWord(Word); !Word.IsEmpty(); String.ExtractWord(Word))
	{
	  fword* NewFWord = CreateFWord(Word);
	  OldFWord->Link.push_back(NewFWord);
	  OldFWord = NewFWord;
	}

      OldFWord->Link.push_back(GetControlFWord());
    }
}

void febot::Reply(festring& String) const
{
  if(GetControlFWord()->Link.empty())
    {
      String.Empty();
      return;
    }

  String.Empty();
  fword* FWord = GetControlFWord()->GetRandomLink();
  String << FWord->String;

  for(FWord = FWord->GetRandomLink(); !FWord->String.IsEmpty(); FWord = FWord->GetRandomLink())
    String << ' ' << FWord->String;

  String.PostProcessForFebot();
}

void febot::Amnesify()
{
  FWordSet.clear();
  FWordSet.insert(fword(CONST_S("")));
}
