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
	bool PullTrigger();

	bool IsDrawed() const;

	float GetAmmoRate() const;
	void AddAmmoWhenRest(float DeltaTime);
	void ResetRestTimer();
	bool IsReloading() const;

private:
	void Reload(float DeltaTime);

	USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	class UNiagaraSystem* ImpactEffect;

	// Spec
	UPROPERTY(EditAnywhere, Category = "Spec")
	float MaxRange = 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Spec")
	float Damage = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Spec")
	int MaxAmmo = 50;
	UPROPERTY(EditAnywhere, Category = "Spec")
	float ReloadTime = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Spec")
	float RestAmmoTime = 1.0f;

	// State
	int Ammo = 50;
	bool bReloading = false;
	float ReloadState = 0.0f;
	float RestTimer = 0.0f;
};
