#include "database.h"
#include "charba.h"
#include "itemba.h"
#include "materba.h"
#include "save.h"
#include "proto.h"
#include "error.h"
#include "game.h"
#include "godba.h"
#include "lterraba.h"

template <class type> void database<type>::ReadFrom(inputfile& SaveFile)
{
  typedef typename type::database typedatabase;
  typedef typename type::prototype typeprototype;
  std::string Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      ushort Index = protocontainer<type>::SearchCodeName(Word);

      if(!Index)
	ABORT("Odd term %s encountered in %s datafile line %d!", Word.c_str(), protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

      typeprototype* Proto = protocontainer<type>::ProtoData[Index];
      Proto->Config[0] = Proto->Base ? typedatabase(Proto->Base->Config.begin()->second) : typedatabase();
      typedatabase& DataBase = Proto->Config.begin()->second;
      DataBase.InitDefaults(0);

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in %s datafile line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	{
	  if(Word == "Config")
	    {
	      ushort ConfigNumber = SaveFile.ReadNumber(game::GetGlobalValueMap());
	      typedatabase TempDataBase(Proto->ChooseBaseForConfig(ConfigNumber));
	      TempDataBase.InitDefaults(ConfigNumber);

	      if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in %s datafile line %d!", protocontainer<type>::GetMainClassId().c_str(), SaveFile.TellLine());

	      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
		if(!AnalyzeData(SaveFile, Word, TempDataBase))
		  ABORT("Illegal datavalue %s found while building up %s config #%d, line %d!", Word.c_str(), Proto->GetClassId().c_str(), ConfigNumber, SaveFile.TellLine());

	      Proto->Config[ConfigNumber] = TempDataBase;
	      continue;
	    }

	  if(!AnalyzeData(SaveFile, Word, DataBase))
	    ABORT("Illegal datavalue %s found while building up %s, line %d!", Word.c_str(), Proto->GetClassId().c_str(), SaveFile.TellLine());
	}

      if(DataBase.CreateDivineConfigurations)
	{
	  for(ushort c = 1; c < protocontainer<god>::GetProtoAmount(); ++c)
	    if(Proto->Config.find(c) == Proto->Config.end())
	      {
		typedatabase TempDataBase(DataBase);
		TempDataBase.InitDefaults(c);
		Proto->Config[c] = TempDataBase;
	      }
	}

      Proto->CreateSpecialConfigurations();
    }
}

#define ANALYZEDATA(data)\
{\
  if(Word == #data)\
    {\
      ReadData(DataBase.data, SaveFile, ValueMap);\
      Found = true;\
    }\
}

#define ANALYZEDATAWITHDEFAULT(data, defaultdata)\
{\
  if(Word == #data)\
    {\
      ReadData(DataBase.data, SaveFile, ValueMap);\
      Found = true;\
    }\
  \
  if(Word == #defaultdata)\
    DataBase.data = DataBase.defaultdata;\
}

#define ANALYZEDATAWITHCOMPLEXDEFAULT(data, defaultdata, statement)\
{\
  if(Word == #data)\
    {\
      ReadData(DataBase.data, SaveFile, ValueMap);\
      Found = true;\
    }\
  \
  if(Word == #defaultdata)\
    DataBase.data = statement;\
}

bool database<character>::AnalyzeData(inputfile& SaveFile, const std::string& Word, character::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

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
  ANALYZEDATA(ConsumeFlags);
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
  ANALYZEDATAWITHCOMPLEXDEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZEDATA(PostFix);
  ANALYZEDATA(ArticleMode);
  ANALYZEDATA(IsAbstract);
  ANALYZEDATA(IsPolymorphable);
  ANALYZEDATA(BaseUnarmedStrength);
  ANALYZEDATAWITHCOMPLEXDEFAULT(BaseBiteStrength, BaseUnarmedStrength, DataBase.BaseUnarmedStrength / 2);
  ANALYZEDATAWITHCOMPLEXDEFAULT(BaseKickStrength, BaseUnarmedStrength, DataBase.BaseUnarmedStrength * 2);
  ANALYZEDATA(AttackStyle);
  ANALYZEDATA(CanUseEquipment);
  ANALYZEDATA(CanKick);
  ANALYZEDATA(CanTalk);
  ANALYZEDATA(ClassStates);
  ANALYZEDATA(CanBeWished);
  ANALYZEDATA(Alias);
  ANALYZEDATA(CreateDivineConfigurations);
  ANALYZEDATA(CreateSolidMaterialConfigurations);
  ANALYZEDATA(Helmet);
  ANALYZEDATA(Amulet);
  ANALYZEDATA(Cloak);
  ANALYZEDATA(BodyArmor);
  ANALYZEDATA(Belt);
  ANALYZEDATA(RightWielded);
  ANALYZEDATA(LeftWielded);
  ANALYZEDATA(RightRing);
  ANALYZEDATA(LeftRing);
  ANALYZEDATA(RightGauntlet);
  ANALYZEDATAWITHDEFAULT(LeftGauntlet, RightGauntlet);
  ANALYZEDATA(RightBoot);
  ANALYZEDATAWITHDEFAULT(LeftBoot, RightBoot);
  ANALYZEDATA(AttributeBonus);
  ANALYZEDATA(KnownCWeaponSkills);
  ANALYZEDATA(CWeaponSkillHits);
  ANALYZEDATA(RightSWeaponSkillHits);
  ANALYZEDATA(LeftSWeaponSkillHits);
  ANALYZEDATA(PanicLevel);
  ANALYZEDATA(CanBeCloned);

  return Found;
}

bool database<item>::AnalyzeData(inputfile& SaveFile, const std::string& Word, item::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZEDATA(Possibility);
  ANALYZEDATA(InHandsPic);
  ANALYZEDATA(Score);
  ANALYZEDATA(IsDestroyable);
  ANALYZEDATA(CanBeWished);
  ANALYZEDATA(IsMaterialChangeable);
  ANALYZEDATA(WeaponCategory);
  ANALYZEDATA(IsPolymorphSpawnable);
  ANALYZEDATA(IsAutoInitializable);
  ANALYZEDATA(Category);
  ANALYZEDATA(SoundResistance);
  ANALYZEDATA(EnergyResistance);
  ANALYZEDATA(AcidResistance);
  ANALYZEDATA(FireResistance);
  ANALYZEDATA(PoisonResistance);
  ANALYZEDATA(BulimiaResistance);
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
  ANALYZEDATAWITHCOMPLEXDEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
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
  ANALYZEDATA(Roundness);
  ANALYZEDATA(GearStates);
  ANALYZEDATA(IsTwoHanded);
  ANALYZEDATA(CreateDivineConfigurations);
  ANALYZEDATA(CanBeCloned);
  ANALYZEDATA(BeamRange);
  ANALYZEDATA(CanBeBroken);
  ANALYZEDATAWITHDEFAULT(WallBitmapPos, BitmapPos);
  ANALYZEDATAWITHDEFAULT(FlexibleNameSingular, NameSingular);
  ANALYZEDATA(MaxCharges);
  ANALYZEDATA(MinCharges);
  ANALYZEDATA(CanBePiled);
  ANALYZEDATA(StorageVolume);
  ANALYZEDATA(MaxGeneratedContainedItems);
  ANALYZEDATA(AffectsArmStrength);
  ANALYZEDATA(AffectsLegStrength);
  ANALYZEDATA(AffectsDexterity);
  ANALYZEDATA(AffectsAgility);
  ANALYZEDATA(AffectsEndurance);
  ANALYZEDATA(AffectsPerception);
  ANALYZEDATA(AffectsIntelligence);
  ANALYZEDATA(AffectsWisdom);
  ANALYZEDATA(AffectsCharisma);
  ANALYZEDATA(AffectsMana);
  ANALYZEDATA(DefaultEnchantment);

  return Found;
}

bool database<glterrain>::AnalyzeData(inputfile& SaveFile, const std::string& Word, glterrain::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZEDATA(BitmapPos);
  ANALYZEDATA(Article);
  ANALYZEDATA(Adjective);
  ANALYZEDATA(AdjectiveArticle);
  ANALYZEDATA(NameSingular);
  ANALYZEDATAWITHCOMPLEXDEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZEDATA(PostFix);
  ANALYZEDATA(ArticleMode);
  ANALYZEDATA(MainMaterialConfig);
  ANALYZEDATA(SecondaryMaterialConfig);
  ANALYZEDATA(ContainedMaterialConfig);
  ANALYZEDATA(MaterialConfigChances);
  ANALYZEDATA(IsAbstract);
  ANALYZEDATA(OKVisualEffects);
  ANALYZEDATA(MaterialColorB);
  ANALYZEDATA(MaterialColorC);
  ANALYZEDATA(MaterialColorD);
  ANALYZEDATA(SitMessage);
  ANALYZEDATA(DefaultMainVolume);
  ANALYZEDATA(DefaultSecondaryVolume);
  ANALYZEDATA(DefaultContainedVolume);
  ANALYZEDATA(CreateDivineConfigurations);
  ANALYZEDATA(ShowMaterial);

  return Found;
}

bool database<olterrain>::AnalyzeData(inputfile& SaveFile, const std::string& Word, olterrain::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZEDATA(BitmapPos);
  ANALYZEDATA(Article);
  ANALYZEDATA(Adjective);
  ANALYZEDATA(AdjectiveArticle);
  ANALYZEDATA(NameSingular);
  ANALYZEDATAWITHCOMPLEXDEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZEDATA(PostFix);
  ANALYZEDATA(ArticleMode);
  ANALYZEDATA(MainMaterialConfig);
  ANALYZEDATA(SecondaryMaterialConfig);
  ANALYZEDATA(ContainedMaterialConfig);
  ANALYZEDATA(MaterialConfigChances);
  ANALYZEDATA(IsAbstract);
  ANALYZEDATA(OKVisualEffects);
  ANALYZEDATA(MaterialColorB);
  ANALYZEDATA(MaterialColorC);
  ANALYZEDATA(MaterialColorD);
  ANALYZEDATA(SitMessage);
  ANALYZEDATA(DefaultMainVolume);
  ANALYZEDATA(DefaultSecondaryVolume);
  ANALYZEDATA(DefaultContainedVolume);
  ANALYZEDATA(CreateDivineConfigurations);
  ANALYZEDATA(ShowMaterial);

  ANALYZEDATA(DigMessage);
  ANALYZEDATA(CanBeDug);
  ANALYZEDATA(IsSafeToDestroy);
  ANALYZEDATA(RestModifier);
  ANALYZEDATA(RestMessage);
  ANALYZEDATA(IsUpLink);

  return Found;
}

bool database<material>::AnalyzeData(inputfile& SaveFile, const std::string& Word, material::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZEDATA(StrengthValue);
  ANALYZEDATA(ConsumeType);
  ANALYZEDATA(Density);
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
  ANALYZEDATA(IsExplosive);
  ANALYZEDATA(NameStem);
  ANALYZEDATAWITHDEFAULT(AdjectiveStem, NameStem);
  ANALYZEDATA(Article);
  ANALYZEDATA(Effect);
  ANALYZEDATA(ConsumeEndMessage);
  ANALYZEDATA(HitMessage);
  ANALYZEDATA(ExplosivePower);
  ANALYZEDATA(Alpha);
  ANALYZEDATA(CreateDivineConfigurations);
  ANALYZEDATA(Flexibility);
  ANALYZEDATA(SpoilModifier);
  ANALYZEDATA(IsSparkling);

  return Found;
}

void databasesystem::Initialize()
{
  {
    /* Must be before character */

    inputfile ScriptFile(GAME_DIR + "Script/material.dat");
    database<material>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(GAME_DIR + "Script/char.dat");
    database<character>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(GAME_DIR + "Script/item.dat");
    database<item>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(GAME_DIR + "Script/glterra.dat");
    database<glterrain>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(GAME_DIR + "Script/olterra.dat");
    database<olterrain>::ReadFrom(ScriptFile);
  }
}

