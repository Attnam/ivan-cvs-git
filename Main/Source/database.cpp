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

#define ANALYZE_DATA(data)\
{\
  if(Word == #data)\
    {\
      ReadData(DataBase.data, SaveFile, ValueMap);\
      Found = true;\
    }\
}

#define ANALYZE_DATA_WITH_DEFAULT(data, defaultdata)\
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

#define ANALYZE_DATA_WITH_COMPLEX_DEFAULT(data, defaultdata, statement)\
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

#define ANALYZE_DATA_AND_ADD_TO_ANOTHER(data, to)\
{\
  if(Word == #data)\
    {\
      ReadData(DataBase.data, SaveFile, ValueMap);\
      DataBase.to.push_back(DataBase.data);\
      Found = true;\
    }\
}

bool database<character>::AnalyzeData(inputfile& SaveFile, const std::string& Word, character::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZE_DATA(DefaultArmStrength);
  ANALYZE_DATA(DefaultLegStrength);
  ANALYZE_DATA(DefaultDexterity);
  ANALYZE_DATA(DefaultAgility);
  ANALYZE_DATA(DefaultEndurance);
  ANALYZE_DATA(DefaultPerception);
  ANALYZE_DATA(DefaultIntelligence);
  ANALYZE_DATA(DefaultWisdom);
  ANALYZE_DATA(DefaultCharisma);
  ANALYZE_DATA(DefaultMana);
  ANALYZE_DATA(DefaultMoney);
  ANALYZE_DATA(TotalSize);
  ANALYZE_DATA(CanRead);
  ANALYZE_DATA(IsCharmable);
  ANALYZE_DATA(Sex);
  ANALYZE_DATA(BloodColor);
  ANALYZE_DATA(CanBeGenerated);
  ANALYZE_DATA(CriticalModifier);
  ANALYZE_DATA(StandVerb);
  ANALYZE_DATA(CanOpen);
  ANALYZE_DATA(CanBeDisplaced);
  ANALYZE_DATA(Frequency);
  ANALYZE_DATA(CanWalk);
  ANALYZE_DATA(CanSwim);
  ANALYZE_DATA(CanFly);
  ANALYZE_DATA(PhysicalDamageResistance);
  ANALYZE_DATA(SoundResistance);
  ANALYZE_DATA(EnergyResistance);
  ANALYZE_DATA(AcidResistance);
  ANALYZE_DATA(FireResistance);
  ANALYZE_DATA(PoisonResistance);
  ANALYZE_DATA(BulimiaResistance);
  ANALYZE_DATA(IsUnique);
  ANALYZE_DATA(ConsumeFlags);
  ANALYZE_DATA(TotalVolume);
  ANALYZE_DATA(HeadBitmapPos);
  ANALYZE_DATA(TorsoBitmapPos);
  ANALYZE_DATA(ArmBitmapPos);
  ANALYZE_DATA(LegBitmapPos);
  ANALYZE_DATA_WITH_DEFAULT(RightArmBitmapPos, ArmBitmapPos);
  ANALYZE_DATA_WITH_DEFAULT(LeftArmBitmapPos, ArmBitmapPos);
  ANALYZE_DATA_WITH_DEFAULT(RightLegBitmapPos, LegBitmapPos);
  ANALYZE_DATA_WITH_DEFAULT(LeftLegBitmapPos, LegBitmapPos);
  ANALYZE_DATA_WITH_DEFAULT(GroinBitmapPos, LegBitmapPos);
  ANALYZE_DATA(ClothColor);
  ANALYZE_DATA(SkinColor);
  ANALYZE_DATA_WITH_DEFAULT(CapColor, ClothColor);
  ANALYZE_DATA(HairColor);
  ANALYZE_DATA(EyeColor);
  ANALYZE_DATA_WITH_DEFAULT(TorsoMainColor, ClothColor);
  ANALYZE_DATA(BeltColor);
  ANALYZE_DATA(BootColor);
  ANALYZE_DATA(TorsoSpecialColor);
  ANALYZE_DATA_WITH_DEFAULT(ArmMainColor, ClothColor);
  ANALYZE_DATA(ArmSpecialColor);
  ANALYZE_DATA_WITH_DEFAULT(LegMainColor, ClothColor);
  ANALYZE_DATA(LegSpecialColor);
  ANALYZE_DATA(IsNameable);
  ANALYZE_DATA(BaseEmitation);
  ANALYZE_DATA(Article);
  ANALYZE_DATA(Adjective);
  ANALYZE_DATA(AdjectiveArticle);
  ANALYZE_DATA(NameSingular);
  ANALYZE_DATA_WITH_COMPLEX_DEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZE_DATA(PostFix);
  ANALYZE_DATA(ArticleMode);
  ANALYZE_DATA(IsAbstract);
  ANALYZE_DATA(IsPolymorphable);
  ANALYZE_DATA(BaseUnarmedStrength);
  ANALYZE_DATA_WITH_COMPLEX_DEFAULT(BaseBiteStrength, BaseUnarmedStrength, DataBase.BaseUnarmedStrength >> 1);
  ANALYZE_DATA_WITH_COMPLEX_DEFAULT(BaseKickStrength, BaseUnarmedStrength, DataBase.BaseUnarmedStrength << 1);
  ANALYZE_DATA(AttackStyle);
  ANALYZE_DATA(CanUseEquipment);
  ANALYZE_DATA(CanKick);
  ANALYZE_DATA(CanTalk);
  ANALYZE_DATA(ClassStates);
  ANALYZE_DATA(CanBeWished);
  ANALYZE_DATA(Alias);
  ANALYZE_DATA(CreateDivineConfigurations);
  ANALYZE_DATA(CreateGolemMaterialConfigurations);
  ANALYZE_DATA(Helmet);
  ANALYZE_DATA(Amulet);
  ANALYZE_DATA(Cloak);
  ANALYZE_DATA(BodyArmor);
  ANALYZE_DATA(Belt);
  ANALYZE_DATA(RightWielded);
  ANALYZE_DATA(LeftWielded);
  ANALYZE_DATA(RightRing);
  ANALYZE_DATA(LeftRing);
  ANALYZE_DATA(RightGauntlet);
  ANALYZE_DATA_WITH_DEFAULT(LeftGauntlet, RightGauntlet);
  ANALYZE_DATA(RightBoot);
  ANALYZE_DATA_WITH_DEFAULT(LeftBoot, RightBoot);
  ANALYZE_DATA(AttributeBonus);
  ANALYZE_DATA(KnownCWeaponSkills);
  ANALYZE_DATA(CWeaponSkillHits);
  ANALYZE_DATA(RightSWeaponSkillHits);
  ANALYZE_DATA(LeftSWeaponSkillHits);
  ANALYZE_DATA(PanicLevel);
  ANALYZE_DATA(CanBeCloned);
  ANALYZE_DATA(Inventory);
  ANALYZE_DATA(DangerModifier);
  ANALYZE_DATA_AND_ADD_TO_ANOTHER(DefaultName, Alias);
  ANALYZE_DATA(FriendlyReplies);
  ANALYZE_DATA(HostileReplies);
  ANALYZE_DATA(CanZap);
  ANALYZE_DATA(FleshMaterial);
  ANALYZE_DATA(HasFeet);
  ANALYZE_DATA(DeathMessage);
  ANALYZE_DATA(IgnoreDanger);
  ANALYZE_DATA(HPRequirementForGeneration);
  ANALYZE_DATA(IsExtraCoward);
  ANALYZE_DATA(SpillsBlood);
  ANALYZE_DATA(HasEyes);

  return Found;
}

bool database<item>::AnalyzeData(inputfile& SaveFile, const std::string& Word, item::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZE_DATA(Possibility);
  ANALYZE_DATA(InHandsPic);
  ANALYZE_DATA(Score);
  ANALYZE_DATA(IsDestroyable);
  ANALYZE_DATA(CanBeWished);
  ANALYZE_DATA(IsMaterialChangeable);
  ANALYZE_DATA(WeaponCategory);
  ANALYZE_DATA(IsPolymorphSpawnable);
  ANALYZE_DATA(IsAutoInitializable);
  ANALYZE_DATA(Category);
  ANALYZE_DATA(SoundResistance);
  ANALYZE_DATA(EnergyResistance);
  ANALYZE_DATA(AcidResistance);
  ANALYZE_DATA(FireResistance);
  ANALYZE_DATA(PoisonResistance);
  ANALYZE_DATA(BulimiaResistance);
  ANALYZE_DATA(StrengthModifier);
  ANALYZE_DATA(FormModifier);
  ANALYZE_DATA(NPModifier);
  ANALYZE_DATA(DefaultSize);
  ANALYZE_DATA(DefaultMainVolume);
  ANALYZE_DATA(DefaultSecondaryVolume);
  ANALYZE_DATA(DefaultContainedVolume);
  ANALYZE_DATA(BitmapPos);
  ANALYZE_DATA(Price);
  ANALYZE_DATA(BaseEmitation);
  ANALYZE_DATA(Article);
  ANALYZE_DATA(Adjective);
  ANALYZE_DATA(AdjectiveArticle);
  ANALYZE_DATA(NameSingular);
  ANALYZE_DATA_WITH_COMPLEX_DEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZE_DATA(PostFix);
  ANALYZE_DATA(ArticleMode);
  ANALYZE_DATA(MainMaterialConfig);
  ANALYZE_DATA(SecondaryMaterialConfig);
  ANALYZE_DATA(ContainedMaterialConfig);
  ANALYZE_DATA(MaterialConfigChances);
  ANALYZE_DATA(IsAbstract);
  ANALYZE_DATA(IsPolymorphable);
  ANALYZE_DATA(Alias);
  ANALYZE_DATA(OKVisualEffects);
  ANALYZE_DATA(CanBeGeneratedInContainer);
  ANALYZE_DATA(ForcedVisualEffects);
  ANALYZE_DATA(Roundness);
  ANALYZE_DATA(GearStates);
  ANALYZE_DATA(IsTwoHanded);
  ANALYZE_DATA(CreateDivineConfigurations);
  ANALYZE_DATA(CanBeCloned);
  ANALYZE_DATA(BeamRange);
  ANALYZE_DATA(CanBeBroken);
  ANALYZE_DATA_WITH_DEFAULT(WallBitmapPos, BitmapPos);
  ANALYZE_DATA_WITH_DEFAULT(FlexibleNameSingular, NameSingular);
  ANALYZE_DATA(MaxCharges);
  ANALYZE_DATA(MinCharges);
  ANALYZE_DATA(CanBePiled);
  ANALYZE_DATA(StorageVolume);
  ANALYZE_DATA(MaxGeneratedContainedItems);
  ANALYZE_DATA(AffectsArmStrength);
  ANALYZE_DATA(AffectsLegStrength);
  ANALYZE_DATA(AffectsDexterity);
  ANALYZE_DATA(AffectsAgility);
  ANALYZE_DATA(AffectsEndurance);
  ANALYZE_DATA(AffectsPerception);
  ANALYZE_DATA(AffectsIntelligence);
  ANALYZE_DATA(AffectsWisdom);
  ANALYZE_DATA(AffectsCharisma);
  ANALYZE_DATA(AffectsMana);
  ANALYZE_DATA(DefaultEnchantment);
  ANALYZE_DATA(PriceIsProportionalToEnchantment);
  ANALYZE_DATA(InElasticityPenaltyModifier);
  ANALYZE_DATA(CanBeUsedBySmith);
  ANALYZE_DATA(AffectsCarryingCapacity);
  ANALYZE_DATA(DamageDivider);
  ANALYZE_DATA(HandleInPairs);
  ANALYZE_DATA(CanBeEnchanted);

  return Found;
}

bool database<glterrain>::AnalyzeData(inputfile& SaveFile, const std::string& Word, glterrain::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZE_DATA(BitmapPos);
  ANALYZE_DATA(Article);
  ANALYZE_DATA(Adjective);
  ANALYZE_DATA(AdjectiveArticle);
  ANALYZE_DATA(NameSingular);
  ANALYZE_DATA_WITH_COMPLEX_DEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZE_DATA(PostFix);
  ANALYZE_DATA(ArticleMode);
  ANALYZE_DATA(MainMaterialConfig);
  ANALYZE_DATA(SecondaryMaterialConfig);
  ANALYZE_DATA(ContainedMaterialConfig);
  ANALYZE_DATA(MaterialConfigChances);
  ANALYZE_DATA(IsAbstract);
  ANALYZE_DATA(OKVisualEffects);
  ANALYZE_DATA(MaterialColorB);
  ANALYZE_DATA(MaterialColorC);
  ANALYZE_DATA(MaterialColorD);
  ANALYZE_DATA(SitMessage);
  ANALYZE_DATA(DefaultMainVolume);
  ANALYZE_DATA(DefaultSecondaryVolume);
  ANALYZE_DATA(DefaultContainedVolume);
  ANALYZE_DATA(CreateDivineConfigurations);
  ANALYZE_DATA(ShowMaterial);

  return Found;
}

bool database<olterrain>::AnalyzeData(inputfile& SaveFile, const std::string& Word, olterrain::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZE_DATA(BitmapPos);
  ANALYZE_DATA(Article);
  ANALYZE_DATA(Adjective);
  ANALYZE_DATA(AdjectiveArticle);
  ANALYZE_DATA(NameSingular);
  ANALYZE_DATA_WITH_COMPLEX_DEFAULT(NamePlural, NameSingular, DataBase.NameSingular + "s");
  ANALYZE_DATA(PostFix);
  ANALYZE_DATA(ArticleMode);
  ANALYZE_DATA(MainMaterialConfig);
  ANALYZE_DATA(SecondaryMaterialConfig);
  ANALYZE_DATA(ContainedMaterialConfig);
  ANALYZE_DATA(MaterialConfigChances);
  ANALYZE_DATA(IsAbstract);
  ANALYZE_DATA(OKVisualEffects);
  ANALYZE_DATA(MaterialColorB);
  ANALYZE_DATA(MaterialColorC);
  ANALYZE_DATA(MaterialColorD);
  ANALYZE_DATA(SitMessage);
  ANALYZE_DATA(DefaultMainVolume);
  ANALYZE_DATA(DefaultSecondaryVolume);
  ANALYZE_DATA(DefaultContainedVolume);
  ANALYZE_DATA(CreateDivineConfigurations);
  ANALYZE_DATA(ShowMaterial);

  ANALYZE_DATA(DigMessage);
  ANALYZE_DATA(CanBeDestroyed);
  ANALYZE_DATA(IsSafeToDestroy);
  ANALYZE_DATA(RestModifier);
  ANALYZE_DATA(RestMessage);
  ANALYZE_DATA(IsUpLink);
  ANALYZE_DATA(StorageVolume);
  ANALYZE_DATA(HPModifier);

  return Found;
}

bool database<material>::AnalyzeData(inputfile& SaveFile, const std::string& Word, material::database& DataBase)
{
  const valuemap& ValueMap = game::GetGlobalValueMap();
  bool Found = false;

  ANALYZE_DATA(StrengthValue);
  ANALYZE_DATA(ConsumeType);
  ANALYZE_DATA(Density);
  ANALYZE_DATA(Color);
  ANALYZE_DATA(PriceModifier);
  ANALYZE_DATA(IsGolemMaterial);
  ANALYZE_DATA(Emitation);
  ANALYZE_DATA(CanBeWished);
  ANALYZE_DATA(Alignment);
  ANALYZE_DATA(NutritionValue);
  ANALYZE_DATA(IsAlive);
  ANALYZE_DATA(IsBadFoodForAI);
  ANALYZE_DATA(IsFlammable);
  ANALYZE_DATA(IsExplosive);
  ANALYZE_DATA(NameStem);
  ANALYZE_DATA_WITH_DEFAULT(AdjectiveStem, NameStem);
  ANALYZE_DATA(Article);
  ANALYZE_DATA(Effect);
  ANALYZE_DATA(ConsumeEndMessage);
  ANALYZE_DATA(HitMessage);
  ANALYZE_DATA(ExplosivePower);
  ANALYZE_DATA(Alpha);
  ANALYZE_DATA(CreateDivineConfigurations);
  ANALYZE_DATA(Flexibility);
  ANALYZE_DATA(SpoilModifier);
  ANALYZE_DATA(IsSparkling);
  ANALYZE_DATA(EffectStrength);
  ANALYZE_DATA(IsMetal);
  ANALYZE_DATA(CanHaveParasite);
  ANALYZE_DATA(DigProductMaterial);

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
