#include <cmath>

#include "itemde.h"
#include "igraph.h"
#include "message.h"
#include "bitmap.h"
#include "stack.h"
#include "game.h"
#include "charba.h"
#include "level.h"
#include "lsquare.h"
#include "lterraba.h"
#include "save.h"
#include "feio.h"
#include "team.h"
#include "config.h"
#include "god.h"
#include "strover.h"
#include "femath.h"

void can::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

item* can::TryToOpen(character* Opener, stack* Stack)
{
	if(Opener->GetStrength() > RAND() % 30)
	{
		item* Item = new lump(GetMaterial(1));
		Stack->AddItem(Item);
		SetMaterial(1,0);
		UpdatePicture();

		if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
		{
			ushort Index = Opener->GetStack()->SearchItem(this);

			if(Index != 0xFFFF)
				Opener->GetStack()->MoveItem(Index, Opener->GetLevelSquareUnder()->GetStack());
		}

		return Item;
	}
	else
	{
		if(Opener->GetIsPlayer())
			ADD_MESSAGE("The can is shut tight.");

		return 0;
	}
}

bool banana::Consume(character* Eater, float Amount)
{
	GetMaterial(1)->EatEffect(Eater, Amount, NPModifier());

	if(Eater->GetIsPlayer() && Eater->CheckCannibalism(GetMaterial(1)->GetType()))
	{
		game::DoEvilDeed(25);
		ADD_MESSAGE("You feel that this was an evil deed.");
	}

	if(!GetMaterial(1)->GetVolume())
	{
		item* Peals = new bananapeals(false);
		Peals->InitMaterials(GetMaterial(0));
		PreserveMaterial(0);

		if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
			Eater->GetLevelSquareUnder()->GetStack()->AddItem(Peals);
		else
			Eater->GetStack()->AddItem(Peals);
	}

	return GetMaterial(1)->GetVolume() ? false : true;
}

bool potion::Consume(character* Eater, float Amount)
{
	GetMaterial(1)->EatEffect(Eater, Amount, NPModifier());

	if(Eater->GetIsPlayer() && Eater->CheckCannibalism(GetMaterial(1)->GetType()))
	{
		game::DoEvilDeed(25);
		ADD_MESSAGE("You feel that this was an evil deed.");
	}

	ushort Emit = GetEmitation();

	if(!GetMaterial(1)->GetVolume())
		ChangeMaterial(1,0);

	if(!game::GetInWilderness() && configuration::GetAutodropLeftOvers())
	{
		ushort Index = Eater->GetStack()->SearchItem(this);

		if(Index != 0xFFFF)
			Eater->GetStack()->MoveItem(Index, Eater->GetLevelSquareUnder()->GetStack());
	}

	if(GetSquareUnder())
	{
		GetSquareUnder()->SetDescriptionChanged(true);

		if(GetSquareUnder()->CanBeSeen())
			GetSquareUnder()->UpdateMemorizedDescription();

		if(!game::GetInWilderness())
			GetLevelSquareUnder()->SignalEmitationDecrease(Emit);

		GetSquareUnder()->SendNewDrawRequest();
		GetSquareUnder()->SendMemorizedUpdateRequest();

		if(GetSquareUnder()->CanBeSeen())
			GetSquareUnder()->UpdateMemorized();
	}

	return false;
}

void lantern::PositionedDrawToTileBuffer(uchar LevelSquarePosition) const
{
	switch(LevelSquarePosition)
	{
	case CENTER:
	case DOWN:
		Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
		break;
	case LEFT:
		Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, uchar(ROTATE_90));
		break;
	case UP:
		Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, uchar(FLIP));
		break;
	case RIGHT:
		Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16, uchar(ROTATE_90 | MIRROR));
		break;
	}
}

bool scroll::CanBeRead(character* Reader) const
{
	return Reader->CanRead();
}

bool scrollofcreatemonster::Read(character* Reader)
{
	vector2d TryToCreate;

	for(uchar c = 0; c < 100; ++c)
	{
		TryToCreate = Reader->GetPos() + game::GetMoveVector(RAND() % DIRECTION_COMMAND_KEYS);

		if(game::IsValidPos(TryToCreate) && game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->GetOverLevelTerrain()->GetIsWalkable() && game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->GetCharacter() == 0)
		{
			game::GetCurrentLevel()->GetLevelSquare(TryToCreate)->AddCharacter(protosystem::CreateMonster());

			if(Reader->GetIsPlayer())
				ADD_MESSAGE("As you read the scroll a monster appears.");
			else
				if(Reader->GetLevelSquareUnder()->CanBeSeen())
					ADD_MESSAGE("The %s reads %s. A monster appears!", Reader->CNAME(DEFINITE), CNAME(DEFINITE));

			return true;
		}
	}

	ADD_MESSAGE("You feel a lost soul fly by you.");
	return true;
}

bool scrollofteleport::Read(character* Reader)
{
	if(Reader->GetIsPlayer())
		ADD_MESSAGE("After you have read the scroll you realize that you have teleported.");
	else
		if(Reader->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("The %s reads %s and disappears!", Reader->CNAME(DEFINITE), CNAME(DEFINITE));

	Reader->Move(game::GetCurrentLevel()->RandomSquare(true), true);
	return true;
}

void lump::ReceiveHitEffect(character* Enemy, character*)
{
	if(RAND() % 2)
	{
		if(Enemy->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("The %s touches %s.", GetMaterial(0)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

		GetMaterial(0)->HitEffect(Enemy);
	}
}

void meleeweapon::ReceiveHitEffect(character* Enemy, character*)
{
	if(GetMaterial(2))
	{
		if(Enemy->GetIsPlayer())
			ADD_MESSAGE("The %s reacts with you!", GetMaterial(2)->CNAME(UNARTICLED));
		else
			if(Enemy->GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("The %s reacts with %s.", GetMaterial(2)->CNAME(UNARTICLED), Enemy->CNAME(DEFINITE));

		GetMaterial(2)->HitEffect(Enemy);
	}
}

void meleeweapon::DipInto(item* DipTo)
{
	ChangeMaterial(2, DipTo->BeDippedInto());
	ADD_MESSAGE("%s is now covered with %s.", CNAME(DEFINITE), GetMaterial(GetMaterials() - 1)->CNAME(UNARTICLED));
}

material* lump::BeDippedInto()
{
	return GetMaterial(0)->Clone(GetMaterial(0)->TakeDipVolumeAway());
}

bool potion::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	item* Remains = new brokenbottle(false);
	if(GetMaterial(1)) 
		GetLevelSquareUnder()->SpillFluid(5, GetMaterial(1)->GetColor());
	Remains->InitMaterials(GetMaterial(0));
	SetMaterial(0,0);
	ItemStack->AddItem(Remains);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The potion shatters to pieces.");
	SetExists(false);
	return true;
}

void potion::PositionedDrawToTileBuffer(uchar) const
{
	Picture->MaskedBlit(igraph::GetTileBuffer(), 0, 0, 0, 0, 16, 16);
}

item* can::PrepareForConsuming(character* Consumer, stack* Stack)
{
	if(!Consumer->GetIsPlayer() || game::BoolQuestion(std::string("Do you want to open ") + CNAME(DEFINITE) + " before eating it? [Y/n]", 'y'))
		return TryToOpen(Consumer, Stack);
	else
		return 0;
}

item* leftnutofpetrus::CreateWishedItem() const
{
	return new cheapcopyofleftnutofpetrus;
}

bool pickaxe::Apply(character* User, stack*)
{
	vector2d Temp;
	
	if((Temp = game::AskForDirectionVector("What direction do you want to dig?")) != vector2d(0,0))
	{
		levelsquare* Square = game::GetCurrentLevel()->GetLevelSquare(User->GetPos() + Temp);

		if(Square->CanBeDigged(User, this))
			if(Square->GetOverLevelTerrain()->GetMaterial(0)->CanBeDigged())
			{
				User->SetSquareBeingDigged(User->GetPos() + Temp);
				User->SetOldWieldedItem(User->GetWielded());
				User->SetWielded(this);
				User->ActivateState(DIGGING);
				User->SetStateCounter(DIGGING, User->GetStrength() < 50 ? (200 - (User->GetStrength() << 2)) : 2);
				User->SetStrengthExperience(User->GetStrengthExperience() + 50);
				return true;
			}
			else
				ADD_MESSAGE("%s is too hard to dig.", Square->GetOverLevelTerrain()->CNAME(DEFINITE));
		else
			ADD_MESSAGE(Square->GetOverLevelTerrain()->DigMessage().c_str());
	}

	return false;
}

ushort platemail::GetArmorValue() const
{
	float Base = 80 - sqrt(Material[0]->GetHitValue()) * 3;

	if(Base < 1)
		Base = 1;

	if(Base > 100)
		Base = 100;

	return ushort(Base);
}

ushort chainmail::GetArmorValue() const
{
	float Base = 90 - sqrt(Material[0]->GetHitValue()) * 2;

	if(Base < 1)
		Base = 1;

	if(Base > 100)
		Base = 100;

	return ushort(Base);
}

ushort brokenplatemail::GetArmorValue() const
{
	float Base = 95 - sqrt(Material[0]->GetHitValue());

	if(Base < 1)
		Base = 1;

	if(Base > 100)
		Base = 100;

	return ushort(Base);
}

bool wand::Apply(character* Terrorist, stack* MotherStack)
{
	if(Terrorist->GetIsPlayer())
		ADD_MESSAGE("%s breaks in two and then explodes!", CNAME(DEFINITE));
	else
		if(Terrorist->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s breaks %s in two. It explodes!", Terrorist->CNAME(DEFINITE), CNAME(INDEFINITE));

	MotherStack->RemoveItem(MotherStack->SearchItem(this));
	SetExists(false);
	Terrorist->GetLevelSquareUnder()->GetLevelUnder()->Explosion(Terrorist, Terrorist->GetLevelSquareUnder()->GetPos(), 40);
	return true;
}

bool wandofpolymorph::Zap(character* Zapper, vector2d, uchar Direction)
{
	if(GetCharges() <= GetTimesUsed())
	{
		ADD_MESSAGE("Nothing happens.");
		return true;
	}

	Beam(Zapper, Direction, 5);

	SetTimesUsed(GetTimesUsed() + 1);
	return true;
}

void wand::Save(outputfile& SaveFile) const
{
	item::Save(SaveFile);

	SaveFile << TimesUsed;
	SaveFile << Charges;
}

void wand::Load(inputfile& SaveFile)
{
	item::Load(SaveFile);

	SaveFile >> TimesUsed;
	SaveFile >> Charges;
}

bool scrollofwishing::Read(character* Reader)
{
	EMPTY_MESSAGES();
	game::DrawEverythingNoBlit();
	std::string Temp = game::StringQuestion("What do you want to wish for?", vector2d(7,7), WHITE, 0, 80);

	item* TempItem = protosystem::CreateItem(Temp, Reader->GetIsPlayer());

	if(TempItem)
	{
		Reader->GetStack()->AddItem(TempItem);
		ADD_MESSAGE("%s appears from nothing and the scroll burns!", TempItem->CNAME(INDEFINITE));
		return true;
	}


	return false;
}

bool lantern::ImpactDamage(ushort, bool IsShown, stack* ItemStack)
{
	brokenlantern* Lantern = new brokenlantern(false);
	Lantern->InitMaterials(GetMaterial(0));
	PreserveMaterial(0);
	Lantern->SignalSquarePositionChange(OnWall);
	ItemStack->AddItem(Lantern);
	ItemStack->RemoveItem(ItemStack->SearchItem(this));
	if (IsShown) ADD_MESSAGE("The lantern shatters to pieces.");
	SetExists(false);
	return true;
}

bool lantern::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(!(RAND() % 75) && Strength > 10 + RAND() % 10)
	{
		ImpactDamage(ushort(Strength), Shown, ItemsStack);
		return true;
	}

	return false;	
}

bool potion::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(!(RAND() % 75) && Strength > 10 + RAND() % 10)
	{
		ImpactDamage(ushort(Strength), Shown, ItemsStack);
		if(Shown)
			ADD_MESSAGE("The potion is destroyed by the sound.");
		return true;
	}

	return false;	
}

bool scrollofchangematerial::Read(character* Reader)
{
	ushort Index;

	if(!Reader->CanRead())
	{
		ADD_MESSAGE("This monster can't read anything.");
		return false;
	}

	if((Index = Reader->GetStack()->DrawContents(Reader, "What item do you wish to change?")) == 0xFFFF)
	{
		ADD_MESSAGE("You have nothing to change.");
		return false;
	}

	if(Index == 0xFFFE)
		return false;

	else
		if(!(Index < Reader->GetStack()->GetItems()))
			return false;

	if(Reader->GetStack()->GetItem(Index) == this)
	{
		ADD_MESSAGE("That would be rather insane.");
		return false;
	}

	if(!Reader->GetStack()->GetItem(Index)->IsMaterialChangeable())
	{
		ADD_MESSAGE("Your magic is not powerful enough to affect %s .", Reader->GetStack()->GetItem(Index)->CNAME(DEFINITE));
		return false;
	}

	EMPTY_MESSAGES();
	game::DrawEverythingNoBlit();
	std::string Temp = game::StringQuestion("What material do you want to wish for?", vector2d(7,7), WHITE, 0, 80);

	material* TempMaterial = protosystem::CreateMaterial(Temp, Reader->GetStack()->GetItem(Index)->GetMaterial(0)->GetVolume());
	
	if(TempMaterial)
		Reader->GetStack()->GetItem(Index)->ChangeMainMaterial(TempMaterial);
	else
	{
		return false;
	}

	return true;
}

item* brokenbottle::BetterVersion() const
{
	material* Stuff;

	if(RAND() % 5)
		Stuff = new bananaflesh;
	else
		Stuff = new omleurine;

	 item* P = new potion(false); 
	 P->InitMaterials(2, new glass, Stuff); 

	 return P;
}

bool wandofstriking::Zap(character* Zapper, vector2d, uchar Direction)
{
	if(GetCharges() <= GetTimesUsed())
	{
		ADD_MESSAGE("Nothing happens.");
		return true;
	}
	Beam(Zapper, Direction, 15);
	
	SetTimesUsed(GetTimesUsed() + 1);

	return true;
}

bool platemail::ReceiveSound(float Strength, bool Shown, stack* ItemsStack)
{
	if(Strength > 20000 + RAND() % 40000)
	{
		character* Wearer = ItemsStack->GetSquareUnder()->GetCharacter();

		if(Wearer && Wearer->GetTorsoArmor() == this)
			Wearer->SetTorsoArmor(0);

		ImpactDamage(ushort(Strength), false, ItemsStack);

		if(Shown)
			ADD_MESSAGE("The plate mail is damaged by the loud sound.");

		return true;
	}

	return false;	
}

bool platemail::ImpactDamage(ushort Strength, bool IsShown, stack* ItemStack)
{
	if(Strength > 2500.0f / GetArmorValue() + RAND() % 11 - RAND() % 11)
	{
		if (IsShown)
			ADD_MESSAGE("%s is damaged.", CNAME(DEFINITE));

		ItemStack->RemoveItem(ItemStack->SearchItem(this));
		item* Plate = new brokenplatemail(false);
		Plate->InitMaterials(GetMaterial(0));
		ItemStack->AddItem(Plate);
		SetMaterial(0,0);
		SetExists(false);
		return true;
	}
	else
		return false;
}

bool brokenbottle::GetStepOnEffect(character* Stepper)
{
	if(!(RAND() % 10))
	{
		if(Stepper->GetIsPlayer())
			ADD_MESSAGE("Auch. You step on sharp glass splinters.");
		else
			if(Stepper->GetSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s steps on sharp glass splinters and is hurt.", Stepper->CNAME(DEFINITE));

		Stepper->SetHP(Stepper->GetHP() - RAND() % 2 - 1);
		Stepper->CheckDeath("stepped on a broken bottle");
	}

	return false;
}

material* corpse::BeDippedInto()
{
	return GetMaterial(0)->Clone(GetMaterial(0)->TakeDipVolumeAway());
}

void potion::ColorChangeSpeciality(uchar Index, bool EmptyMaterial)
{
	if(!Index)
	{
		for(uchar c = 1; c < 4 && c < Material.size(); ++c)
			if(!Material[c])
				GraphicId.Color[c] = GraphicId.Color[0];
	}
	else
		if(EmptyMaterial)
			GraphicId.Color[Index] = GraphicId.Color[0];
}

ulong meleeweapon::Price() const
{
	return ulong(GetWeaponStrength() * GetWeaponStrength() * GetWeaponStrength() / (float(GetWeight()) * 1000000));
}

ulong torsoarmor::Price() const
{
	float ArmorModifier = 100.0f / GetArmorValue();

	return ulong(ArmorModifier * ArmorModifier * ArmorModifier * 200);
}

void lantern::SignalSquarePositionChange(bool NewPosOnWall)
{
	if(OnWall == NewPosOnWall)
		return;

	OnWall = NewPosOnWall;
	UpdatePicture();
}

void lantern::Save(outputfile& SaveFile) const
{
	item::Save(SaveFile);

	SaveFile << OnWall;
}

void lantern::Load(inputfile& SaveFile)
{
	item::Load(SaveFile);

	SaveFile >> OnWall;
}

void corpse::SetBloodColor(ushort Color)
{
	igraph::RemoveUser(GraphicId);
	GraphicId.Color[1] = Color;
	Picture = igraph::AddUser(GraphicId).Bitmap;
}

item* potion::BetterVersion() const
{
	if(!GetMaterial(1))
	{
		material* Stuff;

		if(RAND() % 5)
			Stuff = new bananaflesh;
		else
			Stuff = new omleurine;

		item* P = new potion(false); 
		P->InitMaterials(2, new glass, Stuff);

		return P;
	}
	else
		return 0;
}

item* can::BetterVersion() const
{
	if(!GetMaterial(1))
	{
		material* Stuff;

		if(RAND() % 2)
			Stuff = new schoolfood;
		else
			Stuff = new bananaflesh;

		item* P = new can(false); 
		P->InitMaterials(2, new iron, Stuff); 

		return P;
	}
	else
		return 0;
}

ushort whip::GetFormModifier() const
{
	if(GetMaterial(0)->IsFlexible())
		return 1000;
	else
		return 70;
}

bool backpack::Apply(character* Terrorist, stack* MotherStack)
{
	if(GetMaterial(1) && GetMaterial(1)->IsExplosive())
	{
		if(Terrorist->GetIsPlayer())
			ADD_MESSAGE("You light your %s. It explodes!", CNAME(UNARTICLED));
		else
			if(Terrorist->GetLevelSquareUnder()->CanBeSeen())
				ADD_MESSAGE("%s lights %s. It explodes!", Terrorist->CNAME(DEFINITE), CNAME(INDEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		Terrorist->GetLevelSquareUnder()->GetLevelUnder()->Explosion(Terrorist, Terrorist->GetLevelSquareUnder()->GetPos(), GetMaterial(1)->ExplosivePower());
		return true;
	}
	else
		if(Terrorist->GetIsPlayer())
			ADD_MESSAGE("You can't apply this!");	

	return false;
}

void holybook::Load(inputfile& SaveFile)
{
	item::Load(SaveFile);

	SaveFile >> OwnerGod;
}

void holybook::Save(outputfile& SaveFile) const
{
	item::Save(SaveFile);

	SaveFile << OwnerGod;
}

bool holybook::CanBeRead(character* Reader) const
{
	return Reader->CanRead();
}

bool holybook::Read(character* Reader)
{
	if(Reader->GetIsPlayer())
	{
		if(game::GetGod(GetOwnerGod())->GetKnown())
		{
			ADD_MESSAGE("The book reveals many divine secrets of %s to you.", game::GetGod(OwnerGod)->GOD_NAME);
			game::GetGod(OwnerGod)->AdjustRelation(75);
			game::ApplyDivineAlignmentBonuses(game::GetGod(OwnerGod), true);

			if(RAND() % 3)
				return false;
			else
			{
				ADD_MESSAGE("But then it disappears.");
				return true;
			}
		}

		game::GetGod(GetOwnerGod())->SetKnown(true);
		ADD_MESSAGE("You read the book through and feel that you master the magical rites of %s.", game::GetGod(OwnerGod)->GOD_NAME);
	}

	return false;
}

bool wand::ReceiveFireDamage(character* Burner, stack* MotherStack, long)
{
	if(!(RAND() % 10))
	{
		if(MotherStack->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s catches fire and explodes!", CNAME(DEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		MotherStack->GetLevelSquareUnder()->GetLevelUnder()->Explosion(Burner, MotherStack->GetLevelSquareUnder()->GetPos(), 40);
		return true;
	}
	else
		return false;
}

bool backpack::ReceiveFireDamage(character* Burner, stack* MotherStack, long)
{
	if(!(RAND() % 3))
	{
		if(MotherStack->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s explodes in the heat!", CNAME(DEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		MotherStack->GetLevelSquareUnder()->GetLevelUnder()->Explosion(Burner, MotherStack->GetLevelSquareUnder()->GetPos(), GetMaterial(1)->ExplosivePower());
		return true;
	}
	else
		return false;
}

std::string wand::Name(uchar Case) const 
{ 
	if(!TimesUsed)
		return NameWithMaterial(Case);
	else if(TimesUsed == 1)
		return NameWithMaterial(Case) + " (used 1 time)"; 
	else
		return NameWithMaterial(Case) + " (used " + TimesUsed + " times)";
}

bool scroll::ReceiveFireDamage(character*, stack* MotherStack, long)
{
	if(!(RAND() % 10) && GetMaterial(0)->IsFlammable())
	{
		if(MotherStack->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s catches fire!", CNAME(DEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		return true;
	}
	else
		return false;
}

void wand::Beam(character* Zapper, uchar Direction, uchar Range)
{
	vector2d CurrentPos = Zapper->GetPos();

	if(Direction != '.')
		for(ushort Length = 0; Length < Range; ++Length)
		{
			if(!game::GetCurrentLevel()->IsValid(CurrentPos + game::GetMoveVector(Direction)))
				break;

			levelsquare* Temp = game::GetCurrentLevel()->GetLevelSquare(CurrentPos + game::GetMoveVector(Direction));

			if(!(Temp->GetOverLevelTerrain()->GetIsWalkable()))
			{
				BeamEffect(Zapper, Direction, Temp);
				break;
			}
			else
			{	
				CurrentPos += game::GetMoveVector(Direction);
				BeamEffect(Zapper, Direction, Temp);

				if(Temp->CanBeSeen(true))
					Temp->DrawParticles(GetBeamColor(), Direction);
			}
		}
	else
	{
		levelsquare* Where = Zapper->GetLevelSquareUnder();
		BeamEffect(Zapper, Direction, Where);

		if(Where->CanBeSeen(true))
			Where->DrawParticles(GetBeamColor(), Direction);
	}
}

void wandofpolymorph::BeamEffect(character* Zapper, uchar, levelsquare* LevelSquare)
{
	LevelSquare->PolymorphEverything(Zapper);
}

void wandofstriking::BeamEffect(character* Who, uchar Dir, levelsquare* Where) 
{ 
	Where->StrikeEverything(Who, Dir); 
}

bool holybook::ReceiveFireDamage(character*, stack* MotherStack, long)
{
	if(!(RAND() % 2) && GetMaterial(0)->IsFlammable())
	{
		if(MotherStack->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s catches fire!", CNAME(DEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		return true;
	}
	else
		return false;
}

bool wand::StruckByWandOfStriking(character* Striker, stack* MotherStack) 
{ 
	if(!(RAND() % 10))
	{
		if(MotherStack->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s explodes!", CNAME(DEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		MotherStack->GetLevelSquareUnder()->GetLevelUnder()->Explosion(Striker, MotherStack->GetLevelSquareUnder()->GetPos(), 40);
		return true;
	}
	else
		return false;
}

bool backpack::StruckByWandOfStriking(character* Striker, stack* MotherStack) 
{ 
	if(RAND() % 3)
	{
		if(MotherStack->GetLevelSquareUnder()->CanBeSeen())
			ADD_MESSAGE("%s explodes!", CNAME(DEFINITE));

		MotherStack->RemoveItem(MotherStack->SearchItem(this));
		SetExists(false);
		MotherStack->GetLevelSquareUnder()->GetLevelUnder()->Explosion(Striker, MotherStack->GetLevelSquareUnder()->GetPos(), GetMaterial(1)->ExplosivePower());
		return true;
	}
	else
		return false;
}

material* loaf::CreateLoafMaterials(void)
{
  switch(RAND() % 3)
    {
    case 0:
      return new bread;

    case 1:
      return new pork;

    default:
      return new beef;
    }
}

