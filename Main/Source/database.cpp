/* Compiled through dataset.cpp */

databasecreator<character>::databasemembermap databasecreator<character>::DataBaseMemberMap;
databasecreator<item>::databasemembermap databasecreator<item>::DataBaseMemberMap;
databasecreator<glterrain>::databasemembermap databasecreator<glterrain>::DataBaseMemberMap;
databasecreator<olterrain>::databasemembermap databasecreator<olterrain>::DataBaseMemberMap;
databasecreator<material>::databasemembermap databasecreator<material>::DataBaseMemberMap;

template <class type> void databasecreator<type>::ReadFrom(inputfile& SaveFile)
{
  typedef typename type::prototype prototype;
  typedef typename type::databasemap::iterator configiterator;
  festring Word;

  for(SaveFile.ReadWord(Word, false); !SaveFile.Eof(); SaveFile.ReadWord(Word, false))
    {
      ushort Index = protocontainer<type>::SearchCodeName(Word);

      if(!Index)
	ABORT("Odd term %s encountered in %s datafile line %d!", Word.CStr(), protocontainer<type>::GetMainClassID(), SaveFile.TellLine());

      prototype* Proto = protocontainer<type>::ProtoData[Index];
      database* DataBase;
      configiterator Iterator = Proto->Config.find(0);

      if(Iterator != Proto->Config.end())
	DataBase = &Iterator->second;
      else if(Proto->Base)
	DataBase = &Proto->Config.insert(std::pair<ushort, database>(0, database(Proto->Base->Config.begin()->second))).first->second;
      else
	DataBase = &Proto->Config.insert(std::pair<ushort, database>(0, database())).first->second;

      DataBase->InitDefaults(0);

      if(SaveFile.ReadWord() != "{")
	ABORT("Bracket missing in %s datafile line %d!", protocontainer<type>::GetMainClassID(), SaveFile.TellLine());

      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
	{
	  if(Word == "Config")
	    {
	      ushort ConfigNumber = SaveFile.ReadNumber();
	      database* TempDataBase;
	      Iterator = Proto->Config.find(ConfigNumber);

	      if(Iterator != Proto->Config.end())
		TempDataBase = &Iterator->second;
	      else
		TempDataBase = &Proto->Config.insert(std::pair<ushort, database>(ConfigNumber, Proto->ChooseBaseForConfig(ConfigNumber))).first->second;

	      TempDataBase->InitDefaults(ConfigNumber);

	      if(SaveFile.ReadWord() != "{")
		ABORT("Bracket missing in %s datafile line %d!", protocontainer<type>::GetMainClassID(), SaveFile.TellLine());

	      for(SaveFile.ReadWord(Word); Word != "}"; SaveFile.ReadWord(Word))
		if(!AnalyzeData(SaveFile, Word, *TempDataBase))
		  ABORT("Illegal datavalue %s found while building up %s config #%d, line %d!", Word.CStr(), Proto->GetClassID(), ConfigNumber, SaveFile.TellLine());

	      continue;
	    }

	  if(!AnalyzeData(SaveFile, Word, *DataBase))
	    ABORT("Illegal datavalue %s found while building up %s, line %d!", Word.CStr(), Proto->GetClassID(), SaveFile.TellLine());
	}

      if(DataBase->CreateDivineConfigurations)
	{
	  for(ushort c = 1; c < protocontainer<god>::GetProtoAmount(); ++c)
	    if(Proto->Config.find(c) == Proto->Config.end())
	      {
		database* TempDataBase;
		Iterator = Proto->Config.find(c);

		if(Iterator != Proto->Config.end())
		  TempDataBase = &Iterator->second;
		else
		  TempDataBase = &Proto->Config.insert(std::pair<ushort, database>(c, *DataBase)).first->second;

		TempDataBase->InitDefaults(DEVOUT|c);
		TempDataBase->AttachedGod = c;
	      }
	}

      Proto->CreateSpecialConfigurations();
    }
}

template <class database, class member> struct databasemember : public databasememberbase<database>
{
  databasemember(member Member) : Member(Member) { }
  virtual void ReadData(database& DataBase, inputfile& SaveFile) { ::ReadData(DataBase.*Member, SaveFile); }
  member Member;
};

template <class database, class member> void AddMember(std::map<festring, databasememberbase<database>*>& Map, const char* Str, member Member)
{
  Map.insert(std::pair<festring, databasememberbase<database>*>(Str, new databasemember<database, member>(Member)));
}

/* Explicit instantiations seem to increases compile speed greatly here... */

#define INST_ADD_MEMBER(type, member) template void AddMember<type##database, member type##database::*>(std::map<festring, databasememberbase<type##database>*>&, const char*, member type##database::*)

INST_ADD_MEMBER(character, bool);
INST_ADD_MEMBER(character, uchar);
INST_ADD_MEMBER(character, short);
INST_ADD_MEMBER(character, ushort);
INST_ADD_MEMBER(character, ulong);
INST_ADD_MEMBER(character, vector2d);
INST_ADD_MEMBER(character, festring);
INST_ADD_MEMBER(character, std::vector<festring>);
INST_ADD_MEMBER(character, contentscript<item>);
INST_ADD_MEMBER(character, std::vector<long>);
INST_ADD_MEMBER(character, std::list<contentscript<item> >);

INST_ADD_MEMBER(item, bool);
INST_ADD_MEMBER(item, char);
INST_ADD_MEMBER(item, uchar);
INST_ADD_MEMBER(item, ushort);
INST_ADD_MEMBER(item, long);
INST_ADD_MEMBER(item, ulong);
INST_ADD_MEMBER(item, vector2d);
INST_ADD_MEMBER(item, festring);
INST_ADD_MEMBER(item, std::vector<festring>);
INST_ADD_MEMBER(item, std::vector<long>);

INST_ADD_MEMBER(glterrain, bool);
INST_ADD_MEMBER(glterrain, uchar);
INST_ADD_MEMBER(glterrain, ushort);
INST_ADD_MEMBER(glterrain, ulong);
INST_ADD_MEMBER(glterrain, vector2d);
INST_ADD_MEMBER(glterrain, festring);
INST_ADD_MEMBER(glterrain, std::vector<long>);

INST_ADD_MEMBER(olterrain, bool);
INST_ADD_MEMBER(olterrain, uchar);
INST_ADD_MEMBER(olterrain, ushort);
INST_ADD_MEMBER(olterrain, ulong);
INST_ADD_MEMBER(olterrain, vector2d);
INST_ADD_MEMBER(olterrain, festring);
INST_ADD_MEMBER(olterrain, std::vector<long>);

INST_ADD_MEMBER(material, bool);
INST_ADD_MEMBER(material, uchar);
INST_ADD_MEMBER(material, ushort);
INST_ADD_MEMBER(material, ulong);
INST_ADD_MEMBER(material, festring);

#define ADD_MEMBER(data) AddMember(DataBaseMemberMap, #data, &database::data);

template<> void databasecreator<character>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(DefaultArmStrength);
  ADD_MEMBER(DefaultLegStrength);
  ADD_MEMBER(DefaultDexterity);
  ADD_MEMBER(DefaultAgility);
  ADD_MEMBER(DefaultEndurance);
  ADD_MEMBER(DefaultPerception);
  ADD_MEMBER(DefaultIntelligence);
  ADD_MEMBER(DefaultWisdom);
  ADD_MEMBER(DefaultCharisma);
  ADD_MEMBER(DefaultMana);
  ADD_MEMBER(DefaultMoney);
  ADD_MEMBER(TotalSize);
  ADD_MEMBER(CanRead);
  ADD_MEMBER(IsCharmable);
  ADD_MEMBER(Sex);
  ADD_MEMBER(BloodColor);
  ADD_MEMBER(CanBeGenerated);
  ADD_MEMBER(CriticalModifier);
  ADD_MEMBER(StandVerb);
  ADD_MEMBER(CanOpen);
  ADD_MEMBER(CanBeDisplaced);
  ADD_MEMBER(Frequency);
  ADD_MEMBER(CanWalk);
  ADD_MEMBER(CanSwim);
  ADD_MEMBER(CanFly);
  ADD_MEMBER(FireResistance);
  ADD_MEMBER(PoisonResistance);
  ADD_MEMBER(ElectricityResistance);
  ADD_MEMBER(IsUnique);
  ADD_MEMBER(ConsumeFlags);
  ADD_MEMBER(TotalVolume);
  ADD_MEMBER(HeadBitmapPos);
  ADD_MEMBER(TorsoBitmapPos);
  ADD_MEMBER(ArmBitmapPos);
  ADD_MEMBER(LegBitmapPos);
  ADD_MEMBER(RightArmBitmapPos);
  ADD_MEMBER(LeftArmBitmapPos);
  ADD_MEMBER(RightLegBitmapPos);
  ADD_MEMBER(LeftLegBitmapPos);
  ADD_MEMBER(GroinBitmapPos);
  ADD_MEMBER(ClothColor);
  ADD_MEMBER(SkinColor);
  ADD_MEMBER(CapColor);
  ADD_MEMBER(HairColor);
  ADD_MEMBER(EyeColor);
  ADD_MEMBER(TorsoMainColor);
  ADD_MEMBER(BeltColor);
  ADD_MEMBER(BootColor);
  ADD_MEMBER(TorsoSpecialColor);
  ADD_MEMBER(ArmMainColor);
  ADD_MEMBER(ArmSpecialColor);
  ADD_MEMBER(LegMainColor);
  ADD_MEMBER(LegSpecialColor);
  ADD_MEMBER(IsNameable);
  ADD_MEMBER(BaseEmitation);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(IsPolymorphable);
  ADD_MEMBER(BaseUnarmedStrength);
  ADD_MEMBER(BaseBiteStrength);
  ADD_MEMBER(BaseKickStrength);
  ADD_MEMBER(AttackStyle);
  ADD_MEMBER(CanUseEquipment);
  ADD_MEMBER(CanKick);
  ADD_MEMBER(CanTalk);
  ADD_MEMBER(ClassStates);
  ADD_MEMBER(CanBeWished);
  ADD_MEMBER(Alias);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(CreateGolemMaterialConfigurations);
  ADD_MEMBER(Helmet);
  ADD_MEMBER(Amulet);
  ADD_MEMBER(Cloak);
  ADD_MEMBER(BodyArmor);
  ADD_MEMBER(Belt);
  ADD_MEMBER(RightWielded);
  ADD_MEMBER(LeftWielded);
  ADD_MEMBER(RightRing);
  ADD_MEMBER(LeftRing);
  ADD_MEMBER(RightGauntlet);
  ADD_MEMBER(LeftGauntlet);
  ADD_MEMBER(RightBoot);
  ADD_MEMBER(LeftBoot);
  ADD_MEMBER(AttributeBonus);
  ADD_MEMBER(KnownCWeaponSkills);
  ADD_MEMBER(CWeaponSkillHits);
  ADD_MEMBER(RightSWeaponSkillHits);
  ADD_MEMBER(LeftSWeaponSkillHits);
  ADD_MEMBER(PanicLevel);
  ADD_MEMBER(CanBeCloned);
  ADD_MEMBER(Inventory);
  ADD_MEMBER(DangerModifier);
  ADD_MEMBER(DefaultName);
  ADD_MEMBER(FriendlyReplies);
  ADD_MEMBER(HostileReplies);
  ADD_MEMBER(CanZap);
  ADD_MEMBER(FleshMaterial);
  ADD_MEMBER(HasFeet);
  ADD_MEMBER(DeathMessage);
  ADD_MEMBER(IgnoreDanger);
  ADD_MEMBER(HPRequirementForGeneration);
  ADD_MEMBER(IsExtraCoward);
  ADD_MEMBER(SpillsBlood);
  ADD_MEMBER(HasEyes);
  ADD_MEMBER(HasHead);
  ADD_MEMBER(CanThrow);
  ADD_MEMBER(UsesNutrition);
  ADD_MEMBER(AttackWisdomLimit);
  ADD_MEMBER(CanWalkThroughWalls);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(BodyPartsDisappearWhenSevered);
  ADD_MEMBER(CanBeConfused);
  ADD_MEMBER(CanAttack);
  ADD_MEMBER(CanApply);
  ADD_MEMBER(WieldedPosition);
  ADD_MEMBER(ClothColorIsSparkling);
  ADD_MEMBER(SkinColorIsSparkling);
  ADD_MEMBER(CapColorIsSparkling);
  ADD_MEMBER(HairColorIsSparkling);
  ADD_MEMBER(EyeColorIsSparkling);
  ADD_MEMBER(TorsoMainColorIsSparkling);
  ADD_MEMBER(BeltColorIsSparkling);
  ADD_MEMBER(BootColorIsSparkling);
  ADD_MEMBER(TorsoSpecialColorIsSparkling);
  ADD_MEMBER(ArmMainColorIsSparkling);
  ADD_MEMBER(ArmSpecialColorIsSparkling);
  ADD_MEMBER(LegMainColorIsSparkling);
  ADD_MEMBER(LegSpecialColorIsSparkling);
  ADD_MEMBER(IsPlant);
}

template<> void databasecreator<item>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(Possibility);
  ADD_MEMBER(Score);
  ADD_MEMBER(IsDestroyable);
  ADD_MEMBER(CanBeWished);
  ADD_MEMBER(IsMaterialChangeable);
  ADD_MEMBER(WeaponCategory);
  ADD_MEMBER(IsPolymorphSpawnable);
  ADD_MEMBER(IsAutoInitializable);
  ADD_MEMBER(Category);
  ADD_MEMBER(FireResistance);
  ADD_MEMBER(PoisonResistance);
  ADD_MEMBER(ElectricityResistance);
  ADD_MEMBER(StrengthModifier);
  ADD_MEMBER(FormModifier);
  ADD_MEMBER(DefaultSize);
  ADD_MEMBER(DefaultMainVolume);
  ADD_MEMBER(DefaultSecondaryVolume);
  ADD_MEMBER(DefaultContainedVolume);
  ADD_MEMBER(BitmapPos);
  ADD_MEMBER(Price);
  ADD_MEMBER(BaseEmitation);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(MainMaterialConfig);
  ADD_MEMBER(SecondaryMaterialConfig);
  ADD_MEMBER(ContainedMaterialConfig);
  ADD_MEMBER(MaterialConfigChances);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(IsPolymorphable);
  ADD_MEMBER(Alias);
  ADD_MEMBER(OKVisualEffects);
  ADD_MEMBER(CanBeGeneratedInContainer);
  ADD_MEMBER(ForcedVisualEffects);
  ADD_MEMBER(Roundness);
  ADD_MEMBER(GearStates);
  ADD_MEMBER(IsTwoHanded);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(CanBeCloned);
  ADD_MEMBER(BeamRange);
  ADD_MEMBER(CanBeBroken);
  ADD_MEMBER(WallBitmapPos);
  ADD_MEMBER(FlexibleNameSingular);
  ADD_MEMBER(MaxCharges);
  ADD_MEMBER(MinCharges);
  ADD_MEMBER(CanBePiled);
  ADD_MEMBER(StorageVolume);
  ADD_MEMBER(MaxGeneratedContainedItems);
  ADD_MEMBER(AffectsArmStrength);
  ADD_MEMBER(AffectsLegStrength);
  ADD_MEMBER(AffectsDexterity);
  ADD_MEMBER(AffectsAgility);
  ADD_MEMBER(AffectsEndurance);
  ADD_MEMBER(AffectsPerception);
  ADD_MEMBER(AffectsIntelligence);
  ADD_MEMBER(AffectsWisdom);
  ADD_MEMBER(AffectsCharisma);
  ADD_MEMBER(AffectsMana);
  ADD_MEMBER(DefaultEnchantment);
  ADD_MEMBER(PriceIsProportionalToEnchantment);
  ADD_MEMBER(InElasticityPenaltyModifier);
  ADD_MEMBER(CanBeUsedBySmith);
  ADD_MEMBER(AffectsCarryingCapacity);
  ADD_MEMBER(DamageDivider);
  ADD_MEMBER(HandleInPairs);
  ADD_MEMBER(CanBeEnchanted);
  ADD_MEMBER(BeamColor);
  ADD_MEMBER(BeamEffect);
  ADD_MEMBER(BeamStyle);
  ADD_MEMBER(WearWisdomLimit);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(BreakEffectRange);
  ADD_MEMBER(WieldedBitmapPos);
  ADD_MEMBER(IsQuestItem);
  ADD_MEMBER(IsGoodWithPlants);
}

template<> void databasecreator<glterrain>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(BitmapPos);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(MainMaterialConfig);
  ADD_MEMBER(SecondaryMaterialConfig);
  ADD_MEMBER(ContainedMaterialConfig);
  ADD_MEMBER(MaterialConfigChances);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(OKVisualEffects);
  ADD_MEMBER(MaterialColorB);
  ADD_MEMBER(MaterialColorC);
  ADD_MEMBER(MaterialColorD);
  ADD_MEMBER(SitMessage);
  ADD_MEMBER(DefaultMainVolume);
  ADD_MEMBER(DefaultSecondaryVolume);
  ADD_MEMBER(DefaultContainedVolume);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(ShowMaterial);
  ADD_MEMBER(AttachedGod);
}

template<> void databasecreator<olterrain>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(BitmapPos);
  ADD_MEMBER(Article);
  ADD_MEMBER(Adjective);
  ADD_MEMBER(AdjectiveArticle);
  ADD_MEMBER(NameSingular);
  ADD_MEMBER(NamePlural);
  ADD_MEMBER(PostFix);
  ADD_MEMBER(ArticleMode);
  ADD_MEMBER(MainMaterialConfig);
  ADD_MEMBER(SecondaryMaterialConfig);
  ADD_MEMBER(ContainedMaterialConfig);
  ADD_MEMBER(MaterialConfigChances);
  ADD_MEMBER(IsAbstract);
  ADD_MEMBER(OKVisualEffects);
  ADD_MEMBER(MaterialColorB);
  ADD_MEMBER(MaterialColorC);
  ADD_MEMBER(MaterialColorD);
  ADD_MEMBER(SitMessage);
  ADD_MEMBER(DefaultMainVolume);
  ADD_MEMBER(DefaultSecondaryVolume);
  ADD_MEMBER(DefaultContainedVolume);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(ShowMaterial);
  ADD_MEMBER(AttachedGod);

  ADD_MEMBER(DigMessage);
  ADD_MEMBER(CanBeDestroyed);
  ADD_MEMBER(RestModifier);
  ADD_MEMBER(RestMessage);
  ADD_MEMBER(IsUpLink);
  ADD_MEMBER(StorageVolume);
  ADD_MEMBER(HPModifier);
  ADD_MEMBER(IsSafeToCreateDoor);
  ADD_MEMBER(OpenBitmapPos);
}

template<> void databasecreator<material>::CreateDataBaseMemberMap()
{
  ADD_MEMBER(StrengthValue);
  ADD_MEMBER(ConsumeType);
  ADD_MEMBER(Density);
  ADD_MEMBER(Color);
  ADD_MEMBER(PriceModifier);
  ADD_MEMBER(IsGolemMaterial);
  ADD_MEMBER(Emitation);
  ADD_MEMBER(CanBeWished);
  ADD_MEMBER(NutritionValue);
  ADD_MEMBER(IsAlive);
  ADD_MEMBER(IsFlammable);
  ADD_MEMBER(IsExplosive);
  ADD_MEMBER(NameStem);
  ADD_MEMBER(AdjectiveStem);
  ADD_MEMBER(Article);
  ADD_MEMBER(Effect);
  ADD_MEMBER(ConsumeEndMessage);
  ADD_MEMBER(HitMessage);
  ADD_MEMBER(ExplosivePower);
  ADD_MEMBER(Alpha);
  ADD_MEMBER(CreateDivineConfigurations);
  ADD_MEMBER(Flexibility);
  ADD_MEMBER(SpoilModifier);
  ADD_MEMBER(IsSparkling);
  ADD_MEMBER(EffectStrength);
  ADD_MEMBER(IsMetal);
  ADD_MEMBER(CanHaveParasite);
  ADD_MEMBER(DigProductMaterial);
  ADD_MEMBER(ConsumeWisdomLimit);
  ADD_MEMBER(AttachedGod);
  ADD_MEMBER(BreatheMessage);
  ADD_MEMBER(EffectIsGood);
  ADD_MEMBER(IsWarm);
  ADD_MEMBER(UseMaterialAttributes);
  ADD_MEMBER(CanRegenerate);
  ADD_MEMBER(BreatheWisdomLimit);
}

template<class type> bool databasecreator<type>::AnalyzeData(inputfile& SaveFile, const festring& Word, database& DataBase)
{
  typename databasemembermap::iterator i = DataBaseMemberMap.find(Word);

  if(i != DataBaseMemberMap.end())
    {
      i->second->ReadData(DataBase, SaveFile);
      CheckDefaults(Word, DataBase);
      return true;
   }
  else
    return false;
}

template<> void databasecreator<character>::CheckDefaults(const festring& Word, character::database& DataBase)
{
  if(Word == "ArmBitmapPos")
    DataBase.RightArmBitmapPos = DataBase.LeftArmBitmapPos = DataBase.ArmBitmapPos;
  else if(Word == "LegBitmapPos")
    DataBase.GroinBitmapPos = DataBase.RightLegBitmapPos = DataBase.LeftLegBitmapPos = DataBase.LegBitmapPos;
  else if(Word == "ClothColor")
    DataBase.CapColor = DataBase.TorsoMainColor = DataBase.ArmMainColor = DataBase.LegMainColor = DataBase.ClothColor;
  else if(Word == "ClothColorIsSparkling")
    DataBase.CapColorIsSparkling = DataBase.TorsoMainColorIsSparkling = DataBase.ArmMainColorIsSparkling = DataBase.LegMainColorIsSparkling = DataBase.ClothColorIsSparkling;
  else if(Word == "NameSingular")
    DataBase.NamePlural = DataBase.NameSingular + 's';
  else if(Word == "BaseUnarmedStrength")
    {
      DataBase.BaseBiteStrength = DataBase.BaseUnarmedStrength >> 1;
      DataBase.BaseKickStrength = DataBase.BaseUnarmedStrength << 1;
    }
  else if(Word == "RightGauntlet")
    DataBase.LeftGauntlet = DataBase.RightGauntlet;
  else if(Word == "RightBoot")
    DataBase.LeftBoot = DataBase.RightBoot;
  else if(Word == "DefaultName")
    DataBase.Alias.push_back(DataBase.DefaultName);
}

template<> void databasecreator<item>::CheckDefaults(const festring& Word, item::database& DataBase)
{
  if(Word == "NameSingular")
    {
      DataBase.NamePlural = DataBase.NameSingular + 's';
      DataBase.FlexibleNameSingular = DataBase.NameSingular;
    }
  else if(Word == "BitmapPos")
    DataBase.WallBitmapPos = DataBase.BitmapPos;
}

template<> void databasecreator<glterrain>::CheckDefaults(const festring& Word, glterrain::database& DataBase)
{
  if(Word == "NameSingular")
    DataBase.NamePlural = DataBase.NameSingular + 's';
}

template<> void databasecreator<olterrain>::CheckDefaults(const festring& Word, olterrain::database& DataBase)
{
  if(Word == "NameSingular")
    DataBase.NamePlural = DataBase.NameSingular + 's';
}

template<> void databasecreator<material>::CheckDefaults(const festring& Word, material::database& DataBase)
{
  if(Word == "NameStem")
    DataBase.AdjectiveStem = DataBase.NameStem;
}

void databasesystem::Initialize()
{
  {
    /* Must be before character */

    inputfile ScriptFile(game::GetGameDir() + "Script/material.dat", &game::GetGlobalValueMap());
    databasecreator<material>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/char.dat", &game::GetGlobalValueMap());
    databasecreator<character>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/item.dat", &game::GetGlobalValueMap());
    databasecreator<item>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/glterra.dat", &game::GetGlobalValueMap());
    databasecreator<glterrain>::ReadFrom(ScriptFile);
  }

  {
    inputfile ScriptFile(game::GetGameDir() + "Script/olterra.dat", &game::GetGlobalValueMap());
    databasecreator<olterrain>::ReadFrom(ScriptFile);
  }
}

void databasesystem::CreateDataBaseMemberMaps()
{
  databasecreator<character>::CreateDataBaseMemberMap();
  databasecreator<item>::CreateDataBaseMemberMap();
  databasecreator<glterrain>::CreateDataBaseMemberMap();
  databasecreator<olterrain>::CreateDataBaseMemberMap();
  databasecreator<material>::CreateDataBaseMemberMap();
}

