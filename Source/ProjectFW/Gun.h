// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class PROJECTFW_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Draw(bool bDraw);
	void PullTrigger();

	bool IsDrawed() const;

private:
	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxRange = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float Damage = 20.0f;
};
