#include <typeinfo>

#include "database.h"
#include "charba.h"
#include "itemba.h"
#include "materba.h"
#include "save.h"
#include "proto.h"
#include "error.h"
#include "game.h"

template <class type> void database<type>::ReadFrom(inputfile& SaveFile)
{
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      ushort Index = protocontainer<type>::SearchCodeName(Word);

      if(!Index)
	ABORT("Odd term %s encountered in %s datafile!", Word.c_str(), typeid(type).name());

      typename type::prototype* Proto = protocontainer<type>::ProtoData[Index];

      if(Proto->Base)
	Proto->DataBase = Proto->Base->DataBase;

      Proto->DataBase.InitDefaults();

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in the data script of %s!", typeid(type).name());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	{
	  if(Word == "Config")
	    {
	      ushort ConfigNumber = SaveFile.ReadNumber(game::GetGlobalValueMap());
	      typename type::database TempDataBase = Proto->DataBase;

	      if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in the data script of %s!", typeid(type).name());

	      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
		AnalyzeData(SaveFile, Word, &TempDataBase);

	      Proto->Config[ConfigNumber] = TempDataBase;
	      continue;
	    }

	  AnalyzeData(SaveFile, Word, &Proto->DataBase);
	}
    }
}

#define ANALYZEDATA(data)\
{\
  if(Word == #data)\
    ReadData(DataBase->data, SaveFile, ValueMap);\
}

#define ANALYZEDATAWITHDEFAULT(data, defaultdata)\
{\
  if(Word == #data)\
    ReadData(DataBase->data, SaveFile, ValueMap);\
  \
  if(Word == #defaultdata)\
    DataBase->data = DataBase->defaultdata;\
}

#define ANALYZEDATAWITHCOMPLEXDEFAULT(data, defaultdata, statement)\
{\
  if(Word == #data)\
    ReadData(DataBase->data, SaveFile, ValueMap);\
  \
  if(Word == #defaultdata)\
    DataBase->data = statement;\
}

void database<character>::AnalyzeData(inputfile& SaveFile, const std::string& Word, character::database* DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();

  ANALYZEDATA(DefaultArmStrength);
  ANALYZEDATA(DefaultLegStrength);
  ANALYZEDATA(DefaultDexterity);
  ANALYZEDATA(DefaultAgility);
  ANALYZEDATA(DefaultEndurance);
  ANALYZEDATA(DefaultPerception);
  ANALYZEDATA(DefaultIntelligence);
  ANALYZEDATA(DefaultWisdom);
  ANALYZEDATA(DefaultCharisma);
  ANALYZEDATA(DefaultMana);
  ANALYZEDATA(DefaultMoney);
  ANALYZEDATA(TotalSize);
  ANALYZEDATA(CanRead);
  ANALYZEDATA(IsCharmable);
  ANALYZEDATA(Sex);
  ANALYZEDATA(BloodColor);
  ANALYZEDATA(CanBeGenerated);
  ANALYZEDATA(HasInfraVision);
  ANALYZEDATA(CriticalModifier);
  ANALYZEDATA(StandVerb);
  ANALYZEDATA(CanOpen);
  ANALYZEDATA(CanBeDisplaced);
  ANALYZEDATA(Frequency);
  ANALYZEDATA(CanWalk);
  ANALYZEDATA(CanSwim);
  ANALYZEDATA(CanFly);
  ANALYZEDATA(PhysicalDamageResistance);
  ANALYZEDATA(SoundResistance);
  ANALYZEDATA(EnergyResistance);
  ANALYZEDATA(AcidResistance);
  ANALYZEDATA(FireResistance);
  ANALYZEDATA(PoisonResistance);
  ANALYZEDATA(BulimiaResistance);
  ANALYZEDATA(IsUnique);
  ANALYZEDATA(EatFlags);
  ANALYZEDATA(TotalVolume);
  ANALYZEDATA(TalkVerb);
  ANALYZEDATA(HeadBitmapPos);
  ANALYZEDATA(TorsoBitmapPos);
  ANALYZEDATA(ArmBitmapPos);
  ANALYZEDATA(LegBitmapPos);
  ANALYZEDATAWITHDEFAULT(RightArmBitmapPos, ArmBitmapPos);
  ANALYZEDATAWITHDEFAULT(LeftArmBitmapPos, ArmBitmapPos);
  ANALYZEDATAWITHDEFAULT(RightLegBitmapPos, LegBitmapPos);
  ANALYZEDATAWITHDEFAULT(LeftLegBitmapPos, LegBitmapPos);
  ANALYZEDATAWITHDEFAULT(GroinBitmapPos, LegBitmapPos);
  ANALYZEDATA(ClothColor);
  ANALYZEDATA(SkinColor);
  ANALYZEDATAWITHDEFAULT(CapColor, ClothColor);
  ANALYZEDATA(HairColor);
  ANALYZEDATA(EyeColor);
  ANALYZEDATAWITHDEFAULT(TorsoMainColor, ClothColor);
  ANALYZEDATA(BeltColor);
  ANALYZEDATA(TorsoSpecialColor);
  ANALYZEDATAWITHDEFAULT(ArmMainColor, ClothColor);
  ANALYZEDATA(ArmSpecialColor);
  ANALYZEDATAWITHDEFAULT(LegMainColor, ClothColor);
  ANALYZEDATA(LegSpecialColor);
  ANALYZEDATA(HeadBonePercentile);
  ANALYZEDATA(TorsoBonePercentile);
  ANALYZEDATA(ArmBonePercentile);
  ANALYZEDATAWITHDEFAULT(RightArmBonePercentile, ArmBonePercentile);
  ANALYZEDATAWITHDEFAULT(LeftArmBonePercentile, ArmBonePercentile);
  ANALYZEDATA(GroinBonePercentile);
  ANALYZEDATA(LegBonePercentile);
  ANALYZEDATAWITHDEFAULT(RightLegBonePercentile, LegBonePercentile);
  ANALYZEDATAWITHDEFAULT(LeftLegBonePercentile, LegBonePercentile);
  ANALYZEDATA(IsNameable);
  ANALYZEDATA(BaseEmitation);
  ANALYZEDATA(Article);
  ANALYZEDATA(Adjective);
  ANALYZEDATA(AdjectiveArticle);
  ANALYZEDATA(NameSingular);
  ANALYZEDATAWITHCOMPLEXDEFAULT(NamePlural, NameSingular, DataBase->NameSingular + "s");
  ANALYZEDATA(PostFix);
  ANALYZEDATA(ArticleMode);
  ANALYZEDATA(IsAbstract);
  ANALYZEDATA(IsPolymorphable);
  ANALYZEDATA(UnarmedStrength);
  ANALYZEDATAWITHCOMPLEXDEFAULT(BiteStrength, UnarmedStrength, DataBase->UnarmedStrength / 2);
  ANALYZEDATAWITHCOMPLEXDEFAULT(KickStrength, UnarmedStrength, DataBase->UnarmedStrength * 2);
  ANALYZEDATA(AttackStyle);
}

void database<item>::AnalyzeData(inputfile& SaveFile, const std::string& Word, item::database* DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();

  ANALYZEDATA(Possibility);
  ANALYZEDATA(InHandsPic);
  ANALYZEDATA(OfferModifier);
  ANALYZEDATA(Score);
  ANALYZEDATA(IsDestroyable);
  ANALYZEDATA(CanBeWished);
  ANALYZEDATA(IsMaterialChangeable);
  ANALYZEDATA(WeaponCategory);
  ANALYZEDATA(IsPolymorphSpawnable);
  ANALYZEDATA(IsAutoInitializable);
  ANALYZEDATA(OneHandedStrengthPenalty);
  ANALYZEDATA(OneHandedToHitPenalty);
  ANALYZEDATA(Category);
  ANALYZEDATA(SoundResistance);
  ANALYZEDATA(EnergyResistance);
  ANALYZEDATA(AcidResistance);
  ANALYZEDATA(FireResistance);
  ANALYZEDATA(PoisonResistance);
  ANALYZEDATA(BulimiaResistance);
  ANALYZEDATA(IsStackable);
  ANALYZEDATA(StrengthModifier);
  ANALYZEDATA(FormModifier);
  ANALYZEDATA(NPModifier);
  ANALYZEDATA(DefaultSize);
  ANALYZEDATA(DefaultMainVolume);
  ANALYZEDATA(DefaultSecondaryVolume);
  ANALYZEDATA(DefaultContainedVolume);
  ANALYZEDATA(BitmapPos);
  ANALYZEDATA(Price);
  ANALYZEDATA(BaseEmitation);
  ANALYZEDATA(Article);
  ANALYZEDATA(Adjective);
  ANALYZEDATA(AdjectiveArticle);
  ANALYZEDATA(NameSingular);
  ANALYZEDATAWITHCOMPLEXDEFAULT(NamePlural, NameSingular, DataBase->NameSingular + "s");
  ANALYZEDATA(PostFix);
  ANALYZEDATA(ArticleMode);
  ANALYZEDATA(MainMaterialConfig);
  ANALYZEDATA(SecondaryMaterialConfig);
  ANALYZEDATA(ContainedMaterialConfig);
  ANALYZEDATA(MaterialConfigChances);
  ANALYZEDATA(IsAbstract);
  ANALYZEDATA(IsPolymorphable);
  ANALYZEDATA(Alias);
  ANALYZEDATA(OKVisualEffects);
  ANALYZEDATA(CanBeGeneratedInContainer);
  ANALYZEDATA(ForcedVisualEffects);
}

void database<material>::AnalyzeData(inputfile& SaveFile, const std::string& Word, material::database* DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();

  ANALYZEDATA(StrengthValue);
  ANALYZEDATA(ConsumeType);
  ANALYZEDATA(Density);
  ANALYZEDATA(OfferValue);
  ANALYZEDATA(Color);
  ANALYZEDATA(PriceModifier);
  ANALYZEDATA(IsSolid);
  ANALYZEDATA(Emitation);
  ANALYZEDATA(CanBeWished);
  ANALYZEDATA(Alignment);
  ANALYZEDATA(NutritionValue);
  ANALYZEDATA(IsAlive);
  ANALYZEDATA(IsBadFoodForAI);
  ANALYZEDATA(IsFlammable);
  ANALYZEDATA(IsFlexible);
  ANALYZEDATA(IsExplosive);
  ANALYZEDATA(NameStem);
  ANALYZEDATAWITHDEFAULT(AdjectiveStem, NameStem);
  ANALYZEDATA(Article);
  ANALYZEDATA(Effect);
  ANALYZEDATA(ConsumeEndMessage);
  ANALYZEDATA(HitMessage);
  ANALYZEDATA(ExplosivePower);
  ANALYZEDATA(Alpha);
}

void databasesystem::Initialize()
{
  {
    inputfile ScriptFile(GAME_DIR + "Script/char.dat");
    database<character>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(GAME_DIR + "Script/item.dat");
    database<item>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(GAME_DIR + "Script/material.dat");
    database<material>::ReadFrom(ScriptFile);
  }
}
