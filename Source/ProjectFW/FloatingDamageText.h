// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingDamageText.generated.h"

UCLASS()
class PROJECTFW_API AFloatingDamageText : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingDamageText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetDamageText(int Damage, FLinearColor Color);

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* DamageWidget;

	UPROPERTY(EditAnywhere)
	float LifeTime = 1.0f;

	float Timer = 0.0f;
};
