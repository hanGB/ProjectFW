// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MatchStatDataToLevel();
}


// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FLinearColor UStatComponent::AttributeColor(EAttribute Type)
{
	if (Type == EAttribute::Fire)
	{
		return FLinearColor(1.0f, 0.0f, 0.0f);
	}
	else if (Type == EAttribute::Ice)
	{
		return FLinearColor(0.0f, 0.0f, 1.0f);
	}
	else if (Type == EAttribute::Wind)
	{
		return FLinearColor(0.0f, 1.0f, 0.0f);
	}

	return FLinearColor(0.5f, 0.5f, 0.5f);
}

int UStatComponent::GetLevel() const
{
	return Level;
}

float UStatComponent::GetMaxExp() const
{
	return CurrentStatData.Exp;
}

float UStatComponent::GetMaxHealth() const
{
	return CurrentStatData.Health;
}

float UStatComponent::GetAttack() const
{
	return CurrentStatData.Attack;
}

float UStatComponent::GetDefense() const
{
	return CurrentStatData.Defense;
}

EAttribute UStatComponent::GetAttribute() const
{
	return Attribute;
}

float UStatComponent::Resistaance(EAttribute Type) const
{
	if (Type == EAttribute::Fire)
	{
		return FireResistance;
	}
	else if (Type == EAttribute::Ice)
	{
		return IceResistance;
	}
	else if (Type == EAttribute::Wind)
	{
		return WindResistance;
	}

	return 0.0f;
}

float UStatComponent::GetHealth() const
{
	return Health;
}

float UStatComponent::GetExp() const
{
	return Exp;
}

bool UStatComponent::IsDead() const
{
	return Health <= 0.0f;
}

float UStatComponent::GetHealthRate() const
{
	return Health / CurrentStatData.Health;
}

FLinearColor UStatComponent::GetAttributeColor() const
{
	return AttributeColor(Attribute);
}

float UStatComponent::CaculateDamageCaused(float Damage) const
{
	return Damage + CurrentStatData.Attack;
}

float UStatComponent::CaculateDamageReceivedAndApply(float Damage, EAttribute Type)
{
	float FinalDamage = Damage - CurrentStatData.Defense;
	FinalDamage = FMath::Max(FinalDamage, 1.0f);

	if (Type == EAttribute::Fire)
	{
		FinalDamage *= (1.0f + FireResistance);
	}
	else if (Type == EAttribute::Ice)
	{
		FinalDamage *= (1.0f + IceResistance);
	}
	else if (Type == EAttribute::Wind)
	{
		FinalDamage *= (1.0f + WindResistance);
	}

	FinalDamage = FMath::Min(FinalDamage, Health);
	Health -= FinalDamage;

	return FinalDamage;
}

void UStatComponent::AddExp(float Amount)
{
	Amount = FMath::Min(Amount, CurrentStatData.Exp - Exp);
	Exp += Amount;
}

void UStatComponent::AddHealth(float Amount)
{
	Amount = FMath::Min(Amount, CurrentStatData.Health - Health);
	Health += Amount;
}

void UStatComponent::MatchStatDataToLevel()
{
	CurrentStatData.Health = InitialStatData.Health + IncreaseStatData.Health * (Level -1);
	CurrentStatData.Attack = InitialStatData.Attack + IncreaseStatData.Attack * (Level - 1);
	CurrentStatData.Defense = InitialStatData.Defense + IncreaseStatData.Defense * (Level - 1);
	CurrentStatData.Exp = InitialStatData.Exp + IncreaseStatData.Exp * (Level - 1);

	Health = FMath::Min(Health, CurrentStatData.Health);
	Exp = FMath::Min(Exp, CurrentStatData.Exp);
}
