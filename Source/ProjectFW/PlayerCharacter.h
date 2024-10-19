// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTFW_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	float GetBlendWeightForMovement() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetMode(bool bAttackMode);

	class UStatComponent* GetStat() const;
	float GetAmmoRate() const;
	bool GetIsReloading() const;

	void BeginRest();
	void Rest(float DeltaTime);

	// Movement
	void Move(FVector2D Vector);
	void Look(FVector2D Vector);
	void OnDash();
	void OffDash();
	void StartShoot();

private:
	UFUNCTION(BlueprintCallable)
	void EndShoot();

	// Camera
	UPROPERTY(VisibleDefaultsOnly)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraHeight = 55.0f;
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraOffsetInAttackMode = 40.0f;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	float CameraRotationWithMouseSpeed = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 400.0f;

	// Animation
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UClass* AttackModeAnimationBP;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UClass* NormalModeAnimationBP;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* FireAnimMontage;

	// Mesh
	UPROPERTY(EditAnywhere, Category = "Mesh")
	float AttackModeHeight = -93.0f;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	float NormalModeHeight = -98.0f;

	// Gun
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<class AGun> GunClass;

	UPROPERTY(EditAnywhere, Category = "Stat")
	class UStatComponent* Stat;

	// Widget
	UPROPERTY(EditDefaultsOnly, Category = "DamageText")
	TSubclassOf<class AFloatingDamageText> FloatingDamageTextClass;

	UPROPERTY()
	class AGun* Gun;

	bool bShooting = false;
};
