// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice"),
	Wind UMETA(DisplayName = "Wind"),
	Non UMETA(DisplayName = "Non")
};

USTRUCT(Atomic, BlueprintType)
struct FStatData
{
	GENERATED_BODY()

public:
	FStatData() {}

	FStatData(float Health, float Attack, float Defense, float Exp)
	{
		this->Health = Health;
		this->Attack = Attack;
		this->Defense = Defense;
		this->Exp = Exp;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Exp;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFW_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Getter
	int GetLevel() const;
	float GetMaxExp() const;
	float GetMaxHealth() const;
	float GetAttack() const;
	float GetDefense() const;
	EAttribute GetAttribute() const;
	float Resistaance(EAttribute Type) const;

	float GetHealth() const;
	float GetExp() const;
	bool IsDead() const;


	float GetHealthRate() const;
	FLinearColor GetAttributeColor() const;
	float CaculateDamageCaused(float Damage) const;
	float CaculateDamageReceivedAndApply(float Damage, EAttribute Type);

	void AddExp(float Amount);
	void AddHealth(float Amount);
	void MatchStatDataToLevel();

private:
	UPROPERTY(EditAnywhere, Category = "Stat")
	int Level = 1;

	UPROPERTY(EditAnywhere, Category = "Stat")
	FStatData CurrentStatData = FStatData(1000.0f, 100.0f, 100.0f, 100.0f);
	UPROPERTY(EditAnywhere, Category = "Stat")
	EAttribute Attribute = EAttribute::Fire;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float FireResistance = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float IceResistance = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float WindResistance = 0.0f;

	UPROPERTY(EditAnywhere, Category = "State")
	float Health = 1000.0f;
	float Exp = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Increase")
	FStatData IncreaseStatData = FStatData(50.0f, 5.0f, 5.0f, 10.0f);

	UPROPERTY(EditAnywhere, Category = "Initial")
	FStatData InitialStatData = FStatData(500.0f, 50.0f, 50.0f, 10.0f);
};
